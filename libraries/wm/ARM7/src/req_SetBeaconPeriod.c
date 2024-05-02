#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult)
{
    WMCallback *callback; // r0 - :145
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_SET_BEACON_PERIOD;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_SetBeaconPeriod(void *msg)
{                            // req_SetBeaconPeriod.c:36
    u32 wlBuf[128];          // None - :40
    WlParamSetCfm *pConfirm; // r0 - :42
    WMCallback *cb;          // r0 - :59

    pConfirm = (WlParamSetCfm *)WMSP_WL_ParamSetBeaconPeriod((u16 *)wlBuf, ((u32 *)msg)[1]);

    if (pConfirm->resultCode != 0)
    {
        WmspError(PARAMSET2_BEACON_PERIOD_REQ_CMD, pConfirm->resultCode);
    }
    else
    {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_SET_BEACON_PERIOD;
        cb->errcode = WM_ERRCODE_SUCCESS;
        WMSP_ReturnResult2Wm9(cb);
    }
}