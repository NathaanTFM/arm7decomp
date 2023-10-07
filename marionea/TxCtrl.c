#include "Mongoose.h"

static u32 SetSupRateSet(u8* pBuf);
static u32 SetSSIDElement(u8* pBuf);

/*
void TxqPri(u32 pri) { // TxCtrl.c:64
    WORK_PARAM* pWork; // r11 - :66
    TX_CTRL* pTxCtrl; // r4 - :67
    TXQ* pTxq; // r5 - :68
    HEAPBUF_MAN* pBufMan; // r6 - :69
    WlMaDataReq* pNextTxReq; // r7 - :70
    WlMaDataReq* pTxReq; // None - :70
    TXFRM* pTxFrm; // r8 - :71
    TXFRM_MAC* pMacTxFrm; // r4 - :72
    u16* pMREG; // r0 - :73
    u32 adrs; // r0 - :74
    u32 cam_adrs; // r9 - :74
    u32 fc; // r0 - :74
    u32 x; // None - :74
}
IPA
*/

void CopyTxFrmToMacBuf(TXFRM_MAC* pMacTxFrm, WlMaDataReq* pTxReq) { // TxCtrl.c:298
    TXFRM* pTxFrm = (TXFRM*)&pTxReq->frame; // r0 - :300
    
    if (pTxFrm->Dot11Header.FrameCtrl.Data & 0x4000) { // WEP ?
        // TODO: check the bit (0x4000)
        if (wlMan->Work.Mode == 3)
            WUpdateCounter();
        
        if (pTxReq->header.code == (u16)-1) {
            DMA_WepWriteHeaderData(pMacTxFrm, &pTxFrm->MacHeader, pTxFrm->Data.Body, pTxFrm->FirmHeader.Length); // :316
            
        } else {
            DMA_WepWriteHeaderData(pMacTxFrm, &pTxFrm->MacHeader, pTxFrm->Data.Pointer, pTxFrm->FirmHeader.Length); // :322
        }
        
        *((u16*)(pMacTxFrm->Body + 0)) = (W_RANDOM + (W_RANDOM << 8)); // :330
        *((u16*)(pMacTxFrm->Body + 2)) = (W_RANDOM & 0xFF) | (wlMan->Config.WepKeyId << 14); // :331
        
        if (wlMan->WlOperation & 8) { // :334
            u16* pIcv = (u16*)(((u32)&pMacTxFrm->Dot11Header + pTxFrm->MacHeader.Tx.MPDU - 7) & ~1); // r0 - :336
            pIcv[0] = 0;
            pIcv[1] = 0;
        }
        
    } else {
        if (pTxReq->header.code == (u16)-1) {
            DMA_Write(pMacTxFrm, &pTxFrm->MacHeader, pTxFrm->FirmHeader.Length + 36); // :322
            
        } else {
            DMA_WriteHeaderData(pMacTxFrm, &pTxFrm->MacHeader, pTxFrm->Data.Pointer, pTxFrm->FirmHeader.Length); // :322
        }
    }
    
    if (wlMan->WlOperation & 4) {
        u16* pId = (u16*)(((u32)&pMacTxFrm->Dot11Header + pTxFrm->MacHeader.Tx.MPDU - 1) & ~3); // r0 - :359
        pId[0] = 0xB6B8;
        pId[1] = 0x1D46;
    }
}

// TODO: not matching
u32 CheckFrameTimeout(TXFRM* pTxFrm) { // TxCtrl.c:381
    u16 timeout; // r3 - :383
    WORK_PARAM* pWork = &wlMan->Work; // r12 - :384
    
    timeout = pWork->FrameLifeTime * 8;
    
    if (pTxFrm->Dot11Header.FrameCtrl.Bit.Type != 0) { // :390
        timeout /= 8;
        
    } else if (pWork->Mode == 1) { // :393
        if (pTxFrm->Dot11Header.FrameCtrl.Bit.SubType == 1 || pTxFrm->Dot11Header.FrameCtrl.Bit.SubType == 3 || pTxFrm->Dot11Header.FrameCtrl.Bit.SubType == 0xB)
            timeout /= 8;
        
    }
    
    return timeout < (u16)(pWork->IntervalCount - pTxFrm->FirmHeader.FrameTime); // :415
}

void TxqEndData(TXFRM* pFrm, u32 flag) { // TxCtrl.c:442
    HEAPBUF_MAN* pBufMan; // r5 - :444
    WORK_PARAM* pWork; // r6 - :445
    WlCounter* pCounter; // r0 - :446
    void* pReq; // r7 - :447
}

/*void TxqEndManCtrl(TXFRM* pFrm, u32 flag) { // TxCtrl.c:541
    WORK_PARAM* pWork; // r8 - :543
    MLME_MAN* pMLME; // r9 - :544
    WlCounter* pCounter; // r0 - :545
    HEAPBUF_MAN* pBufMan; // r10 - :546
    AUTH_FRAME* pAuth; // r0 - :547
    ASSRES_FRAME* pAssRes; // r0 - :548
    DEAUTH_FRAME* pDeAuth; // r0 - :550
    u32 cam_adrs; // r4 - :552
    u32 type; // r7 - :552
    u32 i; // r4 - :552
}
(anti IPA gang)*/

