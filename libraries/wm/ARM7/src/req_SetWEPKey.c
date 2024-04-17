#include "Mongoose.h"

void WMSP_SetWEPKey(void* msg) { // req_SetWEPKey.c:34
    u32* buf = (u32*)msg; // r0 - :36
    struct WMStatus* status = wmspW.status;
    
    status->wepMode = buf[1];
    
    switch (status->wepMode) {
        case 0:
            status->wep_flag = 0;
            break;
            
        case 1:
        case 2:
        case 3:
            status->wep_flag = 1;
            break;
            
        default:
            status->wep_flag = 0;
    }
    
    if (status->wep_flag == 1) {
        MI_CpuCopy8((void*)buf[2], status->wepKey, sizeof(status->wepKey));
        
    } else {
        MI_CpuFill8(status->wepKey, 0, sizeof(status->wepKey));
    }
    
    struct WMCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :70
    cb->apiid = 20;
    cb->errcode = 0;
    WMSP_ReturnResult2Wm9(cb);
}

void WMSP_SetWEPKeyEx(void* msg) { // req_SetWEPKey.c:90
    u32* msgbuf; // r0 - :92
    WlCmdCfm* pConfirm; // r0 - :127
    u32 wlBuf[128]; // None - :128
    struct WMCallback* cb; // r0 - :139
}