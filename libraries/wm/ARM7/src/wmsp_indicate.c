#include "Mongoose.h"

// TODO
extern void OSi_Warning(char *file, int line, char *fmt, ...);
#define MAC_EQUALS(a, b) ((a)[0] == (b)[0] && (a)[1] == (b)[1] && (a)[2] == (b)[2] && (a)[3] == (b)[3] && (a)[4] == (b)[4] && (a)[5] == (b)[5])

// This is from a common header file
static u8 WMSP_GetRssi8(u8 rssi)
{
    if (rssi & 2)
    {
        return rssi >> 2;
    }
    else
    {
        return (rssi >> 2) + 25;
    }
}

static void WmspIndicate(WlCmdReq *req);
static void WmspFreeBufOfWL(WlCmdReq *buf);
static void WmspIndicateFuncDummy(WlCmdReq *req);
static void WmspIndicateMlmeBeaconSend(WlCmdReq *req);
static void WmspIndicateMlmeBeaconRecv(WlCmdReq *req);
static void WmspIndicateMlmeBeaconLost(WlCmdReq *req);
static void WmspIndicateMaMultiPollEnd(WlCmdReq *req);
static void WmspMPParentIntervalAlarmCallback(void *arg);
static void WmspKickMPParent(u16 pollbmp);
static void WmspSetRssi(WlMaMpEndInd *pInd, u16 pollBitmap, u16 unused);
static void WmspIndicateMaMultiPoll(WlCmdReq *buf);
static void WmspMaMultiPollAckAlarmCallback();
static void WmspIndicateMaMultiPollAck(WlCmdReq *buf);
static void WmspMPChildIntervalAlarmCallback();
static void WmspKickMPChild();
static void WmspIndicateMaData(WlCmdReq *req);
static void WmspIndicateMaFatalErr(WlCmdReq *req);
static void WmspIndicateMlmeDisAssociate(WlCmdReq *req);
static void WmspIndicateMlmeReAssociate(WlCmdReq *req);
static void WmspIndicateMlmeAssociate(WlCmdReq *req);
static void WmspIndicateMlmeDeAuthenticate(WlCmdReq *req);
static void WmspIndicateMlmeAuthenticate(WlCmdReq *req);

static OSAlarm wmspMPIntervalAlarm; // :23
static OSAlarm wmspMPAckAlarm;      // :24

void WMSP_IndicateThread()
{ // wmsp_indicate.c:99
    WMSPWork *work = &wmspW;
    void *msg;  // None - :104
    u16 cmd_id; // r1 - :106

    for (;;)
    {
        OS_ReceiveMessage(&work->fromWLmsgQ, &msg, 1);
        if (!msg)
        {
            OS_ExitThread();
            break;
        }

        cmd_id = ((u16 *)msg)[6];
        u8 low = cmd_id & 0xFF;
        u16 high = cmd_id & 0xFF00;

        if ((high == 0 || high == 0x100) && (low & 0x80) != 0)
        {
            WmspIndicate((WlCmdReq *)msg);
        }
        else
        {
            OS_SendMessage(&work->confirmQ, msg, 1);
        }
    }
}

static void WmspIndicate(WlCmdReq *req)
{
    if (wmspW.wm7buf && wmspW.status->state != WM_STATE_STOP)
    {
        switch (req->header.code)
        {
        case MLME_AUTH_IND_CMD:
            WmspIndicateMlmeAuthenticate(req);
            break;

        case MLME_DE_AUTH_IND_CMD:
            WmspIndicateMlmeDeAuthenticate(req);
            break;

        case MLME_ASS_IND_CMD:
            WmspIndicateMlmeAssociate(req);
            break;

        case MLME_RE_ASS_IND_CMD:
            WmspIndicateMlmeReAssociate(req);
            break;

        case MLME_DIS_ASS_IND_CMD:
            WmspIndicateMlmeDisAssociate(req);
            break;

        case MLME_BCN_LOST_IND_CMD:
            WmspIndicateMlmeBeaconLost(req);
            break;

        case MLME_BCN_SEND_IND_CMD:
            WmspIndicateMlmeBeaconSend(req);
            break;

        case MLME_BCN_RECV_IND_CMD:
            WmspIndicateMlmeBeaconRecv(req);
            break;

        case MA_DATA_IND_CMD:
            WmspIndicateMaData(req);
            break;

        case MA_MP_IND_CMD:
            WmspIndicateMaMultiPoll(req);
            break;

        case MA_MP_END_IND_CMD:
            WmspIndicateMaMultiPollEnd(req);
            break;

        case MA_MP_ACK_IND_CMD:
            WmspIndicateMaMultiPollAck(req);
            break;

        case MA_FATAL_ERR_IND_CMD:
            WmspIndicateMaFatalErr(req);
            break;

        default:
            WmspIndicateFuncDummy(req);
        }

        WmspFreeBufOfWL(req);
    }
}

