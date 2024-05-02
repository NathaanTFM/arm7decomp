#include "Mongoose.h"

// I haven't found a nef file that contains the definition for this function
// so I don't have its name, parameters, variables, line numbers, etc.
static inline void UnkFunction(WMMPTmpParam *tmpParam)
{
    u32 enabled;
    WMErrCode result = WM_ERRCODE_SUCCESS;
    u32 mask = tmpParam->mask;
    WMStatus *status = wmspW.status;

    if (wmspW.status->state != WM_STATE_MP_PARENT && wmspW.status->state != WM_STATE_MP_CHILD)
    {
        enabled = OS_DisableInterrupts();

        u16 maxFreq = (mask & 0x4) ? tmpParam->maxFrequency : status->mp_maxFreq;
        if (maxFreq == 0)
            maxFreq = 16;

        u16 minFreq = (mask & 0x1) ? tmpParam->minFrequency : status->mp_minFreq;
        if (minFreq == 0)
            minFreq = 16;

        if (minFreq > maxFreq)
            minFreq = maxFreq;

        u16 freq = (mask & 0x2) ? tmpParam->frequency : status->mp_freq;
        if (freq == 0)
            freq = 16;

        if (freq > maxFreq)
            freq = maxFreq;

        status->mp_current_maxFreq = maxFreq;
        status->mp_current_minFreq = minFreq;
        status->mp_current_freq = freq;

        if (status->mp_count > maxFreq)
        {
            status->mp_count = maxFreq;
        }

        status->mp_current_defaultRetryCount = (mask & 0x200) ? tmpParam->defaultRetryCount : status->mp_defaultRetryCount;
        status->mp_current_minPollBmpMode = (mask & 0x400) ? tmpParam->minPollBmpMode : status->mp_minPollBmpMode;
        status->mp_current_singlePacketMode = (mask & 0x800) ? tmpParam->singlePacketMode : status->mp_singlePacketMode;
        status->mp_current_ignoreFatalErrorMode = (mask & 0x1000) ? tmpParam->ignoreFatalErrorMode : status->mp_ignoreFatalErrorMode;

        OS_RestoreInterrupts(enabled);
    }
}