void TxqEndPsPoll(TXFRM* pFrm, u32 flag) { // TxCtrl.c:887
    WlCounter* pCounter = &wlMan->Counter; // r0 - :890
    TXFRM_MAC* pMFrm = (TXFRM_MAC*)pFrm; // r0 - :891
    
    pCounter->tx.retry += pMFrm->MacHeader.Tx.rsv_RetryCount & 0xFF;
    
    if ((pMFrm->MacHeader.Tx.Status & 2) == 0) {
        pCounter->tx.success++;
        pCounter->tx.unicast++;
        
    } else {
        pCounter->tx.failed++;
    }
    
    wlMan->TxCtrl.Txq[2].Busy = 0;
}

void TxqEndBroadCast(TXFRM* pFrm, u32 flag) { // TxCtrl.c:936
    // so both of those variables are still "compiled in", which means 
    // that both should be used (probably?)
    // this function matches without pBufMan, but it seems like it's centered around txpri 2, so let's try this
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r4 - :938
    HEAPBUF_MAN* pBufMan = &pHeapMan->TxPri[2]; // r0 - :939
    
    wlMan->Counter.tx.multicast++; // :942
    
    if (pFrm->Dot11Header.FrameCtrl.Bit.Type == 0) { // :954
        CAM_IncFrameCount(pFrm); // :957
        MoveHeapBuf(pBufMan, &pHeapMan->TxPri[1], SubtractAddr(pFrm, 0x10)); // :958
        TxqEndManCtrl(pFrm, 0); // :959
        
    } else {
        IssueMaDataConfirm(pBufMan, SubtractAddr(pFrm, 0x10)); // :965
    }
    
    wlMan->TxCtrl.Txq[2].Busy = 0; // :969
    
    if (wlMan->TxCtrl.Txq[2].pMacFrm->Dot11Header.FrameCtrl.Bit.MoreData == 0) { // :973
        W_TXREQ_RESET = 8; // :975
        W_TXREQ_SET = 5; // :976
        
        if (flag) { // :978
            if (pHeapMan->TxPri[1].Count) TxqPri(1); // :980
            if (pHeapMan->TxPri[0].Count) TxqPri(0); // :981
        }
    }
    
    if (pBufMan->Count) { // :986
        if (flag) // :988
            TxqPri(2); // :990
        
    } else {
        CAM_ClrTIMElementBitmap(0); // :996
    }
}

void TxEndKeyData(TXQ* pTxq) { // TxCtrl.c:1014
    // This variable isn't defined but it seems like the output
    // isn't correct when removing it
    // TODO: another way to do it? LTO?
    u8 rc = pTxq->pMacFrm->MacHeader.Tx.rsv_RetryCount & 0xFF;
    
    WlCounter* pCounter = &wlMan->Counter; // r0 - :1017
    
    if (rc == 0) {
        pCounter->multiPoll.txNull++; // :1030
    } else {
        pCounter->multiPoll.txKey += rc; // :1036
    }
    
    pTxq->OutCount++; // :1039
    pTxq->Busy = 0; // :1042
}

void ClearTxKeyData() { // TxCtrl.c:1070
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r4 - :1072
    u32 x = OS_DisableIrqMask(0x1000000); // r5 - :1073
    
    if (wlMan->Config.NullKeyRes == 0) {
        WClearKSID(0);
    }
    
    W_TXBUF_RESET = 0xC0;
    pTxCtrl->Key[0].Busy = 0;
    pTxCtrl->Key[1].Busy = 0;
    
    OS_EnableIrqMask(x);
}

void ClearTxMp() { // TxCtrl.c:1109
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r4 - :1111
    u32 x = OS_DisableIrqMask(0x1000000); // r4 - :1112
    
    W_TXBUF_RESET = 2;
    if (pTxCtrl->Mp.Busy)
        WlIntrMpEndTask();
    
    OS_EnableIrqMask(x);
}

void ClearTxData() { // TxCtrl.c:1144
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r4 - :1146
    u32 x = OS_DisableIrqMask(0x1000000); // r5 - :1147
    
    if (wlMan->Work.Mode == 1) {
        W_TXBUF_RESET = 9;
        if (pTxCtrl->Txq[2].Busy)
            ClearQueuedPri(2);
        
        MessageDeleteTx(2, 1);
        
    } else {
        W_TXBUF_RESET = 1;
    }
    
    if (pTxCtrl->Txq[0].Busy)
        ClearQueuedPri(0);
    
    MessageDeleteTx(0, 1);
    OS_EnableIrqMask(x);
}

void ClearQueuedPri(u32 pri) { // TxCtrl.c:1202
    TXQ* pTxq = &wlMan->TxCtrl.Txq[pri]; // r2 - :1204
    
    if (pTxq->Busy) {
        if (pTxq->pMacFrm->MacHeader.Tx.Status == 0) {
            pTxq->pFrm->MacHeader.Tx.Status = 2;
        } else {
            pTxq->pFrm->MacHeader.Tx.Status = pTxq->pMacFrm->MacHeader.Tx.Status;
        }
        pTxq->pEndFunc(pTxq->pFrm, 0);
    }
}

static u16 Pri2QBit[3] = {1, 4, 8}; // :1240

