// temporary: we remove static functions
#define STATIC

#include "Mongoose.h"

STATIC void ElementChecker(ELEMENT_CHECKER* p);

/*
Empty function IPA

void RxDataFrameTask() { // RxCtrl.c:66
    DEAUTH_FRAME* pDeAuth; // r0 - :68
    WlMaDataInd* pInd; // r5 - :69
    RXFRM* pFrm; // r0 - :70
    CONFIG_PARAM* pConfig; // r0 - :71
    WORK_PARAM* pWork; // r6 - :72
    MLME_MAN* pMLME; // r1 - :73
    HEAP_MAN* pHeapMan; // r7 - :74
    WlCounter* pCounter; // r0 - :75
    u32 multiCast; // r2 - :76
    u32 err; // r9 - :76
    u32 camAdrs; // r8 - :76
}
*/

u32 RxMpFrame(RXFRM* pFrm) { // RxCtrl.c:324
    WORK_PARAM* pWork = &wlMan->Work; // r5 - :326
    RX_CTRL* pRxCtrl = &wlMan->RxCtrl; // r6 - :327
    WlMaMpInd* pInd; // r5 - :328
    u32 cnt, x; // r7, r0 - :329
    u16 bitmap, out; // r2, r0 - :330
    
    if (pWork->STA != 0x40)
        return 1;
    
    if (!MatchMacAdrs(pFrm->Dot11Header.Adrs2, pWork->BSSID) || !MatchMacAdrs(pFrm->Dot11Header.Adrs3, pWork->LinkAdrs))
        return 1;
    
    if (( ((WlRxMpFrame*)pFrm)->bitmap & (u16)(1 << pWork->AID)) == 0) // weird cast
        pRxCtrl->TxKeyReg = 0;
    else
        pRxCtrl->TxKeyReg = 0x2000;
    
    out = W_TXBUF_REPLY2;
    if ((out & 0x8000) != 0 && (&W_MACMEM((u16)(2 * out)))[2] != 0) // TODO: ugliest thing in the code
        pRxCtrl->TxKeyReg |= 0x4000;
    
    CAM_UpdateLifeTime(pWork->APCamAdrs);
    pInd = SubtractAddr(pFrm, 0x10);
    pFrm->FirmHeader.Length = pFrm->MacHeader.Rx.MPDU - 28;
    
    pInd->header.code = 0x182; // ?
    pInd->header.length = ((u32)pFrm->FirmHeader.Length + 49) / 2;
    
    bitmap = pInd->frame.bitmap;
    cnt = 0;
    while (bitmap) {
        if (bitmap & 1)
            ++cnt;
        
        bitmap /= 2;
    }
    
    x = OS_DisableIrqMask(0x1000000);
    pInd->frame.txKeySts = pRxCtrl->TxKeyFrm | pRxCtrl->TxKeyReg | ((W_TXBUF_REPLY2 & 0x8000) >> 4) | ((W_TXBUF_REPLY1 & 0x8000) >> 3);
    pRxCtrl->TxKeyFrm = 0;
    OS_EnableIrqMask(x);
    
    pInd->frame.ackTimeStamp = pInd->frame.timeStamp + ((cnt * (pInd->frame.txop + 10) + 252) >> 4);
    SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
    return 0;
}

/*
Empty function IPA

void RxKeyDataFrame(RXFRM* pFrm) { // RxCtrl.c:450
    TX_CTRL* pTxCtrl; // r5 - :452
    WlMpKey* pMpKey; // r0 - :453
    WlMpKeyData* pKeyData; // r6 - :454
    u32 aid; // r0 - :455
    u32 camAdrs; // r6 - :455
    u16 bitmap; // r1 - :456
    DEAUTH_FRAME* pTxDeAuthFrm; // r0 - :519
}
*/

u32 RxMpAckFrame(RXFRM* pFrm) { // RxCtrl.c:645
    WORK_PARAM* pWork = &wlMan->Work; // r6 - :647
    WlMaMpAckInd* pInd; // r0 - :648
    
    if (wlMan->Work.STA != 0x40)
        return 1;
    
    if (!MatchMacAdrs(pFrm->Dot11Header.Adrs2, pWork->BSSID) || !MatchMacAdrs(pFrm->Dot11Header.Adrs3, pWork->LinkAdrs))
        return 1;
    
    pInd = SubtractAddr(pFrm, 0x10);
    pFrm->FirmHeader.Length = pFrm->MacHeader.Rx.MPDU - 28;
    
    pInd->header.code = 0x185; // ?
    pInd->header.length = 24;
    pInd->ack.txKeySts |= wlMan->RxCtrl.TxKeyReg | ((W_TXBUF_REPLY2 & 0x8000) >> 4) | ((W_TXBUF_REPLY1 & 0x8000) >> 3);
    
    SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
    return 0;
}

