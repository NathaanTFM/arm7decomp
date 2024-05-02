#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult)
{
    WMCallback *callback; // r0 - :145
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_GET_W_COUNTER;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}

void WMSP_GetWirelessCounter()
{                                     // req_GetWirelessCounter.c:36
    u32 wlBuf[128];                   // None - :41
    WMGetWirelessCounterCallback *cb; // r0 - :42
    WlDevGetInfoCfm *pConfirm;        // r0 - :43

    pConfirm = (WlDevGetInfoCfm *)WMSP_WL_DevGetWirelessCounter((u16 *)wlBuf);
    if (pConfirm->resultCode != 0)
    {
        WmspError(DEV_GET_WL_INFO_REQ_CMD, pConfirm->resultCode);
    }
    else
    {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_GET_W_COUNTER;
        cb->errcode = WM_ERRCODE_SUCCESS;
        MIi_CpuCopy16(&pConfirm->counter, &cb->TX_Success, sizeof(pConfirm->counter));
        WMSP_ReturnResult2Wm9(cb);
    }
}