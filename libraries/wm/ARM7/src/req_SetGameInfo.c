#include "Mongoose.h"

void WMSP_SetGameInfo(void* msg) { // req_SetGameInfo.c:28
    u32* buf = (u32*)msg; // r0 - :30
    struct WMSPWork* sys = &wmspW; // r0 - :31
    struct WMStatus* status = sys->status; // r5 - :32
    u8 attribute; // r0 - :33
    u16 length; // r0 - :34
    u32 wlBuf[128]; // None - :36
    WlParamSetCfm* wlResult; // r4 - :37
    
    status->pparam.userGameInfo = (u16*)buf[1];
    status->pparam.userGameInfoLength = buf[2];
    status->pparam.ggid = buf[3];
    status->pparam.tgid = buf[4];
    
    attribute = buf[5];
    status->pparam.entryFlag = (attribute & 1) != 0;
    status->pparam.multiBootFlag = (attribute & 2) != 0;
    status->pparam.KS_Flag = (attribute & 4) != 0;
    status->pparam.CS_Flag = (attribute & 8) != 0;
    
    struct WMGameInfo GameInfo; // None - :54
    WMSP_CopyParentParam(&GameInfo, &status->pparam);
    
    length = status->pparam.userGameInfoLength + 16;
    wlResult = WMSP_WL_ParamSetGameInfo((u16*)wlBuf, length, &GameInfo.magicNumber);
    
    struct WMCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :63
    cb->apiid = 24;
    cb->errcode = wlResult->resultCode != 0 ? 1 : 0;
    if (wlResult->resultCode != 0) {
        cb->wlCmdID = 581;
        cb->wlResult = wlResult->resultCode;
    }
    WMSP_ReturnResult2Wm9(cb);
}