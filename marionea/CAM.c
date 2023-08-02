#include "Mongoose.h"

// TODO: put this in a header file 
extern u16 BC_ADRS[3]; // from WlNic.c

static void CAM_InitElement(u32 camAdrs, u16* pMacAdrs);

u32 CAM_Search(u16* pMacAdrs) { // CAM.c:63
    CAM_ELEMENT* pCAM; // r5 - :65
    u32 pos, i; // r6, r7 - :66
    
    if ((pMacAdrs[0] & 1) != 0) // :69
        return 0; // :69
    
    if (wlMan->CamMan.Count > 1) { // :72
        pCAM = wlMan->Config.pCAM + 1; // :74
        
        for (i = 0, pos = 1; pos < wlMan->Config.MaxStaNum; pos++, pCAM++) { // :77
            if (pCAM->state) { // :80
                if (MatchMacAdrs(pCAM->macAdrs, pMacAdrs)) // :83
                    return pos;
                
                if (++i >= wlMan->CamMan.Count) // :86
                    break;
            }
        }
    }
    
    return 0xFF; // :91
}

u32 CAM_SearchAdd(u16* pMacAdrs) { // CAM.c:109
    CONFIG_PARAM* pConfig = &wlMan->Config; // r6 - :111
    CAM_ELEMENT* pCAM; // r7 - :112
    u32 pos, i, free, lifeTime; // r8, r9, r10, r3 - :113
    
    if (pMacAdrs[0] & 1)
        return 0;
    
    // attempt to search    
    if (wlMan->CamMan.Count > 1) {
        pCAM = &pConfig->pCAM[1];
        
        for (free = 0, i = 0, pos = 1; pos < pConfig->MaxStaNum; pos++, pCAM++) {
            if (pCAM->state) {
                if (MatchMacAdrs(pCAM->macAdrs, pMacAdrs)) {
                    return pos;
                }
                
                if (++i >= wlMan->CamMan.Count)
                    break;
                
            } else if (!free) {
                // we found a free index!
                free = pos;
            }
        }
        
        if (free)
            pos = free;
        
    } else {
        pos = 1;
    }
    
    if (pos >= wlMan->Config.MaxStaNum) {
        pCAM = pConfig->pCAM;
        lifeTime = 0x10000;
        
        for (i = 1, pos = 0; i < pConfig->MaxStaNum; i++) {
            if (pCAM[i].state < 0x30 && pCAM[i].frameCount == 0 && lifeTime > pCAM[i].lifeTime) {
                lifeTime = pCAM[i].lifeTime;
                pos = i;
            }
        }
        
        if (pos == 0) {
            return 0xFF;
        }
    }
    
    CAM_InitElement(pos, pMacAdrs);
    return pos;
}

void CAM_AddBcFrame(HEAPBUF_MAN* pBufMan, void* pBuf) { // CAM.c:207
    HEAPBUF_MAN* pBufManBc = &wlMan->HeapMan.TxPri[2]; // r4 - :209
    u32 x = OS_DisableIrqMask(0x1000000); // r5 - :210
    
    if (!pBufManBc->Count)
        CAM_SetTIMElementBitmap(0);
    
    MoveHeapBuf(pBufMan, pBufManBc, pBuf);
    OS_EnableIrqMask(x);
}

void CAM_IncFrameCount(TXFRM* pFrm) { // CAM.c:291
    u32 x, cam_adrs; // r4, r5 - :294
    CAM_ELEMENT* pCAM; // optimized away, mentioned in old nef
    
    cam_adrs = pFrm->FirmHeader.CamAdrs;
    pCAM = &wlMan->Config.pCAM[cam_adrs];
    
    x = OS_DisableIrqMask(0x1000000);
    
    if (wlMan->Work.Mode == 1 && pCAM->frameCount == 0)
        CAM_SetTIMElementBitmap(cam_adrs);  
    
    pCAM->frameCount++;
    OS_EnableIrqMask(x);
    
    if ((wlMan->CamMan.NotSetTIM & (1 << cam_adrs)) == 0)
        pCAM->lifeTime = pCAM->maxLifeTime;
    
}