void ResetTxqPri(u32 pri) { // TxCtrl.c:1241
    u32 x; // r0 - :1243
    TXQ* pTxq = &wlMan->TxCtrl.Txq[pri]; // r6 - :1244
    
    x = OS_DisableIrqMask(0x1000000);
    W_TXBUF_RESET = Pri2QBit[pri];
    
    if (pTxq->Busy) {
        if ((pTxq->pFrm->Dot11Header.FrameCtrl.Data & 0x4000) == 0) {
            // TODO: Rx.TimeStamp or Tx.rsv_RetryCount?
            // FIXME: might not be retrycount (i don't understand this)
            pTxq->pFrm->MacHeader.Tx.rsv_RetryCount = pTxq->pMacFrm->MacHeader.Tx.rsv_RetryCount;
        }
        pTxq->pFrm->Dot11Header.SeqCtrl.Data = pTxq->pMacFrm->Dot11Header.SeqCtrl.Data;
    }
    
    OS_EnableIrqMask(x);
}

void DeleteTxFrames(u32 camAdrs) { // TxCtrl.c:1298
    WlMaDataReq* pReq, *pNextReq; // None - :1300
    TXFRM* pFrm; // r6 - :1301
    u32 bTask = 0; // r7 - :1302 - unused
    u32 i; // r8 - :1303
    
    if (CAM_GetFrameCount(camAdrs) == 0)
        return;
    
    for (i = 0; i < 3; i++) { // :1308
        pReq = (WlMaDataReq*)wlMan->HeapMan.TxPri[i].Head;
        if (pReq == (WlMaDataReq*)-1)
            continue;
        
        do {
            pNextReq = (WlMaDataReq*)GetHeapBufNextAdrs((HEAPBUF_HEADER*)pReq);
            pFrm = (TXFRM*)&pReq->frame;
            
            if (pFrm->FirmHeader.CamAdrs == camAdrs) {
                if (i == 1 || pFrm == wlMan->TxCtrl.Txq[i].pFrm) {
                    CAM_DecFrameCount(pFrm);
                    pFrm->FirmHeader.CamAdrs = 0;
                    CAM_IncFrameCount(pFrm);
                    
                } else {
                    pFrm->MacHeader.Tx.Status = 2;
                    CAM_DecFrameCount(pFrm);
                    IssueMaDataConfirm(&wlMan->HeapMan.TxPri[i], pReq);
                    
                    if (!bTask)
                        bTask = 1;
                }
            }
            
            pReq = pNextReq;
            
        } while (pReq != (WlMaDataReq*)-1);
    }
}

// TODO: Doesn't match twlsdk 5.5, but matches to ntrsdk 3.0. The "for" loop is compiled differently
// There seems to be a variable on line 1369 that was optimised away
void DeleteTxFrameByAdrs(u16* pMacAdrs) { // TxCtrl.c:1367
    u32 i; // r5 - :1370
    
    if (pMacAdrs[0] & 1) {
        for (i = 1; i < wlMan->Config.MaxStaNum; i++) {
            DeleteTxFrames(i);
        }
        
    } else {
        i = CAM_Search(pMacAdrs);
        if (i != 255)
            DeleteTxFrames(i);
        
        if (wlMan->Config.Mode == 1 && CAM_GetStaState(i) == 0x40) {
            CAM_SetStaState(i, 0x20);
            ClearTxKeyData();
        }
    }
}

void DeleteAllTxFrames() { // TxCtrl.c:1413
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r5 - :1415
    
    switch (wlMan->Work.Mode) {
        case 1:
            MessageDeleteTx(0, 1);
            MessageDeleteTx(1, 0);
            MessageDeleteTx(2, 1);
            
            if (pTxCtrl->Mp.Busy) {
                pTxCtrl->Mp.Busy = 0;
                pTxCtrl->Mp.InCount--;
                ReleaseHeapBuf(&wlMan->HeapMan.TmpBuf, pTxCtrl->pMpEndInd);
            }
            break;
            
        case 2:
        case 3:
            MessageDeleteTx(0, 1);
            MessageDeleteTx(1, 0);
            MessageDeleteTx(2, 0);
            break;
            
        default:
            MessageDeleteTx(0, 0);
            MessageDeleteTx(1, 0);
            MessageDeleteTx(2, 0);
            break;
    }
}

void MessageDeleteTx(u32 pri, u32 bMsg) { // TxCtrl.c:1465
    WlMaDataReq* pReq, *pNextReq; // r6, r7 - :1467
    TXFRM* pFrm; // r0 - :1468
    
    pReq = (WlMaDataReq*)wlMan->HeapMan.TxPri[pri].Head;
    if (pReq == (WlMaDataReq*)-1)
        return;
    
    do {
        pNextReq = (WlMaDataReq*)GetHeapBufNextAdrs((HEAPBUF_HEADER*)pReq);
        pFrm = (TXFRM*)&pReq->frame;
        
        if (pri != 2)
            CAM_DecFrameCount(pFrm);
        
        pFrm->MacHeader.Tx.Status = 2;
        
        if (bMsg)
            IssueMaDataConfirm(&wlMan->HeapMan.TxPri[pri], pReq);
        
        pReq = pNextReq;
        
    } while (pReq != (WlMaDataReq*)-1);
}

void TxManCtrlFrame(TXFRM* pFrm) { // TxCtrl.c:1525
    SetManCtrlFrame(pFrm);
    TxqPri(1);
}