void WMSP_StartMP(void *msg)
{                   // req_StartMP.c:30
    u32 wlBuf[128]; // None - :32
    u16 *buf = (u16 *)wlBuf;
    WMSPWork *sys = &wmspW;                             // r0 - :34
    WMStatus *status = wmspW.status;                    // :35
    WMErrCode err = WM_ERRCODE_SUCCESS;                 // r7 - :36
    WMStartMPReq *req = (WMStartMPReq *)msg;            // r0 - :38
    WMMpRecvBuf *recvBuf = (WMMpRecvBuf *)req->recvBuf; // None - :39
    u32 recvBufSize = req->recvBufSize;                 // r5 - :40
    u32 *sendBuf = req->sendBuf;                        // None - :41
    u32 sendBufSize = req->sendBufSize;                 // r6 - :42
    WMMPParam *mpParam = &req->param;                   // r0 - :43
    WMMPTmpParam *mpTmpParam = &req->tmpParam;          // r0 - :44

    if (wmspW.status->mp_ignoreSizePrecheckMode == 0)
    {
        if (sendBufSize < ((status->mp_maxSendSize + 31) & ~31))
        {
            err = WM_ERRCODE_INVALID_PARAM;
        }

        // This is way too long!
        if (recvBufSize < (status->aid == 0 ? ((status->mp_maxRecvSize + 12) * status->pparam.maxEntry + 0xA + 0x1F) & ~0x1F : (status->mp_maxRecvSize + 0x32 + 0x1F) & ~0x1F))
        {
            err = WM_ERRCODE_INVALID_PARAM;
        }
    }

    if (status->mode == MODE_CHILD && ((1 << sys->wm7buf->connectPInfo.channel >> 1) & status->allowedChannel) == 0)
    {
        err = WM_ERRCODE_INVALID_PARAM;
    }

    if (err != WM_ERRCODE_SUCCESS)
    {
        WMStartMPCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :82
        cb->apiid = WM_APIID_START_MP;
        cb->errcode = err;
        cb->state = WM_STATE_MP_CHILD;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }

    int fCleanQueue = 0; // r7 - :91
    u32 e;               // None - :92

    if (status->mp_flag != 0)
    {
        status->mp_flag = 0;
        fCleanQueue = 1;
        OSi_TWarning("req_StartMP.c", 105, "[ARM7] WMSP_StartMP: mp_flag == TRUE");
    }

    if (fCleanQueue)
    {
        WMSP_CleanSendQueue(0xFFFF);
    }

    WMSP_InitSendQueue();
    e = OS_DisableInterrupts();
    WMSP_SetMPParameterCore(mpParam, 0);
    UnkFunction(mpTmpParam);

    if ((u16)(status->state + 0x10000 - WM_STATE_PARENT) <= 1)
    {
        status->mp_waitAckFlag = 0;
        status->mp_vsyncOrderedFlag = 0;
        status->mp_vsyncFlag = 1;
        status->mp_newFrameFlag = 0;
        status->mp_pingFlag = 0;
        status->mp_pingCounter = 60;
        status->sendQueueInUse = 0;
        status->mp_sentDataFlag = 0;
        status->mp_bufferEmptyFlag = 0;
        status->mp_isPolledFlag = 0;
        status->mp_resumeFlag = 0;
        status->mp_recvBuf[0] = recvBuf;
        status->mp_recvBufSize = recvBufSize;
        status->mp_recvBuf[1] = (WMMpRecvBuf *)((u32)recvBuf + recvBufSize);
        status->mp_recvBufSel = 0;
        status->mp_sendBuf = sendBuf;
        status->mp_sendBufSize = sendBufSize;
        status->mp_count = 0;
        status->mp_limitCount = 0;
        status->mp_prevPollBitmap = 0;
        status->mp_prevWmHeader = 0;
        status->minRssi = 0xFFFF;
        status->rssiCounter = 1;

        u64 now = OS_GetTick() | 1; // None - :158
        int i;                      // r7 - :159
        for (i = 0; i < 16; i++)
        {
            status->mp_lastRecvTick[i] = now;
        }

        WMSP_SetThreadPriorityHigh();

        status->valarm_queuedFlag = 0;
        WMSP_SetVAlarm();

        if (status->state == WM_STATE_CHILD)
        {
            status->state = WM_STATE_MP_CHILD;
        }
        else if (status->state == WM_STATE_PARENT)
        {
            status->state = WM_STATE_MP_PARENT;
        }

        WMStartMPCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :189
        cb->apiid = WM_APIID_START_MP;
        cb->errcode = WM_ERRCODE_SUCCESS;
        cb->state = WM_STATECODE_MP_START;
        WMSP_ReturnResult2Wm9(cb);

        status->mp_flag = 1;
        OS_RestoreInterrupts(e);

        WlCmdCfm *p_confirm = (WlCmdCfm *)WMSP_WL_ParamSetNullKeyResponseMode(buf, 1); // r0 - :211

        if (p_confirm->resultCode != 0)
        {
            WMCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :223
            cb->apiid = WM_APIID_START_MP;
            cb->errcode = WM_ERRCODE_FAILED;
            cb->wlCmdID = PARAMSET_NULL_KEY_MODE_REQ_CMD;
            cb->wlResult = p_confirm->resultCode;
            WMSP_ReturnResult2Wm9(cb);
        }

        return; // important!
    }

    OS_RestoreInterrupts(e);

    WMStartMPCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :239
    cb->apiid = WM_APIID_START_MP;
    cb->errcode = WM_ERRCODE_ILLEGAL_STATE;
    cb->state = WM_STATE_MP_CHILD;
    WMSP_ReturnResult2Wm9(cb);
}