void CAM_DecFrameCount(TXFRM* pFrm) { // CAM.c:336
    u32 x, cam_adrs; // r4, r5 - :339
    CAM_ELEMENT* pCAM;
    
    cam_adrs = pFrm->FirmHeader.CamAdrs;
    pCAM = &wlMan->Config.pCAM[cam_adrs];
    
    x = OS_DisableIrqMask(0x1000000);
    
    if (wlMan->Work.Mode == 1 && pCAM->frameCount == 1)
        CAM_ClrTIMElementBitmap(cam_adrs);  
    
    pCAM->frameCount--;
    OS_EnableIrqMask(x);
}

void CAM_SetStaState(u16 camAdrs, u16 state) { // CAM.c:396
    u32 x = OS_DisableIrqMask(0x1000000); // r5 - :398
    
    if (state < 0x40) {
        CAM_SetAwake(camAdrs);
        wlMan->CamMan.NotClass3 |= (1 << camAdrs);
        
        if (wlMan->Work.Mode == 1 && CAM_GetAID(camAdrs))
            CAM_ReleaseAID(camAdrs);
        
    } else {
        wlMan->CamMan.NotClass3 &= ~(1 << camAdrs);
        
        if (CAM_GetPowerMgtMode(camAdrs))
            CAM_SetDoze(camAdrs);
    }
    
    wlMan->Config.pCAM[camAdrs].state = state;
    OS_EnableIrqMask(x);
}

void CAM_SetRSSI(u16 camAdrs, u16 rssi) { // CAM.c:448
    wlMan->Config.pCAM[camAdrs].rssi = rssi;
}

void CAM_SetPowerMgtMode(u16 camAdrs, u16 pmtMode) { // CAM.c:473
    CAM_MAN* pCamMan = &wlMan->CamMan; // r0 - :475
    pCamMan->PowerMgtMode = (pCamMan->PowerMgtMode & ~(1 << camAdrs)) | (pmtMode << camAdrs);
    
    if ((pCamMan->PowerMgtMode & ~pCamMan->NotClass3) != 0) {
        W_TXREQ_RESET = 8;
    } else {
        W_TXREQ_SET = 8;
    }
}

void CAM_SetDoze(u32 camAdrs) { // CAM.c:505
    if (CAM_GetStaState(camAdrs) == 0x40) {
        wlMan->CamMan.PowerState &= ~(1 << camAdrs);
    }
}

void CAM_SetAwake(u32 camAdrs) { // CAM.c:529
    wlMan->CamMan.PowerState |= (1 << camAdrs);
}

void CAM_SetCapaInfo(u32 camAdrs, u32 capInfo) { // CAM.c:550
    wlMan->Config.pCAM[camAdrs].capaInfo = capInfo;
}

void CAM_SetSupRate(u32 camAdrs, u32 SupRate) { // CAM.c:571
    wlMan->Config.pCAM[camAdrs].supRateSet = SupRate;
}

void CAM_SetLastSeqCtrl(u32 camAdrs, u32 seqCtrl) { // CAM.c:592
    wlMan->Config.pCAM[camAdrs].lastSeqCtrl = seqCtrl;
}

void CAM_SetAuthSeed(u32 camAdrs, u32 seed) { // CAM.c:613
    wlMan->Config.pCAM[camAdrs].authSeed = seed;
}

void CAM_UpdateLifeTime(u32 camAdrs) { // CAM.c:633
    wlMan->Config.pCAM[camAdrs].lifeTime = wlMan->Config.pCAM[camAdrs].maxLifeTime;
}

