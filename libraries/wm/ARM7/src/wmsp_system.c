#include "Mongoose.h"

#define FLAG_UNK (*(u16 *)0x2FFFF96UL)

WMSPWork wmspW;                     // :27
static OSThread wmspRequestThread;  // :32
static OSThread wmspIndicateThread; // :33

static void WmspError(u16 wmApiID, u16 wlCommand, u16 wlResult);
STATIC void WmspPxiCallback(PXIFifoTag unused, u32 data, int err);

void WM_sp_init(WlInit *wlInit, WmInit *wmInit)
{ // wmsp_system.c:55
    wmspW.dmaNo = wmInit->dmaNo;
    wmspW.arenaId = wlInit->heapFunc.os.id;
    wmspW.heapHandle = wlInit->heapFunc.os.heapHandle;
    wmspW.wm7buf = 0;
    wmspW.status = 0;

    OS_InitMessageQueue(&wmspW.toWLmsgQ, wmspW.toWLmsg, 2);
    OS_InitMessageQueue(&wmspW.fromWLmsgQ, wmspW.fromWLmsg, 4);
    OS_InitMessageQueue(&wmspW.confirmQ, wmspW.confirm, 4);
    OS_InitMessageQueue(&wmspW.requestQ, wmspW.request, 32);
    wlInit->recvMsgQueuep = &wmspW.fromWLmsgQ;
    wlInit->sendMsgQueuep = &wmspW.toWLmsgQ;

    wmspW.indPrio_high = wmInit->indPrio_high;
    wmspW.wlPrio_high = wmInit->wlPrio_high;
    wmspW.reqPrio_high = wmInit->reqPrio_high;
    wmspW.indPrio_low = wmInit->indPrio_low;
    wmspW.wlPrio_low = wmInit->wlPrio_low;
    wmspW.reqPrio_low = wmInit->reqPrio_low;

    OS_InitMutex(&wmspW.fifoExclusive);

    OS_CreateThread(&wmspIndicateThread, WMSP_IndicateThread, 0, &wmspW.fifoExclusive, 0x400, wmInit->indPrio_low);
    OS_WakeupThreadDirect(&wmspIndicateThread);

    OS_CreateThread(&wmspRequestThread, WMSP_RequestThread, 0, wmspW.indicateStack, 0x1000, wmInit->reqPrio_low);
    OS_WakeupThreadDirect(&wmspRequestThread);

    long i; // r5 - :135
    for (i = 0; i < 32; i++)
    {
        wmspW.rssiHistory[i] = 0;
    }

    wmspW.rssiIndex = 0;
    if (!OS_IsVAlarmAvailable())
    {
        OS_InitVAlarm();
    }

    PXI_Init();
    PXI_SetFifoRecvCallback(10, WmspPxiCallback);

    wlInit->dmaChannel = 2;
    wlInit->priority = wmInit->wlPrio_low;
    WL_InitDriver(wlInit);
}

void WMSP_ReturnResult2Wm9(void *ptr)
{                                   // wmsp_system.c:192
    void SVC_WaitByLoop(u32 count); // TODO?

    while (PXI_SendWordByFifo(10, (u32)ptr, 0) < 0)
        SVC_WaitByLoop(256);

    OS_UnlockMutex(&wmspW.fifoExclusive);
}

void *WMSP_GetBuffer4Callback2Wm9()
{                                   // wmsp_system.c:237
    void SVC_WaitByLoop(u32 count); // TODO?

    OS_LockMutex(&wmspW.fifoExclusive);
    while ((FLAG_UNK & 1) != 0)
        SVC_WaitByLoop(256);

    FLAG_UNK |= 1;
    return wmspW.wm7buf->fifo7to9;
}

