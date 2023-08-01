#include "Mongoose.h"

static void MLME_MeasChanTimeOut(void *unused);
static void MLME_AssTimeOut(void *unused);

u16 MLME_ResetReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:69
    WlMlmeResetReq* pReq = (WlMlmeResetReq*)pReqt; // r0 - :71
    
    pCfmt->header.length = 1;
    
    if (pReq->mib > 1)
        return 5;
    
    WStop();
    
    if (pReq->mib == 1)
        WInitCounter();
    
    return 0;
}

u16 MLME_PwrMgtReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:105
    WlMlmePowerMgtReq* pReq = (WlMlmePowerMgtReq*)pReqt; // r0 - :107
    
    pCfmt->header.length = 9;
    
    if (pReq->pwrMgtMode > 1) return 5;
    if (pReq->wakeUp > 1) return 5;
    if (pReq->recieveDtims > 1) return 5;
    
    WSetPowerMgtMode(pReq->pwrMgtMode);
    
    if (pReq->pwrMgtMode == 1) {
        if (pReq->wakeUp == 1)
            WSetForcePowerState(0x8001);
        else
            WSetForcePowerState(0);
        
        wlMan->Work.RxDtims = pReq->recieveDtims;
        
    } else {
        WSetForcePowerState(0x8000);
        WSetPowerState(2);
    }
    
    return 0;
}

u16 MLME_ScanReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:166
    u32 ch; // r0 - :168
    u32 i; // r8 - :168
    WlMlmeScanReq* pReq = (WlMlmeScanReq*)pReqt; // r0 - :169
    MLME_MAN* pMLME = &wlMan->MLME; // r4 - :170
    
    pMLME->Work.Scan.MaxConfirmLength = pCfmt->header.length - 3; // :177
    pCfmt->header.length = 3; // :180
    
    if (!(wlMan->Config.Mode == 1 || wlMan->Config.Mode == 2 || wlMan->Config.Mode == 3)) // :183
        return 11; // :187
    
    if (wlMan->Work.STA < 0x20) return 1; // :191
    if (pReq->ssidLength > 0x20) return 5; // :198
    if (pReq->scanType > 1) return 5; // :199
    if (!WL_ReadByte(pReq->channelList)) return 5; // :200
    if (pReq->maxChannelTime > 0x3E8) return 5; // :201
    if (pReq->maxChannelTime < 0xA) return 5; // :202
    if (pReq->bssidMaskCount > 0x10) return 5; // :203
    
    for (i = 0; i < 0x10; i++) { // :204
        ch = WL_ReadByte(&pReq->channelList[i]); // :206
        if (!ch) break; // :208
        if (!CheckEnableChannel(ch)) return 5; // :211
    }
    
    WSetBssid(pReq->bssid); // :215
    WSetSsid(pReq->ssidLength, pReq->ssid); // :216
    
    pMLME->pReq.Scan = pReq; // :219
    pMLME->pCfm.Scan = (WlMlmeScanCfm*)pCfmt; // :220
    
    pMLME->State = 16; // :223
    AddTask(2, 0); // :226
    
    return 128; // :228
}

u16 MLME_JoinReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:245
    WORK_PARAM* pWork; // r0 - :247
    MLME_MAN* pMLME; // r4 - :249
    WlMlmeJoinReq* pReq; // r0 - :250
}

u16 MLME_AuthReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:336
    WORK_PARAM* pWork; // r0 - :338
    MLME_MAN* pMLME; // r4 - :340
    WlMlmeAuthReq* pReq; // r0 - :341
    WlMlmeAuthCfm* pCfm; // r0 - :342
}

u16 MLME_DeAuthReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:403
    WlMlmeDeAuthReq* pReq; // r0 - :405
    WlMlmeDeAuthCfm* pCfm; // r0 - :406
    MLME_MAN* pMLME; // r5 - :407
    TXFRM* pFrm; // r0 - :408
}

u16 MLME_AssReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:497
    WlMlmeAssReq* pReq; // r0 - :499
    WORK_PARAM* pWork; // r4 - :500
    MLME_MAN* pMLME; // r5 - :502
}

u16 MLME_ReAssReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:566
    WlMlmeReAssReq* pReq; // r0 - :568
    WORK_PARAM* pWork; // r12 - :570
    MLME_MAN* pMLME; // r14 - :571
}

u16 MLME_DisAssReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:632
    WlMlmeDisAssReq* pReq; // r0 - :634
    MLME_MAN* pMLME; // r5 - :635
    TXFRM* pFrm; // r0 - :636
}