u32 CAM_AllocateAID(u16 camAdrs) { // CAM.c:665
    CAM_MAN* pCamMan = &wlMan->CamMan; // r4 - :667
    u32 i, map, x; // r5, r0, r6 - :668
    
    x = OS_DisableIrqMask(0x1000000);
    
    for (i = 1, map = 2; i < 0x10; i++, map <<= 1) {
        if ((pCamMan->UseAidMap & map) == 0) {
            pCamMan->UseAidMap |= map;
            
            pCamMan->ConnectSta++;
            if (pCamMan->ConnectSta == 1) {
                WEnableTmpttPowerSave();
            }
            
            wlMan->Config.pCAM[camAdrs].aid = i;
            OS_EnableIrqMask(x);
            return i;
        }
    }
    
    OS_EnableIrqMask(x);
    return 0;
}

void CAM_ReleaseAID(u16 camAdrs) { // CAM.c:736
    WL_MAN* pWlMan = wlMan; // r4 - :738
    u32 aid; // r0 - :740
    
    CAM_ClrTIMElementBitmap(camAdrs);
    aid = CAM_GetAID(camAdrs);
    if (aid) {
        wlMan->Config.pCAM[camAdrs].aid = 0;
        pWlMan->CamMan.UseAidMap &= ~(1 << aid);
        pWlMan->CamMan.ConnectSta--;
        if (pWlMan->CamMan.ConnectSta == 0)
            WDisableTmpttPowerSave();
    }
}

u32 CAM_GetStaState(u32 camAdrs) { // CAM.c:826
    return wlMan->Config.pCAM[camAdrs].state;
}

u32 CAM_IsActive(u32 camAdrs) { // CAM.c:845
    return (wlMan->CamMan.PowerState >> camAdrs) & 1;
}

u32 CAM_GetPowerMgtMode(u32 camAdrs) { // CAM.c:864
    return (wlMan->CamMan.PowerMgtMode >> camAdrs) & 1;
}

u16* CAM_GetMacAdrs(u32 camAdrs) { // CAM.c:883
    return wlMan->Config.pCAM[camAdrs].macAdrs;
}

u32 CAM_GetAuthSeed(u32 camAdrs) { // CAM.c:902
    return wlMan->Config.pCAM[camAdrs].authSeed;
}

u32 CAM_GetLastSeqCtrl(u32 camAdrs) { // CAM.c:921
    return wlMan->Config.pCAM[camAdrs].lastSeqCtrl;
}

u32 CAM_GetTxRate(u32 camAdrs) { // CAM.c:940
    if ((wlMan->Config.pCAM[camAdrs].supRateSet & 2) != 0)
        return 20;
    else
        return 10;
}

u32 CAM_GetAID(u32 camAdrs) { // CAM.c:961
    return wlMan->Config.pCAM[camAdrs].aid;
}

u32 CAM_GetFrameCount(u32 camAdrs) { // CAM.c:980
    return wlMan->Config.pCAM[camAdrs].frameCount;
}

void CAM_SetTIMElementBitmap(u32 camAdrs) { // CAM.c:1030
    TIM_ELEMENT* pTIM; // r7 - :1032
    u32 tmp, x, aid; // r0, r5, r0 - :1033
    
    if (CAM_GetStaState(camAdrs) != 0x40)
        return;
    
    if ((wlMan->CamMan.NotSetTIM & (1 << camAdrs)) != 0)
        return;
    
    pTIM = (TIM_ELEMENT*)((char*)0x480425C + wlMan->Work.Ofst.Beacon.TIM);
    x = OS_DisableIrqMask(0x1000000);
    
    if (camAdrs == 0) {
        tmp = WL_ReadByte(&pTIM->BitmapCtrl);
        WL_WriteByte(&pTIM->BitmapCtrl, tmp | 1);
        
    } else {
        aid = CAM_GetAID(camAdrs);
        tmp = WL_ReadByte(&pTIM->VitrualBitmap[aid >> 3]);
        WL_WriteByte(&pTIM->VitrualBitmap[aid >> 3], tmp | (1 << (aid & 7)));
    }
    
    OS_EnableIrqMask(x);
}

