#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult);

void WMSP_EndMP(void *msg) { // req_EndMP.c:94
    u32 wlBuf[128]; // None - :100
    WlCmdCfm* pConfirm; // r0 - :101
    int fCleanQueue = 0; // r5 - :103
    WMStatus* status = wmspW.status;
    
    if (wmspW.status->state != WM_STATE_MP_PARENT && wmspW.status->state != WM_STATE_MP_CHILD) {
        WMCallback* callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :108
        callback->apiid = WM_APIID_END_MP;
        callback->errcode = WM_ERRCODE_ILLEGAL_STATE;
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
    
    if (status->state == WM_STATE_MP_CHILD) {
        status->state = WM_STATE_CHILD;
        
    } else if (status->state == WM_STATE_MP_PARENT) {
        status->state = WM_STATE_PARENT;
    }
    
    OS_RestoreInterrupts(e);
    
    pConfirm = (WlCmdCfm*)WMSP_WL_ParamSetNullKeyResponseMode((u16*)wlBuf, 0);
    if (pConfirm->resultCode != 0) {
        WmspError(PARAMSET_NULL_KEY_MODE_REQ_CMD, pConfirm->resultCode);
        return;
    }
    
    pConfirm = (WlCmdCfm*)WMSP_WL_MaClearData((u16*)wlBuf, 7);
    if (pConfirm->resultCode != 0) {
        WmspError(MA_CLR_DATA_REQ_CMD, pConfirm->resultCode);
        return;
    }
    
    if (fCleanQueue) {
        WMSP_CleanSendQueue(0xFFFF);
    }
    
    WMCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :172
    cb->apiid = WM_APIID_END_MP;
    cb->errcode = WM_ERRCODE_SUCCESS;
    WMSP_ReturnResult2Wm9(cb);
}

static void WmspError(u16 wlCommand, u16 wlResult) { // req_EndMP.c:193
    WMCallback* cb; // r0 - :195
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_END_MP;
    cb->errcode = WM_ERRCODE_FAILED;
    cb->wlCmdID = wlCommand;
    cb->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(cb);
}