void SetManCtrlFrame(TXFRM* pFrm) { // TxCtrl.c:1545
    pFrm->FirmHeader.CamAdrs = CAM_Search(pFrm->Dot11Header.Adrs1);
    if (pFrm->FirmHeader.CamAdrs == 0xFF)
        pFrm->FirmHeader.CamAdrs = 0;
    
    pFrm->FirmHeader.FrameTime = wlMan->Work.IntervalCount;
    if (pFrm->Dot11Header.FrameCtrl.Data & 0x4000)
        pFrm->MacHeader.Tx.MPDU += 8;
    
    CAM_IncFrameCount(pFrm);
    MoveHeapBuf(&wlMan->HeapMan.TmpBuf, &wlMan->HeapMan.TxPri[1], SubtractAddr(pFrm, 0x10)); // get heap buf
}

void TxPsPollFrame() { // TxCtrl.c:1579
    TXQ* pTxq = &wlMan->TxCtrl.Txq[2]; // r4 - :1581
    
    pTxq->InCount++;
    
    if (pTxq->Busy) {
        pTxq->pMacFrm->MacHeader.Tx.rsv_RetryCount = 0;
        
    } else {
        pTxq->Busy = 1;
        pTxq->pMacFrm->MacHeader.Tx.Status = 0;
        pTxq->pMacFrm->MacHeader.Tx.rsv_RetryCount = 0;
        pTxq->pMacFrm->MacHeader.Tx.Service_Rate = WCalcManRate();
        
        W_TXBUF_LOC3 = (u16)GetTxBufAddr(pTxq->pMacFrm) | 0x8000;
    }
}

void StartBeaconFrame() { // TxCtrl.c:1621
    TXQ* pTxq = &wlMan->TxCtrl.Beacon; // r0 - :1623
    
    pTxq->Busy = 1;
    W_TXBUF_BEACON = GetTxBufAddr(pTxq->pMacFrm) | 0x8000;
}

void StopBeaconFrame() { // TxCtrl.c:1652
    TXQ* pTxq = &wlMan->TxCtrl.Beacon; // r0 - :1654
    
    W_TXBUF_BEACON = 0;
    pTxq->Busy = 0;
}

extern u16 BC_ADRS[3]; // from WlNic.c

void MakeBeaconFrame() { // TxCtrl.c:1692
    TXFRM_MAC* pFrm = wlMan->TxCtrl.Beacon.pMacFrm; // r0 - :1694
    CONFIG_PARAM* pConfig = &wlMan->Config; // r5 - :1695
    WORK_PARAM* pWork = &wlMan->Work; // r6 - :1696
    BEACON_BODY* pBody; // r0 - :1697
    u8* pBuf; // r0 - :1698
    u8* p; // r4 - :1699
    u32 i, vtsf; // r7, r4 - :1700
    u16* p16; // r8 - :1701
    
    pFrm->MacHeader.Tx.Status = 0; // :1707
    pFrm->MacHeader.Tx.Status2 = 0; // :1708
    pFrm->MacHeader.Tx.rsv_RetryCount = 0; // :1709
    pFrm->MacHeader.Tx.rsv_AppRate = 0; // :1710
    pFrm->MacHeader.Tx.Service_Rate = WCalcManRate(); // :1711
    
    pFrm->Dot11Header.FrameCtrl.Data = 128; // :1722
    pFrm->Dot11Header.DurationID = 0; // :1723
    WSetMacAdrs3(pFrm->Dot11Header.Adrs1, BC_ADRS, pConfig->MacAdrs, pConfig->MacAdrs); // :1724
    pFrm->Dot11Header.SeqCtrl.Data = 0; // :1725
    
    pBody = (BEACON_BODY*)pFrm->Body; // :1730
    pBody->TimeStamp[0] = 0; // :1732
    pBody->TimeStamp[1] = 0; // :1733
    pBody->BeaconInterval = pWork->BeaconPeriod; // :1734
    pBody->CapaInfo.Data = pWork->CapaInfo; // :1735
    
    pBuf = pBody->Buf; // :1737
    
    if (pConfig->BeaconType == 0) { // :1739
        pWork->Ofst.Beacon.SSID = (u32)pBuf - (u32)pBody; // :1741
        WL_WriteByte(pBuf++, 0); // :1742
        WL_WriteByte(pBuf++, pWork->SSIDLength); // :1743
        
        for (i = 0; i < pWork->SSIDLength; i++) { // :1744
            WL_WriteByte(pBuf++, WL_ReadByte(&pWork->SSID[i])); // :1746
        }
        
        // why are we writing length again?
        WL_WriteByte(&pBuf[-i-1], i); // :1749
    
    } else {
        pWork->Ofst.Beacon.SSID = 0; // :1753
    }
    
    pBuf += SetSupRateSet(pBuf); // :1757
    WL_WriteByte(pBuf++, 3);
    WL_WriteByte(pBuf++, 1);
    WL_WriteByte(pBuf++, pWork->CurrChannel);
    
    pWork->Ofst.Beacon.TIM = (u32)pBuf - (u32)pBody;
    W_TXBUF_TIM = pWork->Ofst.Beacon.TIM + 2;
    
    WL_WriteByte(pBuf++, 5);
    WL_WriteByte(pBuf++, 5);
    WL_WriteByte(pBuf++, 0);
    WL_WriteByte(pBuf++, pWork->DTIMPeriod);
    WL_WriteByte(pBuf++, 0);
    WL_WriteByte(pBuf++, 0);
    WL_WriteByte(pBuf++, 0);
    
    pWork->Ofst.Beacon.GameInfo = (u32)pBuf - (u32)pBody;
    pWork->GameInfoAlign = pWork->Ofst.Beacon.GameInfo & 1;
    
    WL_WriteByte(pBuf++, 0xDD);
    WL_WriteByte(pBuf++, pWork->GameInfoLength + 8);
    WL_WriteByte(pBuf++, 0x00); // 00:09:BF:00 (OUI)
    WL_WriteByte(pBuf++, 0x09);
    WL_WriteByte(pBuf++, 0xBF);
    WL_WriteByte(pBuf++, 0x00);
    
    if (pWork->PowerMgtMode == 1) {
        WL_WriteByte(pBuf++, pConfig->ActiveZone);
        WL_WriteByte(pBuf++, pConfig->ActiveZone >> 8);
    } else {
        WL_WriteByte(pBuf++, 0xFF);
        WL_WriteByte(pBuf++, 0xFF);
    }
    
    vtsf = global_vtsf_var; // :1794
    WL_WriteByte(pBuf++, vtsf); // :1795
    WL_WriteByte(pBuf++, vtsf >> 8); // :1796
    
    p = pWork->GameInfoAdrs; // :1797
    for (i = 0; i < pWork->GameInfoLength; i++) { // :1798
        WL_WriteByte(pBuf++, WL_ReadByte(p)); // :1800
        p++; // :1801
    }
    
    if (pWork->GameInfoAlign != 0) {
        p = (u8*)((u32)pWork->GameInfoAdrs + pWork->GameInfoLength - 1);
        
        for (i = 0; i < pWork->GameInfoLength; p--, i++) { // :1808
            WL_WriteByte(p+1, WL_ReadByte(p)); // :1810
        }
    }
    
    if (wlMan->WlOperation & 4) { // :1814
        u16* pId = (u16*)(((u32)pBuf + 3) & ~3); // r0 - :1816
        pId[0] = 0xB6B8;
        pId[1] = 0x1D46;
    }
    
    pWork->bUpdateGameInfo = 0;
    pFrm->MacHeader.Tx.MPDU = (u32)pBuf + 28 - (u32)pBody;
}