/*
Empty function IPA

void RxBeaconFrame(BEACON_FRAME* pFrm) { // RxCtrl.c:740
    BEACON_BODY* pBeacon; // r0 - :742
    WORK_PARAM* pWork; // r7 - :743
    MLME_MAN* pMLME; // r4 - :744
    CONFIG_PARAM* pConfig; // r8 - :745
    HEAP_MAN* pHeapMan; // r9 - :746
    ELEMENT_CHECKER elementCheck; // None - :747
    u32 ofst; // r4 - :748
    u32 n2; // r0 - :748
    u32 n1; // r4 - :748
    u32 n; // r0 - :748
    u32 cam_adrs; // r11 - :749
    u32 BodyLen; // r5 - :749
    u64 lltsf; // None - :750
    u16 t2[4]; // None - :751
    u16 t1[4]; // None - :751
    u16* ptsf; // r0 - :752
    u32 actZone; // r0 - :886
    u16 dtimCnt; // r0 - :1039
}
*/

STATIC void RxDisAssFrame(DISASS_FRAME* pFrm) { // RxCtrl.c:1146
    WORK_PARAM* pWork = &wlMan->Work; // r1 - :1148
    DEAUTH_FRAME* pDeAuth; // r0 - :1149
    u32 st, cam_adrs = pFrm->FirmHeader.CamAdrs; // r0, r5 - :1150
    
    switch (pWork->Mode) {
        case 1:
            st = CAM_GetStaState(cam_adrs);
            
            if (st == 0x40) {
                CAM_SetStaState(cam_adrs, 0x30);
                MLME_IssueDisAssIndication(pFrm->Dot11Header.SA, pFrm->Body.ReasonCode);
                DeleteTxFrames(cam_adrs);
                
            } else {
                if (st == 0x30) {
                    // ugly cast is ugly. should just have been a TXFRM*
                    pDeAuth = (DEAUTH_FRAME*)MakeDisAssFrame(pFrm->Dot11Header.SA, 7);
                    
                } else {
                    pDeAuth = MakeDeAuthFrame(pFrm->Dot11Header.SA, 7, 1);
                }
                
                if (pDeAuth)
                    TxManCtrlFrame((TXFRM*)pDeAuth);
            }
            
            break;
            
        case 2:
        case 3:
            if (pWork->STA == 0x40 && MatchMacAdrs(pFrm->Dot11Header.SA, pWork->LinkAdrs)) {
                WSetStaState(0x30);
                WClearAids();
                MLME_IssueDisAssIndication(pFrm->Dot11Header.SA, pFrm->Body.ReasonCode);
            }
            break;
    }
}

