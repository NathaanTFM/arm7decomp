#include "Mongoose.h"

void WMSP_AutoDeAuth(void* msg) { // req_AutoDeAuth.c:42
    u32 wlBuf[128]; // None - :44
    u16 wMac[3]; // None - :46
    WlCmdCfm* pConfirm; // r4 - :47
    long auth_retry; // r8 - :60
    
    MI_CpuCopy8((char *)msg + 4, wMac, sizeof(wMac));
    
    auth_retry = 0;
    while (auth_retry < 2) {
        pConfirm = (WlCmdCfm *)WMSP_WL_MlmeDeAuthenticate((u16 *)wlBuf, wMac, 0x13);
        if (pConfirm->resultCode == 0 || (pConfirm->resultCode != 7 && pConfirm->resultCode != 12)) {
            break;
        }
        auth_retry++;
    }
    
    struct WMCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :86
    cb->apiid = 34;
    if (pConfirm->resultCode == 0) {
        cb->errcode = 0;
    } else {
        cb->errcode = 1;
        cb->wlCmdID = 5;
        cb->wlResult = pConfirm->resultCode;
    }
    WMSP_ReturnResult2Wm9(cb);
    
}