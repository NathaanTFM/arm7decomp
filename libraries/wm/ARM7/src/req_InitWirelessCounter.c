#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    WMCallback* callback; // r0 - :145
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_INIT_W_COUNTER;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_InitWirelessCounter() { // req_InitWirelessCounter.c:36
    u32 wlBuf[128]; // None - :41
    WMCallback* cb; // r0 - :42
    WlCmdCfm* pConfirm; // r0 - :43
    
    pConfirm = (WlCmdCfm*)WMSP_WL_DevSetInitializeWirelessCounter((u16*)wlBuf);
    
    if (pConfirm->resultCode != 0) {
        WmspError(DEV_CLEAR_WL_INFO_REQ_CMD, pConfirm->resultCode);
        
    } else {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_INIT_W_COUNTER;
        cb->errcode = WM_ERRCODE_SUCCESS;
        WMSP_ReturnResult2Wm9(cb);
    }
}