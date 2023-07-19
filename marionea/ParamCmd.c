#include "Mongoose.h"

u16 PARAMSET_AllReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:64
    u16 ret; // r4 - :66
    WlParamSetAllReq* pReq = (WlParamSetAllReq*)pReqt; // r0 - :67
    
    pCfmt->header.length = 1;
    
    if (wlMan->Work.STA != 0x10)
        return 1;

    ret = WSetMacAdrs(pReq->staMacAdrs);
    ret |= WSetRetryLimit(pReq->retryLimit);
    ret |= WSetEnableChannel(pReq->enableChannel);
    ret |= WSetMode(pReq->mode);
    ret |= WSetRate(pReq->rate);
    ret |= WSetWepMode(pReq->wepMode);
    ret |= WSetWepKeyId(pReq->wepKeyId);
    ret |= WSetWepKey((u16*)pReq->wepKey);
    ret |= WSetBeaconType(pReq->beaconType);
    ret |= WSetBcSsidResponse(pReq->probeRes);
    ret |= WSetBeaconLostThreshold(pReq->beaconLostTh);
    ret |= WSetActiveZoneTime(pReq->activeZoneTime, 0);
    ret |= WSetSsidMask((u16*)pReq->ssidMask);
    ret |= WSetPreambleType(pReq->preambleType);
    ret |= WSetAuthAlgo(pReq->authAlgo);
    return ret;
}

u16 PARAMSET_MacAdrsReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:109
    WlParamSetMacAdrsReq* pReq = (WlParamSetMacAdrsReq*)pReqt; // r0 - :111
    
    pCfmt->header.length = 1;
    
    if (wlMan->Work.STA != 0x10)
        return 1;
    
    return WSetMacAdrs(pReq->staMacAdrs);
}

u16 PARAMSET_RetryReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:138
    pCfmt->header.length = 1;
    return WSetRetryLimit(((WlParamSetRetryLimitReq*)pReqt)->retryLimit);
}

u16 PARAMSET_EnableChannelReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:164
    WlParamSetEnableChannelReq* pReq = (WlParamSetEnableChannelReq*)pReqt; // r0 - :166
    
    pCfmt->header.length = 1;
    
    if (wlMan->Work.STA != 0x10)
        return 1;
    
    return WSetEnableChannel(pReq->enableChannel);
}

u16 PARAMSET_ModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:192
    WlParamSetModeReq* pReq = (WlParamSetModeReq*)pReqt; // r0 - :194
    pCfmt->header.length = 1;
    
    if (wlMan->Work.STA > 0x20)
        return 1;
    
    if (wlMan->Work.STA == 0x20 && wlMan->Work.bSynchro)
        return 1;
    
    return WSetMode(pReq->mode); // :203
}

u16 PARAMSET_RateReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:221
    pCfmt->header.length = 1;
    return WSetRate(((WlParamSetRateReq*)pReqt)->rate);
}

u16 PARAMSET_WepModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:249
    pCfmt->header.length = 1;
    return WSetWepMode(((WlParamSetWepModeReq*)pReqt)->wepMode);
}

u16 PARAMSET_WepKeyIdReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:277
    pCfmt->header.length = 1;
    return WSetWepKeyId(((WlParamSetWepKeyIdReq*)pReqt)->wepKeyId);
}

u16 PARAMSET_WepKeyReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:305
    pCfmt->header.length = 1;
    return WSetWepKey((u16*)((WlParamSetWepKeyReq*)pReqt)->wepKey);
}

u16 PARAMSET_BeaconTypeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:333
    WlParamSetBeaconTypeReq* pReq = (WlParamSetBeaconTypeReq*)pReqt; // r0 - :335
    pCfmt->header.length = 1;
    
    if (wlMan->Work.STA > 0x20)
        return 1;
    
    return WSetBeaconType(pReq->beaconType);
}

u16 PARAMSET_ResBcSsidReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:362
    pCfmt->header.length = 1;
    return WSetBcSsidResponse(((WlParamSetProbeResReq*)pReqt)->probeRes);
}

u16 PARAMSET_BeaconLostThReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:388
    pCfmt->header.length = 1;
    return WSetBeaconLostThreshold(((WlParamSetBeaconLostThresholdReq*)pReqt)->beaconLostTh);
}

u16 PARAMSET_ActiveZoneReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:414
    pCfmt->header.length = 1;
    return WSetActiveZoneTime(((WlParamSetActiveZoneReq*)pReqt)->activeZoneTime, 0);
}

