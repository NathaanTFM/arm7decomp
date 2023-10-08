#include "Mongoose.h"

STATIC void MLME_ScanTimeOut(void *unused);
static void MLME_JoinTimeOut(void *unused);
static void MLME_AuthTimeOut(void *unused);
static void MLME_AssTimeOut(void *unused);
static void MLME_ReAssTimeOut(void *unused);
static void MLME_MeasChanTimeOut(void *unused);

u16 MLME_ResetReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:69
    WlMlmeResetReq* pReq = (WlMlmeResetReq*)pReqt; // r0 - :71
    WlMlmeResetCfm* pCfm = (WlMlmeResetCfm*)pCfmt; // r0 - :71
    
    pCfm->header.length = 1;
    
    if (pReq->mib > 1)
        return 5;
    
    WStop();
    
    if (pReq->mib == 1)
        WInitCounter();
    
    return 0;
}

u16 MLME_PwrMgtReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:105
    WlMlmePowerMgtReq* pReq = (WlMlmePowerMgtReq*)pReqt; // r0 - :107
    WlMlmePowerMgtCfm* pCfm = (WlMlmePowerMgtCfm*)pCfmt; // r0 - :107
    
    pCfm->header.length = 9;
    
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
    WlMlmeScanCfm* pCfm = (WlMlmeScanCfm*)pCfmt; // not in nef, probably absent
    MLME_MAN* pMLME = &wlMan->MLME; // r4 - :170
    CONFIG_PARAM* pConfig = &wlMan->Config; // not in nef, probably exists
    
    pMLME->Work.Scan.MaxConfirmLength = pCfm->header.length - 3; // :177
    pCfm->header.length = 3; // :180
    
    if (pConfig->Mode != 1 && pConfig->Mode != 3 && pConfig->Mode != 2) // :183
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
    pMLME->pCfm.Scan = pCfm; // :220
    
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
    WORK_PARAM* pWork = &wlMan->Work; // r0 - :338
    CONFIG_PARAM* pConfig = &wlMan->Config; // not in nef, probably exists
    MLME_MAN* pMLME = &wlMan->MLME; // r4 - :340
    WlMlmeAuthReq* pReq = (WlMlmeAuthReq*)pReqt; // r0 - :341
    WlMlmeAuthCfm* pCfm = (WlMlmeAuthCfm*)pCfmt; // r0 - :342
    
    pCfm->header.length = 6;
    
    if (pConfig->Mode != 3 && pConfig->Mode != 2)
        return 11;
    
    if (pWork->STA < 0x20) return 1;
    if (pReq->peerMacAdrs[0] & 1) return 5;
    if (pReq->algorithm > 1) return 5;
    if (pReq->timeOut > 2000) return 5;
    if (pReq->timeOut < 10) return 5;
    
    WSetStaState(0x20);
    
    pMLME->pReq.Auth = pReq;
    pMLME->pCfm.Auth = pCfm;
    pMLME->State = 48;
    
    pCfm->algorithm = pReq->algorithm;
    WSetMacAdrs1(pCfm->peerMacAdrs, pMLME->pReq.Auth->peerMacAdrs);
    
    MLME_AuthTask();
    return 128;
}

u16 MLME_DeAuthReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:403
    WlMlmeDeAuthReq* pReq; // r0 - :405
    WlMlmeDeAuthCfm* pCfm; // r0 - :406
    MLME_MAN* pMLME; // r5 - :407
    TXFRM* pFrm; // r0 - :408
}

u16 MLME_AssReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:497
    WlMlmeAssReq* pReq = (WlMlmeAssReq*)pReqt; // r0 - :499
    WlMlmeAssCfm* pCfm = (WlMlmeAssCfm*)pCfmt; // not in nef
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :500
    CONFIG_PARAM* pConfig = &wlMan->Config; // not in nef, exists probably
    MLME_MAN* pMLME = &wlMan->MLME; // r5 - :502
    
    pCfm->header.length = 3;
    if (pConfig->Mode != 3 && pConfig->Mode != 2)
        return 11;
    
    if (pWork->STA < 0x30) return 1;
    if (pReq->peerMacAdrs[0] & 1) return 5;
    if (pReq->listenInterval == 0) return 5;
    if (pReq->listenInterval > 0xFF) return 5;
    if (pReq->timeOut > 2000) return 5;
    if (pReq->timeOut < 10) return 5;
    
    WSetStaState(0x30);
    WClearAids();
    
    pWork->ListenInterval = pReq->listenInterval;
    pWork->CurrListenInterval = pReq->listenInterval;
    
    pMLME->pReq.Ass = pReq;
    pMLME->pCfm.Ass = pCfm;
    pMLME->State = 80;
    
    MLME_AssTask();
    return 128;
}

