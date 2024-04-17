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
    u32 wlBuf[128]; // None - :78
    struct WMCallback* cb; // r0 - :79
    WlCmdCfm* pConfirm; // r0 - :80
    
    if (wmspW.status->state != 2) {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 2;
        cb->errcode = 3;
        WMSP_ReturnResult2Wm9(cb);
        
    } else {
        pConfirm = (WlCmdCfm*)WMSP_WL_DevShutdown((u16*)wlBuf);
        if (pConfirm->resultCode != 0) {
            WmspError(0x301, pConfirm->resultCode);
            
        } else {
            wmspW.status->state = 1;
            cb = WMSP_GetBuffer4Callback2Wm9();
            cb->apiid = 2;
            cb->errcode = 3;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}