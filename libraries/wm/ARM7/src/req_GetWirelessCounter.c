#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    struct WMCallback* callback; // r0 - :145
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = 32;
    callback->errcode = 1;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_GetWirelessCounter() { // req_GetWirelessCounter.c:36
    u32 wlBuf[128]; // None - :41
    struct WMGetWirelessCounterCallback* cb; // r0 - :42
    WlDevGetInfoCfm* pConfirm; // r0 - :43
    
    pConfirm = (WlDevGetInfoCfm*)WMSP_WL_DevGetWirelessCounter((u16*)wlBuf);
    if (pConfirm->resultCode != 0) {
        WmspError(0x307, pConfirm->resultCode);
        
    } else {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 32;
        cb->errcode = 0;
        MIi_CpuCopy16(&pConfirm->counter, &cb->TX_Success, sizeof(pConfirm->counter));
        WMSP_ReturnResult2Wm9(cb);
    }
}