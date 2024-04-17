#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    struct WMCallback* callback; // r0 - :145
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = 36;
    callback->errcode = 1;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_SetBeaconPeriod(void* msg) { // req_SetBeaconPeriod.c:36
    u32 wlBuf[128]; // None - :40
    WlParamSetCfm* pConfirm; // r0 - :42
    struct WMCallback* cb; // r0 - :59
    
    pConfirm = (WlParamSetCfm*)WMSP_WL_ParamSetBeaconPeriod((u16*)wlBuf, ((u32*)msg)[1]);
    
    if (pConfirm->resultCode != 0) {
        WmspError(0x242, pConfirm->resultCode);
        
    } else {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 36;
        cb->errcode = 0;
        WMSP_ReturnResult2Wm9(cb);
    }
}