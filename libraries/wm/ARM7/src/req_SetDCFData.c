#include "Mongoose.h"

void WMSP_SetDCFData(void* msg) { // req_SetDCFData.c:28
    u32* buf = (u32*)msg; // r0 - :30
    struct WMStatus* status = wmspW.status;
    u32 wlBuf[128]; // None - :33
    WlTxFrame wlTxFrame; // None - :34
    WlMaDataCfm* wlResult; // r4 - :35
    
    MI_CpuCopy8(&buf[1], status->dcf_destAdr, 6);
    status->dcf_sendData = (u16*)buf[3];
    status->dcf_sendSize = buf[4];
    status->dcf_sendFlag = 1;
    
    MIi_CpuClear16(0, &wlTxFrame, 0x30);
    wlTxFrame.frameId = 0;
    wlTxFrame.length = buf[4];
    wlTxFrame.rate = status->rate == 2 ? 20 : 10;
    
    MI_CpuCopy8(&buf[1], wlTxFrame.destAdrs, 6);
    MI_CpuCopy8(status->MacAddress, wlTxFrame.srcAdrs, 6);
    wlTxFrame.datap = (u16*)buf[3];
    
    wlResult = WMSP_WL_MaData((u16*)wlBuf, &wlTxFrame);
    
    struct WMCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :60
    cb->apiid = 18;
    cb->errcode = wlResult->resultCode != 0;
    if (wlResult->resultCode != 0) {
        cb->wlCmdID = 256;
        cb->wlResult = wlResult->resultCode;
    }
    WMSP_GetBuffer4Callback2Wm9(cb);
}