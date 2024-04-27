#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult);

void WMSP_EndMP(void *msg) { // req_EndMP.c:94
    u32 wlBuf[128]; // None - :100
    WlCmdCfm* pConfirm; // r0 - :101
    int fCleanQueue = 0; // r5 - :103
    struct WMStatus* status = wmspW.status;
    
    if (wmspW.status->state != 9 && wmspW.status->state != 10) {
        struct WMCallback* callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :108
        callback->apiid = 16;
        callback->errcode = 3;
        WMSP_ReturnResult2Wm9(callback);
        return;
    }
    
    u32 e = OS_DisableInterrupts(); // r6 - :116
    if (status->mp_flag == 1) {
        WMSP_FlushSendQueue(0, 0xFFFF);
        fCleanQueue = 1;
    }
    
    status->mp_flag = 0;
    WMSP_CancelVAlarm();
    WMSP_SetThreadPriorityLow();
    
    if (status->state == 10) {
        status->state = 8;
        
    } else if (status->state == 9) {
        status->state = 7;
    }
    
    OS_RestoreInterrupts(e);
    
    pConfirm = (WlCmdCfm*)WMSP_WL_ParamSetNullKeyResponseMode((u16*)wlBuf, 0);
    if (pConfirm->resultCode != 0) {
        WmspError(534, pConfirm->resultCode);
        return;
    }
    
    pConfirm = (WlCmdCfm*)WMSP_WL_MaClearData((u16*)wlBuf, 7);
    if (pConfirm->resultCode != 0) {
        WmspError(260, pConfirm->resultCode);
        return;
    }
    
    if (fCleanQueue) {
        WMSP_CleanSendQueue(0xFFFF);
    }
    
    struct WMCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :172
    cb->apiid = 16;
    cb->errcode = 0;
    WMSP_ReturnResult2Wm9(cb);
}

static void WmspError(u16 wlCommand, u16 wlResult) { // req_EndMP.c:193
    struct WMCallback* cb; // r0 - :195
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = 16;
    cb->errcode = 1;
    cb->wlCmdID = wlCommand;
    cb->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(cb);
}