static void WmspFreeBufOfWL(WlCmdReq *buf)
{                // wmsp_indicate.c:227
    u32 enabled; // r0 - :229

    enabled = OS_DisableInterrupts();
    OS_FreeToHeap(wmspW.arenaId, wmspW.heapHandle, buf);
    OS_RestoreInterrupts(enabled);
}

static void WmspIndicateFuncDummy(WlCmdReq *req)
{
    // OS_TPrintf("ARM7: Dummy indication function is called.\n");
}

static void WmspIndicateMlmeBeaconSend(WlCmdReq *req)
{
    if (wmspW.status->beaconIndicateFlag)
    {
        WMStartParentCallback *callback = WMSP_GetBuffer4Callback2Wm9();
        callback->apiid = WM_APIID_START_PARENT;
        callback->errcode = WM_ERRCODE_SUCCESS;
        callback->state = WM_STATECODE_BEACON_SENT;
        WMSP_ReturnResult2Wm9(callback);
    }
}

static void WmspIndicateMlmeBeaconRecv(WlCmdReq *req)
{
    WMStatus *status = wmspW.status;
    WlMlmeBeaconRecvInd *pInd = (WlMlmeBeaconRecvInd *)req;
    WMGameInfo *pGameInfo = (WMGameInfo *)pInd->gameInfo;

    u16 rssi8 = WMSP_GetRssi8(pInd->rssi);
    RSSI_UNK = rssi8 ^ (RSSI_UNK << 1) ^ ((rssi8 ^ ((u32)RSSI_UNK << 1)) >> 16);

    if (status->state == WM_STATE_CHILD || status->state == WM_STATE_MP_CHILD)
    {
        if (status->curr_tgid != pGameInfo->tgid)
        {
            u32 *buf = WMSP_GetInternalRequestBuf();
            int result;
            if (!buf)
            {
                result = 0;
            }
            else
            {
                buf[0] = 37;
                buf[1] = 1;
                buf[2] = 32770;

                result = OS_SendMessage(&wmspW.requestQ, buf, 0);
            }

            if (result == 0)
            {
                WMIndCallback *cb = WMSP_GetBuffer4Callback2Wm9();
                cb->apiid = WM_APIID_INDICATION;
                cb->errcode = WM_ERRCODE_FIFO_ERROR;
                cb->state = WM_STATECODE_FIFO_ERROR;
                cb->reason = 37;
                WMSP_ReturnResult2Wm9(cb);
            }
        }
        else
        {
            if (wmspW.status->beaconIndicateFlag)
            {
                WMBeaconRecvIndCallback *callback = WMSP_GetBuffer4Callback2Wm9();
                callback->apiid = WM_APIID_INDICATION;
                callback->errcode = WM_ERRCODE_SUCCESS;
                callback->state = WM_STATECODE_BEACON_RECV;
                callback->tgid = pGameInfo->tgid;
                callback->wmstate = status->state;
                callback->gameInfoLength = pInd->gameInfoLength;

                if (callback->gameInfoLength <= 0x80)
                {
                    MIi_CpuCopy16(pGameInfo, &callback->gameInfo, (callback->gameInfoLength + 1) & ~1);
                }

                WMSP_ReturnResult2Wm9(callback);
            }
        }
    }
}

static void WmspIndicateMlmeBeaconLost(WlCmdReq *req)
{
    if (wmspW.status->beaconIndicateFlag)
    {
        WMStartConnectCallback *callback = WMSP_GetBuffer4Callback2Wm9();
        callback->apiid = WM_APIID_START_CONNECT;
        callback->errcode = WM_ERRCODE_SUCCESS;
        callback->state = WM_STATECODE_BEACON_LOST;
        WMSP_ReturnResult2Wm9(callback);
    }
}