void UpdateGameInfoElement() { // TxCtrl.c:1842
    WORK_PARAM* pWork = &wlMan->Work; // r5 - :1844
    GAME_INFO_ELEMENT* pGMIF = (GAME_INFO_ELEMENT*)&wlMan->TxCtrl.Beacon.pMacFrm->Body[pWork->Ofst.Beacon.GameInfo]; // r6 - :1845
    
    if (pWork->GameInfoLength != 0) {
        if (pWork->GameInfoAlign & 1) {
            DMA_Write((char*)pGMIF->GameInfo - 1, pWork->GameInfoAdrs, pWork->GameInfoLength + 2);
            WL_WriteByte(&pGMIF->VTSF[1], (global_vtsf_var >> 8));
            
        } else {
            DMA_Write(pGMIF->GameInfo, pWork->GameInfoAdrs, pWork->GameInfoLength + 1);
        }
    }
    
    wlMan->TxCtrl.Beacon.pMacFrm->MacHeader.Tx.MPDU = pWork->Ofst.Beacon.GameInfo + 38 + pWork->GameInfoLength;
    WL_WriteByte(&pGMIF->Length, pWork->GameInfoLength + 8);
    
    if (wlMan->WlOperation & 4) {
        u16* pId = (u16*)(((u32)pGMIF + sizeof(GAME_INFO_ELEMENT) + pWork->GameInfoLength) & ~3); // r0 - :1874
        pId[0] = 0xB6B8;
        pId[1] = 0x1D46;
    }
    
    pWork->bUpdateGameInfo = 0;
}

static u32 IsEnableManagement() { // TxCtrl.c:1897
    return wlMan->HeapMan.TxPri[1].Count < 24 - wlMan->CamMan.ConnectSta;
}

DISASS_FRAME* MakeDisAssFrame(u16* pDA, u16 reasonCode) { // TxCtrl.c:1923
    WlMaDataReq* pReq; // r0 - :1925
    DISASS_FRAME* pFrm; // r0 - :1926
    
    // TODO: sizeof..?
    pReq = (WlMaDataReq*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, 0x36);
    if (pReq == 0) {
        SetFatalErr(2);
        
         // okay, this is actually really ugly, but "return 0" (which behaves the same)
         // makes the decomp inaccurate
        return (DISASS_FRAME*)pReq;
    }
    
    pReq->header.code = -1;
    
    pFrm = (DISASS_FRAME*)&pReq->frame;
    InitManHeader((TXFRM*)pFrm, pDA);
    
    pFrm->Body.ReasonCode = reasonCode;
    
    pFrm->FirmHeader.Length = 2;
    pFrm->MacHeader.Tx.MPDU = pFrm->FirmHeader.Length + 28;
    pFrm->Dot11Header.FrameCtrl.Data = 0xA0;
    
    return pFrm;
}