u16 MLME_StartReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:719
    WlMlmeStartReq* pReq; // r0 - :721
    WORK_PARAM* pWork; // r4 - :722
    CONFIG_PARAM* pConfig; // r5 - :723
}

u16 MLME_MeasChanReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:803
    MLME_MAN* pMLME; // r4 - :805
    WlMlmeMeasChanReq* pReq; // r0 - :806
    u32 ch; // r0 - :808
    u32 i; // r8 - :808
}

void MLME_ScanTask() { // MLME.c:886
    MLME_MAN* pMLME; // r8 - :888
    WORK_PARAM* pWork; // r9 - :889
    void* pFrm; // r0 - :890
    u32 bTask; // r4 - :891
    u16 ch; // r0 - :892
}

static void MLME_ScanTimeOut() { // MLME.c:1114
    MLME_MAN* pMLME = &wlMan->MLME; // r0 - :1117
    
    pMLME->Work.Scan.ElapseTime += pMLME->Work.Scan.TxPeriod;
    
    if (pMLME->Work.Scan.ElapseTime >= pMLME->pReq.Scan->maxChannelTime) {
        if (pMLME->Work.Scan.ChannelCount < 0x10) {
            pMLME->State = 17;
        } else {
            pMLME->State = 21;
        }
    }
    AddTask(2, 0);
}

void MLME_JoinTask() { // MLME.c:1173
    MLME_MAN* pMLME; // r4 - :1175
}

static void MLME_JoinTimeOut() { // MLME.c:1249
    MLME_MAN* pMLME = &wlMan->MLME; // r0 - :1252
    pMLME->Work.Join.Result = 7;
    pMLME->State = 37;
    AddTask(2, 1);
}

void MLME_AuthTask() { // MLME.c:1296
    MLME_MAN* pMLME; // r4 - :1298
    AUTH_FRAME* pFrm; // r0 - :1299
}

static void MLME_AuthTimeOut() { // MLME.c:1388
    MLME_MAN* pMLME = &wlMan->MLME; // r0 - :1391
    pMLME->pCfm.Auth->resultCode = 7;
    pMLME->State = 53;
    AddTask(2, 2);
}

void MLME_AssTask() { // MLME.c:1436
    MLME_MAN* pMLME = &wlMan->MLME; // r4 - :1438
    void* pFrm; // r0 - :1439
    
    switch (pMLME->State) {
        case 80:
            pFrm = MakeAssReqFrame(pMLME->pReq.Ass->peerMacAdrs);
            if (!pFrm) {
                pMLME->pCfm.Ass->resultCode = 8;
                pMLME->State = 83;
                AddTask(2, 3);
                break;
            }
            
            pMLME->State = 81;
            TxManCtrlFrame(pFrm);
            SetupTimeOut(pMLME->pReq.Ass->timeOut, MLME_AssTimeOut);
            break;
            
        case 83:
            ResetTxqPri(1);
            ClearQueuedPri(1);
            MessageDeleteTx(1, 0);
            pMLME->State = 0;
            IssueMlmeConfirm();
            break;
    }
}

static void MLME_AssTimeOut(void *unused) { // MLME.c:1523
    MLME_MAN* pMLME = &wlMan->MLME; // r0 - :1526
    pMLME->pCfm.Ass->resultCode = 7;
    pMLME->State = 83;
    AddTask(2, 3);
}

void MLME_ReAssTask() { // MLME.c:1570
    MLME_MAN* pMLME; // r4 - :1573
    void* pFrm; // r0 - :1574
}

static void MLME_ReAssTimeOut() { // MLME.c:1651
    MLME_MAN* pMLME = &wlMan->MLME; // r0 - :1654
    pMLME->pCfm.ReAss->resultCode = 7;
    pMLME->State = 99;
    AddTask(2, 4);
}

