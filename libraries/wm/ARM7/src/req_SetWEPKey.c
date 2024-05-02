#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult) {
    WMCallback* callback;
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_SET_WEPKEY;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_SetWEPKey(void* msg) { // req_SetWEPKey.c:34
    u32* buf = (u32*)msg; // r0 - :36
    WMStatus* status = wmspW.status;
    
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
    
    WMCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :70
    cb->apiid = WM_APIID_SET_WEPKEY;
    cb->errcode = WM_ERRCODE_SUCCESS;
    WMSP_ReturnResult2Wm9(cb);
}

void WMSP_SetWEPKeyEx(void* msg) { // req_SetWEPKey.c:90
    u32* msgbuf = (u32*)msg; // r0 - :92
    WMStatus* status = wmspW.status;
    
    status->wepMode = msgbuf[1];
    
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
        MI_CpuCopy8((void*)msgbuf[2], status->wepKey, sizeof(status->wepKey));
        
    } else {
        MI_CpuFill8(status->wepKey, 0, sizeof(status->wepKey));
    }
    
    status->wepKeyId = msgbuf[3];
    
    WlCmdCfm* pConfirm; // r0 - :127
    u32 wlBuf[128]; // None - :128
    pConfirm = (WlCmdCfm*)WMSP_WL_ParamSetWepKeyId((u16*)wlBuf, status->wepKeyId);
    
    if (pConfirm->resultCode != 0) {
        WmspError(PARAMSET_WEP_KEY_ID_REQ_CMD, pConfirm->resultCode);
    }
    
    WMCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :139
    cb->apiid = WM_APIID_SET_WEPKEY_EX;
    cb->errcode = WM_ERRCODE_SUCCESS;
    WMSP_ReturnResult2Wm9(cb);
}