u16 PARAMSET_SSIDMaskReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:440
    pCfmt->header.length = 1;
    return WSetSsidMask((u16*)((WlParamSetSsidMaskReq*)pReqt)->mask);
}

u16 PARAMSET_PreambleTypeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:466
    pCfmt->header.length = 1;
    return WSetPreambleType(((WlParamSetPreambleTypeReq*)pReqt)->type);
}

u16 PARAMSET_AuthAlgoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:492
    pCfmt->header.length = 1;
    return WSetAuthAlgo(pReqt->buf[0]);
}

u16 PARAMSET_CCAModeEDThReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:518
    WlParamSetCCAModeEDThReq* pReq = (WlParamSetCCAModeEDThReq*)pReqt; // r0 - :520
    u16 ret; // r4 - :521
    
    pCfmt->header.length = 1;
    
    if (pReq->agcLimit > 0x3F)
        return 5;
    
    ret = WSetCCA_ED(pReq->ccaMode, pReq->edThreshold);
    if (ret == 0)
        BBP_Write(0x2E, pReq->agcLimit);
    
    return ret;
}

u16 PARAMSET_LifeTimeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:555
    WlParamSetLifeTimeReq* pReq; // r0 - :557
    CAM_ELEMENT* pCAM; // r2 - :558
    u32 i; // r4 - :559
}

u16 PARAMSET_MaxConnReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:617
    WlParamSetMaxConnReq* pReq = (WlParamSetMaxConnReq*)pReqt; // r0 - :619
    
    pCfmt->header.length = 1;
    
    if (wlMan->Work.STA > 0x20)
        return 1;
    
    pReq->count++;
    
    if (pReq->count > wlMan->Config.CamMaxStaNum)
        return 5;
    
    wlMan->Config.MaxStaNum = pReq->count;
    return 0;
}

u16 PARAMSET_MainAntennaReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:651
    WlParamSetMainAntennaReq* pReq = (WlParamSetMainAntennaReq*)pReqt; // r0 - :653
    
    pCfmt->header.length = 1;
    
    if (wlMan->Work.STA < 0x10)
        return 1;
    
    return WSetMainAntenna(pReq->mainAntenna);
}

u16 PARAMSET_DiversityReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:681
    WlParamSetDiversityReq* pReq; // r0 - :683
}

u16 PARAMSET_BcnSendRecvIndReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:710
    WlParamSetBeaconSendRecvIndReq* pReq; // r0 - :712
}

u16 PARAMSET_NullKeyModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:738
    WlParamSetNullKeyModeReq* pReq; // r0 - :740
}

u16 PARAMSET_McPassModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:766
    WlParamSetMcPassModeReq* pReq; // r0 - :768
}

u16 PARAMSET_BSSIDReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:809
    pCfmt->header.length = 1;
    return WSetBssid(((WlParamSetBssidReq*)pReqt)->bssid);
}

u16 PARAMSET_SSIDReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:835
    WlParamSetSsidReq* pReq = (WlParamSetSsidReq*)pReqt; // r0 - :837
    
    pCfmt->header.length = 1;
    return WSetSsid(pReq->ssidLength, pReq->ssid);
}

u16 PARAMSET_BeaconPeriodReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:861
    WlParamSetBeaconPeriodReq* pReq; // r0 - :863
}

u16 PARAMSET_DTIMPeriodReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:890
    WlParamSetDtimPeriodReq* pReq; // r0 - :892
}

u16 PARAMSET_ListenIntervalReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:919
    WlParamSetIntervalReq* pReq; // r0 - :921
}

u16 PARAMSET_GameInfoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:948
    WlParamSetGameInfoReq* pReq; // r0 - :950
}

u16 PARAMGET_AllReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:993
    WlParamGetAllCfm* pCfm; // r0 - :996
}

u16 PARAMGET_MacAdrsReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1037
    WlParamGetMacAdrsCfm* pCfm; // r0 - :1040
}

u16 PARAMGET_RetryReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1069
    WlParamGetRetryLimitCfm* pCfm; // r0 - :1072
}

u16 PARAMGET_EnableChannelReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1098
    WlParamGetEnableChannelCfm* pCfm; // r0 - :1101
}

u16 PARAMGET_ModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1127
    WlParamGetModeCfm* pCfm; // r0 - :1130
}

u16 PARAMGET_RateReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1156
    WlParamGetRateCfm* pCfm; // r0 - :1159
}

