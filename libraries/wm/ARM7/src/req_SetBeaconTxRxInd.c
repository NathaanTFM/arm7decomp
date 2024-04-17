#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    struct WMCallback* callback; // r0 - :145
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = 25;
    callback->errcode = 1;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_SetBeaconTxRxInd(void* msg) { // req_SetBeaconTxRxInd.c:37
    u32 wlBuf[128]; // None - :41
    WlParamSetCfm* pConfirm; // r0 - :44
    struct WMCallback* cb; // r0 - :45
    
    // TODO: msg seems to be WlParamSetBeaconSendRecvIndReq+0x0C
    pConfirm = (WlParamSetCfm*)WMSP_WL_ParamSetBeaconSendRecvInd((u16*)wlBuf, ((u32*)msg)[1]);
    
    if (pConfirm->resultCode != 0) {
        WmspError(0x215, pConfirm->resultCode);
        
    } else {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 25;
        cb->errcode = 0;
        WMSP_ReturnResult2Wm9(cb);
    }
}