void MLME_MeasChannelTask() { // MLME.c:1695
    WORK_PARAM* pWork = &wlMan->Work; // r6 - :1697
    MLME_MAN* pMLME = &wlMan->MLME; // r7 - :1698
    u32 ch, ratio; // r0, r2 - :1699
    
    switch (pMLME->State) {
        case 128:
            pMLME->Work.Measure.Channel = 0;
            pMLME->Work.Measure.bkCCAMode = BBP_Read(0x13);
            pMLME->Work.Measure.bkEdTh = BBP_Read(0x35);
            WSetCCA_ED(pMLME->pReq.MeasChannel->ccaMode, pMLME->pReq.MeasChannel->edThreshold);
            pWork->Mode = 4;
            pMLME->Work.Measure.sts = 0;
            // go to 129
            
        case 129:
            pMLME->Work.Measure.Counter = 0;
            pMLME->Work.Measure.CCA = 0;
            ch = WL_ReadByte(&pMLME->pReq.MeasChannel->channelList[pMLME->Work.Measure.Channel]);
            if (ch == 0 || pMLME->Work.Measure.Channel >= 16) {
                pMLME->State = 132;
                break;
            }
                
            if (FLASH_VerifyChecksum(0)) {
                pMLME->Work.Measure.sts = 14;
                pMLME->State = 132;
                break;
            }
                
            if (pMLME->State == 128) {
                WSetChannel(ch, 0);
                WStart();
                pMLME->Work.Measure.bkPowerMode = W_POWERFORCE;
                WSetForcePowerState(0x8000);
                
            } else {
                WSetChannel(ch, 0);
            }
            
            pMLME->State = 130;
            SetupTimeOut(pMLME->pReq.MeasChannel->measureTime, MLME_MeasChanTimeOut);
            // go to 130
            
        case 130:
            pMLME->Work.Measure.Counter++;
            if (W_RF_PINS & 1)
                pMLME->Work.Measure.CCA += 100;
            
            break;
            
        case 131:
            ch = WL_ReadByte(&pMLME->pReq.MeasChannel->channelList[pMLME->Work.Measure.Channel]);
            ratio = 0;
            
            if (pMLME->Work.Measure.Counter != 0 && pMLME->Work.Measure.CCA != 0) {
                ratio = 1 + (pMLME->Work.Measure.CCA / pMLME->Work.Measure.Counter);
                ratio = (ratio > 100) ? 100 : ratio;
            }
            
            pMLME->pCfm.MeasChannel->ccaBusyInfo[pMLME->Work.Measure.Channel] = ch | (ratio << 8);
            pMLME->Work.Measure.Channel++;
            pMLME->State = 129;
            break;
            
        case 132:
            WStop();
            pWork->Mode = wlMan->Config.Mode;
            BBP_Write(0x13, pMLME->Work.Measure.bkCCAMode);
            BBP_Write(0x35, pMLME->Work.Measure.bkEdTh);
            WSetForcePowerState(pMLME->Work.Measure.bkPowerMode);
            pMLME->pCfm.MeasChannel->resultCode = pMLME->Work.Measure.sts;
            pMLME->State = 0;
            
            for (ch = pMLME->Work.Measure.Channel; ch < 16; ch++) {
                pMLME->pCfm.MeasChannel->ccaBusyInfo[ch] = 0;
            }
            
            IssueMlmeConfirm();
            break;
            
        default:
            break;
    }
    
    if (pMLME->State)
        AddTask(2, 5);
}

static void MLME_MeasChanTimeOut(void *unused) { // MLME.c:1871
    wlMan->MLME.State = 131;
    AddTask(2, 5);
}

void MLME_BeaconLostTask() { // MLME.c:1903
    MLME_IssueBeaconLostIndication(wlMan->Work.LinkAdrs);
}

void IssueMlmeConfirm() { // MLME.c:1934
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r4 - :1936
    CMDIF_MAN* pCmdIf = &wlMan->CmdIf; // r0 - :1937
    
    pCmdIf->Busy &= ~1;
    SendMessageToWmDirect(&pHeapMan->RequestCmd, pCmdIf->pCmd);
    
    if (pHeapMan->RequestCmd.Count)
        AddTask(2, 11);
}

u32 MLME_IssueAuthIndication(u16* pMacAdrs, u16 algorithm) { // MLME.c:1981
    WlMlmeAuthInd* pInd; // r0 - :1983
    
    pInd = (WlMlmeAuthInd*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, sizeof(*pInd));
    
    if (!pInd) {
        SetFatalErr(1);
        return 0;
    }
    
    pInd->header.code = 132;
    pInd->header.length = 4;
    WSetMacAdrs1(pInd->peerMacAdrs, pMacAdrs);
    pInd->algorithm = algorithm;
    
    SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
    return 1;
}

u32 MLME_IssueDeAuthIndication(u16* pMacAdrs, u16 reason) { // MLME.c:2022
    WlMlmeDeAuthInd* pInd; // r0 - :2024
    
    pInd = (WlMlmeDeAuthInd*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, sizeof(*pInd));
    
    if (!pInd) {
        SetFatalErr(1);
        return 0;
    }
    
    pInd->header.code = 133;
    pInd->header.length = 4;
    WSetMacAdrs1(pInd->peerMacAdrs, pMacAdrs);
    pInd->reasonCode = reason;
    
    SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
    return 1;
}

