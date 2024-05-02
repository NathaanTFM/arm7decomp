#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    WMCallback* callback;
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_SET_P_PARAM;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_SetParentParam(void* msg) { // req_SetParentParam.c:35
    WMStatus* status = wmspW.status;
    u32* buf = (u32*)msg;
    u32 wlBuf[128]; // None - :39
    WMCallback* cb; // r0 - :40
    WlCmdCfm* pConfirm; // r0 - :42
    
    MI_CpuCopy8((void*)buf[1], &status->pparam, 0x40);
    
    if (((1 << status->pparam.channel) & status->enableChannel) == 0) {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_SET_P_PARAM;
        cb->errcode = WM_ERRCODE_INVALID_PARAM;
        WMSP_ReturnResult2Wm9(cb);
        
    } else {
        pConfirm = (WlCmdCfm*)WMSP_WL_ParamSetMaxConnectableChild((u16*)wlBuf, status->pparam.maxEntry);
        
        if (pConfirm->resultCode != 0) {
            WmspError(PARAMSET_MAX_CONN_REQ_CMD, pConfirm->resultCode);
            
        } else {
            cb = WMSP_GetBuffer4Callback2Wm9();
            cb->apiid = WM_APIID_SET_P_PARAM;
            cb->errcode = WM_ERRCODE_SUCCESS;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}