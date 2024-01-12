#include "Mongoose.h"

STATIC RXFRM* TakeoutRxFrame(RXFRM_MAC* pMFrm, u32 length);

void WlIntrTxBeaconTask() { // WlIntrTask.c:77
    if (wlMan->Work.bUpdateGameInfo)
        UpdateGameInfoElement();
    
    if (wlMan->Config.BcnTxRxIndMsg)
        MLME_IssueBeaconSendIndication();
}

void WlIntrTxEndTask() { // WlIntrTask.c:106
    WORK_PARAM* pWork = &wlMan->Work; // r0 - :108
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r8 - :109
    TXQ* pTxq; // r9 - :110
    long i; // r10 - :114
    
    for (i = 2; i >= 0; i--) { // :116
        pTxq = &pTxCtrl->Txq[i]; // :118
        
        if (((*(u16*)((u32)&W_TXBUF_LOC1 + 4 * i) & 0x8000) != 0) || !pTxq->Busy) { // :120
            continue;
        }
        
        if (pTxq->pFrm) {
            pTxq->pFrm->MacHeader.Tx.Status = pTxq->pMacFrm->MacHeader.Tx.Status;
            pTxq->pFrm->Dot11Header.FrameCtrl.Data = pTxq->pMacFrm->Dot11Header.FrameCtrl.Data;
            
            if (pTxq->pMacFrm->Dot11Header.FrameCtrl.Data & 0x4000) { // :137
                if (wlMan->WlOperation & 8) { // :141
                    u16 *pIcv = (u16*)(((u32)&pTxq->pMacFrm->Dot11Header + pTxq->pMacFrm->MacHeader.Tx.MPDU - 7) & ~0x1); // r0 - :143
                    
                    if (pIcv[0] == 0 && pIcv[1] == 0) { // :145
                        W_WEP_CNT = 0;
                        W_WEP_CNT = 0x8000;
                        ++pWork->WepErrCount;
                        
                        if (CheckFrameTimeout(pTxq->pFrm)) {
                            pTxq->pFrm->MacHeader.Tx.Status = 2;
                            ++pTxq->OutCount;
                            ++pTxCtrl->TimeOutFrm;
                            pTxq->pEndFunc(pTxq->pFrm, 1);
                            
                        } else {
                            pTxq->pFrm->MacHeader.Tx.rsv_RetryCount = 0;
                            *(u16*)((u32)&W_TXBUF_LOC1 + 4 * i) |= 0x8000u;
                        }
                        
                        continue;
                        
                    }
                }
                
                pTxq->pFrm->MacHeader.Tx.rsv_RetryCount += pTxq->pMacFrm->MacHeader.Tx.rsv_RetryCount & 0xFF;
                
            } else {
                pTxq->pFrm->MacHeader.Tx.rsv_RetryCount = pTxq->pMacFrm->MacHeader.Tx.rsv_RetryCount & 0xFF;
            }
            
            ++pTxq->OutCount; 
            pTxq->pEndFunc(pTxq->pFrm, 2);
            
        } else {
            ++pTxq->OutCount;
            pTxq->pEndFunc((TXFRM *)pTxq->pMacFrm, 3);
        }
    }
    
    
}