u16 PARAMGET_WepModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1185
    WlParamGetWepModeCfm* pCfm; // r0 - :1188
}

u16 PARAMGET_WepKeyIdReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1214
    WlParamGetWepKeyIdCfm* pCfm; // r0 - :1217
}

u16 PARAMGET_BeaconTypeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1244
    WlParamGetBeaconTypeCfm* pCfm; // r0 - :1247
}

u16 PARAMGET_ResBcSsidReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1274
    WlParamGetProbeResCfm* pCfm; // r0 - :1277
}

u16 PARAMGET_BeaconLostThReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1304
    WlParamGetBeaconLostThCfm* pCfm; // r0 - :1307
}

u16 PARAMGET_ActiveZoneReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1334
    WlParamGetActiveZoneCfm* pCfm; // r0 - :1337
}

u16 PARAMGET_SSIDMaskReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1365
    u32 i; // r1 - :1368
    WlParamGetSsidMaskCfm* pCfm; // r0 - :1369
    u16* p2; // r0 - :1370
    u16* p1; // r0 - :1370
}

u16 PARAMGET_PreambleTypeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1399
    WlParamGetPreambleTypeCfm* pCfm; // r0 - :1402
}

u16 PARAMGET_AuthAlgoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1429
}

u16 PARAMGET_CCAModeEDThReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1459
    WlParamGetCCAModeEDThCfm* pCfm; // r0 - :1462
}

u16 PARAMGET_MaxConnReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1491
}

u16 PARAMGET_MainAntennaReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1520
    WlParamGetMainAntennaCfm* pCfm; // r0 - :1523
}

u16 PARAMGET_DiversityReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1553
    WlParamGetDiversityCfm* pCfm; // r0 - :1556
}

u16 PARAMGET_BcnSendRecvIndReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1587
    WlParamGetBeaconSendRecvIndCfm* pCfm; // r0 - :1590
}

u16 PARAMGET_NullKeyModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1620
    WlParamGetNullKeyModeCfm* pCfm; // r0 - :1623
}

u16 PARAMGET_McPassModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1653
    WlParamGetMcPassModeCfm* pCfm; // r0 - :1656
}

u16 PARAMGET_BSSIDReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1698
    WlParamGetBssidCfm* pCfm = (WlParamGetBssidCfm*)pCfmt; // r0 - :1701
    
    pCfm->header.length = 4;
    WSetMacAdrs1(pCfm->bssid, wlMan->Work.BSSID);
    return 0;
}

u16 PARAMGET_SSIDReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1731
    u32 i; // r3 - :1734
    WlParamGetSsidCfm* pCfm = (WlParamGetSsidCfm*)pCfmt; // r0 - :1736
    u16 *p1, *p2; // r0, r0 - :1737
    
    pCfm->header.length = 18;
    pCfm->ssidLength = wlMan->Work.SSIDLength;
    
    p1 = (u16*)wlMan->Work.SSID;
    p2 = (u16*)pCfm->ssid;
    
    for (i = 0; i < 0x20; i += 2) {
        *(p2++) = *(p1++);
    }
    
    return 0;
}

u16 PARAMGET_BeaconPeriodReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1767
    WlParamGetBeaconPeriodCfm* pCfm = (WlParamGetBeaconPeriodCfm*)pCfmt; // r0 - :1770
    
    pCfm->header.length = 2;
    pCfm->beaconPeriod = wlMan->Work.BeaconPeriod;
    return 0;
}

u16 PARAMGET_DTIMPeriodReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1797
    WlParamGetDtimPeriodCfm* pCfm = (WlParamGetDtimPeriodCfm*)pCfmt; // r0 - :1800

    pCfm->header.length = 2;
    pCfm->dtimPeriond = wlMan->Work.DTIMPeriod;
    return 0;
}

u16 PARAMGET_ListenIntervalReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1827
    WlParamGetIntervalCfm* pCfm = (WlParamGetIntervalCfm*)pCfmt; // r0 - :1830
    
    pCfm->header.length = 2;
    pCfm->listenInterval = wlMan->Work.ListenInterval;
    return 0;
}

u16 PARAMGET_GameInfoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // ParamCmd.c:1857
    WlParamGetGameInfoCfm* pCfm; // r0 - :1860
    u8* p2; // r6 - :1861
    u8* p1; // r5 - :1861
    u32 i; // r7 - :1862
}
