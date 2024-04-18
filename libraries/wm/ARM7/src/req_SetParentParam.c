#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    struct WMCallback* callback;
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = 7;
    callback->errcode = 1;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_SetParentParam(void* msg) { // req_SetParentParam.c:35
    struct WMStatus* status = wmspW.status;
    u32* buf = (u32*)msg;
    u32 wlBuf[128]; // None - :39
    struct WMCallback* cb; // r0 - :40
    WlCmdCfm* pConfirm; // r0 - :42
    
    MI_CpuCopy8((void*)buf[1], &status->pparam, 0x40);
    
    if (((1 << status->pparam.channel) & status->enableChannel) == 0) {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 7;
        cb->errcode = 6;
        WMSP_ReturnResult2Wm9(cb);
        
    } else {
        pConfirm = (WlCmdCfm*)WMSP_WL_ParamSetMaxConnectableChild((u16*)wlBuf, status->pparam.maxEntry);
        
        if (pConfirm->resultCode != 0) {
            WmspError(0x212, pConfirm->resultCode);
            
        } else {
            cb = WMSP_GetBuffer4Callback2Wm9();
            cb->apiid = 7;
            cb->errcode = 0;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}