#include "Mongoose.h"

void WMSP_SetPowerSaveMode(void* msg) { // req_SetPowerSaveMode.c:29
    struct WMStatus* status = wmspW.status;
    u32 wlBuf[128]; // None - :32
    u16* buf = (u16*)wlBuf; // r6 - :33
    struct WMCallback* callback = WMSP_GetBuffer4Callback2Wm9(); // r7 - :36
    
    callback->apiid = 40;
    
    if (status->state != 11) {
        callback->errcode = 3;
        WMSP_ReturnResult2Wm9(callback);
        
    } else {
        WlMlmePowerMgtCfm* p_confirm = WMSP_WL_MlmePowerManagement(buf, ((u32*)msg)[1] == 1, 0, 1); // r0 - :61
        
        if (p_confirm->resultCode != 0) {
            callback->errcode = 1;
            callback->wlCmdID = 1;
            callback->wlResult = p_confirm->resultCode;
            WMSP_ReturnResult2Wm9(callback);
            
        } else {
            WlMaDataCfm* p_confirm; // r0 - :81
            WlTxFrame wlTxFrame; // None - :82
            struct WMStatus* status = wmspW.status; // ???????????????
            
            MI_CpuCopy8(status->parentMacAddress, status->dcf_destAdr, 6);
            status->dcf_sendData = buf;
            status->dcf_sendSize = 0;
            status->dcf_sendFlag = 1;
            
            MIi_CpuClear16(0, &wlTxFrame, 0x30);
            wlTxFrame.frameId = 0;
            wlTxFrame.length = 0;
            wlTxFrame.rate = status->rate == 2 ? 20 : 10;
            MI_CpuCopy8(status->parentMacAddress, wlTxFrame.destAdrs, 6);
            MI_CpuCopy8(status->MacAddress, wlTxFrame.srcAdrs, 6);
            wlTxFrame.datap = (u16*)wlBuf;
            
            p_confirm = WMSP_WL_MaData((u16*)wlBuf, &wlTxFrame);
            
            if (p_confirm->resultCode != 0) {
                callback->errcode = 1;
                callback->wlCmdID = 256;
                callback->wlResult = p_confirm->resultCode;
                WMSP_ReturnResult2Wm9(callback);
                
            } else {
                callback->errcode = 0;
                WMSP_ReturnResult2Wm9(callback);
            }
        }
    }
}