#include "Mongoose.h"

STATIC void WmspError(u16 wlCommand, u16 wlResult);

void WMSP_EndParent()
{ // req_EndParent.c:131
    WMStatus *status = wmspW.status;
    u32 wlBuf[128];     // None - :136
    WMCallback *cb;     // r0 - :137
    WlCmdCfm *pConfirm; // r0 - :138

    if (wmspW.status->state != WM_STATE_PARENT)
    {
        WMCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :145
        callback->apiid = WM_APIID_END_PARENT;
        callback->errcode = WM_ERRCODE_ILLEGAL_STATE;
        WMSP_ReturnResult2Wm9(callback);
        return;
    }

    u16 wMac[3]; // None - :157
    long i;      // r8 - :158
    u32 e;       // r0 - :159

    status->pparam.entryFlag = 0;

    for (i = 1; i < 16; i++)
    {
        if ((status->child_bitmap & (1 << i)) != 0)
        {
            MI_CpuCopy8(status->childMacAddress[i - 1], wMac, sizeof(wMac));

            long auth_retry; // r9 - :175
            for (auth_retry = 0; auth_retry < 2; auth_retry++)
            {
                pConfirm = (WlCmdCfm *)WMSP_WL_MlmeDeAuthenticate((u16 *)wlBuf, wMac, 3);
                if (pConfirm->resultCode == 0 || (pConfirm->resultCode != 7 && pConfirm->resultCode != 12))
                {
                    break;
                }
            }

            e = OS_DisableInterrupts();

            if ((status->child_bitmap & (1 << i)) != 0)
            {
                status->child_bitmap &= ~(1 << i);
                status->mp_readyBitmap &= ~(1 << i);
                status->mp_lastRecvTick[i] = 0;
                OS_RestoreInterrupts(e);
                WMSP_IndicateDisconnectionFromMyself(1, i, wMac);
            }
            else
            {
                OS_RestoreInterrupts(e);
            }
        }
    }

    pConfirm = (WlCmdCfm *)WMSP_WL_MlmeReset((u16 *)wlBuf, 1);

    if (pConfirm->resultCode != 0)
    {
        WmspError(MLME_RESET_REQ_CMD, pConfirm->resultCode);
        return;
    }

    status->beaconIndicateFlag = 0;
    status->state = WM_STATE_CLASS1;

    pConfirm = (WlCmdCfm *)WMSP_WL_DevIdle((u16 *)wlBuf);

    if (pConfirm->resultCode != 0)
    {
        WmspError(DEV_IDLE_REQ_CMD, pConfirm->resultCode);
        return;
    }

    status->state = WM_STATE_IDLE;
    status->wep_flag = 0;
    status->wepMode = 0;
    MI_CpuFill8(status->wepKey, 0, sizeof(status->wepKey));

    WMSP_ResetSizeVars();

    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_END_PARENT;
    cb->errcode = WM_ERRCODE_SUCCESS;
    WMSP_ReturnResult2Wm9(cb);
}

STATIC void WmspError(u16 wlCommand, u16 wlResult)
{                   // req_EndParent.c:269
    WMCallback *cb; // r0 - :271
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_END_PARENT;
    cb->errcode = WM_ERRCODE_FAILED;
    cb->wlCmdID = wlCommand;
    cb->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(cb);
}