static void WmspIndicateMaMultiPollEnd(WlCmdReq *req)
{
    WMStatus *status = wmspW.status;
    WlMaMpEndInd *pInd = (WlMaMpEndInd *)req;

    int retryFlag = 0;

    if (status->mp_flag)
    {
        if (pInd->mpKey.errBitmap || status->mp_resumeFlag == 1 && pInd->mpKey.bitmap != 0)
        {
            WMSP_RequestResumeMP();
        }
        else
        {
            if (status->mp_resumeFlag)
            {
                status->mp_resumeFlag = 0;
            }

            WMMpRecvHeader *bufp = (WMMpRecvHeader *)status->mp_recvBuf[status->mp_recvBufSel];
            u32 size = pInd->mpKey.length * pInd->mpKey.count + 10;

            if (status->mp_recvBufSize < size)
            {
                OSi_TWarning("wmsp_indicate.c", 487, "recvBuf < recvData. %d < %d * %d + %d", status->mp_recvBufSize, pInd->mpKey.length, pInd->mpKey.count, 10);
                size = status->mp_recvBufSize;
            }

            MI_CpuCopy8(&pInd->mpKey, bufp, size);
            WmspSetRssi(pInd, bufp->bitmap, status->mp_minPollBmpMode);

            u64 now = OS_GetTick() | 1;

            WMMpRecvData *datap = bufp->data;
            u32 pollbmp = bufp->bitmap;

            u16 i = 0;
            while (i < bufp->count)
            {
                u16 aid = datap->aid;
                u16 length = datap->length;

                if (aid >= 1 && aid <= 0xF)
                {
                    if (length >= 2 && length != 0xFFFF)
                    {
                        length -= 2;
                        datap->length = length;
                        status->mp_readyBitmap |= (1 << aid);
                        status->mp_lastRecvTick[aid] = now;

                        if (length != 0)
                        {
                            WMSP_ParsePortPacket(aid, datap->wmHeader, datap->cdata, WMSP_GetRssi8(datap->rate_rssi >> 8), length, (WMMpRecvBuf *)bufp);
                        }
                    }
                    else
                    {
                        if (length == 0)
                        {
                            u64 last = status->mp_lastRecvTick[aid];
                            pollbmp |= (1 << aid);

                            if (status->mp_lifeTimeTick != 0 && last != 0 && (now - last) > status->mp_lifeTimeTick)
                            {
                                u32 *buf = WMSP_GetInternalRequestBuf();
                                int result;

                                status->mp_lastRecvTick[aid] = 0;

                                if (buf)
                                {
                                    buf[0] = 37;
                                    buf[1] = (1 << aid);
                                    buf[2] = 32769;
                                    result = OS_SendMessage(&wmspW.requestQ, buf, 0);
                                }
                                else
                                {
                                    result = 0;
                                }

                                if (result == 0)
                                {
                                    WMIndCallback *cb = WMSP_GetBuffer4Callback2Wm9();
                                    cb->apiid = WM_APIID_INDICATION;
                                    cb->errcode = WM_ERRCODE_FIFO_ERROR;
                                    cb->state = WM_STATECODE_FIFO_ERROR;
                                    cb->reason = 37;
                                    WMSP_ReturnResult2Wm9(cb);
                                }
                            }
                        }
                    }
                }

                i++;
                datap = (WMMpRecvData *)((u32)datap + bufp->length);
            }

            WMSP_FlushSendQueue(0, pollbmp);
            if (bufp->bitmap)
            {
                retryFlag = 1;
            }

            WMStartMPCallback *callback = WMSP_GetBuffer4Callback2Wm9();
            callback->apiid = WM_APIID_START_MP;
            callback->errcode = WM_ERRCODE_SUCCESS;
            callback->state = WM_STATECODE_MPEND_IND;
            callback->recvBuf = (WMMpRecvBuf *)bufp;
            WMSP_ReturnResult2Wm9(callback);

            status->mp_recvBufSel ^= 1;

            u32 enabled = OS_DisableInterrupts();
            if (!retryFlag)
            {
                status->mp_count--;
            }

            if (status->mp_limitCount > 0)
            {
                status->mp_limitCount--;
            }

            int flag = status->mp_count > 0 && status->mp_limitCount > 0; // not in nef
            OS_RestoreInterrupts(enabled);

            if (flag)
            {
                u16 recvErrorBitmap;
                if (retryFlag == 1)
                {
                    recvErrorBitmap = pInd->mpKey.bitmap;
                }
                else
                {
                    recvErrorBitmap = 0xFFFF;
                }

                if (status->mp_parentInterval)
                {
                    OS_CancelAlarm(&wmspMPIntervalAlarm);
                    OS_SetAlarm(&wmspMPIntervalAlarm, status->mp_parentIntervalTick, WmspMPParentIntervalAlarmCallback, (void *)recvErrorBitmap);
                }
                else
                {
                    WmspKickMPParent(recvErrorBitmap);
                }
            }
        }
    }
}

void WMSP_RequestResumeMP()
{ // wmsp_indicate.c:673
    WMStatus *status = wmspW.status;
    u32 *buf;          // r0 - :676
    int result;        // r0 - :677
    WMIndCallback *cb; // r0 - :698

    buf = WMSP_GetInternalRequestBuf();
    if (buf)
    {
        buf[0] = 45;
        buf[1] = status->mp_prevPollBitmap;
        result = OS_SendMessage(&wmspW.requestQ, buf, 0);
    }
    else
    {
        result = 0;
    }

    if (result != 0)
    {
        status->mp_resumeFlag = 1;
    }
    else
    {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_INDICATION;
        cb->errcode = WM_ERRCODE_FIFO_ERROR;
        cb->state = WM_STATECODE_FIFO_ERROR;
        cb->reason = 45;
        WMSP_ReturnResult2Wm9(cb);
    }
}

