#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    struct WMCallback* callback; // r0 - :145
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = 31;
    callback->errcode = 1;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_InitWirelessCounter() { // req_InitWirelessCounter.c:36
    u32 wlBuf[128]; // None - :41
    struct WMCallback* cb; // r0 - :42
    WlCmdCfm* pConfirm; // r0 - :43
    
    pConfirm = (WlCmdCfm*)WMSP_WL_DevSetInitializeWirelessCounter((u16*)wlBuf);
    
    if (pConfirm->resultCode != 0) {
        WmspError(0x305, pConfirm->resultCode);
        
    } else {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 31;
        cb->errcode = 0;
        WMSP_ReturnResult2Wm9(cb);
    }
}