ASSREQ_FRAME* MakeAssReqFrame(u16* pDA) { // TxCtrl.c:1969
    WORK_PARAM* pWork = &wlMan->Work; // r5 - :1971
    WlMaDataReq* pReq; // r0 - :1972
    ASSREQ_FRAME* pFrm; // r0 - :1973
    u32 ofst; // r0 - :1974
    
    // TODO: sizeof..?
    pReq = (WlMaDataReq*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, 0x5E);
    if (pReq == 0) {
        SetFatalErr(2);
        
         // okay, this is actually really ugly, but "return 0" (which behaves the same)
         // makes the decomp inaccurate
        return (ASSREQ_FRAME*)pReq;
    }
    
    pReq->header.code = -1;
    
    pFrm = (ASSREQ_FRAME*)&pReq->frame;
    InitManHeader((TXFRM*)pFrm, pDA);
    
    pFrm->Body.CapaInfo.Data = pWork->CapaInfo;
    pFrm->Body.ListenInterval = pWork->ListenInterval;
    
    ofst = SetSSIDElement(pFrm->Body.Buf);
    ofst += SetSupRateSet(pFrm->Body.Buf + ofst);
    
    pFrm->FirmHeader.Length = ofst + 4;
    pFrm->MacHeader.Tx.MPDU = pFrm->FirmHeader.Length + 28;
    pFrm->Dot11Header.FrameCtrl.Data = 0;
    return pFrm;
}

REASSREQ_FRAME* MakeReAssReqFrame(u16* pDA) { // TxCtrl.c:2021
    WORK_PARAM* pWork = &wlMan->Work; // r5 - :2023
    WlMaDataReq* pReq; // r0 - :2024
    REASSREQ_FRAME* pFrm; // r0 - :2025
    u32 ofst; // r0 - :2026
    
    // TODO: sizeof..?
    pReq = (WlMaDataReq*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, 0x64);
    if (pReq == 0) {
        SetFatalErr(2);
        
         // okay, this is actually really ugly, but "return 0" (which behaves the same)
         // makes the decomp inaccurate
        return (REASSREQ_FRAME*)pReq;
    }
    
    pReq->header.code = -1;
    
    pFrm = (REASSREQ_FRAME*)&pReq->frame;
    InitManHeader((TXFRM*)pFrm, pDA);
    
    pFrm->Body.CapaInfo.Data = pWork->CapaInfo;
    pFrm->Body.ListenInterval = pWork->ListenInterval;
    
    WSetMacAdrs1(pFrm->Body.CurrAPMacAdrs, pWork->LinkAdrs);
    ofst = SetSSIDElement(pFrm->Body.Buf);
    ofst += SetSupRateSet(pFrm->Body.Buf + ofst);
    
    pFrm->FirmHeader.Length = ofst + 10;
    pFrm->MacHeader.Tx.MPDU = pFrm->FirmHeader.Length + 28;
    pFrm->Dot11Header.FrameCtrl.Data = 0x20;
    return pFrm;
}

ASSRES_FRAME* MakeAssResFrame(u16 camAdrs, u16 statusCode, SSID_ELEMENT* pSSID) { // TxCtrl.c:2074
    WlMaDataReq* pReq; // r0 - :2076
    ASSRES_FRAME* pFrm; // r0 - :2077
    u32 aid; // r5 - :2078
    u32 i; // r7 - :2078
    u32 ofst; // r0 - :2078
    u8* p; // r8 - :2079
    u32 len; // r9 - :2129
}

REASSRES_FRAME* MakeReAssResFrame(u16 camAdrs, u16 statusCode, SSID_ELEMENT* pSSID) { // TxCtrl.c:2157
    WlMaDataReq* pReq; // r0 - :2159
    REASSRES_FRAME* pFrm; // r0 - :2160
    u32 aid; // r9 - :2161
    u32 i; // r6 - :2161
    u32 ofst; // r0 - :2161
    u8* p; // r7 - :2162
    u32 len; // r8 - :2211
}

PRBREQ_FRAME* MakeProbeReqFrame(u16* pDA) { // TxCtrl.c:2245
    WlMaDataReq* pReq; // r0 - :2247
    PRBREQ_FRAME* pFrm; // r0 - :2248
    u32 ofst; // r0 - :2249
    
    // TODO: sizeof..?
    pReq = (WlMaDataReq*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, 0x5A);
    if (pReq == 0) {
        SetFatalErr(2);
        
         // okay, this is actually really ugly, but "return 0" (which behaves the same)
         // makes the decomp inaccurate
        return (PRBREQ_FRAME*)pReq;
    }
    
    pReq->header.code = -1;
    
    pFrm = (PRBREQ_FRAME*)&pReq->frame;
    InitManHeader((TXFRM*)pFrm, pDA);
    
    ofst = SetSSIDElement(pFrm->Body.Buf);
    ofst += SetSupRateSet(pFrm->Body.Buf + ofst);
    
    pFrm->FirmHeader.Length = ofst;
    pFrm->MacHeader.Tx.MPDU = ofst + 28;
    pFrm->Dot11Header.FrameCtrl.Data = 0x40;
    return pFrm;
}