u16 MLME_ReAssReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:566
    WlMlmeReAssReq* pReq = (WlMlmeReAssReq*)pReqt; // r0 - :568
    WlMlmeReAssCfm* pCfm = (WlMlmeReAssCfm*)pCfmt; // not in nef
    CONFIG_PARAM* pConfig = &wlMan->Config; // not in nef, exists probably (:569)
    WORK_PARAM* pWork = &wlMan->Work; // r12 - :570
    MLME_MAN* pMLME = &wlMan->MLME; // r14 - :571
    
    pCfm->header.length = 3;
    if (pConfig->Mode != 3 && pConfig->Mode != 2)
        return 11;
    
    if (pWork->STA < 0x30) return 1;
    if (pReq->newApMacAdrs[0] & 1) return 5;
    if (pReq->listenInterval < 1) return 5;
    if (pReq->listenInterval > 0xFF) return 5;
    if (pReq->timeOut > 2000) return 5;
    if (pReq->timeOut < 10) return 5;
    
    pWork->ListenInterval = pReq->listenInterval;
    pWork->CurrListenInterval = pReq->listenInterval;
    
    pMLME->pReq.ReAss = pReq;
    pMLME->pCfm.ReAss = pCfm;
    pMLME->State = 96;
    
    MLME_ReAssTask();
    return 128;
}

u16 MLME_DisAssReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:632
    WlMlmeDisAssReq* pReq = (WlMlmeDisAssReq*)pReqt; // r0 - :634
    WlMlmeDisAssCfm *pCfm = (WlMlmeDisAssCfm*)pCfmt; // not in nef
    MLME_MAN* pMLME = &wlMan->MLME; // r5 - :635
    TXFRM* pFrm; // r0 - :636
    
    pCfm->header.length = 1;
    if (wlMan->Config.Mode == 0)
        return 11;
    
    if (wlMan->Config.Mode != 1 && (pReq->peerMacAdrs[0] & 1) != 0)
        return 5;
    
    if (wlMan->Work.STA != 64)
        return 1;
    
    pFrm = (TXFRM*)MakeDisAssFrame(pReq->peerMacAdrs, pReq->reasonCode);
    if (!pFrm)
        return 8;
    
    pMLME->pReq.DisAss = pReq;
    pMLME->pCfm.DisAss = pCfm;
    pMLME->Work.DisAss.pTxFrm = pFrm;
    pMLME->State = 113;
    
    if (pReq->peerMacAdrs[0] & 1) {
        pFrm->FirmHeader.FrameTime = wlMan->Work.IntervalCount;
        CAM_AddBcFrame(&wlMan->HeapMan.TmpBuf, SubtractAddr(pFrm, 0x10));
        if ((wlMan->CamMan.PowerMgtMode & ~wlMan->CamMan.NotClass3) == 0)
            TxqPri(2);
        
    } else {
        DeleteTxFrameByAdrs(pReq->peerMacAdrs);
        TxManCtrlFrame(pFrm);
    }
    return 128;
}

u16 MLME_StartReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:719
    WlMlmeStartReq* pReq; // r0 - :721
    WORK_PARAM* pWork; // r4 - :722
    CONFIG_PARAM* pConfig; // r5 - :723
}

u16 MLME_MeasChanReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MLME.c:803
    MLME_MAN* pMLME = &wlMan->MLME; // r4 - :805
    WlMlmeMeasChanReq* pReq = (WlMlmeMeasChanReq*)pReqt; // r0 - :806
    WlMlmeMeasChanCfm *pCfm = (WlMlmeMeasChanCfm*)pCfmt; // not in nef
    u32 i, ch; // r8, r0 - :808
    
    pCfm->header.length = 18;
    if (wlMan->Work.STA != 0x20) return 1;
    if (pReq->ccaMode > 3) return 5;
    if (pReq->edThreshold > 0x3F) return 5;
    if (pReq->measureTime == 0) return 5;
    if (pReq->measureTime > 1000) return 5;
    
    for (i = 0; i < sizeof(pReq->channelList) / sizeof(*pReq->channelList); i++) {
        ch = WL_ReadByte(&pReq->channelList[i]);
        if (ch == 0)
            break;
        
        if (!CheckEnableChannel(ch))
            return 5;
    }
    
    if (i == 0)
        return 5;
    
    pMLME->pReq.MeasChannel = pReq;
    pMLME->pCfm.MeasChannel = pCfm;
    pMLME->State = 128;
    
    pCfm->resultCode = 128;
    
    MLME_MeasChannelTask();
    return 128;
}

void MLME_ScanTask() { // MLME.c:886
    MLME_MAN* pMLME; // r8 - :888
    WORK_PARAM* pWork; // r9 - :889
    void* pFrm; // r0 - :890
    u32 bTask; // r4 - :891
    u16 ch; // r0 - :892
}