void CAM_ClrTIMElementBitmap(u32 camAdrs) { // CAM.c:1089
    TIM_ELEMENT* pTIM; // r6 - :1091
    u32 tmp, x, aid; // r0, r4, r0 - :1092
    
    if (CAM_GetStaState(camAdrs) != 0x40)
        return;
    
    pTIM = (TIM_ELEMENT*)((char*)0x480425C + wlMan->Work.Ofst.Beacon.TIM);
    x = OS_DisableIrqMask(0x1000000);
    
    if (camAdrs == 0) {
        tmp = WL_ReadByte(&pTIM->BitmapCtrl);
        WL_WriteByte(&pTIM->BitmapCtrl, tmp & 0xFE);
        
    } else {
        aid = CAM_GetAID(camAdrs);
        tmp = WL_ReadByte(&pTIM->VitrualBitmap[aid >> 3]);
        WL_WriteByte(&pTIM->VitrualBitmap[aid >> 3], ~(1 << (aid & 7)) & tmp);
    }
    
    OS_EnableIrqMask(x);
}

void CAM_TimerTask() { // CAM.c:1158
    WL_MAN* pWlMan; // r2 - :1160
    CAM_ELEMENT* pCAM; // r7 - :1161
    DEAUTH_FRAME* pTxDeAuthFrm; // r0 - :1162
    u32 i; // r9 - :1163
    u32 pos; // r8 - :1163
    u32 state; // r0 - :1164
    u32 cnt; // r10 - :1164
}

void CAM_Delete(u16 camAdrs) { // CAM.c:1267
    DeleteTxFrames(camAdrs);
    wlMan->Config.pCAM[camAdrs].state = 0;
    wlMan->CamMan.Count--;
}

void InitializeCAM() { // CAM.c:1307
    CAM_ELEMENT* pCAM = wlMan->Config.pCAM; // r4 - :1309
    u32 i, max; // r3, r5 - :1310
    
    max = wlMan->Config.MaxStaNum;
    MIi_CpuClear16(0, pCAM, sizeof(*pCAM) * max);
    MIi_CpuClear16(0, &wlMan->CamMan, sizeof(wlMan->CamMan));
    
    pCAM[0].maxLifeTime = 0xFFFF; // why is it not in the loop?
    for (i = 1; i < max; i++)
        pCAM[i].maxLifeTime = 0xFFFF;
    
    CAM_InitElement(0, BC_ADRS);
    CAM_SetStaState(0, 0x40);
}

void InitCAM() { // CAM.c:1346
    CAM_ELEMENT* pCAM = wlMan->Config.pCAM; // r4 - :1348
    CAM_MAN* pCamMan = &wlMan->CamMan; // r5 - :1349
    u32 i, max; // r6, r7 - :1350
    
    max = wlMan->Config.MaxStaNum;
    for (i = 1; i < max; i++)
        MIi_CpuClear16(0, &pCAM[i], sizeof(pCAM[i]) - sizeof(pCAM[i].maxLifeTime)); // does not include maxLifeTime
    
    pCamMan->Count = 1;
    pCamMan->PowerMgtMode = 0;
    pCamMan->PowerState = 1;
    pCamMan->NotClass3 = 0xFFFE;
    pCamMan->ConnectSta = 0;
    pCamMan->NotSetTIM = 0;
    pCamMan->UseAidMap = 1;
}

static void CAM_InitElement(u32 camAdrs, u16* pMacAdrs) { // CAM.c:1385
    CAM_ELEMENT* pCAM = &wlMan->Config.pCAM[camAdrs]; // r7 - :1387
    
    if (!pCAM->state)
        wlMan->CamMan.Count++;
    
    MIi_CpuClear16(0, pCAM, sizeof(CAM_ELEMENT)-2); // maxLifeTime IS NOT reset
    
    wlMan->CamMan.NotSetTIM &= ~(1 << camAdrs);
    CAM_SetPowerMgtMode(camAdrs, 0);
    CAM_SetAwake(camAdrs);
    
    WSetMacAdrs1(pCAM->macAdrs, pMacAdrs);
    pCAM->lastSeqCtrl = -1;
    pCAM->supRateSet = wlMan->Work.RateSet.Support;
    pCAM->lifeTime = pCAM->maxLifeTime;
    
    CAM_SetStaState(camAdrs, 0x20);
}