STATIC void RxAssReqFrame(ASSREQ_FRAME* pFrm) { // RxCtrl.c:1228
    CONFIG_PARAM* pConfig = &wlMan->Config; // r4 - :1230
    ASSREQ_BODY* pAssReq = &pFrm->Body; // r0 - :1231
    DEAUTH_FRAME* pTxDeAuthFrm; // r0 - :1232
    ASSRES_FRAME* pTxFrm; // r0 - :1233
    u32 bodyLen; // r5 - :1234
    u16 stsCode, cam_adrs; // r0, r0 - :1235
    ELEMENT_CHECKER elementCheck; // None - :1236
    
    bodyLen = pFrm->FirmHeader.Length; // :1239
    
    if (bodyLen <= 4) // :1244
        return;
    
    if (wlMan->Work.Mode != 1) // :1258
        return;
    
    if (IsExistManFrame(pFrm->Dot11Header.SA, 0x10)) // :1265
        return;
    
    cam_adrs = pFrm->FirmHeader.CamAdrs; // :1268
    
    if (cam_adrs == 0 || CAM_GetStaState(cam_adrs) < 0x30) { // :1272
        if (IsExistManFrame(pFrm->Dot11Header.SA, 0xC0)) // :1276
            return;
            
        pTxDeAuthFrm = MakeDeAuthFrame(pFrm->Dot11Header.SA, 6, 1); // :1279
        if (pTxDeAuthFrm) // :1280
            TxManCtrlFrame((TXFRM*)pTxDeAuthFrm); // :1282
        
        return;
    }
    
    if (CAM_GetStaState(cam_adrs) == 0x40) {
        CAM_SetStaState(cam_adrs, 0x30);
        MLME_IssueDisAssIndication(pFrm->Dot11Header.SA, 1);    
    }
    
    else if (CAM_GetAID(cam_adrs) != 0) {
        return;
    }
    
    MIi_CpuClear32(0, &elementCheck, sizeof(ELEMENT_CHECKER));
    elementCheck.pElement = pAssReq->Buf;
    elementCheck.bodyLength = bodyLen - 4;
    ElementChecker(&elementCheck);
    
    // holy
    if (
        (pAssReq->CapaInfo.Data & 0xFFC2) != 0
        || (pAssReq->CapaInfo.Bit.ESS == 0)
        || (!pConfig->WepMode && pAssReq->CapaInfo.Bit.Privacy == 1)
        || (pConfig->WepMode && pAssReq->CapaInfo.Bit.Privacy == 0)
        || (pConfig->PreambleType == 1 && pAssReq->CapaInfo.Bit.ShortPreamble == 0)
    ) {
        stsCode = 10;
        
    } else {
        CAM_SetCapaInfo(cam_adrs, pAssReq->CapaInfo.Data); // :1325
        
        if ((elementCheck.matchFlag & 1) == 0) { // :1328
            stsCode = 1; // :1331
            
        } else if ((elementCheck.matchFlag & 4) == 0) { // :1336
            stsCode = 18; // :1341
            
        } else {
            CAM_SetSupRate(cam_adrs, elementCheck.rateSet.Support); // :1346
            stsCode = 0; // :1349
        }
    }
    
    pTxFrm = MakeAssResFrame(cam_adrs, stsCode, elementCheck.pSSID); // :1354
    if (pTxFrm)
        TxManCtrlFrame((TXFRM*)pTxFrm);
}

STATIC void RxProbeReqFrame(PRBREQ_FRAME* pFrm) { // RxCtrl.c:1703
    PRBRES_FRAME* pTxFrm; // r0 - :1705
    ELEMENT_CHECKER elementCheck; // None - :1706
    
    if (IsExistManFrame(pFrm->Dot11Header.SA, 0x50))
        return;
    
    if ((pFrm->Dot11Header.BSSID[0] & 1) || (pFrm->MacHeader.Rx.Status & 0x8000)) {
        MIi_CpuClear32(0, &elementCheck, sizeof(ELEMENT_CHECKER));
        elementCheck.pElement = pFrm->Body.Buf;
        elementCheck.bodyLength = pFrm->FirmHeader.Length;
        if (!wlMan->Config.BcSsidResponse)
            elementCheck.foundFlag = 0x800;
        
        ElementChecker(&elementCheck);
        
        if ((elementCheck.matchFlag & 1) == 1) {
            pTxFrm = MakeProbeResFrame(pFrm->Dot11Header.SA);
            if (pTxFrm)
                TxManCtrlFrame((TXFRM*)pTxFrm);
        }
    }
    
}

/*
IPA prevention stuff

STATIC void RxProbeResFrame(PRBRES_FRAME* pFrm, ELEMENT_CHECKER* pChk) { // RxCtrl.c:1758
    MLME_MAN* pMLME; // r6 - :1760
    WlMlmeScanCfm* pCfm; // r7 - :1761
    PRBRES_BODY* pPrbRes; // r0 - :1762
    WlBssDesc* pDesc; // r8 - :1763
    u32 len; // r0 - :1764
    u32 id; // r0 - :1764
    u32 j; // r5 - :1764
    u32 i; // r8 - :1764
    u32 bodyLen; // r9 - :1764
    ELEMENT_CHECKER elementCheck; // None - :1765
    u16* pBssidMask; // r9 - :1766
    u8* pDst; // r11 - :1767
    u8* pSrc; // r9 - :1767
}

STATIC void RxAuthFrame(AUTH_FRAME* pFrm) { // RxCtrl.c:1993
    WORK_PARAM* pWork; // r4 - :1995
    MLME_MAN* pMLME; // r5 - :1996
    AUTH_BODY* pAuth; // r0 - :1997
    AUTH_FRAME* pTxFrm; // r4 - :1998
    u32 cam_adrs; // r6 - :1999
    u32 bTxAuth; // r7 - :2000
    u16 stsCode; // r9 - :2001
    u16 seqNum; // r8 - :2001
    TXQ* pTxq; // r1 - :2013
    u16 map; // r0 - :2014
}
*/

