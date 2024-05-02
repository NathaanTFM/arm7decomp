#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    WMCallback* callback;
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_END;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_End() { // req_End.c:73
    WMStatus* status = wmspW.status;
    u32 wlBuf[128]; // None - :78
    WMCallback* cb; // r0 - :79
    WlCmdCfm* pConfirm; // r0 - :80
    
    if (status->state != WM_STATE_IDLE) {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_END;
        cb->errcode = WM_ERRCODE_ILLEGAL_STATE;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }
    
    pConfirm = (WlCmdCfm*)WMSP_WL_DevShutdown((u16*)wlBuf);
    if (pConfirm->resultCode != 0) {
        WmspError(DEV_SHUTDOWN_REQ_CMD, pConfirm->resultCode);
        return;
    }
    
    status->state = WM_STATE_STOP;
    PM_SetLEDPattern(PM_LED_PATTERN_ON);
    
    status->state = WM_STATE_READY;
    
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_END;
    cb->errcode = WM_ERRCODE_SUCCESS;
    WMSP_ReturnResult2Wm9(cb);
}