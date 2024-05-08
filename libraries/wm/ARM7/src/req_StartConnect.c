#include "Mongoose.h"

STATIC void WmspError(u16 wlCommand, u16 wlResult, u16 wlStatus);

void WMSP_StartConnectEx(void *msg)
{                   // req_StartConnect.c:48
    u32 wlBuf[128]; // None - :51
    u16 *buf = (u16 *)wlBuf;
    WMStatus *status = wmspW.status;
    WMArm7Buf *p = wmspW.wm7buf;                        // r8 - :54
    WMStartConnectReq *args = (WMStartConnectReq *)msg; // r0 - :58
    WlMlmeAssociateCfm *assConfirm;                     // r9 - :59

    if (wmspW.status->state != WM_STATE_IDLE || (wmspW.status->miscFlags & 1) != 0)
    {
        WMStartConnectCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :64
        callback->apiid = WM_APIID_START_CONNECT;
        callback->errcode = WM_ERRCODE_ILLEGAL_STATE;
        callback->state = WM_STATECODE_CONNECT_START;
        WMSP_ReturnResult2Wm9(callback);
        return;
    }

    MI_CpuCopy8(args->pInfo, &p->connectPInfo, sizeof(p->connectPInfo));
    if (p->connectPInfo.gameInfoLength >= 16 && (p->connectPInfo.gameInfo.attribute & 1) == 0)
    {
        WMStartConnectCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :88
        cb->apiid = WM_APIID_START_CONNECT;
        cb->errcode = WM_ERRCODE_NO_ENTRY;
        cb->state = WM_STATECODE_CONNECT_START;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }

    int tmp = (1 << p->connectPInfo.channel); // not defined anywhere!
    if ((tmp & status->enableChannel) == 0 || ((tmp >> 1) & 0x1FFF) == 0)
    {
        WMStartConnectCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :107
        cb->apiid = WM_APIID_START_CONNECT;
        cb->errcode = WM_ERRCODE_INVALID_PARAM;
        cb->state = WM_STATECODE_CONNECT_START;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }

    {
        WMStartConnectCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :120
        callback->apiid = WM_APIID_START_CONNECT;
        callback->errcode = WM_ERRCODE_SUCCESS;
        callback->state = WM_STATECODE_CONNECT_START;
        WMSP_ReturnResult2Wm9(callback);
    }

    if (status->rate == 1)
    {
        if (p->connectPInfo.rateSet.basic & 1)
            status->rate = 1;
        else
            status->rate = 2;
    }
    else if ((p->connectPInfo.rateSet.basic & 2) != 0)
    {
        status->rate = 2;
    }
    else
    {
        status->rate = 1;
    }

    if ((p->connectPInfo.capaInfo & 0x20) != 0)
    {
        status->preamble = 1;
    }
    else
    {
        status->preamble = 0;
    }

    if (p->connectPInfo.gameInfoLength == 0)
    {
        status->mode = MODE_HOTSPOT;
    }
    else
    {
        status->mode = MODE_CHILD;
    }

    if (!WMSP_SetAllParams(0xC, buf))
    {
        return;
    }

    {
        WlCmdCfm *p_confirm = (WlCmdCfm *)WMSP_WL_ParamSetNullKeyResponseMode(buf, 0); // r0 - :166

        if (p_confirm->resultCode != 0)
        {
            WmspError(PARAMSET_NULL_KEY_MODE_REQ_CMD, p_confirm->resultCode, 0);
            return;
        }
    }

    if (p->connectPInfo.gameInfoLength < 0x10)
    {
        u16 beaconLostTh;         // r0 - :184
        WlParamSetCfm *p_confirm; // r0 - :185

        // TODO: small register swap here, but should behave the same?
        beaconLostTh = (p->connectPInfo.beaconPeriod != 0) ? (10000 / p->connectPInfo.beaconPeriod) + 1 : 1;
        if (beaconLostTh > 0xFF)
            beaconLostTh = 0xFF;

        p_confirm = WMSP_WL_ParamSetBeaconLostThreshold(buf, beaconLostTh & 0xFFFF);
        if (p_confirm->resultCode != 0)
        {
            WmspError(PARAMSET_BEACON_LOST_TH_REQ_CMD, p_confirm->resultCode, 0);
            return;
        }
    }

    {
        WlDevClass1Cfm *p_confirm = WMSP_WL_DevClass1(buf); // r0 - :206

        if (p_confirm->resultCode != 0)
        {
            WmspError(DEV_CLASS1_REQ_CMD, p_confirm->resultCode, 0);
            return;
        }

        status->state = WM_STATE_CLASS1;
    }

    {
        WlMlmePowerMgtCfm *p_confirm; // r0 - :220
        u16 pwrMgtMode;               // r11 - :222

        // TODO: slight mismatch (movne/moveq in original, mov #1, moveq #0 in ours)
        pwrMgtMode = (args->powerSave != 0) ? 1 : 0;
        p_confirm = WMSP_WL_MlmePowerManagement(buf, pwrMgtMode, 0, 1);

        if (p_confirm->resultCode != 0)
        {
            WmspError(MLME_PWR_MGT_REQ_CMD, p_confirm->resultCode, 0);
            return;
        }

        status->pwrMgtMode = pwrMgtMode;
    }

    {
        WlMlmeJoinCfm *p_confirm; // r0 - :240
        WlBssDesc bss_desc;       // None - :243

        MI_CpuCopy8(&p->connectPInfo, &bss_desc, OFFSETOF(WlBssDesc, gameInfo));
        if (status->mode == MODE_CHILD)
        {
            bss_desc.ssidLength = 32;
            *(u16 *)&bss_desc.ssid[0] = p->connectPInfo.gameInfo.ggid;
            *(u16 *)&bss_desc.ssid[2] = p->connectPInfo.gameInfo.ggid >> 16;
            *(u16 *)&bss_desc.ssid[4] = p->connectPInfo.gameInfo.tgid;
            *(u16 *)&bss_desc.ssid[6] = 0;
            MI_CpuCopy8(args->ssid, &bss_desc.ssid[8], sizeof(bss_desc.ssid) - 8);
        }

        p_confirm = WMSP_WL_MlmeJoin(buf, 2000, &bss_desc);

        if (p_confirm->resultCode != 0 || p_confirm->statusCode != 0)
        {
            WmspError(MLME_JOIN_REQ_CMD, p_confirm->resultCode, p_confirm->statusCode);
            return;
        }

        MI_CpuCopy8(p_confirm->peerMacAdrs, status->parentMacAddress, sizeof(status->parentMacAddress));
    }

    {
        WlMlmeAuthCfm *p_confirm; // r0 - :304
        u16 mac[3];               // None - :308

        MI_CpuCopy8(status->parentMacAddress, mac, sizeof(mac));

        // TODO: 3 instructions are shuffled here for some reason - should behave the same anyway
        p_confirm = WMSP_WL_MlmeAuthenticate(buf, mac, args->authMode, 0x7D0);

        if (p_confirm->resultCode == 12 && p_confirm->statusCode == 19)
        {
            WMStartConnectCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :324
            callback->apiid = WM_APIID_START_CONNECT;
            callback->errcode = WM_ERRCODE_OVER_MAX_ENTRY;
            callback->state = WM_STATECODE_CONNECT_START;
            WMSP_ReturnResult2Wm9(callback);
            return;
        }

        if (p_confirm->resultCode != 0 || p_confirm->statusCode != 0)
        {
            WmspError(MLME_AUTH_REQ_CMD, p_confirm->resultCode, p_confirm->statusCode);
            return;
        }
    }

    u16 mac[3]; // None - :344

    MI_CpuCopy8(status->parentMacAddress, mac, sizeof(mac));
    assConfirm = WMSP_WL_MlmeAssociate(buf, mac, 1, 0x7D0);

    u32 e = OS_DisableInterrupts(); // r11 - :359

    if (assConfirm->resultCode == 12 && assConfirm->statusCode == 19)
    {
        OS_RestoreInterrupts(e);

        WMStartConnectCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :366
        callback->apiid = WM_APIID_START_CONNECT;
        callback->errcode = WM_ERRCODE_OVER_MAX_ENTRY;
        callback->state = WM_STATECODE_CONNECT_START;
        WMSP_ReturnResult2Wm9(callback);
        return;
    }

    if (assConfirm->resultCode != 0 || assConfirm->statusCode != 0)
    {
        OS_RestoreInterrupts(e);
        WmspError(MLME_ASS_REQ_CMD, assConfirm->resultCode, assConfirm->statusCode);
        return;
    }

    status->aid = assConfirm->aid;
    status->curr_tgid = p->connectPInfo.gameInfo.tgid;
    MIi_CpuClear16(1, &status->portSeqNo[0], sizeof(status->portSeqNo[0]));

    u16 linkLevel = WMSP_GetRssi8(p->connectPInfo.rssi);

    status->linkLevel = WMSP_GetLinkLevel(linkLevel);
    WMSP_FillRssiIntoList(linkLevel);

    {
        u32 e = OS_DisableInterrupts(); // r6 - :405

        status->child_bitmap = 1;
        status->mp_readyBitmap = 1;

        if (status->mp_lifeTimeTick != 0)
        {
            status->mp_lastRecvTick[0] = OS_GetTick() | 1;
        }

        status->state = WM_STATE_CHILD;

        WMSP_SetParentMaxSize(p->connectPInfo.gameInfo.parentMaxSize + ((p->connectPInfo.gameInfo.attribute & 4) ? 42 : 0));
        WMSP_SetChildMaxSize(p->connectPInfo.gameInfo.childMaxSize + ((p->connectPInfo.gameInfo.attribute & 4) ? 6 : 0));

        OS_RestoreInterrupts(e);
    }

    status->beaconIndicateFlag = 1;

    WMStartConnectCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :438
    callback->apiid = WM_APIID_START_CONNECT;
    callback->errcode = WM_ERRCODE_SUCCESS;
    callback->state = WM_STATECODE_CONNECTED;
    callback->aid = status->aid;
    MI_CpuCopy8(status->parentMacAddress, callback->macAddress, sizeof(callback->macAddress));
    callback->parentSize = status->mp_parentSize;
    callback->childSize = status->mp_childSize;
    WMSP_ReturnResult2Wm9(callback);

    OS_RestoreInterrupts(e);
}

STATIC void WmspError(u16 wlCommand, u16 wlResult, u16 wlStatus)
{                                     // req_StartConnect.c:473
    WMStartConnectCallback *callback; // r0 - :475
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_START_CONNECT;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    callback->wlStatus = wlStatus;
    WMSP_ReturnResult2Wm9(callback);
}