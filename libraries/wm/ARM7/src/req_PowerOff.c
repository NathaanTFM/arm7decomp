#include "Mongoose.h"

void WMSP_PowerOff() { // req_PowerOff.c:32
    struct WMStatus* status = wmspW.status;
    u32 wlBuf[128]; // None - :37
    struct WMCallback* cb; // r0 - :38
    WlCmdCfm* pConfirm; // r0 - :39
    
    if (status->state != 2) {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 6;
        cb->errcode = 3;
        WMSP_ReturnResult2Wm9(cb);
        
    } else {
        pConfirm = (WlCmdCfm*)WMSP_WL_DevShutdown((u16*)wlBuf);
        if (pConfirm->resultCode != 0) {
            cb = WMSP_GetBuffer4Callback2Wm9();
            cb->apiid = 6;
            cb->errcode = 1;
            cb->wlCmdID = 0x301;
            cb->wlResult = pConfirm->resultCode;
            WMSP_ReturnResult2Wm9(cb);
            
        } else {
            status->state = 1;
            cb = WMSP_GetBuffer4Callback2Wm9();
            cb->apiid = 6;
            cb->errcode = 0;
            WMSP_ReturnResult2Wm9(cb);
        }
        
    }
}