u32 MLME_IssueAssIndication(u16* pMacAdrs, u16 aid, SSID_ELEMENT* pSSID) { // MLME.c:2063
    WlMlmeAssInd* pInd; // r4 - :2065
    u32 i; // r5 - :2066
    
    pInd = (WlMlmeAssInd*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, sizeof(*pInd));
    
    if (!pInd) {
        SetFatalErr(1);
        return 0;
    }
    
    pInd->header.code = 134;
    pInd->header.length = 21;
    
    WSetMacAdrs1(pInd->peerMacAdrs, pMacAdrs);
    pInd->aid = aid & 0xFFF;
    pInd->ssidLength = WL_ReadByte(&pSSID->Length);
    
    // write ssid
    for (i = 0; i < pInd->ssidLength; i++) {
        if (i >= sizeof(pInd->ssid))
            break;
        
        WL_WriteByte(&pInd->ssid[i], WL_ReadByte(&pSSID->SSID[i]));
    }
    
    // zero the remaining bytes
    for (; i < sizeof(pInd->ssid); i++) {
        WL_WriteByte(&pInd->ssid[i], 0);
    }
    
    SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
    return 1;
}

u32 MLME_IssueReAssIndication(u16* pMacAdrs, u16 aid, SSID_ELEMENT* pSSID) { // MLME.c:2124
    WlMlmeReAssInd* pInd; // r4 - :2126
    u32 i; // r5 - :2127
    
    pInd = (WlMlmeReAssInd*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, sizeof(*pInd));
    
    if (!pInd) {
        SetFatalErr(1);
        return 0;
    }
    
    pInd->header.code = 135;
    pInd->header.length = 21;
    
    WSetMacAdrs1(pInd->peerMacAdrs, pMacAdrs);
    pInd->aid = aid;
    pInd->ssidLength = WL_ReadByte(&pSSID->Length);
    
    // write ssid
    for (i = 0; i < pInd->ssidLength; i++) {
        if (i >= sizeof(pInd->ssid))
            break;
        
        WL_WriteByte(&pInd->ssid[i], WL_ReadByte(&pSSID->SSID[i]));
    }
    
    // zero the remaining bytes
    for (; i < sizeof(pInd->ssid); i++) {
        WL_WriteByte(&pInd->ssid[i], 0);
    }
    
    SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
    
    return 1;
}

u32 MLME_IssueDisAssIndication(u16* pMacAdrs, u16 reason) { // MLME.c:2185
    WlMlmeDisAssInd* pInd; // r0 - :2187
    
    pInd = (WlMlmeDisAssInd*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, sizeof(*pInd));
    
    if (!pInd) {
        SetFatalErr(1);
        return 0;
    }
    
    pInd->header.code = 136;
    pInd->header.length = 4;
    WSetMacAdrs1(pInd->peerMacAdrs, pMacAdrs);
    pInd->reasonCode = reason;
    
    SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
    return 1;
}

u32 MLME_IssueBeaconLostIndication(u16* pMacAdrs) { // MLME.c:2227
    WlMlmeBeaconLostInd* pInd; // r0 - :2229
    
    pInd = (WlMlmeBeaconLostInd*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, sizeof(*pInd));
    
    if (!pInd) {
        SetFatalErr(1);
        return 0;
    }
    
    pInd->header.code = 139;
    pInd->header.length = 3;
    WSetMacAdrs1(pInd->apMacAdrs, pMacAdrs);
    
    SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
    return 1;
}

u32 MLME_IssueBeaconSendIndication() { // MLME.c:2267
    WlMlmeBeaconSendInd* pInd; // r0 - :2269
    
    pInd = (WlMlmeBeaconSendInd*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, sizeof(*pInd));
    
    if (!pInd) {
        SetFatalErr(1);
        return 0;
    }
    
    pInd->header.code = 140;
    pInd->header.length = 0;
    SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
    return 1;
}

u32 MLME_IssueBeaconRecvIndication(void* pRxFrm) { // MLME.c:2304
    WORK_PARAM* pWork; // r4 - :2306
    RXFRM* pFrm; // r0 - :2307
    WlMlmeBeaconRecvInd* pInd; // r5 - :2308
    u32 i; // r4 - :2309
    u8* p1; // r6 - :2310
    u8* p2; // r7 - :2310
}

void InitializeMLME() { // MLME.c:2382
    MIi_CpuClear(0, &wlMan->MLME, sizeof(wlMan->MLME));
}
