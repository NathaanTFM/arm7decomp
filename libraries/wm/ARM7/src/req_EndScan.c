#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult);

void WMSP_EndScan() { // req_EndScan.c:74
    struct WMStatus* status = wmspW.status;
    u32 wlBuf[128]; // None - :79
    u16* buf = (u16*)wlBuf; // r0 - :80
    
    if (status->state != 5) {
        struct WMCallback* callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :87
        callback->apiid = 11;
        callback->errcode = 3;
        WMSP_ReturnResult2Wm9(callback);
        return;   
    }
    
    WlDevIdleCfm* p_confirm = WMSP_WL_DevIdle(buf); // r0 - :95
    if (p_confirm->resultCode != 0) {
        WmspError(770, p_confirm->resultCode);
        return;
    }
    
    status->state = 2;
    if (status->preamble == 0) {
        WlParamSetCfm* p_confirm = WMSP_WL_ParamSetPreambleType(buf, 1); // r0 - :109
        if (p_confirm->resultCode != 0) {
            WmspError(526, p_confirm->resultCode);
            return;
        }
        
        status->preamble = 1;    
    }
    
    struct WMCallback* callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :122
    callback->apiid = 11;
    callback->errcode = 0;
    WMSP_ReturnResult2Wm9(callback);
}

static void WmspError(u16 wlCommand, u16 wlResult) { // req_EndScan.c:143
    struct WMCallback* callback; // r0 - :145
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = 11;
    callback->errcode = 1;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}