void WlIntrRxEndTask() { // WlIntrTask.c:240
    RX_CTRL* pRxCtrl = &wlMan->RxCtrl; // r6 - :242
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r0 - :243
    RXFRM_MAC* pMFrm; // r0 - :244
    RXFRM* pFrm; // r0 - :245
    u32 bnry, length, status, bRelease, next_bnry; // r7, r0, r0, r7, r7 - :246
    u16* pClr; // r0 - :584
    u32 j; // r3 - :585

    for (u32 i = 0;; i++) { // variable present in older versions, probably still exists
        bnry = W_RXBUF_READCSR;
        if (bnry == pRxCtrl->wlCurr)
            break;

        if (bnry >= 0x8C6)
            WUpdateCounter();

        pMFrm = (RXFRM_MAC*)&W_MACMEM(2 * bnry);
        next_bnry = *(u16*)AdjustRingPointer(&pMFrm->MacHeader.Rx.NextBnry);
        
        if (*(u16*)pMFrm == 0xFFFF) {
            W_RXBUF_READCSR = next_bnry;
            
        } else {
            length = *(u16*)AdjustRingPointer(&pMFrm->MacHeader.Rx.MPDU);
            pFrm = TakeoutRxFrame(pMFrm, length);
            W_RXBUF_READCSR = next_bnry;

            if (!pFrm) {
                if ((pMFrm->MacHeader.Rx.Status & 0xF) == 12) {
                    SetFatalErr(0x10);
                } else {
                    SetFatalErr(8);
                }
                
            } else {
                if ((wlMan->WlOperation & 8) != 0 && (pFrm->Dot11Header.FrameCtrl.Data & 0x4000) != 0) {
                    pRxCtrl->IcvOkCntFlag = 0;
                }

                status = pFrm->MacHeader.Rx.Status;
                bRelease = 1;

                if ((status & 0x200) != 0) {
                    if (pFrm->Dot11Header.FrameCtrl.Bit.MoreFrag == 1 || pFrm->Dot11Header.SeqCtrl.Bit.FragNum != 0) {
                        bRelease = 0;
                        MoveHeapBuf(&pHeapMan->TmpBuf, &pHeapMan->Defrag, GET_HEADER(pFrm));
                        AddTask(2, 9);
                    }
                    
                } else {
                    switch (status & 0xF) {
                        case 8:
                            if ((pFrm->Dot11Header.FrameCtrl.Data & 0xF) == 8) {
                                bRelease = 0;
                                MoveHeapBuf(&pHeapMan->TmpBuf, &pHeapMan->RxData, GET_HEADER(pFrm));
                                AddTask(2, 6u);
                            }
                            break;
                        
                        case 1:
                            if (pFrm->Dot11Header.FrameCtrl.Data == 0x80) {
                                RxBeaconFrame((BEACON_FRAME*)pFrm);
                            }
                            break;
                        
                        case 0:
                            if ((pFrm->Dot11Header.FrameCtrl.Data & 0xF) == 0) {
                                bRelease = 0;
                                MoveHeapBuf(&pHeapMan->TmpBuf, &pHeapMan->RxManCtrl, GET_HEADER(pFrm));
                                AddTask(1, 7u);
                            }
                            break;
                        
                        case 5:
                            if ((pFrm->Dot11Header.FrameCtrl.Data & 0xE7FF) == 0xA4) {
                                bRelease = 0;
                                MoveHeapBuf(&pHeapMan->TmpBuf, &pHeapMan->RxManCtrl, GET_HEADER(pFrm));
                                AddTask(1, 7u);
                            }
                            break;
                        
                        case 14:
                        case 15:
                            if ((pFrm->Dot11Header.FrameCtrl.Data & 0xE7BF) == 0x118) {
                                RxKeyDataFrame(pFrm);
                            }
                            break;
                        
                        case 12:
                            if ((pFrm->Dot11Header.FrameCtrl.Data & 0xE7FF) == 0x228) {
                                if (wlMan->Work.PowerState == 0) {
                                    W_POWERSTATE = 1;
                                }
                                wlMan->Counter.multiPoll.rxMp++;
                                bRelease = RxMpFrame(pFrm);
                            }
                            break;
                        
                        case 13:
                            if ((pFrm->Dot11Header.FrameCtrl.Data & 0xE7FF) == 0x218) {
                                wlMan->Counter.multiPoll.rxMpAck++;
                                bRelease = RxMpAckFrame(pFrm);
                            }
                            break;
                    }
                }

                if (bRelease)
                    ReleaseHeapBuf(&pHeapMan->TmpBuf, GET_HEADER(pFrm));

                if ((wlMan->WlOperation & 1) != 0) {
                    pClr = (u16*)pMFrm;
                    for (j = 0; j < 7; j++) {
                        if ((u32)pClr >= 0x4805F60) {
                            pClr = (u16*)((u8*)pClr - wlMan->Work.Ofst.RxBuf.Size);
                        }
                        *(pClr++) = 0xFFFF;
                    }
                }
            }
        }
    }
}

void WlIntrMpEndTask() { // WlIntrTask.c:615
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r5 - :617
    
    if (pTxCtrl->Mp.Busy) {
        if (pTxCtrl->pMpEndInd->mpKey.bitmap != pTxCtrl->Mp.pMacFrm->MacHeader.Tx.Status2) // TODO: Rx.NextBnry or Tx.Status2?
            WlIntrRxEndTask();
        
        if (pTxCtrl->Mp.pMacFrm->MacHeader.Tx.rsv_RetryCount & 0xFF) {
            wlMan->Counter.multiPoll.txMp += pTxCtrl->Mp.pMacFrm->MacHeader.Tx.rsv_RetryCount & 0xFF;
        } else {
            wlMan->Counter.multiPoll.txMp += 1;
        }
        pTxCtrl->Mp.OutCount++;
        pTxCtrl->pMpEndInd->mpKey.errBitmap = pTxCtrl->pMpEndInd->mpKey.bitmap ^ pTxCtrl->Mp.pMacFrm->MacHeader.Tx.Status2;
        pTxCtrl->RestBitmap = pTxCtrl->pMpEndInd->mpKey.bitmap;
        pTxCtrl->Mp.Busy = 0;
        
        if (wlMan->Work.TmpttPs)
            WDisableTmpttPowerSave();
        
        SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pTxCtrl->pMpEndInd);
    }
}

void SetParentTbttTxqTask() { // WlIntrTask.c:770
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r4 - :772
    
    if (W_LISTENCOUNT == 0 && pHeapMan->TxPri[2].Count) { // :782
        W_TXREQ_SET = 8; // :785
        TxqPri(2); // :786
        return;
    }
    
    if ((wlMan->CamMan.PowerMgtMode & ~wlMan->CamMan.NotClass3) == 0) { // :791
        W_TXREQ_SET = 8; // :793
        if (pHeapMan->TxPri[2].Count) { // :795
            TxqPri(2); // :797
        }
    }
    
    W_TXREQ_SET = 5; // :802
    
    if (pHeapMan->TxPri[1].Count) TxqPri(1); // :803
    if (pHeapMan->TxPri[0].Count) TxqPri(0); // :804
}

STATIC RXFRM* TakeoutRxFrame(RXFRM_MAC* pMFrm, u32 length) { // WlIntrTask.c:832
    RXPACKET* pPacket; // r0 - :834
    RXFRM* pFrm; // r0 - :835
    
    // TODO find what is 0x22 (sizeof)
    pPacket = (RXPACKET*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, length + 0x22);
    if (!pPacket)
        return 0;
    
    pFrm = &pPacket->frame;
    DMA_Read(&pFrm->MacHeader, pMFrm, length + 0xC); // TODO sizeof ? = 0xC
    pFrm->FirmHeader.Length = length - 0x18; // TODO sizeof ? = 0x18
    pFrm->MacHeader.Rx.Service_Rate = (pFrm->MacHeader.Rx.Service_Rate & 0xFF) | ((u16)(pFrm->MacHeader.Rx.rsv_RSSI & 0xFF) << 8);
    return pFrm;
}