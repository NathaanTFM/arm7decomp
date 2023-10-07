#include "Mongoose.h"

#define MAC_U16(a, b, c, d, e, f) {((b) << 8) | (a), ((d) << 8) | (c), ((f) << 8) | (e)}

u16 NULL_ADRS[3] = MAC_U16(0x00, 0x00, 0x00, 0x00, 0x00, 0x00); // :56
u16 BC_ADRS[3] = MAC_U16(0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF); // :57
u16 MP_ADRS[3] = MAC_U16(0x03, 0x09, 0xBF, 0x00, 0x00, 0x00); // :58
u16 MPKEY_ADRS[3] = MAC_U16(0x03, 0x09, 0xBF, 0x00, 0x00, 0x10); // :59

#define REG_ADRS(reg) ((u32)&(reg) - (u32)&WIFI_REG(0))

#define INIT_REG(reg, value) { REG_ADRS(reg), value},

static const MAC_INIT_REGS macInitRegs[25] = { // :64
    INIT_REG(W_MODE_RST, 0x00)
    INIT_REG(W_TXSTATCNT, 0)
    INIT_REG(W_X_00Ah, 0)
    INIT_REG(W_IE, 0)
    INIT_REG(W_IF, 0xFFFF)
    INIT_REG(W_CONFIG_254h, 0)
    INIT_REG(W_TXBUF_RESET, 0xFFFF)
    INIT_REG(W_TXBUF_BEACON, 0)
    INIT_REG(W_LISTENINT, 1)
    INIT_REG(W_LISTENCOUNT, 0)
    INIT_REG(W_AID_FULL, 0)
    INIT_REG(W_AID_LOW, 0)
    INIT_REG(W_US_COUNTCNT, 0)
    INIT_REG(W_US_COMPARECNT, 0)
    INIT_REG(W_CMD_COUNTCNT, 1)
    INIT_REG(W_CONFIG_0ECh, 0x3F03)
    INIT_REG(W_X_1A2h, 1)
    INIT_REG(W_X_1A0h, 0)
    INIT_REG(W_PRE_BEACON, 0x800)
    INIT_REG(W_PREAMBLE, 1)
    INIT_REG(W_CONFIG_0D4h, 3)
    INIT_REG(W_CONFIG_0D8h, 4)
    INIT_REG(W_RX_LEN_CROP, 0x602)
    INIT_REG(W_TXBUF_GAPDISP, 0)
    INIT_REG(W_CONFIG_130h, 0x146)
};

static const u16 macTxRxRegAdrs[16] = { // :193
    REG_ADRS(W_CONFIG_146h),
    REG_ADRS(W_CONFIG_148h),
    REG_ADRS(W_CONFIG_14Ah),
    REG_ADRS(W_CONFIG_14Ch),
    REG_ADRS(W_CONFIG_120h),
    REG_ADRS(W_CONFIG_122h),
    REG_ADRS(W_CONFIG_154h),
    REG_ADRS(W_CONFIG_144h),
    REG_ADRS(W_CONFIG_132h),
    REG_ADRS(W_CONFIG_132h),
    REG_ADRS(W_CONFIG_140h),
    REG_ADRS(W_CONFIG_142h),
    REG_ADRS(W_POWER_TX),
    REG_ADRS(W_CONFIG_124h),
    REG_ADRS(W_CONFIG_128h),
    REG_ADRS(W_CONFIG_150h)
};

const u16 RateBit2Element[16] = { // :221
    0x02, 0x04, 0x0B, 0x0C, 0x12, 0x16, 0x18, 0x24, 0x30, 0x48
};

