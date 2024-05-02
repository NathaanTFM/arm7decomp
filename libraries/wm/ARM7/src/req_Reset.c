#include "Mongoose.h"

STATIC void WmspError(u16 wlCommand, u16 wlResult);

void WMSP_Reset()
{ // req_Reset.c:36
    WMStatus *status = wmspW.status;
    u32 wlBuf[128]; // None - :41
    u16 *buf = (u16 *)wlBuf;
    WMCallback *cb;     // r0 - :43
    WlCmdCfm *pConfirm; // r0 - :44
    u16 station;        // r4 - :45
    u16 mode;           // r1 - :46
    u16 child_bitmap;   // r0 - :47
    int fparent;        // r0 - :48

    int fCleanQueue = 0;            // r4 - :52
    u32 e = OS_DisableInterrupts(); // r5 - :53

    if (status->mp_flag == 1)
    {
        WMSP_FlushSendQueue(0, 0xFFFF);
        status->mp_flag = 0;
        fCleanQueue = 1;
        WMSP_CancelVAlarm();
        WMSP_SetThreadPriorityLow();

        if (status->state == WM_STATE_MP_CHILD)
        {
            status->state = WM_STATE_CHILD;
        }
        else if (status->state == WM_STATE_MP_PARENT)
        {
            status->state = WM_STATE_PARENT;
        }
    }

    if ((u16)(status->state + 0x10000 - WM_STATE_PARENT) <= 1)
    {
        child_bitmap = status->child_bitmap;
        fparent = (status->state == WM_STATE_PARENT);
    }
    else
    {
        child_bitmap = 0;
    }

    status->child_bitmap = 0;
    status->mp_readyBitmap = 0;
    status->ks_flag = 0;
    status->dcf_flag = 0;
    status->VSyncFlag = 0;
    status->beaconIndicateFlag = 0;
    OS_RestoreInterrupts(e);

    if (fCleanQueue)
    {
        WMSP_CleanSendQueue(0xFFFF);
    }

    // Variable may not be defined!
    if (fparent)
    {
        status->pparam.entryFlag = 0;
    }

    if (child_bitmap != 0)
    {
        int i; // r7 - :110
        for (i = 0; i < 16; i++)
        {
            if ((child_bitmap & (1 << i)) != 0)
            {
                WMSP_IndicateDisconnectionFromMyself(fparent, i, i == 0 ? status->parentMacAddress : status->childMacAddress[i - 1]);
            }
        }
    }

    MI_CpuFill8(status->childMacAddress, 0, 0x5A);

    // get station
    pConfirm = (WlCmdCfm *)WMSP_WL_DevGetStationState(buf);
    if (pConfirm->resultCode != 0)
    {
        WmspError(DEV_GET_STATE_REQ_CMD, pConfirm->resultCode);
        return;
    }

    station = ((WlDevGetStationStateCfm *)pConfirm)->state;

    // get mode
    pConfirm = (WlCmdCfm *)WMSP_WL_ParamGetMode(buf);
    if (pConfirm->resultCode != 0)
    {
        WmspError(PARAMGET_MODE_REQ_CMD, pConfirm->resultCode);
        return;
    }

    mode = ((WlParamGetModeCfm *)pConfirm)->mode;

    switch (station)
    {
    case STA_CLASS2:
    case STA_CLASS3:
        if ((u16)(mode + 0x10000 - 2) <= 1)
        {
            u16 wMac[3]; // None - :165
            MI_CpuCopy8(status->parentMacAddress, wMac, 6);

            long auth_retry; // r5 - :176
            for (auth_retry = 0; auth_retry < 2;)
            {
                if (status->state != WM_STATE_CHILD)
                {
                    break;
                }

                pConfirm = (WlCmdCfm *)WMSP_WL_MlmeDeAuthenticate(buf, wMac, 3);
                switch (pConfirm->resultCode)
                {
                case 0:
                    status->state = WM_STATE_CLASS1;
                    break;

                case 7:
                case 12:
                    auth_retry++;
                    continue;
                }

                break;
            }
        }
        else
        {
            if (mode == 1)
            {
                u16 wMac[3]; // None - :214
                MI_CpuFill8(wMac, 0xFF, 6);

                pConfirm = (WlCmdCfm *)WMSP_WL_MlmeDeAuthenticate(buf, wMac, 3);
                if (pConfirm->resultCode == 0)
                {
                    status->state = WM_STATE_CLASS1;
                }
            }
        }

    case STA_CLASS1:
        pConfirm = (WlCmdCfm *)WMSP_WL_MlmeReset(buf, 1);
        if (pConfirm->resultCode != 0)
        {
            WmspError(MLME_RESET_REQ_CMD, pConfirm->resultCode);
            return;
        }

    case STA_SHUTDOWN:
        pConfirm = (WlCmdCfm *)WMSP_WL_DevIdle(buf);
        if (pConfirm->resultCode != 0)
        {
            WmspError(DEV_IDLE_REQ_CMD, pConfirm->resultCode);
            return;
        }

    case STA_IDLE:
        if (status->preamble == 0)
        {
            WlParamSetCfm *p_confirm; // r0 - :261
            p_confirm = WMSP_WL_ParamSetPreambleType(buf, 1);
            if (p_confirm->resultCode != 0)
            {
                WmspError(PARAMSET_PREAMBLE_TYPE_REQ_CMD, p_confirm->resultCode);
                return;
            }

            status->preamble = 1;
        }

        status->state = WM_STATE_IDLE;
        status->wep_flag = 0;
        WMSP_ResetSizeVars();
        break;

    case STA_IDLE_TEST1:
    case STA_IDLE_TEST2:;

    default:
        WmspError(DEV_GET_STATE_REQ_CMD, 0); // :314
        return;
    }

    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_RESET;
    cb->errcode = WM_ERRCODE_SUCCESS;
    WMSP_ReturnResult2Wm9(cb);
}

STATIC void WmspError(u16 wlCommand, u16 wlResult)
{                   // req_Reset.c:337
    WMCallback *cb; // r0 - :339
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_RESET;
    cb->errcode = WM_ERRCODE_FAILED;
    cb->wlCmdID = wlCommand;
    cb->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(cb);
}