PRBRES_FRAME* MakeProbeResFrame(u16* pDA) { // TxCtrl.c:2294
    WORK_PARAM* pWork; // r4 - :2296
    WlMaDataReq* pReq; // r0 - :2297
    PRBRES_FRAME* pFrm; // r0 - :2298
    u32 ofst; // r9 - :2299
}

AUTH_FRAME* MakeAuthFrame(u16* pDA, u16 txtLen, u32 bCheck) { // TxCtrl.c:2355
    WlMaDataReq* pReq; // r0 - :2357
    AUTH_FRAME* pFrm; // r0 - :2358
    
    if (bCheck && !IsEnableManagement())
        return 0;
    
    // TODO: sizeof..?
    pReq = (WlMaDataReq*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, txtLen + 0x3D);
    if (pReq == 0) {
        SetFatalErr(2);
        
         // okay, this is actually really ugly, but "return 0" (which behaves the same)
         // makes the decomp inaccurate
        return (AUTH_FRAME*)pReq;
    }
    
    pReq->header.code = -1;
    
    pFrm = (AUTH_FRAME*)&pReq->frame;
    InitManHeader((TXFRM*)pFrm, pDA);
    
    if (txtLen != 0) {
        WL_WriteByte(&pFrm->Body.ChallengeText.ID, 0x10);
        WL_WriteByte(&pFrm->Body.ChallengeText.Length, txtLen);
        txtLen += 2;
    }
    
    pFrm->FirmHeader.Length = txtLen + 6;
    pFrm->MacHeader.Tx.MPDU = pFrm->FirmHeader.Length + 28; // check
    pFrm->Dot11Header.FrameCtrl.Data = 0xB0;
    return pFrm;
}

DEAUTH_FRAME* MakeDeAuthFrame(u16* pDA, u16 reasonCode, u32 bCheck) { // TxCtrl.c:2416
    WlMaDataReq* pReq; // r0 - :2418
    DEAUTH_FRAME* pFrm; // r0 - :2419
    
    if (bCheck && !IsEnableManagement())
        return 0;
    
    // TODO: sizeof..?
    pReq = (WlMaDataReq*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, 0x36);
    if (pReq == 0) {
        SetFatalErr(2);
        
         // okay, this is actually really ugly, but "return 0" (which behaves the same)
         // makes the decomp inaccurate
        return (DEAUTH_FRAME*)pReq;
    }
    
    pReq->header.code = -1;
    
    pFrm = (DEAUTH_FRAME*)&pReq->frame;
    InitManHeader((TXFRM*)pFrm, pDA);
    
    pFrm->Body.ReasonCode = reasonCode;
    pFrm->FirmHeader.Length = 2;
    pFrm->MacHeader.Tx.MPDU = pFrm->FirmHeader.Length + 28;
    pFrm->Dot11Header.FrameCtrl.Data = 0xC0;
    return pFrm;
}

void MakePsPollFrame(u16 aid) { // TxCtrl.c:2469
    TXFRM_MAC* pMFrm = wlMan->TxCtrl.Txq[2].pMacFrm; // r0 - :2471
    pMFrm->MacHeader.Tx.Status = 0;
    pMFrm->MacHeader.Tx.Status2 = 0;
    pMFrm->MacHeader.Tx.rsv_RetryCount = 0;
    pMFrm->MacHeader.Tx.MPDU = 20;
    pMFrm->Dot11Header.FrameCtrl.Data = 0xA4;
    pMFrm->Dot11Header.DurationID = aid | 0xC000;
    WSetMacAdrs2(pMFrm->Dot11Header.Adrs1, wlMan->Work.BSSID, wlMan->Config.MacAdrs);
}

void InitManHeader(TXFRM* pFrm, u16* pDA) { // TxCtrl.c:2503
    MIi_CpuClear16(0, pFrm, sizeof(FIRM_HEADER) + sizeof(MAC_HEADER) + sizeof(DATA_HEADER)); // 0x2C
    pFrm->MacHeader.Tx.Service_Rate = WCalcManRate();
    WSetMacAdrs3(pFrm->Dot11Header.Adrs1, pDA, wlMan->Config.MacAdrs, wlMan->Work.BSSID);
}

u32 IsExistManFrame(u16* pDA, u16 frameCtrl) { // TxCtrl.c:2526
    WlMaDataReq* pReq; // r5 - :2528
    TXFRM* pFrm; // r0 - :2529
    
    pReq = (WlMaDataReq*)wlMan->HeapMan.TxPri[1].Head;
    while (pReq != (WlMaDataReq*)-1) {
        pFrm = (TXFRM*)&pReq->frame;
        if ((pFrm->Dot11Header.FrameCtrl.Data == frameCtrl) && MatchMacAdrs(pFrm->Dot11Header.Adrs1, pDA))
            return 1;
        
        pReq = GetHeapBufNextAdrs((HEAPBUF_HEADER*)pReq);
    }
    return 0;
}

static u32 SetSSIDElement(u8* pBuf) { // TxCtrl.c:2576
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :2578
    u32 len = 0, ssid_len = pWork->SSIDLength; // r5, r6 - :2579
    u32 i; // r7 - :2580
    
    WL_WriteByte(&pBuf[0], 0);          // also works with [len+0]
    WL_WriteByte(&pBuf[1], ssid_len);   // also works with [len+1]
    
    len += 2;
    
    for (i = 0; i < ssid_len; i++) {
        WL_WriteByte(&pBuf[len], WL_ReadByte(&pWork->SSID[i]));
        len++;
    }
    
    return len;
}

