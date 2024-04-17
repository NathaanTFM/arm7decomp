#include "Mongoose.h"

void WMSP_PowerOn() { // req_PowerOn.c:47
    struct WMStatus *status = wmspW.status;
    struct WMCallback* cb; // r0 - :51
    
    if (status->state != 1) {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 5;
        cb->errcode = 3;
        WMSP_ReturnResult2Wm9(cb);
        
    } else {
        u16 wlcom; // None - :74
        u16 wlres; // None - :75
        
        if (!WMSPi_CommonWlIdle(&wlcom, &wlres)) {
            cb = WMSP_GetBuffer4Callback2Wm9();
            cb->apiid = 5;
            cb->errcode = 1;
            cb->wlCmdID = wlcom;
            cb->wlResult = wlres;
            WMSP_ReturnResult2Wm9(cb);
            
        } else {
            status->state = 2;
            
            cb = WMSP_GetBuffer4Callback2Wm9();
            cb->apiid = 5;
            cb->errcode = 0;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}

#if 0
// TODO IPA
int WMSPi_CommonWlIdle(u16* pWlCommand, u16* pWlResult) { // req_PowerOn.c:120
    WlCmdCfm* pConfirm; // r0 - :123
    u32 wlBuf[128]; // None - :124
    u16 enableChannel; // r0 - :125
    WlParamGetEnableChannelCfm* pECConfirm; // r0 - :153
    WlDevGetVersionCfm* pVConfirm; // r0 - :203
    WlParamGetMacAddressCfm* pMConfirm; // r0 - :222
}
#endif