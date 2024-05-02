#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult);

void WMSP_EndScan()
{ // req_EndScan.c:74
    WMStatus *status = wmspW.status;
    u32 wlBuf[128];          // None - :79
    u16 *buf = (u16 *)wlBuf; // r0 - :80

    if (status->state != WM_STATE_SCAN)
    {
        WMCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :87
        callback->apiid = WM_APIID_END_SCAN;
        callback->errcode = WM_ERRCODE_ILLEGAL_STATE;
        WMSP_ReturnResult2Wm9(callback);
        return;
    }

    WlDevIdleCfm *p_confirm = WMSP_WL_DevIdle(buf); // r0 - :95
    if (p_confirm->resultCode != 0)
    {
        WmspError(DEV_IDLE_REQ_CMD, p_confirm->resultCode);
        return;
    }

    status->state = WM_STATE_IDLE;
    if (status->preamble == 0)
    {
        WlParamSetCfm *p_confirm = WMSP_WL_ParamSetPreambleType(buf, 1); // r0 - :109
        if (p_confirm->resultCode != 0)
        {
            WmspError(PARAMSET_PREAMBLE_TYPE_REQ_CMD, p_confirm->resultCode);
            return;
        }

        status->preamble = 1;
    }

    WMCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :122
    callback->apiid = WM_APIID_END_SCAN;
    callback->errcode = WM_ERRCODE_SUCCESS;
    WMSP_ReturnResult2Wm9(callback);
}

static void WmspError(u16 wlCommand, u16 wlResult)
{                         // req_EndScan.c:143
    WMCallback *callback; // r0 - :145
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_END_SCAN;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}