static void WmspMPParentIntervalAlarmCallback(void *arg)
{ // wmsp_indicate.c:724
    WmspKickMPParent((u16)arg);
}

static void WmspKickMPParent(u16 pollbmp)
{                                            // wmsp_indicate.c:739
    u32 *buf = WMSP_GetInternalRequestBuf(); // r0 - :741
    WMSPWork *sys = &wmspW;                  // r4 - :742
    int result;                              // r0 - :743

    if (!buf)
    {
        result = 0;
    }
    else
    {
        buf[0] = 43;
        buf[1] = pollbmp;
        result = OS_SendMessage(&sys->requestQ, buf, 0);
    }

    if (result == 0)
    {
        if (sys->wm7buf)
        {
            WMIndCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :765
            cb->apiid = WM_APIID_INDICATION;
            cb->errcode = WM_ERRCODE_FIFO_ERROR;
            cb->state = WM_STATECODE_FIFO_ERROR;
            cb->reason = 43;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}

static void WmspSetRssi(WlMaMpEndInd *pInd, u16 pollBitmap, u16 unused)
{
    WMStatus *status = wmspW.status;
    u16 min = status->minRssi;

    if (pollBitmap == 0 && pInd->mpKey.count >= 1)
    {
        for (int i = 0; i < pInd->mpKey.count; i++)
        {
            u16 temp = ((WlMpKeyData *)((u32)pInd->mpKey.data + pInd->mpKey.length * i))->rssi;
            temp = WMSP_GetRssi8(temp);

            if (temp < min)
            {
                min = temp;
            }
        }

        status->minRssi = min;
    }
}

static void WmspIndicateMaMultiPoll(WlCmdReq *req)
{
    WMStatus *status = wmspW.status;
    WlMaMpInd *pInd = (WlMaMpInd *)req;

    u16 rssi8 = WMSP_GetRssi8(pInd->frame.rssi);
    if (status->minRssi > rssi8)
    {
        status->minRssi = rssi8;
    }

    if (status->mp_flag != 0)
    {
        if (status->mp_vsyncFlag == 1)
        {
            status->mp_vsyncFlag = 0;
        }

        u16 oldEmptyFlag = status->mp_bufferEmptyFlag;
        status->mp_recvBufSel ^= 1;
        WMMpRecvBuf *bufp = status->mp_recvBuf[status->mp_recvBufSel];

        u32 size = pInd->frame.length + 48;
        if (status->mp_recvBufSize < size)
        {
            OSi_TWarning("wmsp_indicate.c", 908, "recvBuf < recvData. %d < %d + %d", status->mp_recvBufSize, pInd->frame.length, 48);
            size = status->mp_recvBufSize;
        }

        MI_CpuCopy8(&pInd->frame, bufp, size);
        u32 enabled = OS_DisableInterrupts();
        int timeout = 0;
        if (status->mp_waitAckFlag == 1)
        {
            timeout = 1;
            OS_CancelAlarm(&wmspMPAckAlarm);
        }

        status->mp_waitAckFlag = 1;
        status->mp_ackTime = bufp->ackTimeStamp;
        u16 txKeySts = pInd->frame.txKeySts;

        // FIXME what the fuck is this
        int empty = (txKeySts & 0x2000) != 0; // not sure about the name
        status->mp_isPolledFlag = empty;

        OS_SetAlarm(
            &wmspMPAckAlarm,
            (((33514ULL * (16 * ((u16)(bufp->ackTimeStamp - bufp->timeStamp) + 128))) / 64) / 1024),
            WmspMaMultiPollAckAlarmCallback,
            0);

        // FIXME what the fuck is also this (this is worse)
        int sent = 0;
        if ((txKeySts & 0x6000) == 0x6000)
        {
            sent = 1;
        }

        status->mp_bufferEmptyFlag = ((txKeySts & 0x400) == 0x400);
        status->mp_sentDataFlag = sent != 0;

        if (empty)
        {
            long childSize = (bufp->txop - 102) / 4 - 32;
            if (childSize >= 0)
            {
                if (childSize > 512)
                    childSize = 512;

                if (childSize != status->mp_childSize)
                    WMSP_SetChildSize(childSize);
            }
        }

        OS_RestoreInterrupts(enabled);
        if (timeout)
        {
            if (oldEmptyFlag == 1)
            {
                WMSP_FlushSendQueue(timeout, 0);
            }

            WMStartMPCallback *callback = WMSP_GetBuffer4Callback2Wm9();
            callback->apiid = WM_APIID_START_MP;
            callback->errcode = WM_ERRCODE_TIMEOUT;
            callback->state = WM_STATECODE_MPACK_IND;
            callback->recvBuf = 0;
            WMSP_ReturnResult2Wm9(callback);
        }

        if (!empty)
        {
            if (bufp->length >= 2)
            {
                status->mp_vsyncOrderedFlag = (bufp->wmHeader & 0x8000) != 0;
            }
        }
        else
        {
            MI_CpuCopy8(pInd->frame.destAdrs, bufp->destAdrs, 6);
            MI_CpuCopy8(pInd->frame.srcAdrs, bufp->srcAdrs, 6);

            if (bufp->length >= 2)
            {
                bufp->length -= 2;
                status->mp_vsyncOrderedFlag = (bufp->wmHeader & 0x8000) != 0;

                WMStartMPCallback *callback = WMSP_GetBuffer4Callback2Wm9();
                callback->apiid = WM_APIID_START_MP;
                callback->errcode = WM_ERRCODE_SUCCESS;
                callback->state = WM_STATECODE_MP_IND;
                callback->recvBuf = bufp;
                WMSP_ReturnResult2Wm9(callback);

                if (bufp->length != 0)
                {
                    WMSP_ParsePortPacket(0, bufp->wmHeader, bufp->data, WMSP_GetRssi8(pInd->frame.rssi), bufp->length, bufp);
                }
            }
            else
            {
                bufp->length = 0;
                status->mp_vsyncOrderedFlag = 0;

                WMStartMPCallback *callback = WMSP_GetBuffer4Callback2Wm9();
                callback->apiid = WM_APIID_START_MP;
                callback->errcode = WM_ERRCODE_NO_DATA;
                callback->state = WM_STATECODE_MP_IND;
                callback->recvBuf = bufp;
                WMSP_ReturnResult2Wm9(callback);
            }

            if (status->mp_lifeTimeTick != 0)
            {
                status->mp_lastRecvTick[0] = OS_GetTick() | 1;
            }
        }
    }
}

static void WmspMaMultiPollAckAlarmCallback()
{                           // wmsp_indicate.c:1119
    WMSPWork *sys = &wmspW; // r4 - :1122
    WlMaMpAckInd *pInd;     // r5 - :1124
    int result;             // r0 - :1125

    pInd = (WlMaMpAckInd *)OS_AllocFromHeap(sys->arenaId, sys->heapHandle, 0x40);
    if (!pInd)
    {
        result = 0;
    }
    else
    {
        pInd->header.code = MA_MP_ACK_IND_CMD;
        pInd->header.length = 0;
        result = OS_SendMessage(&sys->fromWLmsgQ, pInd, 0);
    }

    if (result == 0)
    {
        if (pInd)
        {
            WmspFreeBufOfWL((WlCmdReq *)pInd);
        }

        if (sys->wm7buf)
        {
            WMIndCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :1157
            cb->apiid = WM_APIID_INDICATION;
            cb->errcode = WM_ERRCODE_FIFO_ERROR;
            cb->state = WM_STATECODE_FIFO_ERROR;
            cb->reason = 128;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}

static void WmspIndicateMaMultiPollAck(WlCmdReq *req)
{
    WMStartMPCallback *callback;
    WlMaMpAckInd *pInd = (WlMaMpAckInd *)req;
    WlRxMpAckFrame *pFrame = &pInd->ack;
    WMStatus *status = wmspW.status;
    u32 enabled;
    int timeout, retryFlag, polled;
    u16 lower0, lower1, higher;

    retryFlag = 0;

    if (status->mp_flag)
    {
        if (pInd->header.length == 0)
        {
            timeout = 1;

            lower0 = W_US_COUNT0;
            higher = W_US_COUNT1;
            lower1 = W_US_COUNT0;

            if (lower0 > lower1)
            {
                higher = W_US_COUNT1;
            }

            u16 tmp = ((higher << 12) | (lower1 >> 4));
            if ((s16)(tmp - status->mp_ackTime) <= 0)
            {
                return;
            }
        }
        else
        {
            timeout = 0;
        }

        enabled = OS_DisableInterrupts();

        if (status->mp_waitAckFlag == 0)
        {
            OS_RestoreInterrupts(enabled);
        }
        else
        {
            status->mp_waitAckFlag = 0;
            polled = status->mp_isPolledFlag;
            OS_CancelAlarm(&wmspMPAckAlarm);
            OS_RestoreInterrupts(enabled);

            int unk = 1;
            if (status->mp_sentDataFlag != 0 && (timeout != 0 || (pFrame->bitmap & (1 << status->aid)) == 0))
            {
                unk = 0;
            }

            if (status->mp_sentDataFlag)
            {
                status->mp_sentDataFlag = 0;
            }

            if (status->mp_bufferEmptyFlag)
            {
                status->mp_bufferEmptyFlag = 0;
                retryFlag = WMSP_FlushSendQueue(timeout, unk != 0);
            }

            if (!polled)
            {
                return;
            }

            callback = WMSP_GetBuffer4Callback2Wm9();
            callback->apiid = WM_APIID_START_MP;
            if (timeout)
            {
                callback->errcode = WM_ERRCODE_TIMEOUT;
            }
            else if ((pFrame->bitmap & (1 << status->aid)) != 0)
            {
                callback->errcode = WM_ERRCODE_SEND_FAILED;
            }
            else
            {
                callback->errcode = WM_ERRCODE_SUCCESS;
            }

            callback->state = WM_STATECODE_MPACK_IND;
            callback->recvBuf = 0;

            if (!timeout)
            {
                callback->timeStamp = pFrame->timeStamp;
                callback->rate_rssi = *(u16 *)&pFrame->rate;
                MI_CpuCopy8(pFrame->destAdrs, callback->destAdrs, 6);
                MI_CpuCopy8(pFrame->srcAdrs, callback->srcAdrs, 6);
                callback->seqNum = pFrame->seqCtrl;
                callback->tmptt = pFrame->tmptt;
                callback->pollbmp = pFrame->bitmap;
            }

            WMSP_ReturnResult2Wm9(callback);

            if (polled)
            {
                if (retryFlag == 1 || status->mp_vsyncOrderedFlag == 0)
                {
                    if (status->mp_childInterval != 0)
                    {
                        OS_CancelAlarm(&wmspMPIntervalAlarm);
                        OS_SetAlarm(&wmspMPIntervalAlarm, status->mp_childIntervalTick, WmspMPChildIntervalAlarmCallback, 0);
                    }
                    else
                    {
                        WmspKickMPChild();
                    }
                }
                else
                {
                    status->mp_vsyncOrderedFlag = 0;
                    status->mp_vsyncFlag = 1;
                    status->mp_newFrameFlag = 0;
                }
            }
        }
    }
}

static void WmspMPChildIntervalAlarmCallback()
{ // wmsp_indicate.c:1392
    WmspKickMPChild();
}

static void WmspKickMPChild()
{                                            // wmsp_indicate.c:1407
    u32 *buf = WMSP_GetInternalRequestBuf(); // r0 - :1409
    WMSPWork *sys = &wmspW;                  // r4 - :1410
    WMStatus *status = sys->status;
    int result = 0; // r0 - :1412

    status->mp_vsyncOrderedFlag = 0;
    status->mp_vsyncFlag = 0;
    status->mp_newFrameFlag = 0;

    if (buf)
    {
        buf[0] = 44;
        result = OS_SendMessage(&sys->requestQ, buf, 0);
    }

    if (result == 0)
    {
        if (sys->wm7buf)
        {
            WMIndCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :1437
            cb->apiid = WM_APIID_INDICATION;
            cb->errcode = WM_ERRCODE_FIFO_ERROR;
            cb->state = WM_STATECODE_FIFO_ERROR;
            cb->reason = 44;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}

static void WmspIndicateMaData(WlCmdReq *req)
{
    WMStatus *status = wmspW.status;
    WlMaDataInd *pInd = (WlMaDataInd *)req;

    if (status->dcf_flag)
    {
        u8 rssi8 = WMSP_GetRssi8(pInd->frame.rssi);
        WMSP_AddRssiToList(rssi8);

        status->linkLevel = WMSP_GetAverageLinkLevel();
        WlRxFrame *pFrame = &pInd->frame;

        if (WMSP_CheckMacAddress((u8 *)pFrame->srcAdrs) != 1 && pFrame->length <= 0x5E4)
        {
            status->dcf_recvBufSel ^= 1;

            WMDcfRecvBuf *bufp = status->dcf_recvBuf[status->dcf_recvBufSel];
            MI_CpuCopy8(pFrame, bufp, ((u32)(pFrame->length + 44) + 1) & ~1); // How much did they torture the compiler to get this
            MI_CpuCopy8(pFrame->destAdrs, bufp->destAdrs, 6);
            MI_CpuCopy8(pFrame->srcAdrs, bufp->srcAdrs, 6);

            WMStartDCFCallback *callback = WMSP_GetBuffer4Callback2Wm9();
            callback->apiid = WM_APIID_START_DCF;
            callback->errcode = WM_ERRCODE_SUCCESS;
            callback->state = WM_STATECODE_DCF_IND;
            callback->recvBuf = bufp;
            WMSP_ReturnResult2Wm9(callback);
        }
    }
}

static void WmspIndicateMaFatalErr(WlCmdReq *req)
{
    WMStatus *status = wmspW.status;
    WlMaFatalErrInd *pInd = (WlMaFatalErrInd *)req;

    OS_TPrintf("[ARM7] MaFatalErr.Indicate(0x%04x)\n", pInd->errCode);

    if (status->mp_current_ignoreFatalErrorMode == 1 && pInd->errCode == 32)
    {
        OS_TPrintf("    but ignored\n");

        u32 enabled = OS_DisableInterrupts();
        OS_CancelAlarm(&wmspMPAckAlarm);
        status->mp_waitAckFlag = 0;
        OS_RestoreInterrupts(enabled);
        // status->mp_setDataFlag = 0;
        WMSP_FlushSendQueue(1, 0);

        WMIndCallback *cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_INDICATION;
        cb->errcode = WM_ERRCODE_SUCCESS;
        cb->state = WM_STATECODE_INFORMATION;
        cb->reason = 1;
        WMSP_ReturnResult2Wm9(cb);
    }
    else
    {
        u32 *buf = WMSP_GetInternalRequestBuf();
        int result;

        if (!buf)
        {
            result = 0;
        }
        else
        {
            buf[0] = 37;
            buf[2] = 0x8003;

            if (status->state == WM_STATE_MP_PARENT || status->state == WM_STATE_PARENT)
            {
                buf[1] = 0x7FFE;
            }
            else if (status->state == WM_STATE_MP_CHILD || status->state == WM_STATE_CHILD)
            {
                buf[1] = 1;
            }

            result = OS_SendMessage(&wmspW.requestQ, buf, 0);
        }

        if (result == 0)
        {
            WMIndCallback *cb = WMSP_GetBuffer4Callback2Wm9();
            cb->apiid = WM_APIID_INDICATION;
            cb->errcode = WM_ERRCODE_FIFO_ERROR;
            cb->state = WM_STATECODE_FIFO_ERROR;
            cb->reason = 37;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}

static void WmspIndicateMlmeDisAssociate(WlCmdReq *req)
{
    WMIndCallback *callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_INDICATION;
    callback->errcode = WM_ERRCODE_SUCCESS;
    callback->state = WM_STATECODE_DISASSOCIATE;
    WMSP_ReturnResult2Wm9(callback);
}

static void WmspIndicateMlmeReAssociate(WlCmdReq *req)
{
    WMIndCallback *callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_INDICATION;
    callback->errcode = WM_ERRCODE_SUCCESS;
    callback->state = WM_STATECODE_REASSOCIATE;
    WMSP_ReturnResult2Wm9(callback);
}

static void WmspIndicateMlmeAssociate(WlCmdReq *req)
{
    WMStatus *status = wmspW.status;
    WlMlmeAssociateInd *pInd = (WlMlmeAssociateInd *)req;
    u32 aid = pInd->aid;

    if (aid <= 0 || aid >= 0x10)
    {
        OSi_TWarning("wmsp_indicate.c", 2173, "invalid aid %d !", aid);
        return;
    }

    if (!status->pparam.entryFlag)
    {
        u32 *buf = WMSP_GetInternalRequestBuf();
        int result;
        if (!buf)
        {
            result = 0;
        }
        else
        {
            buf[0] = 34;
            MI_CpuCopy8(pInd->peerMacAdrs, buf + 1, 6);
            result = OS_SendMessage(&wmspW.requestQ, buf, 0);
        }

        if (result == 0)
        {
            WMIndCallback *cb = WMSP_GetBuffer4Callback2Wm9();
            cb->apiid = WM_APIID_INDICATION;
            cb->errcode = WM_ERRCODE_FIFO_ERROR;
            cb->state = WM_STATECODE_FIFO_ERROR;
            cb->reason = 34;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
    else
    {
        u32 e = OS_DisableInterrupts();

        status->child_bitmap |= (1 << aid);
        status->mp_readyBitmap &= ~(1 << aid);

        status->mp_lastRecvTick[aid] = OS_GetTick() | 1;
        MI_CpuCopy8(pInd->peerMacAdrs, status->childMacAddress[aid - 1], 6);

        OS_RestoreInterrupts(e);

        MIi_CpuClear16(1, status->portSeqNo[aid], 0x10);

        WMStartParentCallback *callback = WMSP_GetBuffer4Callback2Wm9();
        callback->apiid = WM_APIID_START_PARENT;
        callback->errcode = WM_ERRCODE_SUCCESS;
        callback->state = WM_STATECODE_CHILD_CONNECTED;
        MI_CpuCopy8(pInd->peerMacAdrs, callback->macAddress, sizeof(callback->macAddress));
        callback->aid = aid;
        MIi_CpuCopy16(&pInd->ssid[8], callback->ssid, 0x18);
        callback->parentSize = status->mp_parentSize;
        callback->childSize = status->mp_childSize;
        WMSP_ReturnResult2Wm9(callback);
    }
}

static void WmspIndicateMlmeDeAuthenticate(WlCmdReq *req)
{
    int i;
    WMStatus *status = wmspW.status;
    WlMlmeDeAuthenticateInd *pInd = (WlMlmeDeAuthenticateInd *)req;

    if (status->state == WM_STATE_PARENT || status->state == WM_STATE_MP_PARENT)
    {
        u8 tmpMacAddress[6];
        u16 tmpAID;

        MI_CpuCopy8(pInd->peerMacAdrs, tmpMacAddress, 6);
        tmpAID = 0;

        for (i = 0; i < 15; i++)
        {
            u32 e = OS_DisableInterrupts();
            if ((status->child_bitmap & (1 << (i + 1))) != 0)
            {
                if (MAC_EQUALS(tmpMacAddress, status->childMacAddress[i]))
                {
                    tmpAID = i + 1;
                    status->child_bitmap &= ~(1 << tmpAID);
                    status->mp_readyBitmap &= ~(1 << tmpAID);
                    status->mp_lastRecvTick[tmpAID] = 0;
                    MI_CpuFill8(status->childMacAddress[i], 0, 6);
                    OS_RestoreInterrupts(e);
                    break;
                }
            }

            OS_RestoreInterrupts(e);
        }

        if (tmpAID != 0)
        {
            WMStartParentCallback *callback = WMSP_GetBuffer4Callback2Wm9();
            callback->apiid = WM_APIID_START_PARENT;
            callback->errcode = WM_ERRCODE_SUCCESS;
            callback->state = WM_STATECODE_DISCONNECTED;
            callback->reason = pInd->reasonCode;
            callback->aid = tmpAID;
            MI_CpuCopy8(pInd->peerMacAdrs, callback->macAddress, 6);
            callback->parentSize = status->mp_parentSize;
            callback->childSize = status->mp_childSize;
            WMSP_ReturnResult2Wm9(callback);

            if (status->mp_flag == 1)
            {
                WMSP_CleanSendQueue(1 << tmpAID);
            }
        }
    }
    else
    {
        int fCleanQueue = 0;
        u32 e = OS_DisableInterrupts();

        if (status->child_bitmap == 0)
        {
            OS_RestoreInterrupts(e);
        }
        else
        {
            if (status->mp_flag == 1)
            {
                status->mp_flag = 0;
                fCleanQueue = 1;
                WMSP_CancelVAlarm();
                WMSP_FlushSendQueue(0, 0);
                WMSP_SetThreadPriorityLow();
            }

            status->child_bitmap = 0;
            status->mp_readyBitmap = 0;
            status->ks_flag = 0;
            status->dcf_flag = 0;
            status->VSyncFlag = 0;
            status->wep_flag = 0;
            status->wepMode = 0;
            MI_CpuFill8(status->wepKey, 0, 0x50);
            WMSP_ResetSizeVars();
            status->beaconIndicateFlag = 0;
            status->state = WM_STATE_CLASS1;
            OS_RestoreInterrupts(e);

            WMStartConnectCallback *callback = WMSP_GetBuffer4Callback2Wm9();
            callback->apiid = WM_APIID_START_CONNECT;
            callback->errcode = WM_ERRCODE_SUCCESS;
            callback->state = WM_STATECODE_DISCONNECTED;
            callback->reason = pInd->reasonCode;
            callback->aid = status->aid;
            MI_CpuCopy8(status->parentMacAddress, callback->macAddress, 6);
            callback->parentSize = status->mp_parentSize;
            callback->childSize = status->mp_childSize;
            WMSP_ReturnResult2Wm9(callback);

            if (fCleanQueue)
            {
                WMSP_CleanSendQueue(1);
            }
        }
    }
}

static void WmspIndicateMlmeAuthenticate(WlCmdReq *req)
{
    WMIndCallback *callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_INDICATION;
    callback->errcode = WM_ERRCODE_SUCCESS;
    callback->state = WM_STATECODE_AUTHENTICATE;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_InitAlarm()
{ // wmsp_indicate.c:1951
    OS_CreateAlarm(&wmspMPIntervalAlarm);
    OS_CreateAlarm(&wmspMPAckAlarm);
}