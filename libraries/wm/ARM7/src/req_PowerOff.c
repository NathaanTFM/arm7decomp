#include "Mongoose.h"

void WMSP_PowerOff()
{ // req_PowerOff.c:32
    WMStatus *status = wmspW.status;
    u32 wlBuf[128];     // None - :37
    WMCallback *cb;     // r0 - :38
    WlCmdCfm *pConfirm; // r0 - :39

    if (status->state != WM_STATE_IDLE)
    {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_POWER_OFF;
        cb->errcode = WM_ERRCODE_ILLEGAL_STATE;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }

    pConfirm = (WlCmdCfm *)WMSP_WL_DevShutdown((u16 *)wlBuf);
    if (pConfirm->resultCode != 0)
    {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_POWER_OFF;
        cb->errcode = WM_ERRCODE_FAILED;
        cb->wlCmdID = 0x301;
        cb->wlResult = pConfirm->resultCode;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }

#ifdef TWL_MODE
    if (OS_IsRunOnTwl() == 1)
        EXI2i_SetBitVibration(0);
#endif

    status->state = WM_STATE_STOP;
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_POWER_OFF;
    cb->errcode = WM_ERRCODE_SUCCESS;
    WMSP_ReturnResult2Wm9(cb);
}