STATIC void RxDeAuthFrame(DEAUTH_FRAME* pFrm) { // RxCtrl.c:2418
    WORK_PARAM* pWork = &wlMan->Work; // r1 - :2420
    u32 cam_adrs = pFrm->FirmHeader.CamAdrs; // r5 - :2422
    
    switch (pWork->Mode) {
        case 1:
            if (CAM_GetStaState(cam_adrs) > 0x20) {
                CAM_SetStaState(cam_adrs, 0x20);
                MLME_IssueDeAuthIndication(pFrm->Dot11Header.SA, pFrm->Body.ReasonCode);
                DeleteTxFrames(cam_adrs);
            }
            break;
            
        case 2:
        case 3:
            if (pWork->STA > 0x20 && MatchMacAdrs(pFrm->Dot11Header.SA, pWork->LinkAdrs)) {
                WSetStaState(0x20);
                WClearAids();
                MLME_IssueDeAuthIndication(pFrm->Dot11Header.SA, pFrm->Body.ReasonCode);
            }
            break;
    }
}

/*
The following functions have been removed to prevent IPA

STATIC void ElementChecker(ELEMENT_CHECKER* p) { // RxCtrl.c:2564
    WORK_PARAM* pWork; // r4 - :2566
    u8* pBuf; // r5 - :2567
    long i; // r6 - :2568
    u32 len; // r7 - :2569
}

void RxManCtrlTask() { // RxCtrl.c:2717
    HEAP_MAN* pHeapMan; // r10 - :2719
    WlCounter* pCounter; // r0 - :2720
    u32 mode; // r5 - :2721
    RXFRM* pFrm; // r0 - :2722
    RXPACKET* pPacket; // r6 - :2723
    MAN_HEADER* pDot11Header; // r0 - :2724
    u32 subtype; // r9 - :2725
    u32 type; // r8 - :2725
    u32 cam_adrs; // r7 - :2725
    u32 seqctrl; // None - :2789
}

void DefragTask() { // RxCtrl.c:3011
    HEAP_MAN* pHeapMan; // None - :3013
    RXPACKET* pPacket; // r8 - :3014
    RXFRM* pFrm; // r0 - :3015
    DEFRAG_TBL defragTbl; // None - :3016
    u32 fc; // r4 - :3017
}
*/

void DefragTimerTask() { // RxCtrl.c:3348
    DEFRAG_LIST* pList = wlMan->RxCtrl.DefragList; // r5 - :3350
    u32 i; // r6 - :3351
    
    for (i = 0; i < 3; i++) {
        if (pList[i].RestTime) {
            if (--pList[i].RestTime == 0) {
                ReleaseHeapBuf(&wlMan->HeapMan.TmpBuf, pList[i].pPacket);
            }
        }
    }
}

void InitRxCtrl() { // RxCtrl.c:3395
    WORK_PARAM* pWork = &wlMan->Work; // r6 - :3397
    RX_CTRL* pRxCtrl = &wlMan->RxCtrl; // r7 - :3398
    u32 str_madrs; // r8 - :3399
    
    MIi_CpuClear32(0, &wlMan->RxCtrl, sizeof(wlMan->RxCtrl));
    W_RXCNT = 0x8000;
    
    // TODO: are those sizeofs?
    switch (wlMan->Work.Mode) {
        case 0:
            str_madrs = 0x794;
            break;
            
        case 1:
            str_madrs = 0x10C4;
            break;
            
        case 2:
            str_madrs = 0xBFC;
            break;
            
        case 3:
            str_madrs = 0x794;
            break;
    }
    
    W_RXBUF_BEGIN = (u32)&W_MACMEM(str_madrs);
    W_RXBUF_WR_ADDR = str_madrs >> 1;
    W_RXBUF_END = 0x5F60;
    W_RXBUF_READCSR = str_madrs >> 1;
    
    pRxCtrl->wlCurr = str_madrs >> 1;
    pRxCtrl->LastMpSeq = -1;
    pWork->Ofst.RxBuf.Size = 0x1F60 - str_madrs;
    
    W_RXBUF_GAP = 0x5F5E;
    W_RXCNT = 0x8001;
    W_INTERNAL_11 = 0xFFFF;
    W_INTERNAL_12 = 0xFFFF;
    W_RX_MAC_0 = 0xFFFF;
    W_RX_MAC_1 = 0xFFFF;
    W_RX_UNK = 0xFFFF;
    W_RX_SEQ = 0xFFFF;
}