const u16 RateElement2Bit[120] = { // :226
    0xFF, 0, 0xFF, 1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    2, 3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 4, 0xFF, 0xFF,
    0xFF, 5, 0xFF, 6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 7, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 8,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 9, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xB, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

static const u16 def_WepKey[40] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // :239
static const u16 def_SsidMask[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,}; // :240

static void TimeoutDummy(void* arg);

void InitializeParam(CAM_ELEMENT* pCam, u32 staNum) { // WlNic.c:330
    void* pGameInfo; // r5 - :332
    pGameInfo = wlMan->Work.GameInfoAdrs;
    
    MIi_CpuClear16(0, &wlMan->Config, sizeof(wlMan->Config));
    MIi_CpuClear16(0, &wlMan->Work, sizeof(wlMan->Work));
    
    wlMan->Config.pCAM = pCam;
    wlMan->Config.CamMaxStaNum = staNum;
    wlMan->Config.MaxStaNum = staNum;
    wlMan->Work.GameInfoAdrs = pGameInfo;
}

u16 WSetMacAdrs(u16* pMacAdrs) { // WlNic.c:369
    if ((pMacAdrs[0] & 1) != 0)
        return 5;
    
    WSetMacAdrs1(wlMan->Config.MacAdrs, pMacAdrs);
    WSetMacAdrs1((u16*)&W_MACADDR_0, pMacAdrs);
    wlMan->Config.ParamFlag |= 2;
    return 0;
}

u16 WSetRetryLimit(u16 retry) { // WlNic.c:400
    if (retry > 0xFF)
        return 5;
    
    wlMan->Config.RetryLimit = retry;
    W_TX_RETRYLIMIT = retry;
    return 0;
}

u16 WSetEnableChannel(u16 enableChannel) { // WlNic.c:427
    if ((enableChannel & 0x7FFE) == 0)
        return 5;
    
    wlMan->Config.EnableChannel = enableChannel;
    wlMan->Config.ParamFlag |= 4;
    return 0;
}

u16 WSetMode(u16 mode) { // WlNic.c:454
    if (mode > 3)
        return 5;
    
    wlMan->Config.Mode = mode;
    wlMan->Work.Mode = mode;
    
    W_MODE_WEP = W_MODE_WEP & 0xFFF8 | mode;
    WSetPowerMgtMode(wlMan->Work.PowerMgtMode);
    
    wlMan->Config.ParamFlag |= 8;
    return 0;
}

u16 WSetRate(u16 rate) { // WlNic.c:488
    if (rate > 2)
        return 5;
    
    wlMan->Config.Rate = rate;
    WSetTxTimeStampOffset();
    return 0;
}

u16 WSetWepMode(u16 mode) { // WlNic.c:516
    WORK_PARAM* pWork = &wlMan->Work; // r1 - :519
    
    if (mode > 3)
        return 5;
    
    wlMan->Config.WepMode = mode;
    
    if (!mode) {
        pWork->CapaInfo &= ~0x10;
        pWork->FrameCtrl &= ~0x4000;
        
    } else {
        pWork->CapaInfo |= 0x10;
        pWork->FrameCtrl |= 0x4000;
    }
    
    if (pWork->STA == 0x40 && mode == 1) {
        ((BEACON_BODY*)&wlMan->TxCtrl.Beacon.pMacFrm->Body)->CapaInfo.Data = pWork->CapaInfo;
    }
    
    if (!mode) mode = 1;
    
    W_MODE_WEP = (W_MODE_WEP & 0xFFC7) | (mode << 3);
    return 0;
}

u16 WSetWepKeyId(u16 keyId) { // WlNic.c:565
    if (keyId > 3)
        return 5;
    
    wlMan->Config.WepKeyId = keyId;
    return 0;
}

u16 WSetWepKey(const u16* pKey) { // WlNic.c:589
    MIi_CpuCopy16(pKey, (void*)&W_WEPKEY_0, 20);
    MIi_CpuCopy16(pKey + 10, (void*)&W_WEPKEY_1, 20);
    MIi_CpuCopy16(pKey + 20, (void*)&W_WEPKEY_2, 20);
    MIi_CpuCopy16(pKey + 30, (void*)&W_WEPKEY_3, 20);
    return 0;
}

u16 WSetBeaconType(u16 type) { // WlNic.c:614
    if (type > 1)
        return 5;
    
    wlMan->Config.BeaconType = type;
    return 0;
}

u16 WSetBcSsidResponse(u16 response) { // WlNic.c:638
    if (response > 1)
        return 5;
    
    wlMan->Config.BcSsidResponse = response;
    return 0;
}

u16 WSetBeaconLostThreshold(u16 threshold) { // WlNic.c:663
    if (threshold > 0xFF)
        return 5;
    
    wlMan->Work.BeaconLostCnt = 0;
    wlMan->Work.BeaconLostTh = threshold;
    return 0;
}

u16 WSetActiveZoneTime(u16 time, u32 update) { // WlNic.c:690
    u8* p; // r0 - :692
    
    if (time < 10)
        return 5;
    
    wlMan->Config.ActiveZone = time;
    if (update)
        W_POST_BEACON = time;
    
    if (wlMan->TxCtrl.Beacon.Busy) {
        p = &wlMan->TxCtrl.Beacon.pMacFrm->Body[wlMan->Work.Ofst.Beacon.GameInfo];
        if (wlMan->Work.PowerMgtMode == 1) {
            WL_WriteByte(&p[6], time & 0xFF);
            WL_WriteByte(&p[7], time >> 8);
            
        } else {
            WL_WriteByte(&p[6], 0xFF);
            WL_WriteByte(&p[7], 0xFF);
        }
    }
    return 0;
}

u16 WSetSsidMask(const u16* pMask) { // WlNic.c:741
    u32 i; // r2 - :743
    u16* pDest; // r0 - :744
    
    pDest = (u16*)wlMan->Work.SSIDMask;
    for (i = 0; i < 0x10; i++) {
        *pDest++ = *pMask++;
    }
    return 0;
}

u16 WSetPreambleType(u16 type) { // WlNic.c:769
    WORK_PARAM* pWork = &wlMan->Work; // r1 - :772
    
    if (type > 1)
        return 5;
    
    wlMan->Config.PreambleType = type;
    
    if (type == 0) {
        pWork->CapaInfo &= ~0x20;
    } else {
        pWork->CapaInfo |= 0x20;
    }
    
    if (pWork->STA == 0x40 && wlMan->Config.Mode == 1) {
        ((BEACON_BODY*)&wlMan->TxCtrl.Beacon.pMacFrm->Body)->CapaInfo.Data = pWork->CapaInfo;
    }
    
    if (type == 0) {
        W_PREAMBLE &= ~6;
    } else {
        W_PREAMBLE |= 6;
    }
    
    WSetTxTimeStampOffset();
    return 0;
}

u16 WSetAuthAlgo(u16 type) { // WlNic.c:825
    if (type > 1)
        return 5;
    
    wlMan->Config.AuthAlgo = type;
    return 0;
}

u16 WSetCCA_ED(u32 ccaMode, u32 edThreshold) { // WlNic.c:849
    if (ccaMode > 3)
        return 5;
    if (edThreshold > 0x3F)
        return 5;
    
    BBP_Write(0x13, ccaMode);
    BBP_Write(0x35, edThreshold);
    return 0;
}

u16 WSetMainAntenna(u32 mainAntenna) { // WlNic.c:877
    if (mainAntenna > 1)
        return 5;
    
    wlMan->Config.MainAntenna = mainAntenna;
    W_X_290h = wlMan->Config.UseAntenna ^ wlMan->Config.MainAntenna;
    return 0;
}

u16 WSetDiversity(u32 diversity, u32 useAntenna) { // WlNic.c:902
    if (diversity > 1 || useAntenna > 1)
        return 5;
    
    switch (diversity) {
        case 0:
            wlMan->Config.UseAntenna = useAntenna; // :910
            break;
            
        case 1:
            if (wlMan->Config.Mode != 1) // :914
                return 11;
        
            wlMan->Config.UseAntenna = 0; // :915
            break;
    }
    
    wlMan->Config.Diversity = diversity; // :919
    W_X_290h = wlMan->Config.UseAntenna ^ wlMan->Config.MainAntenna; // :921
    
    return 0;
}

u16 WSetBeaconSendRecvIndicate(u32 mode) { // WlNic.c:940
    if (mode > 1)
        return 5;
    
    wlMan->Config.BcnTxRxIndMsg = mode;
    return 0;
}

u16 WSetNullKeyMode(u32 mode) { // WlNic.c:963
    if (mode > 1)
        return 5;
    
    wlMan->Config.NullKeyRes = mode;
    if (mode == 1) {
        W_AID_LOW = W_AID_FULL;
    }
    
    return 0;
}

u16 WSetMcPassMode(u32 mode) { // WlNic.c:991
    if (mode > 1)
        return 5;
    
    wlMan->Config.MulticastPass = mode;
    return 0;
}

u16 WSetBssid(u16* pBssid) { // WlNic.c:1029
    WSetMacAdrs1(wlMan->Work.BSSID, pBssid);
    WSetMacAdrs1((u16*)&W_BSSID_0, pBssid);
    
    if ((pBssid[0] & 1) != 0) {
        W_RXFILTER &= ~0x400;
        
    } else {
        W_RXFILTER |= 0x400;
    }
    
    return 0;
}

u16 WSetSsid(u16 length, u8* pSsid) { // WlNic.c:1065
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :1067
    u32 i, b = 0; // r5, r6 - :1068
    u8* pBuf; // r6 - :1069
    
    if (length > 0x20)
        return 5;
    
    if (pWork->STA == 0x40 && wlMan->Config.Mode == 1) {
        if (pWork->SSIDLength != length)
            return 6;
        
        if (pWork->Ofst.Beacon.SSID) {
            b = 1;
        }
    }
    
    for (i = 0; i < length; i++) {
        WL_WriteByte(&pWork->SSID[i], WL_ReadByte(pSsid));
        pSsid++;
    }
    
    for (; i < 0x20; i++) {
        WL_WriteByte(&pWork->SSID[i], 0);
    }
    
    pWork->SSIDLength = length;
    
    if (b) {
        pBuf = wlMan->TxCtrl.Beacon.pMacFrm->Body + pWork->Ofst.Beacon.SSID + 2;
        
        for (i = 0; i < length; i++) {
            WL_WriteByte(&pBuf[i], WL_ReadByte(&pWork->SSID[i]));
        }
    }
    
    return 0;
}

u16 WSetBeaconPeriod(u16 period) { // WlNic.c:1121
    if (period < 10 || period > 1000)
        return 5;
    
    wlMan->Work.BeaconPeriod = period;
    W_BEACONINT = period;
    WSetFrameLifeTime(wlMan->Config.FrameLifeTimePerBeacon);
    return 0;
}

u16 WSetDTIMPeriod(u16 period) { // WlNic.c:1152
    if (period < 1 || period > 0xFF)
        return 5;
    
    wlMan->Work.DTIMPeriod = period;
    W_LISTENINT = period;
    W_LISTENCOUNT = 0;
    return 0;
}

u16 WSetListenInterval(u16 interval) { // WlNic.c:1182
    if (interval < 1 || interval > 0xFF)
        return 5;
    
    wlMan->Work.ListenInterval = interval;
    return 0;
}

#pragma dont_inline on
// Seems like one of those functions gets inlined.

void WSetDefaultParameters() { // WlNic.c:1223
    static RATE_SET c_RateSet = {3, 3};
    u16 macAdrs[3]; // None - :1226
    u16 ench; // None - :1227
    
    FLASH_Read(0x36, 6, (u8*)macAdrs); // :1233
    FLASH_Read(0x3C, 2, (u8*)&ench); // :1234
    WSetMacAdrs(macAdrs);
    WSetRetryLimit(7);
    WSetEnableChannel(ench & 0x7FFE);
    WSetMode(2);
    WSetRate(0);
    WSetWepMode(0);
    WSetWepKeyId(0);
    WSetWepKey(def_WepKey);
    WSetBeaconPeriod(500);
    WSetBeaconType(0);
    WSetBcSsidResponse(0);
    WSetBeaconLostThreshold(0x10);
    WSetActiveZoneTime(0xFFFF, 0);
    WSetSsidMask(def_SsidMask);
    WSetPreambleType(1);
    WSetAuthAlgo(0);
    WSetRateSet(&c_RateSet);
    WSetCCA_ED(0, 0x1F);
    WSetFrameLifeTime(5);
    WSetDiversity(0, 0);
    WSetMainAntenna(0);
    WSetBeaconSendRecvIndicate(0);
    WSetNullKeyMode(0);
    WSetMcPassMode(0);
    
    RND_init(W_RANDOM + (W_RANDOM << 8), W_RANDOM);
    
    wlMan->Work.RxDtims = 1;
}

#pragma dont_inline off

/*
u16 WSetChannel(u16 channel, u32 bDirect) { // WlNic.c:1312
    u32 reg; // None - :1315
    u32 bkpwr; // r8 - :1316
    u32 adrs; // None - :1318
    u32 i; // r10 - :1318
    u32 flash_adrs; // r9 - :1318
    void (*pFlashReadFunc)(u32, u32, u8*); // r0 - :1319
    u32 st; // r0 - :1340
    u32 ps; // r0 - :1340
}
Guys, check out my IPA!
*/

u16 WSetRateSet(RATE_SET* pRateSet) { // WlNic.c:1487
    RATE_SET* pWRS = &wlMan->Work.RateSet; // r0 - :1489
    
    pWRS->Basic = pRateSet->Basic;
    pWRS->Support = pRateSet->Support | pRateSet->Basic;
    
    WSetTxTimeStampOffset();
    return 0;
}

void WSetTxTimeStampOffset() { // WlNic.c:1514
    u32 ofst = 0xE2E2; // None - :1516
    FLASH_Read(0x58, 2, (u8*)&ofst);
    ofst += 0x202;
    
    if (WCalcManRate() == 20) {
        ofst -= 0x6161;
        if ((W_PREAMBLE & 2) != 0) {
            ofst -= 0x6060;
        }
    }
    
    W_CONFIG_140h = ofst;
}

u16 WSetPowerMgtMode(u32 mode) { // WlNic.c:1574
    CONFIG_PARAM* pConfig = &wlMan->Config; // r12 - :1576
    
    wlMan->Work.PowerMgtMode = mode;
    
    if (mode != 0 && pConfig->Mode != 1) {
        W_MODE_WEP |= 0x40;
        
    } else {
        W_MODE_WEP &= ~0x40;
        WSetActiveZoneTime(pConfig->ActiveZone, 0);
    }
    
    return 0;
}

u16 WSetPowerState(u32 state) { // WlNic.c:1610
    wlMan->Work.PowerState = state / 2;
    W_POWERSTATE = state;
    return 0;
}

u16 WSetForcePowerState(u32 state) { // WlNic.c:1661
    W_POWERFORCE = state;
    return 0;
}

void WShutdown() { // WlNic.c:1706
    if (wlMan->Rf.Id == 2)
        RF_Write(0xC008);
    
    // BBP_Read got inlined
    BBP_Write(0x1E, BBP_Read(0x1E) | 0x3F);
    
    W_BB_POWER = 0x800D;
    W_POWER_US = 1;
}

void WWakeUp() { // WlNic.c:1760
    W_POWER_US = 0;
    WaitLoop_Waitus(8000, TimeoutDummy);
    W_BB_POWER = 0;
    
    switch (wlMan->Rf.Id) {
        case 2: {
            u32 bp = BBP_Read(1);
            BBP_Write(1, bp & 0x7F);
            BBP_Write(1, bp);
            
            WaitLoop_Waitus(40000, TimeoutDummy);
            
            InitRF();
            break;
        }
        
        case 3:
            InitRF();
            break;
    }
}

u16 WSetFrameLifeTime(u32 lifeTimePerBeacon) { // WlNic.c:1830
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :1832
    CONFIG_PARAM* pConfig = &wlMan->Config; // r5 - :1833
    
    if (lifeTimePerBeacon == 0xFFFF) {
        pConfig->FrameLifeTimePerBeacon = 0xFFFF;
        pWork->FrameLifeTime = 0xFFFF;
        
    } else {
        u32 lifeTime = (lifeTimePerBeacon * pWork->BeaconPeriod) / 100; // r0 - :1842
        if (lifeTime > 0x10000)
            return 5;
        
        pConfig->FrameLifeTimePerBeacon = lifeTimePerBeacon;
        pWork->FrameLifeTime = lifeTime;
    }
    
    return 0;
}

void WDisableTmpttPowerSave() { // WlNic.c:1881
    wlMan->Work.TmpttPs = 1;
    if (!wlMan->TxCtrl.Mp.Busy) {
        W_POWER_TX &= ~2;
        W_POWER_unk = 0;
    }
}

void WEnableTmpttPowerSave() { // WlNic.c:1907
    wlMan->Work.TmpttPs = 0;
    W_POWER_TX |= 2;
}

u16 WInitGameInfo(u32 length, u8* pGameInfo) { // WlNic.c:1929
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :1931
    
    if (length > 0x80)
        return 4;
    
    MIi_CpuCopy16(pGameInfo, pWork->GameInfoAdrs, length + 1);
    pWork->GameInfoLength = length;
    return 0;
}

u16 WSetGameInfo(u32 length, u8* pGameInfo) { // WlNic.c:1958
    WORK_PARAM* pWork = &wlMan->Work; // r5 - :1960
    u8* p; // r6 - :1961
    u32 i; // r7 - :1962
    
    if (length > 0x80)
        return 4;
    
    if (length != 0) {
        if (pWork->GameInfoAlign & 1) {
            p = pWork->GameInfoAdrs;
            
            WL_WriteByte(p, 0xFF);
            p++;
            
            for (i = 0; i < length; i++) {
                WL_WriteByte(p, WL_ReadByte(pGameInfo));
                p++;
                pGameInfo++;
            }
            
        } else {
            MIi_CpuCopy16(pGameInfo, pWork->GameInfoAdrs, length + 1);
        }
    }
    
    pWork->GameInfoLength = length;
    pWork->bUpdateGameInfo = 1;
    return 0;
}

void WSetAids(u16 aid) { // WlNic.c:2009
    wlMan->Work.AID = aid;
    W_AID_FULL = aid;
    if (wlMan->Config.NullKeyRes)
        W_AID_LOW = aid;
}

void WClearAids() { // WlNic.c:2036
    WORK_PARAM* pWork = &wlMan->Work; // r5 - :2038
    pWork->AID = 0;
    WaitLoop_ClrAid();
    W_AID_FULL = 0;
    if (pWork->APCamAdrs) {
        DeleteTxFrames(pWork->APCamAdrs);
        CAM_SetStaState(pWork->APCamAdrs, 0x20);
        pWork->APCamAdrs = 0;
    }
}

void WSetKSID() { // WlNic.c:2067
    W_AID_LOW = wlMan->Work.AID;
}

void WClearKSID(int unused) { // WlNic.c:2087
    if ((W_TXBUF_REPLY1 & 0x8000) == 0) {
        WaitLoop_ClrAid();
    }
}

void WSetStaState(u32 state) { // WlNic.c:2111
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :2113
    
    if (pWork->STA != state) {
        if (pWork->STA == 0x40)
            OS_CancelAlarm(&wlMan->Alarm);
        
        switch (state) {
            case 0:
                WShutdown();
                break;
                
            case 0x10:
                W_POWERFORCE = 0;
                WStop();
                WWakeUp();
                break;
                
            case 0x40:
                if (pWork->Mode == 2)
                    WEnableTmpttPowerSave();
                
                SetupPeriodicTimeOut(0x64, WIntervalTimer);
                break;
        }
        
        pWork->STA = state;
    }
}

void WSetMacAdrs1(u16* dst, u16* src1) { // WlNic.c:2157
    dst[0] = src1[0];
    dst[1] = src1[1];
    dst[2] = src1[2];
}

void WSetMacAdrs2(u16* dst, u16* src1, u16* src2) { // WlNic.c:2178
    dst[0] = src1[0];
    dst[1] = src1[1];
    dst[2] = src1[2];
    dst[3] = src2[0];
    dst[4] = src2[1];
    dst[5] = src2[2];
}

void WSetMacAdrs3(u16* dst, u16* src1, u16* src2, u16* src3) { // WlNic.c:2202
    dst[0] = src1[0];
    dst[1] = src1[1];
    dst[2] = src1[2];
    dst[3] = src2[0];
    dst[4] = src2[1];
    dst[5] = src2[2];
    dst[6] = src3[0];
    dst[7] = src3[1];
    dst[8] = src3[2];
}

void WInitCounter() { // WlNic.c:2243
    WUpdateCounter();
    MIi_CpuClear32(0, &wlMan->Counter, sizeof(wlMan->Counter));
}

/*
void WUpdateCounter() { // WlNic.c:2277
    WlCounter* pCounter; // r0 - :2279
    u16 tmp; // r0 - :2281
    
    // this function is a mess, TODO
}
IPA blablablabah
*/

u32 WCheckSSID(u16 len, u8* pSSID) { // WlNic.c:2494
    WORK_PARAM* pWork = &wlMan->Work; // r2 - :2496
    u8 *pBuf, *pMask; // r5, r6 - :2497
    u32 mask, ssid, myss; // r0, r0, r0 - :2498
    u32 i; // r7 - :2499
    
    if (len > 0x20)
        return 0;
    
    if (pWork->SSIDLength == 0)
        return 1;
    
    if (wlMan->MLME.State == 0x13) {
        if (len < pWork->SSIDLength)
            return 0;
        
        len = pWork->SSIDLength;
        
    } else {
        if (len != pWork->SSIDLength) {
            return 0;
        }
    }
    
    pBuf = pWork->SSID;
    pMask = pWork->SSIDMask;
    
    for (i = 0; i < len; i++) {
        mask = WL_ReadByte(pMask++);
        ssid = WL_ReadByte(pSSID++);
        myss = WL_ReadByte(pBuf++);
        
        if ((ssid | mask) != (myss | mask)) {
            return 0;
        }
    }
    
    return 1;
}

u32 MatchMacAdrs(u16* pAdrs1, u16* pAdrs2) { // WlNic.c:2552
    return pAdrs1[2] == pAdrs2[2] && pAdrs1[1] == pAdrs2[1] && pAdrs1[0] == pAdrs2[0];
}

u32 CheckEnableChannel(u32 ch) { // WlNic.c:2571
    return (wlMan->Config.EnableChannel & (1 << ch));
}

void WElement2RateSet(SUP_RATE_ELEMENT* pSup, RATE_SET* pRateSet) { // WlNic.c:2602
    u32 i, length, brate, srate; // r7, r8, r0, r0 - :2604
    
    pRateSet->Basic = 0; // :2606
    pRateSet->Support = 0; // :2607
    
    length = WL_ReadByte(&pSup->Length); // :2618
    for (i = 0; i < length; i++) { // :2619
        srate = WL_ReadByte(&pSup->SupportedRate[i]); // brate? :2621
        brate = (srate & 0x7F) - 1; // srate? :2622
        
        if (brate < 0x78 && RateElement2Bit[brate] != 0xFF) { // :2625
            pRateSet->Support |= (1 << RateElement2Bit[brate]); // :2628
            
            if ((srate & 0x80) != 0) {
                pRateSet->Basic |= (1 << RateElement2Bit[brate]); // :2632
            }
            
        } else {
            pRateSet->Support |= 0x8000; // :2637
            
            if ((srate & 0x80) != 0) { // :2638
                pRateSet->Basic |= 0x8000; // :2639
            }
        }
    }
}

u32 WCalcManRate() { // WlNic.c:2658
    switch (wlMan->Config.Rate) {
        case 0:
            if ((wlMan->Work.RateSet.Basic & 1) != 0) {
                return 10; // :2663
            }
            break;
            
        case 1:
            return 10; // :2667
            
    };
    return 20; // :2672
}

/*void WStart() { // WlNic.c:2700
    WORK_PARAM* pWork; // r9 - :2702
    CONFIG_PARAM* pConfig; // r4 - :2703
    u16* ptsf; // r0 - :2705
    u64 lltsf; // None - :2706
}

This is the story of an IPA named WlNic.
WlNic worked for a driver in a big console where he was
employee number WiFi.
*/

void WStop() { // WlNic.c:2919
    WORK_PARAM* pWork = &wlMan->Work; // r5 - :2921
    
    OS_CancelAlarm(&wlMan->PeriodicAlarm);
    OS_CancelAlarm(&wlMan->Alarm);
    WSetStaState(0x20);
    
    pWork->bUpdateGameInfo = 0;
    pWork->bSynchro = 0;
    
    W_IE = 0;
    W_MODE_RST = 0;
    W_US_COMPARECNT = 0;
    W_US_COUNTCNT = 0;
    W_TXSTATCNT = 0;
    W_X_00Ah = 0;
    
    if (pWork->Mode == 1)
        StopBeaconFrame();
    
    W_TXREQ_RESET = 0xFFFF;
    W_TXBUF_RESET = 0xFFFF;
    
    DeleteAllTxFrames();
    ReleaseAllWlHeapBuf();
}

u32 BBP_Read(u32 adrs) { // WlNic.c:3073
    W_BB_CNT = adrs | 0x6000;
    WaitLoop_BbpAccess();
    return W_BB_READ;
}

u32 BBP_Write(u32 adrs, u32 data) { // WlNic.c:3100
    W_BB_WRITE = data;
    W_BB_CNT = adrs | 0x5000;
    if (WaitLoop_BbpAccess())
        return -1;
    return 0;
}

void RF_Write(u32 data) { // WlNic.c:3140
    W_RF_DATA1 = data;
    W_RF_DATA2 = (data >> 16);
    WaitLoop_RfAccess();
}

u16 CalcBbpCRC() { // WlNic.c:3198
    u32 adrs, i, data; // r6, r7, None - :3200
    u16 crc; // r8 - :3201
    
    adrs = 0x64; // :3204
    
    crc = data = 0; // :3208
    for (i = 0; i < 105; i++, adrs++) { // :3209
        FLASH_Read(adrs, 1, (u8*)&data);
        if (i == 1) // :3218
            data &= 0x80;
        
        crc = calc_NextCRC(data, crc); // :3220
    }
    
    return crc; // :3224
}

void WConfigDevice() { // WlNic.c:3322
    RF_CONFIG* pRf = &wlMan->Rf; // r5 - :3324
    
    MIi_CpuClear16(0, &wlMan->Rf, 0x10u);
    FLASH_Read(0x40u, 1u, (u8*)&pRf->Id);
    FLASH_Read(0x41u, 1u, (u8*)&pRf->Bits);
    FLASH_Read(0x42u, 1u, (u8*)&pRf->InitNum);
    FLASH_Read(0x43u, 1u, (u8*)&pRf->ChanNum);
}

void InitMac() { // WlNic.c:3357
    u32 i; // r3 - :3359
    
    for (i = 0; i < sizeof(macInitRegs) / sizeof(*macInitRegs); i++) {
        WIFI_REG(macInitRegs[i].adrs) = macInitRegs[i].value;
    }
}

void InitBaseBand() { // WlNic.c:3412
    u32 i, data, eep_adrs; // r6, None, r7 - :3414
    
    W_BB_MODE = 0x100;
    
    data = 0;
    eep_adrs = 0x64;
    
    for (i = 0; i < 0x69; i++, eep_adrs++) {
        FLASH_Read(eep_adrs, 1, (u8*)&data);
        BBP_Write(i, data);
    }
    
    BBP_Write(0x5A, 2);
}

void InitRF() { // WlNic.c:3455
    // NB: reg and data might be swapped. depends on which one makes more sense
    
    u32 eep_adrs, num, reg, bytes, i; // r7, r8, None, r9, r9 - :3458
    RF_CONFIG* pRf = &wlMan->Rf; // r4 - :3459
    u32 data = 0; // None - :3461
    
    for (i = 0; i < 0x10; i++) { // :3510
        FLASH_Read(2 * i + 0x44, 2, (u8*)&data); 
        WIFI_REG(macTxRxRegAdrs[i]) = data; // :3526
    }
    
    reg = ((u32)pRf->Bits >> 7) << 8; // :3545
    reg |= (pRf->Bits & 0x7F); // :3547
    W_RF_CNT = reg; // :3548
    
    eep_adrs = 0xCE; // :3552
    
    bytes = ((pRf->Bits & 0x7F) + 7) / 8; // :3556
    num = pRf->InitNum; // :3557
    
    if (pRf->Id == 3) {
        FLASH_Read(num + 0xCE, 1, (u8*)&pRf->BbpCnt);
        for (i = 0; i < num; i++, eep_adrs++) {
            reg = 0;
            FLASH_Read(eep_adrs, 1, (u8*)&reg);
            reg |= (i << 8) + 0x50000;
            RF_Write(reg);
        }
        
    } else {
        // who writes these kind of loops what the fuck
        for (reg = 0; num != 0; num--, eep_adrs += bytes) {
            FLASH_Read(eep_adrs, bytes, (u8*)&reg);
            RF_Write(reg);
            
            if (pRf->Id == 2) {
                if ((reg >> 18) == 9)
                    pRf->BkReg = reg & ~0x7C00;
            }
        }
    }
}

void InitializeAlarm() { // WlNic.c:3672
    WL_MAN* pWl = wlMan; // r4 - :3674
    // This line above is probably the weirdest line
    
    if (OS_IsAlarmAvailable()) {
        OS_CreateAlarm(&pWl->PeriodicAlarm);
        OS_CreateAlarm(&pWl->Alarm);
        OS_CreateAlarm(&pWl->KeyAlarm);
        
    } else {
        pWl->Config.DiagResult |= 0x40;
    }
}

static void TimeoutDummy(void* arg) { // WlNic.c:3704
    *(u32*)arg = 0;
}

void WWaitus(u32 us) { // WlNic.c:3729
    WaitLoop_Waitus(us, TimeoutDummy);
}

/*
void SetupPeriodicTimeOut(u32 ms, void (*pFunc)(void*)) { // WlNic.c:3748
    OSTick startTime; // None - :3750
    
    // This one just won't. I give up. I hate uint64s
    // Commenting it away to prevent IPA
}
*/

void ClearPeriodicTimeOut() { // WlNic.c:3776
    OS_CancelAlarm(&wlMan->PeriodicAlarm);
}

void WIntervalTimer() { // WlNic.c:3797
    WL_MAN* pWl = wlMan; // r6 - :3800
    pWl->Work.IntervalCount++;
    
    AddTask(1, 10);
    AddTask(2, 18);
    AddTask(1, 17);
    if (pWl->HeapMan.ToWM.Count) AddTask(2, 19);
    if (pWl->Work.FatalErr) AddTask(2, 21);
}

void SetupTimeOut(u32 ms, void (*pFunc)(void*)) { // WlNic.c:3876
    OS_CancelAlarm(&wlMan->Alarm);
    OS_SetAlarm(&wlMan->Alarm, OS_MilliSecondsToTicks(ms), pFunc, 0);
}

void SetupUsTimeOut(u32 us, void (*pFunc)(void*)) { // WlNic.c:3898
    OS_CancelAlarm(&wlMan->Alarm);
    OS_SetAlarm(&wlMan->Alarm, OS_MicroSecondsToTicks(us), pFunc, 0);
}

void ClearTimeOut() { // WlNic.c:3921
    OS_CancelAlarm(&wlMan->Alarm);
}

void DMA_Read(void* destAdrs, void* srcAdrs, u32 length) { // WlNic.c:3960
    u32 count; // r0 - :3963
    u32 dst; // r0 - :3964
    u32 src; // r0 - :3965
    u32 cpy1; // r4 - :3966
    u32 cpy2; // r7 - :3966
    
    length = (length + 1) & ~0x1;
    
    if ((char*)srcAdrs + length > (char*)RING_END_PTR) {
        cpy1 = (char*)RING_END_PTR - (char*)srcAdrs;
        cpy2 = length - cpy1;
        
    } else {
        cpy1 = length;
        cpy2 = 0;
    }
    
    MIi_CpuCopy16(srcAdrs, destAdrs, cpy1);
    if (cpy2)
        MIi_CpuCopy16((char *)srcAdrs + cpy1 - wlMan->Work.Ofst.RxBuf.Size, (char *)destAdrs + cpy1, cpy2);
}

void DMA_Write(void* destAdrs, void* srcAdrs, u32 length) { // WlNic.c:4101
    MIi_CpuCopy16(srcAdrs, destAdrs, (length + 1) & ~0x1);
}

void DMA_WriteHeaderData(TXFRM_MAC* destAdrs, MAC_HEADER* header, u8* data, u32 length) { // WlNic.c:4120
    MIi_CpuCopy16(header, destAdrs, sizeof(MAC_HEADER) + sizeof(DATA_HEADER)); // 0x24, check if it matches
    if (length)
        MIi_CpuCopy16(data, destAdrs->Body, (length + 1) & ~0x1);
}

void DMA_WepWriteHeaderData(TXFRM_MAC* destAdrs, MAC_HEADER* header, u8* data, u32 length) { // WlNic.c:4148
    MIi_CpuCopy16(header, destAdrs, sizeof(MAC_HEADER) + sizeof(DATA_HEADER));
    if (length)
        MIi_CpuCopy16(data, &destAdrs[1], (length + 1) & ~0x1);
}

void WL_WriteByte(void* p, u8 data) { // WlNic.c:4341
    if ((u32)p & 1)
        *(u16*)((u8*)p - 1) = (*(u16*)((u8*)p - 1) & 0xFF) | (data << 8);
    else
        *(u16*)p = ((*(u16*)p) & 0xFF00) | (data & 0xFF); // & 0xFF is useless, but it's in the code
}

u8 WL_ReadByte(void* p) { // WlNic.c:4372
    return ((u32)p & 1) ? (u8)(*(u16*)((u8*)p - 1) >> 8) : (u8)(*(u16*)p);
}

void RND_init(u32 a, u32 b) { // WlNic.c:4512
    RAND_CTRL* pRand = &wlMan->Rand; // r0 - :4514
    
    pRand->a = (a & 0xFFF8) + 5;
    pRand->b = b | 1;
}

void RND_seed(u32 seed) { // WlNic.c:4534
    wlMan->Rand.seed = seed;
}

u16 RND_rand() { // WlNic.c:4553
    RAND_CTRL* pRand = &wlMan->Rand; // r0 - :4555

    pRand->seed = pRand->b + pRand->seed * pRand->a;
    return pRand->seed;
}

u16 calc_NextCRC(u8 data, u16 total) { // WlNic.c:4609
    static u16 crc16_table[16] = {
        0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
        0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
    }; // :4580
    u16 r1; // r0 - :4611
    
    r1 = crc16_table[total & 0xF];
    total = (total >> 4) & 0xFFF;
    total = total ^ r1 ^ crc16_table[data & 0xF];
    
    r1 = crc16_table[total & 0xF];
    total = (total >> 4) & 0xFFF;
    total = total ^ r1 ^ crc16_table[(data >> 4) & 0xF];
    
    return total;
}

static u32 WCheckTxBufIdBeforeFrame(TXQ* pTxq) { // WlNic.c:4704
    u16* pId = (u16*)((u32)(pTxq->pMacFrm) - 0x4); // r14 - :4706
    
    if (!(pId[0] == 0xB6B8 && pId[1] == 0x1D46)) {
        // I don't get it.
        pTxq->pMacFrm->MacHeader.Tx.MPDU = 1;
        
        // That one is understandable
        pId[0] = 0xB6B8;
        pId[1] = 0x1D46;
        
        // Again, what's up with that? Why do we get it again?
        pId = (u16*)pTxq->pMacFrm;
        pId[6] = 0xB6B8; // Also is pTxq->pMacFrm->Dot11Header.FrameCtrl.Data
        pId[7] = 0x1D46; // Also is pTxq->pMacFrm->Dot11Header.DurationID
        
        wlMan->Work.TxBufErrCount++;
        return 1;
        
    }
    return 0;
    
}

static void WaitMacStop() { // WlNic.c:4747
    u32 i, state; // r2, r0 - :4749
    
    W_MODE_RST = 0;
    for (i = 16; i > 0; i--) {
        state = W_RF_STATUS;
        if (state == 0 || state == 9)
            break;
    }
}

static void RestoreTxFrame(TXQ* pTxq) { // WlNic.c:4787
    if (pTxq->Busy) {
        WaitMacStop();
        // TODO: figure out the types and logic behind this
        CopyTxFrmToMacBuf(pTxq->pMacFrm, (WlMaDataReq*)SubtractAddr(pTxq->pFrm, 0x10)); // TODO: figure out what should go here
        wlMan->Work.TxBufResCount++;
    }
}

void WCheckTxBuf() { // WlNic.c:4819
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :4821
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r5 - :4822
    
    switch (wlMan->Work.Mode) {
        case 1:
            if (WCheckTxBufIdBeforeFrame(&pTxCtrl->Beacon)) { // :4867
                WaitMacStop();
                MakeBeaconFrame();
            }
            
            if (WCheckTxBufIdBeforeFrame(&pTxCtrl->Txq[2]))
                RestoreTxFrame(&pTxCtrl->Txq[2]);
            
            break;
            
        case 2:
            WCheckTxBufIdBeforeFrame(&pTxCtrl->Key[1]); // :4890
            
            if (WCheckTxBufIdBeforeFrame(&pTxCtrl->Txq[2])) {
                if (pTxCtrl->Txq[2].Busy)
                    WaitMacStop();
                
                MakePsPollFrame(pWork->AID);
                pWork->TxBufResCount++;
            }
            break;
    }
    
    if (WCheckTxBufIdBeforeFrame(&pTxCtrl->Txq[1]))
        RestoreTxFrame(&pTxCtrl->Txq[1]);
    
    if (WCheckTxBufIdBeforeFrame(&pTxCtrl->Txq[0]))
        RestoreTxFrame(&pTxCtrl->Txq[0]);
    
    if (!W_MODE_RST)
        W_MODE_RST = 1;
}

void SetFatalErr(u32 errCode) { // WlNic.c:4992
    u32 x; // r0 - :4994
    
    x = OS_DisableIrqMask(0x1000000);
    wlMan->Work.FatalErr |= errCode;
    OS_EnableIrqMask(x);
    AddTask(2, 21);
}

void SendFatalErrMsgTask() { // WlNic.c:5026
    WORK_PARAM* pWork = &wlMan->Work; // r6 - :5028
    WlMaFatalErrInd* pInd; // r0 - :5029
    u32 x; // r0 - :5030
    
    if (pWork->FatalErr) {
        pInd = (WlMaFatalErrInd*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, sizeof(*pInd));
        if (pInd) {
            pInd->header.code = 0x186;
            pInd->header.length = 1;
            
            x = OS_DisableIrqMask(0x1000000);
            pInd->errCode = pWork->FatalErr;
            pWork->FatalErr = 0;
            OS_EnableIrqMask(x);
            
            SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
        }
    }
}

void TerminateWlTask() { // WlNic.c:5209
    wlMan->Config.DiagResult |= 0x8000;
    
    if (wlMan->Work.STA) {
        WStop();
        if (wlMan->MLME.State != 0) {
            wlMan->MLME.State = 0;
            wlMan->MLME.pCfm.Cfm->resultCode = 6; // TODO check that union!
            IssueMlmeConfirm();
        }
        WShutdown();
    }
    
    while (DeleteTask(3) != 0xFFFF)
        ;
    
    AddTask(3, 23);
}

void ReleaseWlTask() { // WlNic.c:5253
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r0 - :5255
    ReleaseIntr();
    ReleaseHeapBuf(&pHeapMan->TmpBuf, (char*)wlMan->pFlashImg - 12);
    ReleaseHeapBuf(&pHeapMan->TmpBuf, (char*)wlMan->Work.GameInfoAdrs - 12);
    OS_ExitThread();
}
