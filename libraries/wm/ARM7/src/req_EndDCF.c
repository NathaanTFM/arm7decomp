#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    WMCallback* callback;
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_END_DCF;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_EndDCF() { // req_EndDCF.c:59
    WMStatus* status = wmspW.status;
    u32 e; // r0 - :63
    u32 wlBuf[128]; // None - :65
    WlCmdCfm* pConfirm; // r0 - :66
    
    e = OS_DisableInterrupts();
    if (status->state != WM_STATE_DCF_CHILD) {
        OS_RestoreInterrupts(e);
        
        WMCallback* callback = WMSP_GetBuffer4Callback2Wm9();; // r0 - :72
        callback->apiid = WM_APIID_END_DCF;
        callback->errcode = WM_ERRCODE_ILLEGAL_STATE;
        WMSP_ReturnResult2Wm9(callback);
        
    } else {
        status->dcf_flag = 0;
        status->state = WM_STATE_CHILD;
        OS_RestoreInterrupts(e);
        
        pConfirm = (WlCmdCfm*)WMSP_WL_MaClearData((u16*)wlBuf, 7);
        if (pConfirm->resultCode != 0) {
            WmspError(MA_CLR_DATA_REQ_CMD, pConfirm->resultCode);
            
        } else {
            WMCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :103
            cb->apiid = WM_APIID_END_DCF;
            cb->errcode = WM_ERRCODE_SUCCESS;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}