extern const u16 RateBit2Element[16]; // WlNic.c

static u32 SetSupRateSet(u8* pBuf) { // TxCtrl.c:2614
    WORK_PARAM* pWork = &wlMan->Work; // r6 - :2616
    u32 len = 0; // r7 - :2618
    u32 i; // r8 - :2619
    
    WL_WriteByte(pBuf, 1); // :2622
    len += 2; // :2626
    
    for (i = 0; i < 0x10; i++) { // :2629
        if (pWork->RateSet.Support & (1 << i)) { // :2631
            if (pWork->RateSet.Basic & (1 << i)) { // :2633
                WL_WriteByte(&pBuf[len], RateBit2Element[i] | 0x80); // :2635
            } else {
                WL_WriteByte(&pBuf[len], RateBit2Element[i]); // :2639
            }
            len++; // :2641
        }
    }
    
    WL_WriteByte(&pBuf[1], len - 2);
    return len;
}

// This macro doesn't exist in the og code
#define AddBufId(adr) do { \
    W_MACMEM(adr-4) = 0xB6B8; \
    W_MACMEM(adr-2) = 0x1D46; \
} while (0)
    
void InitTxCtrl() { // TxCtrl.c:2825
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r7 - :2827
    WORK_PARAM* pWork = &wlMan->Work; // r8 - :2828
    CONFIG_PARAM* pConfig = &wlMan->Config; // r9 - :2829
    
    MIi_CpuClear32(0, pTxCtrl, sizeof(*pTxCtrl));
    
    pTxCtrl->Txq[0].Busy = 0;
    pTxCtrl->Txq[0].pFrm = 0;
    
    pTxCtrl->Txq[1].Busy = 0;
    pTxCtrl->Txq[1].pFrm = 0;
    
    pTxCtrl->Txq[2].Busy = 0;
    pTxCtrl->Txq[2].pFrm = 0;
    
    pTxCtrl->BkKeyIn = -1;
    pTxCtrl->BkKeyOut = -1;
    
    switch (pWork->Mode) {
        case 0:
            pTxCtrl->Txq[0].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x170);
            pTxCtrl->Txq[1].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x028);
            pTxCtrl->Txq[2].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x000);
            
            pTxCtrl->Txq[0].pEndFunc = TxqEndData;
            pTxCtrl->Txq[1].pEndFunc = TxqEndManCtrl;
            pTxCtrl->Txq[2].pEndFunc = TxqEndPsPoll;
            
            // add buf ids
            AddBufId(0x028);
            AddBufId(0x170);
            AddBufId(0x794); // why 0x794?
            
            pWork->FrameCtrl = 8;
            W_TXREQ_SET = 1;
            break;
            
        case 1:
            pTxCtrl->Txq[0].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0xAA0);
            pTxCtrl->Txq[1].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x958);
            pTxCtrl->Txq[2].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x334);
            
            pTxCtrl->Txq[0].pEndFunc = TxqEndData;
            pTxCtrl->Txq[1].pEndFunc = TxqEndManCtrl;
            pTxCtrl->Txq[2].pEndFunc = TxqEndBroadCast;
            
            pTxCtrl->Beacon.pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x238);
            pTxCtrl->Mp.pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x000);
            
            AddBufId(0x238);
            AddBufId(0x334);
            AddBufId(0x958);
            AddBufId(0xAA0);
            AddBufId(0x10C4); // ??
            
            pWork->FrameCtrl = 0x208;
            pTxCtrl->Beacon.pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x238); // why is this set again
            MakeBeaconFrame();
            break;
            
        case 2:
            pTxCtrl->Txq[0].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x5D8);
            pTxCtrl->Txq[1].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x490);
            pTxCtrl->Txq[2].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x468);
            
            pTxCtrl->Txq[0].pEndFunc = TxqEndData;
            pTxCtrl->Txq[1].pEndFunc = TxqEndManCtrl;
            pTxCtrl->Txq[2].pEndFunc = TxqEndPsPoll;
            
            pTxCtrl->Key[0].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x000);
            pTxCtrl->Key[1].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x234);
            
            AddBufId(0x234);
            AddBufId(0x468);
            AddBufId(0x490);
            AddBufId(0x5D8);
            AddBufId(0xBFC); // ??
            
            pWork->FrameCtrl = 0x108;
            W_TXREQ_SET = 0x0D;
            break;
            
        case 3:
            pTxCtrl->Txq[0].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x170);
            pTxCtrl->Txq[1].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x028);
            pTxCtrl->Txq[2].pMacFrm = (TXFRM_MAC *)&W_MACMEM(0x000);
            
            pTxCtrl->Txq[0].pEndFunc = TxqEndData;
            pTxCtrl->Txq[1].pEndFunc = TxqEndManCtrl;
            pTxCtrl->Txq[2].pEndFunc = TxqEndPsPoll;
            
            AddBufId(0x028);
            AddBufId(0x170);
            AddBufId(0x794); // why 0x794?
            
            pWork->FrameCtrl = 0x108;
            W_TXREQ_SET = 0x0D;
            break;
    }
    
    if (pConfig->WepMode)
        pWork->FrameCtrl |= 0x4000;
}
