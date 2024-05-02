#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    WMCallback* callback; // r0 - :145
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_SET_BEACON_IND;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_SetBeaconTxRxInd(void* msg) { // req_SetBeaconTxRxInd.c:37
    u32 wlBuf[128]; // None - :41
    WlParamSetCfm* pConfirm; // r0 - :44
    WMCallback* cb; // r0 - :45
    
    // TODO: msg seems to be WlParamSetBeaconSendRecvIndReq+0x0C
    pConfirm = (WlParamSetCfm*)WMSP_WL_ParamSetBeaconSendRecvInd((u16*)wlBuf, ((u32*)msg)[1]);
    
    if (pConfirm->resultCode != 0) {
        WmspError(PARAMSET_BCN_SEND_RECV_IND_REQ_CMD, pConfirm->resultCode);
        
    } else {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_SET_BEACON_IND;
        cb->errcode = WM_ERRCODE_SUCCESS;
        WMSP_ReturnResult2Wm9(cb);
    }
}