#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    struct WMCallback* callback;
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = 19;
    callback->errcode = 1;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_EndDCF() { // req_EndDCF.c:59
    struct WMStatus* status = wmspW.status;
    u32 e; // r0 - :63
    u32 wlBuf[128]; // None - :65
    WlCmdCfm* pConfirm; // r0 - :66
    
    e = OS_DisableInterrupts();
    if (status->state != 11) {
        OS_RestoreInterrupts(e);
        
        struct WMCallback* callback = WMSP_GetBuffer4Callback2Wm9();; // r0 - :72
        callback->apiid = 19;
        callback->errcode = 3;
        WMSP_ReturnResult2Wm9(callback);
        
    } else {
        status->dcf_flag = 0;
        status->state = 8;
        OS_RestoreInterrupts(e);
        
        pConfirm = (WlCmdCfm*)WMSP_WL_MaClearData((u16*)wlBuf, 7);
        if (pConfirm->resultCode != 0) {
            WmspError(0x104, pConfirm->resultCode);
            
        } else {
            struct WMCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :103
            cb->apiid = 19;
            cb->errcode = 0;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}