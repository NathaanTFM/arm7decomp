#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    struct WMCallback* callback;
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = 2;
    callback->errcode = 1;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_End() { // req_End.c:73
    struct WMStatus* status = wmspW.status;
    u32 wlBuf[128]; // None - :78
    struct WMCallback* cb; // r0 - :79
    WlCmdCfm* pConfirm; // r0 - :80
    
    if (status->state != 2) {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 2;
        cb->errcode = 3;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }
    
    pConfirm = (WlCmdCfm*)WMSP_WL_DevShutdown((u16*)wlBuf);
    if (pConfirm->resultCode != 0) {
        WmspError(0x301, pConfirm->resultCode);
        return;
    }
    
    status->state = 1;
    PM_SetLEDPattern(PM_LED_PATTERN_ON);
    
    status->state = 0;
    
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = 2;
    cb->errcode = 0;
    WMSP_ReturnResult2Wm9(cb);
}