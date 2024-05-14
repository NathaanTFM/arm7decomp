#include "Mongoose.h"

void WMSP_Enable(void *msg)
{                             // req_Enable.c:41
    u32 *reqBuf = (u32 *)msg; // r0 - :43
    WMCallback *cb;           // r0 - :44
    WMArm7Buf *p;             // r0 - :46
    u32 miscFlags;            // :47

    p = wmspW.wm7buf = (WMArm7Buf *)reqBuf[1];
    wmspW.status = (WMStatus *)reqBuf[2];
    p->status = wmspW.status;
    p->fifo7to9 = (u32 *)reqBuf[3];
    miscFlags = reqBuf[4];

#ifdef TWL_MODE
    if (NWMSP_CheckInitialized())
    {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 0;
        cb->errcode = 3;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }
#endif

    WMSPi_CommonInit(miscFlags);

    cb = (WMCallback *)WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_ENABLE;
    cb->errcode = WM_ERRCODE_SUCCESS;
    WMSP_ReturnResult2Wm9(cb);
}

void WMSPi_CommonInit(u32 miscFlags)
{                                    // req_Enable.c:87
    WMArm7Buf *p = wmspW.wm7buf;     // r6 - :89
    WMStatus *status = wmspW.status; // r9, not in nef
    int fCleanQueue = 0;             // r7 - :98
    u32 e = OS_DisableInterrupts();  // r8 - :99

    if (status->mp_flag == 1)
    {
        status->mp_flag = 0;
        fCleanQueue = 1;
        WMSP_CancelVAlarm();
        WMSP_SetThreadPriorityLow();
    }

    status->child_bitmap = 0;
    status->mp_readyBitmap = 0;
    status->ks_flag = 0;
    status->dcf_flag = 0;
    status->VSyncFlag = 0;
    status->valarm_queuedFlag = 0;
    status->beaconIndicateFlag = 0;
    status->mp_minFreq = 1;
    status->mp_freq = 1;
    status->mp_maxFreq = 6;
    status->mp_defaultRetryCount = 0;
    status->mp_minPollBmpMode = 0;
    status->mp_singlePacketMode = 0;
    status->mp_ignoreFatalErrorMode = 0;
    status->mp_ignoreSizePrecheckMode = 0;
    status->mp_current_minFreq = status->mp_minFreq;
    status->mp_current_freq = status->mp_freq;
    status->mp_current_maxFreq = status->mp_maxFreq;
    status->mp_current_defaultRetryCount = status->mp_defaultRetryCount;
    status->mp_current_minPollBmpMode = status->mp_minPollBmpMode;
    status->mp_current_singlePacketMode = status->mp_singlePacketMode;
    status->mp_current_ignoreFatalErrorMode = status->mp_ignoreFatalErrorMode;
    status->wep_flag = 0;
    status->wepMode = 0;
    MI_CpuFill8(status->wepKey, 0, sizeof(status->wepKey));
    WMSP_ResetSizeVars();
    status->mp_parentVCount = 260;
    status->mp_childVCount = 240;
    status->mp_parentInterval = 1000;
    status->mp_childInterval = 0;
    status->mp_parentIntervalTick = 523;
    status->mp_childIntervalTick = 0;
    status->pwrMgtMode = 0;
    status->preamble = 1;
    status->miscFlags = miscFlags;
    OS_RestoreInterrupts(e);

    if (fCleanQueue)
    {
        WMSP_CleanSendQueue(0xFFFF);
    }

    for (int i = 0; i < 32; i++)
    { // r2 - :170
        p->requestBuf[i * 4] = 0x8000;
    }

    MIi_CpuClear16(1, status->portSeqNo, sizeof(status->portSeqNo));
    WMSP_InitAlarm();
    OS_InitMutex(&status->sendQueueMutex);
    WMSP_InitVAlarm();

#ifdef TWL_MODE
    wmspW.wmInitialized = 1;
#endif

    if ((miscFlags & 2) == 0)
    {
        PM_SetLEDPattern(PM_LED_PATTERN_WIRELESS);
    }
    status->state = WM_STATE_STOP;
}