u16 *WMSP_WlRequest(u16 *request)
{                            // wmsp_system.c:264
    void *msg;               // None - :266
    WMIndCallback *callback; // r0 - :278
    WlCmdCfm *pCfm;          // not in nef

    OS_SendMessage(&wmspW.toWLmsgQ, request, 1);
    OS_ReceiveMessage(&wmspW.confirmQ, &msg, 1);

    pCfm = GET_CFM((WlCmdReq *)msg);
    if (pCfm->resultCode == 14)
    {
        callback = (WMIndCallback *)WMSP_GetBuffer4Callback2Wm9();
        callback->apiid = WM_APIID_INDICATION;
        callback->errcode = WM_ERRCODE_FLASH_ERROR;
        callback->state = WM_STATECODE_UNKNOWN;
        WMSP_ReturnResult2Wm9(callback);
        SND_BeginSleep();
        OS_Terminate();
    }

    return (u16 *)msg;
}

STATIC void WmspPxiCallback(PXIFifoTag unused, u32 data, int err)
{               // wmsp_system.c:306
    int result; // r0 - :310

    if (err == 0 && OS_SendMessage(&wmspW.requestQ, (void *)data, 0) == 0)
    {
        if (wmspW.wm7buf)
        {
            WMCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :328
            cb->apiid = *(u16 *)data;
            cb->errcode = WM_ERRCODE_FIFO_ERROR;
            cb->wlCmdID = 0;
            cb->wlResult = 0;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}

int WMSP_CheckMacAddress(u8 *macAdr)
{                     // wmsp_system.c:348
    u8 *myMacAddress; // r0 - :351

    myMacAddress = wmspW.status->MacAddress;

    return (macAdr[0] == myMacAddress[0] && macAdr[1] == myMacAddress[1] && macAdr[2] == myMacAddress[2] && macAdr[3] == myMacAddress[3] && macAdr[4] == myMacAddress[4] && macAdr[5] == myMacAddress[5]);
}

void WMSP_CopyParentParam(WMGameInfo *gameInfop, WMParentParam *pparamp)
{ // wmsp_system.c:422
    gameInfop->ggid = pparamp->ggid;
    gameInfop->tgid = pparamp->tgid;

    gameInfop->gameNameCount_attribute = (pparamp->entryFlag ? 1 : 0) | (pparamp->multiBootFlag ? 2 : 0) | (pparamp->KS_Flag ? 4 : 0);
    gameInfop->userGameInfoLength = pparamp->userGameInfoLength;
    gameInfop->magicNumber = 1;
    gameInfop->ver = 1;
    gameInfop->platform = 0;
    gameInfop->parentMaxSize = pparamp->parentMaxSize;

    gameInfop->childMaxSize = (pparamp->multiBootFlag && pparamp->childMaxSize >= 8) ? 8 : pparamp->childMaxSize;
    if (gameInfop->userGameInfoLength)
    {
        MI_CpuCopy8(pparamp->userGameInfo, gameInfop->userGameInfo, (gameInfop->userGameInfoLength + 1) & ~1);
    }
}

int WMSP_SetAllParams(u16 wmApiID, u16 *buf)
{ // wmsp_system.c:461
    WMStatus *status = wmspW.status;
    WlParamSetCfm *pConfirm;                          // r0 - :468
    WlParamSetAllReq *pReq = (WlParamSetAllReq *)buf; // r0 - :472

    MI_CpuCopy8(status->MacAddress, pReq->staMacAdrs, 6);
    pReq->retryLimit = 7;
    pReq->enableChannel = status->enableChannel;
    pReq->rate = status->rate;
    pReq->mode = status->mode;

    if (status->wep_flag == 0)
    {
        pReq->wepMode = 0;
        pReq->wepKeyId = 0;
        MIi_CpuClear16(0, pReq->wepKey, 0x50);
        pReq->authAlgo = 0;
    }
    else
    {
        pReq->wepMode = status->wepMode;
        pReq->wepKeyId = status->wepKeyId;
        MI_CpuCopy8(status->wepKey, pReq->wepKey, 0x50);
        pReq->authAlgo = 1;
    }

    pReq->beaconType = 1;
    pReq->probeRes = 1;

    if (status->mode == 1)
    {
        pReq->beaconLostTh = 0;
    }
    else
    {
        pReq->beaconLostTh = 16;
    }

    pReq->activeZoneTime = 10;

    if (wmApiID == WM_APIID_START_SCAN_EX)
    {
        MIi_CpuClear16(0, pReq->ssidMask, 32);
    }
    else
    {
        MIi_CpuClear16(0, pReq->ssidMask, 8);
        MIi_CpuClear16(-1, pReq->ssidMask + 8, 24);
    }

    pReq->preambleType = status->preamble;

    pConfirm = WMSP_WL_ParamSetAll(pReq);
    if (pConfirm->resultCode != 0)
    {
        WmspError(wmApiID, 512, pConfirm->resultCode);
        return 0;
    }

    return 1;
}

u16 WMSP_GetAllowedChannel(u16 bitField)
{                // wmsp_system.c:576
    u16 temp;    // r0 - :578
    long max;    // r12 - :579
    long min;    // r0 - :580
    long center; // r14 - :581

    temp = (bitField & 0x1FFF);
    if (temp == 0)
        return 0;

    for (min = 0; min < 16; min++)
    {
        if (temp & (1 << min))
            break;
    }

    for (max = 15; max != 0; --max)
    {
        if (temp & (1 << max))
            break;
    }

    if (max - min < 5)
        return (1 << min);

    center = (max + min) / 2;

    long i; // r4 - :615
    for (i = 0; i < max - min; i++)
    {
        center += i * (2 * (i % 2) - 1);
        if (temp & (1 << center))
            break;
    }

    if (max - center < 5 || center - min < 5)
    {
        return (1 << min) | (1 << max);
    }
    else
    {
        return (1 << max) | (1 << center) | (1 << min);
    }
}

void WMSP_AddRssiToList(u8 rssi8)
{ // wmsp_system.c:650
    wmspW.rssiHistory[wmspW.rssiIndex] = rssi8;
    wmspW.rssiIndex = (wmspW.rssiIndex + 1) & 0x1F;

    // Apparently, this is used for RNG?
    RSSI_UNK = rssi8 ^ (RSSI_UNK << 1) ^ ((rssi8 ^ ((u32)RSSI_UNK << 1)) >> 16);
}

void WMSP_FillRssiIntoList(u8 rssi8)
{          // wmsp_system.c:666
    int i; // r3 - :668

    for (i = 0; i < 32; i++)
    {
        wmspW.rssiHistory[i] = rssi8;
    }

    wmspW.rssiIndex = 0;
}

u16 WMSP_GetAverageLinkLevel()
{             // wmsp_system.c:685
    long i;   // r2 - :687
    u32 temp; // r3 - :688

    temp = 0;
    for (i = 0; i < 32; i++)
    {
        temp += wmspW.rssiHistory[i];
    }
    return WMSP_GetLinkLevel(temp / 32);
}

u16 WMSP_GetLinkLevel(u32 rssi)
{ // wmsp_system.c:715
    if (wmspW.wm7buf->connectPInfo.gameInfo.platform == 8)
    {
        if (rssi < 0x16)
            return 0;
        if (rssi < 0x1C)
            return 1;
        if (rssi < 0x22)
            return 2;
        return 3;
    }
    else
    {
        if (rssi < 0x8)
            return 0;
        if (rssi < 0xE)
            return 1;
        if (rssi < 0x14)
            return 2;
        return 3;
    }
}

void WMSP_SetThreadPriorityLow()
{          // wmsp_system.c:756
    u32 e; // r0 - :758

    e = OS_DisableInterrupts();
    OS_DisableScheduler();
    OS_SetThreadPriority(&wmspRequestThread, wmspW.reqPrio_low);
    OS_SetThreadPriority(WL_GetThreadStruct(), wmspW.wlPrio_low);
    OS_SetThreadPriority(&wmspIndicateThread, wmspW.indPrio_low);
    OS_EnableScheduler();
    OS_RestoreInterrupts(e);
}

void WMSP_SetThreadPriorityHigh()
{          // wmsp_system.c:792
    u32 e; // r0 - :794

    e = OS_DisableInterrupts();
    OS_DisableScheduler();
    OS_SetThreadPriority(&wmspIndicateThread, wmspW.indPrio_high);
    OS_SetThreadPriority(WL_GetThreadStruct(), wmspW.wlPrio_high);
    OS_SetThreadPriority(&wmspRequestThread, wmspW.reqPrio_high);
    OS_EnableScheduler();
    OS_RestoreInterrupts(e);
}

static void WmspError(u16 wmApiID, u16 wlCommand, u16 wlResult)
{
    WMStartConnectCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :1135
    callback->apiid = wmApiID;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

u32 *WMSP_GetInternalRequestBuf()
{             // wmsp_system.c:914
    long i;   // r3 - :916
    u32 *ret; // r4 - :917
    u32 e;    // r0 - :918

    ret = 0;
    e = OS_DisableInterrupts();

    if (wmspW.wm7buf)
    {
        for (i = 0; i < 32; i++)
        {
            if ((wmspW.wm7buf->requestBuf[4 * i] & 0x8000) != 0)
            {
                ret = &wmspW.wm7buf->requestBuf[4 * i];
                *ret &= ~0x8000;
                break;
            }
        }
    }

    OS_RestoreInterrupts(e);
    return ret;
}

void WMSP_ResetSizeVars()
{ // wmsp_system.c:943
    WMStatus *status = wmspW.status;

    status->mp_sendSize = 0;
    status->mp_recvSize = 0;
    status->mp_parentSize = 0;
    status->mp_childSize = 0;
    status->mp_maxSendSize = 0;
    status->mp_maxRecvSize = 0;
    status->mp_parentMaxSize = 0;
    status->mp_childMaxSize = 0;
}

void WMSP_SetParentMaxSize(u16 parentMaxSize)
{                    // wmsp_system.c:962
    u16 maxSendSize; // r0 - :976
    u16 maxRecvSize; // r0 - :983
    WMStatus *status = wmspW.status;

    if (parentMaxSize > 0x200)
        parentMaxSize = 0x200;

    status->mp_parentSize = parentMaxSize;
    status->mp_parentMaxSize = parentMaxSize;

    if (status->aid == 0)
    {
        status->mp_maxSendSize = parentMaxSize + 4;
        status->mp_sendSize = parentMaxSize + 4;
    }
    else
    {
        status->mp_maxRecvSize = parentMaxSize + 4;
        status->mp_recvSize = parentMaxSize + 4;
    }
}

void WMSP_SetChildMaxSize(u16 childMaxSize)
{                    // wmsp_system.c:994
    u16 maxRecvSize; // r0 - :1008
    u16 maxSendSize; // r0 - :1015
    WMStatus *status = wmspW.status;

    if (childMaxSize > 0x200)
        childMaxSize = 0x200;

    status->mp_childMaxSize = childMaxSize;
    status->mp_childSize = childMaxSize;

    if (status->aid == 0)
    {
        status->mp_maxRecvSize = childMaxSize + 2;
        status->mp_recvSize = childMaxSize + 2;
    }
    else
    {
        status->mp_maxSendSize = childMaxSize + 2;
        status->mp_sendSize = childMaxSize + 2;
    }
}

void WMSP_SetParentSize(u16 parentSize)
{ // wmsp_system.c:1026
    WMStatus *status = wmspW.status;
    status->mp_parentSize = parentSize;

    if (status->aid == 0)
    {
        status->mp_sendSize = parentSize + 4;
    }
    else
    {
        status->mp_recvSize = parentSize + 4;
    }
}

void WMSP_SetChildSize(u16 childSize)
{ // wmsp_system.c:1051
    // FIXME: this variable isn't a thing(?) but it made compilation accurate
    // for that specific function because the compiler won't optimize
    WMStatus *status = wmspW.status;

    status->mp_childSize = childSize;

    if (status->aid == 0)
    {
        status->mp_recvSize = childSize + 2;
    }
    else
    {
        status->mp_sendSize = childSize + 2;
    }
}