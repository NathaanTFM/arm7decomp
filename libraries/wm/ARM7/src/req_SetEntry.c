#include "Mongoose.h"

void WMSP_SetEntry(void *msg)
{                                    // req_SetEntry.c:28
    u32 *buf = (u32 *)msg;           // r0 - :30
    WMStatus *status = wmspW.status; // r5 - :31
    WlParamSetCfm *wlResult;         // r0 - :32
    u32 wlBuf[128];                  // None - :33
    WMGameInfo GameInfo;             // None - :40
    u16 length;                      // r0 - :41

    status->pparam.entryFlag = buf[1];
    WMSP_CopyParentParam(&GameInfo, &status->pparam);
    wlResult = WMSP_WL_ParamSetGameInfo((u16 *)wlBuf, status->pparam.userGameInfoLength + 16, (u16 *)&GameInfo);

    WMCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :50
    cb->apiid = WM_APIID_SET_ENTRY;

    if (wlResult->resultCode == 0)
    {
        cb->errcode = WM_ERRCODE_SUCCESS;
    }
    else
    {
        cb->errcode = WM_ERRCODE_FAILED;
        cb->wlCmdID = 0x245;
        cb->wlResult = wlResult->resultCode;
    }

    WMSP_ReturnResult2Wm9(cb);
}