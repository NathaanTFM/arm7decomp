#include "Mongoose.h"

STATIC void WmspError(u16 wlCommand, u16 wlResult);

void WMSP_StartParent(void *msg)
{                                    // req_StartParent.c:51
    WMStatus *status = wmspW.status; // r7 - :53
    u32 wlBuf[128];                  // None - :54
    u16 *buf = (u16 *)wlBuf;         // r11 - :55
    WMStartParentCallback *cb;       // r0 - :56
    WlCmdCfm *pConfirm;              // r0 - :57
    u16 ssidLength;                  // r0 - :58
    int powerSave;                   // r8 - :59

    if (status->state != WM_STATE_IDLE || (status->miscFlags & 1) != 0)
    {
        WMStartParentCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :64
        callback->apiid = WM_APIID_START_PARENT;
        callback->errcode = WM_ERRCODE_ILLEGAL_STATE;
        callback->state = WM_STATECODE_PARENT_START;
        WMSP_ReturnResult2Wm9(callback);
        return;
    }

    powerSave = ((u32 *)msg)[1];

    if ((status->allowedChannel & (1 << status->pparam.channel >> 1)) == 0)
    {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_START_PARENT;
        cb->errcode = WM_ERRCODE_INVALID_PARAM;
        cb->state = WM_STATECODE_PARENT_START;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }

    status->mode = MODE_PARENT;
    status->aid = 0;

    u32 e = OS_DisableInterrupts(); // r0 - :94
    status->child_bitmap = 0;
    status->mp_readyBitmap = 0;
    OS_RestoreInterrupts(e);
    status->preamble = 1;

    if (!WMSP_SetAllParams(8, buf))
    {
        return;
    }

    pConfirm = (WlCmdCfm *)WMSP_WL_DevClass1(buf);

    if (pConfirm->resultCode != 0)
    {
        WmspError(DEV_CLASS1_REQ_CMD, pConfirm->resultCode);
        return;
    }

    u16 pwrMgtMode; // r8 - :117
    pwrMgtMode = powerSave == 0 ? 0 : 1;

    pConfirm = (WlCmdCfm *)WMSP_WL_MlmePowerManagement(buf, pwrMgtMode, 0, 1);

    if (pConfirm->resultCode != 0)
    {
        WmspError(MLME_PWR_MGT_REQ_CMD, pConfirm->resultCode);
        return;
    }

    status->pwrMgtMode = pwrMgtMode;

    u16 ssid[16];                            // None - :136
    WMParentParam *pparam = &status->pparam; // r9 - :139
    WMGameInfo GameInfo;                     // None - :140

    MIi_CpuClear16(0, &GameInfo, sizeof(GameInfo));
    WMSP_CopyParentParam(&GameInfo, pparam);
    MIi_CpuClear16(0, &ssid, sizeof(ssid));
    ssidLength = 0x20;

    ssid[0] = pparam->ggid;
    ssid[1] = pparam->ggid >> 16;
    ssid[2] = pparam->tgid;
    ssid[3] = 0;

    pConfirm = (WlCmdCfm *)WMSP_WL_MlmeStart(buf, ssidLength, (u8 *)ssid, pparam->beaconPeriod, 2, pparam->channel, 3, 3, pparam->userGameInfoLength + 16, &GameInfo);

    if (pConfirm->resultCode != 0)
    {
        WmspError(MLME_START_REQ_CMD, pConfirm->resultCode);
        return;
    }

    WMSP_SetParentMaxSize(pparam->parentMaxSize + (pparam->KS_Flag ? 42 : 0));
    WMSP_SetChildMaxSize(pparam->childMaxSize + (pparam->KS_Flag ? 6 : 0));

    cb = WMSP_GetBuffer4Callback2Wm9();
    status->state = WM_STATE_PARENT;

    cb->apiid = WM_APIID_START_PARENT;
    cb->errcode = WM_ERRCODE_SUCCESS;
    cb->state = WM_STATECODE_PARENT_START;
    cb->parentSize = status->mp_parentSize;
    cb->childSize = status->mp_childSize;
    WMSP_ReturnResult2Wm9(cb);

    status->beaconIndicateFlag = 1;
}

STATIC void WmspError(u16 wlCommand, u16 wlResult)
{                              // req_StartParent.c:227
    WMStartParentCallback *cb; // r0 - :229
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_START_PARENT;
    cb->errcode = WM_ERRCODE_FAILED;
    cb->state = WM_STATECODE_PARENT_START;
    cb->wlCmdID = wlCommand;
    cb->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(cb);
}