#include "Mongoose.h"

/* TODO */
static inline u8 WMSP_GetRssi8(u8 rssi) {
    if (rssi & 2) {
        return rssi >> 2;
    } else {
        return (rssi >> 2) + 25;
    }
}

STATIC void WmspError(u16 wlCommand, u16 wlResult, u16 wlStatus);

void WMSP_StartConnectEx(void* msg) { // req_StartConnect.c:48
    u32 wlBuf[128]; // None - :51
    struct WMStatus* status = wmspW.status;
    struct WMArm7Buf* p = wmspW.wm7buf; // r8 - :54
    struct WMStartConnectReq* args = (struct WMStartConnectReq*)msg; // r0 - :58
    WlMlmeAssociateCfm* assConfirm; // r9 - :59
    
    if (wmspW.status->state != 2 || (wmspW.status->miscFlags & 1) != 0) {
        struct WMStartConnectCallback* callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :64
        callback->apiid = 12;
        callback->errcode = 3;
        callback->state = 6;
        WMSP_ReturnResult2Wm9(callback);
        return;
    }
    
    MI_CpuCopy8(args->pInfo, &p->connectPInfo, 0xC0);
    if (p->connectPInfo.gameInfoLength >= 16 && (p->connectPInfo.gameInfo.attribute & 1) == 0) {
        struct WMStartConnectCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :88
        cb->apiid = 12;
        cb->errcode = 11;
        cb->state = 6;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }
    
    u32 tmp = (1 << p->connectPInfo.channel); // not defined anywhere!
    if ((tmp & status->enableChannel) == 0 || ((tmp >> 1) & 0x1FFF) == 0) {
        struct WMStartConnectCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :107
        cb->apiid = 12;
        cb->errcode = 6;
        cb->state = 6;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }
    
    {
        struct WMStartConnectCallback* callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :120
        callback->apiid = 12;
        callback->errcode = 0;
        callback->state = 6;
        WMSP_ReturnResult2Wm9(callback);
    }
    
    if (status->rate == 1) {
        if (p->connectPInfo.rateSet.basic & 1)
            status->rate = 1;
        else
            status->rate = 2;
        
    } else if ((p->connectPInfo.rateSet.basic & 2) != 0) {
        status->rate = 2;
    
    } else {
        status->rate = 1;
    }
    
    if ((p->connectPInfo.capaInfo & 0x20) != 0) {
        status->preamble = 1;
    } else {
        status->preamble = 0;
    }
    
    if (p->connectPInfo.gameInfoLength == 0) {
        status->mode = 3;
    } else {
        status->mode = 2;
    }
    
    if (!WMSP_SetAllParams(0xC, (u16*)wlBuf)) {
        return;
    }
    
    {
        WlCmdCfm* p_confirm = (WlCmdCfm*)WMSP_WL_ParamSetNullKeyResponseMode((u16*)wlBuf, 0); // r0 - :166
        
        if (p_confirm->resultCode != 0) {
            WmspError(534, p_confirm->resultCode, 0);
            return;
        }
    }
    
    if (p->connectPInfo.gameInfoLength < 0x10) {
        u16 beaconLostTh; // r0 - :184
        WlParamSetCfm* p_confirm; // r0 - :185
        
        // TODO: small register swap here, but should behave the same?
        beaconLostTh = (p->connectPInfo.beaconPeriod != 0) ? (10000 / p->connectPInfo.beaconPeriod) + 1 : 1;
        if (beaconLostTh > 0xFF)
            beaconLostTh = 0xFF;
        
        p_confirm = WMSP_WL_ParamSetBeaconLostThreshold((u16*)wlBuf, beaconLostTh & 0xFFFF);
        if (p_confirm->resultCode != 0) {
            WmspError(523, p_confirm->resultCode, 0);
            return;
        }
    }
    
    {
        WlDevClass1Cfm* p_confirm = WMSP_WL_DevClass1((u16*)wlBuf); // r0 - :206
        
        if (p_confirm->resultCode != 0) {
            WmspError(771, p_confirm->resultCode, 0);
            return;
        }
        
        status->state = 3;
    }
        
    {
        WlMlmePowerMgtCfm* p_confirm; // r0 - :220
        u16 pwrMgtMode; // r11 - :222
        
        // TODO: slight mismatch (movne/moveq in original, mov #1, moveq #0 in ours)
        pwrMgtMode = (args->powerSave != 0) ? 1 : 0;
        p_confirm = WMSP_WL_MlmePowerManagement((u16*)wlBuf, pwrMgtMode, 0, 1);
        
        if (p_confirm->resultCode != 0) {
            WmspError(1, p_confirm->resultCode, 0);
            return;
        }
        
        status->pwrMgtMode = pwrMgtMode;
    }
    
    {
        WlMlmeJoinCfm* p_confirm; // r0 - :240
        WlBssDesc bss_desc; // None - :243
        
        MI_CpuCopy8(&p->connectPInfo, &bss_desc, 0x40);
        if (status->mode == 2) {
            bss_desc.ssidLength = 32;
            *(u16*)&bss_desc.ssid[0] = p->connectPInfo.gameInfo.ggid;
            *(u16*)&bss_desc.ssid[2] = p->connectPInfo.gameInfo.ggid >> 16;
            *(u16*)&bss_desc.ssid[4] = p->connectPInfo.gameInfo.tgid;
            *(u16*)&bss_desc.ssid[6] = 0;
            MI_CpuCopy8(args->ssid, &bss_desc.ssid[8], 0x18);
        }
        
        p_confirm = WMSP_WL_MlmeJoin((u16*)wlBuf, 2000, &bss_desc);
        
        if (p_confirm->resultCode != 0 || p_confirm->statusCode != 0) {
            WmspError(3, p_confirm->resultCode, p_confirm->statusCode);
            return;
        }
        
        MI_CpuCopy8(p_confirm->peerMacAdrs, status->parentMacAddress, 6);
    }
    
    {
        WlMlmeAuthCfm* p_confirm; // r0 - :304
        u16 mac[3]; // None - :308
        
        MI_CpuCopy8(status->parentMacAddress, mac, 6);
        
        // TODO: 3 instructions are shuffled here for some reason - should behave the same anyway
        p_confirm = WMSP_WL_MlmeAuthenticate((u16*)wlBuf, mac, args->authMode, 0x7D0);
        
        if (p_confirm->resultCode == 12 && p_confirm->statusCode == 19) {
            struct WMStartConnectCallback* callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :324
            callback->apiid = 12;
            callback->errcode = 12;
            callback->state = 6;
            WMSP_ReturnResult2Wm9(callback);
            return;
        }
        
        if (p_confirm->resultCode != 0 || p_confirm->statusCode != 0) {
            WmspError(4, p_confirm->resultCode, p_confirm->statusCode);
            return;
        }
    }
    
    u16 mac[3]; // None - :344
    
    MI_CpuCopy8(status->parentMacAddress, mac, 6);
    assConfirm = WMSP_WL_MlmeAssociate((u16*)wlBuf, mac, 1, 0x7D0);
    
    u32 e = OS_DisableInterrupts(); // r11 - :359
    
    if (assConfirm->resultCode == 12 && assConfirm->statusCode == 19) {
        OS_RestoreInterrupts(e);
        
        struct WMStartConnectCallback* callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :366
        callback->apiid = 12;
        callback->errcode = 12;
        callback->state = 6;
        WMSP_ReturnResult2Wm9(callback);
        return;
    }
    
    if (assConfirm->resultCode != 0 || assConfirm->statusCode != 0) {
        OS_RestoreInterrupts(e);
        WmspError(6, assConfirm->resultCode, assConfirm->statusCode);
        return;
    }
    
    status->aid = assConfirm->aid;
    status->curr_tgid = p->connectPInfo.gameInfo.tgid;
    MIi_CpuClear16(1, status->portSeqNo, 0x10);
    
    u16 linkLevel = WMSP_GetRssi8(p->connectPInfo.rssi);
    
    status->linkLevel = WMSP_GetLinkLevel(linkLevel);
    WMSP_FillRssiIntoList(linkLevel);
    
    {
        u32 e = OS_DisableInterrupts(); // r6 - :405
        
        status->child_bitmap = 1;
        status->mp_readyBitmap = 1;
        
        if (status->mp_lifeTimeTick != 0) {
            status->mp_lastRecvTick[0] = OS_GetTick() | 1;
        }
        
        status->state = 8;
        
        WMSP_SetParentMaxSize(p->connectPInfo.gameInfo.parentMaxSize + ((p->connectPInfo.gameInfo.attribute & 4) ? 42 : 0));
        WMSP_SetChildMaxSize(p->connectPInfo.gameInfo.childMaxSize + ((p->connectPInfo.gameInfo.attribute & 4) ? 6 : 0));
        
        OS_RestoreInterrupts(e);
    }
    
    status->beaconIndicateFlag = 1;
    
    struct WMStartConnectCallback* callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :438
    callback->apiid = 12;
    callback->errcode = 0;
    callback->state = 7;
    callback->aid = status->aid;
    MI_CpuCopy8(status->parentMacAddress, callback->macAddress, 6);
    callback->parentSize = status->mp_parentSize;
    callback->childSize = status->mp_childSize;
    WMSP_ReturnResult2Wm9(callback);
    
    OS_RestoreInterrupts(e);
}

STATIC void WmspError(u16 wlCommand, u16 wlResult, u16 wlStatus) { // req_StartConnect.c:473
    struct WMStartConnectCallback* callback; // r0 - :475
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = 12;
    callback->errcode = 1;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    callback->wlStatus = wlStatus;
    WMSP_ReturnResult2Wm9(callback);
}