STATIC void MLME_ScanTimeOut(void *unused) { // MLME.c:1114
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
    MLME_MAN* pMLME = &wlMan->MLME; // r4 - :1175
    
    switch (pMLME->State) {
        case 32:
            WStart();
            pMLME->Work.Join.Result = 0;
            pMLME->Work.Join.Status = 0;
            pMLME->State = 33;
            SetupTimeOut(pMLME->pReq.Join->timeOut, MLME_JoinTimeOut);
            break;
            
        case 37:
            pMLME->pCfm.Join->resultCode = pMLME->Work.Join.Result;
            pMLME->pCfm.Join->statusCode = pMLME->Work.Join.Status;
            
            if (pMLME->Work.Join.Result != 0)
                WStop();
            
            pMLME->State = 0;
            IssueMlmeConfirm();
            break;
    }
}

static void MLME_JoinTimeOut(void *unused) { // MLME.c:1249
    MLME_MAN* pMLME = &wlMan->MLME; // r0 - :1252
    pMLME->Work.Join.Result = 7;
    pMLME->State = 37;
    AddTask(2, 1);
}

void MLME_AuthTask() { // MLME.c:1296
    MLME_MAN* pMLME = &wlMan->MLME; // r4 - :1298
    AUTH_FRAME* pFrm; // r0 - :1299
    
    switch (pMLME->State) {
        case 48:
            pFrm = MakeAuthFrame(pMLME->pReq.Auth->peerMacAdrs, 0, 0);
            if (!pFrm) {
                pMLME->pCfm.Auth->resultCode = 8;
                pMLME->State = 53;
                AddTask(2, 2);
                break;
            }
            
            pFrm->Body.AlgoType = pMLME->pReq.Auth->algorithm;
            pFrm->Body.SeqNum = 1;
            pFrm->Body.StatusCode = 0;
            
            pMLME->State = 49;
            
            TxManCtrlFrame((TXFRM*)pFrm);
            SetupTimeOut(pMLME->pReq.Auth->timeOut, MLME_AuthTimeOut);
            break;
            
        case 53:
            ResetTxqPri(1);
            ClearQueuedPri(1);
            MessageDeleteTx(1, 0);
            pMLME->State = 0;
            IssueMlmeConfirm();
            break;
    }
}

static void MLME_AuthTimeOut(void *unused) { // MLME.c:1388
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
    MLME_MAN* pMLME = &wlMan->MLME; // r4 - :1573
    void* pFrm; // r0 - :1574
    
    switch (pMLME->State) {
        case 96:
            pFrm = MakeReAssReqFrame(pMLME->pReq.ReAss->newApMacAdrs);
            if (!pFrm) {
                pMLME->pCfm.ReAss->resultCode = 8;
                pMLME->State = 99;
                AddTask(2, 4);
                break;
            }
            
            pMLME->State = 97;
            TxManCtrlFrame(pFrm);
            SetupTimeOut(pMLME->pReq.ReAss->timeOut, MLME_ReAssTimeOut);
            break;
            
        case 99:
            ClearQueuedPri(1);
            MessageDeleteTx(1, 0);
            pMLME->State = 0;
            IssueMlmeConfirm();
            break;
    }
}

static void MLME_ReAssTimeOut(void *unused) { // MLME.c:1651
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
                
            if (FLASH_VerifyCheckSum(0)) {
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
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :2306
    RXFRM* pFrm = (RXFRM*)pRxFrm; // r0 - :2307
    WlMlmeBeaconRecvInd* pInd; // r5 - :2308
    u32 i; // r4 - :2309
    u8 *p1, *p2; // r6, r7 - :2310
    
    pInd = (WlMlmeBeaconRecvInd*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, wlMan->Work.GameInfoLength + 62); // sizeof?
    if (!pInd) {
        SetFatalErr(1);
        return 0;
    }
    
    pInd->header.code = 141;
    pInd->header.length = (pWork->GameInfoLength + 1) / 2 + 22;
    WL_WriteByte(&pInd->rssi, pFrm->MacHeader.Rx.rsv_RSSI);
    WL_WriteByte(&pInd->rate, pFrm->MacHeader.Rx.Service_Rate);
    WSetMacAdrs1(pInd->srcMacAdrs, pFrm->Dot11Header.Adrs2);
    pInd->gameInfoLength = pWork->GameInfoLength;
    
    if (pInd->gameInfoLength) {
        if (pWork->GameInfoAlign & 1) {
            p1 = (u8*)pWork->GameInfoAdrs + 1;
            p2 = (u8*)pInd->gameInfo;
            
            for (i = 0; i < pInd->gameInfoLength; i++) {
                WL_WriteByte(p2, WL_ReadByte(p1));
                p1++;
                p2++;
            }
            
            
        } else {
            MIi_CpuCopy16(pWork->GameInfoAdrs, pInd->gameInfo, pInd->gameInfoLength + 1);
        }
    }
    
    SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
    return 1;
}

void InitializeMLME() { // MLME.c:2382
    MIi_CpuClear16(0, &wlMan->MLME, sizeof(wlMan->MLME));
}
