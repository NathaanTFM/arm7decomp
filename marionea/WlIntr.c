#define MARIONEA_INTERNAL 1
#include "WlIntr.h"

#define EXC_IRQ_IF (*(volatile u32*)0x0380FFF8)

static void MultiPollRevicedClearSeq();
static void WlIntrPreTbtt();
static void WlIntrTbtt();
static void WlIntrActEnd();
static void WlIntrRfWakeup();
static void WlIntrCntOvf();
static void WlIntrTxErr();
static void WlIntrRxCntup();
static void WlIntrTxEnd();
static void WlIntrRxEnd();
static void WlIntrMpEnd(u32 bMacBugPatch); // :1531
static void WlIntrStartTx();
static void WlIntrStartRx();
static void SetParentTbttTxq();
static u32 CheckKeyTxEnd();
static u32 CheckKeyTxEndMain(TXQ* pTxq);

// missing function from latest, but exists
static void MultiPollRevicedClearSeq() { 
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl;
    u16 bkKeyIn;
    
    if (pTxCtrl->BkKeyOut != 0xFFFF) {
        bkKeyIn = W_TXBUF_REPLY1;
        W_TXBUF_REPLY1 = pTxCtrl->BkKeyOut;
        W_RXCNT = W_RXCNT | 0x80;
        W_TXBUF_REPLY1 = bkKeyIn;
        pTxCtrl->BkKeyOut = 0xFFFF;
    }
}

#pragma dont_inline on

void WlIntr() { // WlIntr.c:80
    while (1) {
        u32 isr = W_IF & W_IE; // r6 - :82
        if (isr == 0)
            break;
        
        if ((isr & 0x80) != 0)  WlIntrStartTx();
        if ((isr & 0x40) != 0)  WlIntrStartRx();
        if ((isr & 0x8000) != 0)  WlIntrPreTbtt();
        if ((isr & 0x4000) != 0)  WlIntrTbtt();
        if ((isr & 0x2000) != 0)  WlIntrActEnd();
        if ((isr & 0x800) != 0)  WlIntrRfWakeup();
        if ((isr & 0x8) != 0)  WlIntrTxErr();
        if ((isr & 0x4) != 0)  WlIntrRxCntup();
        if ((isr & 0x1) != 0)  WlIntrRxEnd();
        if ((isr & 0x30) != 0)  WlIntrCntOvf();
        if ((isr & 0x2) != 0)  WlIntrTxEnd();
        if ((isr & 0x1000) != 0)  WlIntrMpEnd(1);
    }
    EXC_IRQ_IF |= 0x1000000;
}

#pragma dont_inline off

static void WlIntrPreTbtt() { // WlIntr.c:175
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :177
    
    W_IF = 0x8000;
    
    if (pWork->STA == 0x40 && pWork->BeaconLostTh) {
        if (pWork->CurrListenInterval == pWork->ListenInterval) {
            pWork->BeaconLostCnt++;
            if (pWork->BeaconLostCnt > pWork->BeaconLostTh) {
                pWork->BeaconLostCnt = 0;
                AddTask(PRIORITY_HIGH, TASK_BEACON_LOST);
            }
        }
    }
    
    pWork->PowerState = 1;
}

static void WlIntrTbtt() { // WlIntr.c:240
    WORK_PARAM* pWork = &wlMan->Work; // r5 - :242
    CONFIG_PARAM* pConfig = &wlMan->Config; // r7 - :243
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r8 - :244
    u8* p; // r0 - :245
    u32 txq, i, vtsf; // r0, r7, r0 - :246
    u32 bWakeUp; // r1 - :247
    TXQ* pTxq; // r9 - :411

    W_IF = 0x4000;
    switch (pWork->Mode) {
        case 1:
            p = &pTxCtrl->Beacon.pMacFrm->Body[pWork->Ofst.Beacon.GameInfo];
            vtsf = global_vtsf_var;
            WL_WriteByte(&p[8], vtsf);
            WL_WriteByte(&p[9], vtsf >> 8);

            if (pWork->PowerMgtMode == 1) {
                W_POST_BEACON = pConfig->ActiveZone + W_POST_BEACON + 1;
            }

            wlMan->CamMan.PowerState = ~wlMan->CamMan.PowerMgtMode | wlMan->CamMan.NotClass3;

            txq = W_TXBUSY;
            if ((txq & 0x18) != 0 || (txq & 0x6) == 0x2) {
                pTxCtrl->Flag &= ~2;
                SetParentTbttTxq();
            } else {
                pTxCtrl->Flag |= 2;
            }
            break;
        
        case 2:
            if (pWork->bSynchro) {
                W_POST_BEACON = pConfig->ActiveZone + W_POST_BEACON + 1;
            } else {
                W_POST_BEACON = 0xFFFF;
            }

            if (pWork->bFirstTbtt == 2)
                WSetPowerState(2);
            
            //break;
        
        case 3:
            if (pWork->STA != 0x40) {
                bWakeUp = 1;
                
            } else {
                bWakeUp = 0;
                if (pWork->CurrListenInterval == 1)
                    bWakeUp = 1;

                if (pWork->RxDtims != 0 && (pWork->DTIMCount == 1 || (pWork->DTIMCount == 0 && pWork->DTIMPeriod == 1)))
                    bWakeUp = 1;
            }

            if (bWakeUp) {
                W_POWER_TX |= 1;
            } else {
                W_POWER_TX &= ~1;
            }

            if (W_CMD_COUNT > 10)
                W_POWER_unk = 0;

            pWork->CurrListenInterval--;
            if (pWork->CurrListenInterval == 0) {
                pWork->CurrListenInterval = pWork->ListenInterval;
            }

            if ((pWork->DTIMCount--) == 0) {
                pWork->DTIMCount = pWork->DTIMPeriod - 1;
            }

            for (i = 0; i < 2; i++) {
                pTxq = &pTxCtrl->Txq[i];
                if (pTxq->Busy && pTxq->pFrm->MacHeader.Tx.Status == 0 && CheckFrameTimeout(pTxq->pFrm)) {
                    ResetTxqPri(i);
                    pTxq->pMacFrm->MacHeader.Tx.Status = 2;
                    AddTask(PRIORITY_HIGHEST, TASK_WL_INTR_TX_END);
                    pTxCtrl->TimeOutFrm++;
                }
            }
            W_TXREQ_SET = 13;
            break;
    }
}

static void WlIntrActEnd() { // WlIntr.c:457
    WORK_PARAM* pWork = &wlMan->Work; // r0 - :459
    
    W_IF = 0x2000;
    W_TXREQ_RESET = 0xD;
    
    if (pWork->bFirstTbtt == 1) {
        pWork->bFirstTbtt = 2;
        
    } else if (pWork->bFirstTbtt == 2) {
        pWork->bFirstTbtt = 0;
        
    } else if (pWork->Mode == 2 && pWork->STA != 0x40) {
        W_POWER_unk = 0;
    }
}

static void WlIntrRfWakeup() { // WlIntr.c:559
    W_IF = 0x800;
}

static void WlIntrCntOvf() { // WlIntr.c:625
    WUpdateCounter();
    W_RXSTAT_OVF_IF = 0xFFFF;
    W_IF = 0x30;
}

static void WlIntrTxErr() { // WlIntr.c:625
    TXFRM_MAC* pMFrm; // r0 - :627
    u32 i; // r2 - :628
    
    W_IF = 8;
    if (wlMan->Config.Diversity) {
        if ((W_RF_PINS & 1) == 0) {
            W_X_290h ^= 1;
        }
    }
    
    if (wlMan->WlOperation & 8) {
        WORK_PARAM* pWork = &wlMan->Work; // r0 - :650
        TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r3 - :651
        
        for (i = 0; i < 3; i++) {
            if (pTxCtrl->Txq[i].Busy) {
                pMFrm = pTxCtrl->Txq[i].pMacFrm;
                
                if (pMFrm->Dot11Header.FrameCtrl.Data & 0x4000) {
                    if (pMFrm->MacHeader.Tx.rsv_RetryCount & 0xFF) {
                        u16 *pIcv = (u16*)(((u32)&pMFrm->Dot11Header + pMFrm->MacHeader.Tx.MPDU - 7) & ~0x1); // r0 - :662
                        
                        if (pIcv[0] == 0 && pIcv[1] == 0) {
                            pMFrm->MacHeader.Tx.rsv_RetryCount = 0;
                            W_WEP_CNT = 0;
                            W_WEP_CNT = 0x8000;
                            pWork->WepErrCount++;
                        }
                    }
                }
            }
        }
    }
}

static void WlIntrRxCntup() { // WlIntr.c:705
    RX_CTRL* pRxCtrl = &wlMan->RxCtrl; // r1 - :707
    TXQ* pTxq = wlMan->TxCtrl.Txq; // r12 - :708
    u32 isr; // r2 - :709
    
    W_IF = 4;
    isr = W_RXSTAT_INC_IF;
    
    if ((wlMan->WlOperation & 8) != 0 && (isr & 0x400) != 0) {
        u16 map = W_TXREQ_READ; // r0 - :726
        
        if (!( ((map & 1) && pTxq[0].Busy)
            || ((map & 4) && pTxq[1].Busy)
            || ((map & 8) && pTxq[2].Busy)
            || (W_RF_PINS & 1)))
        {
            W_WEP_CNT = 0; // :743
            W_WEP_CNT = 0x8000;
            pRxCtrl->IcvOkCntFlag = 0;
            
        } else {
            if (pRxCtrl->IcvOkCntFlag++ > 12) { // :757
                pRxCtrl->IcvOkCntFlag = 0; // :765
                W_WEP_CNT = 0;
                W_WEP_CNT = 0x8000;
                wlMan->Work.WepErrCount++;
            }
            
        }
    }
    
    if ((wlMan->WlOperation & 1) != 0 && (isr & 0x60) != 0) {
        u16 curr = W_RXBUF_WRCSR; // r0 - :784
        
        if (curr >= (W_RXBUF_END - 0x4000) / 2 || curr < ((W_RXBUF_BEGIN - 0x4000) / 2)) {
            W_RXBUF_WR_ADDR = W_RXBUF_READCSR;
            W_RXCNT = 0x8001;
        }
        
        WCheckTxBuf();
    }
    
}

static void WlIntrTxEnd() { // WlIntr.c:857
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r7 - :859
    u32 txFrm; // r8 - :860
    
    W_IF = 2;
    if (wlMan->Work.STA == 0x12) {
        IntrCarrierSuppresionSignal(); // :873
        
    } else {
        txFrm = W_TXSTAT & 0xF00;
        
        switch (txFrm) { // :882
            case 0x300:
                if (pTxCtrl->Flag & 2)
                    SetParentTbttTxq();
                
                wlMan->Counter.tx.beacon++;
                AddTask(PRIORITY_HIGHEST, TASK_WL_INTR_TX_BEACON);
                break;
                
            case 0x800:
                if (pTxCtrl->RetryLimit <= (u16)(pTxCtrl->Mp.pMacFrm->MacHeader.Tx.rsv_RetryCount & 0xFF)) {
                    W_TXREQ_RESET = 2;
                    pTxCtrl->MpBlkCnt++;
                }
                pTxCtrl->pMpEndInd->mpKey.txCount++;
                break;
                
            case 0xB00:
                W_RX_MAC_0 = 0xFFFF;
                W_RX_MAC_1 = 0xFFFF;
                W_INTERNAL_11 = 0xFFFF;
                W_INTERNAL_12 = 0xFFFF;
                
                if (pTxCtrl->Mp.Busy) {
                    if (pTxCtrl->Mp.pMacFrm->Dot11Header.SeqCtrl.Data == 0xFFFF && pTxCtrl->Mp.pMacFrm->MacHeader.Tx.rsv_RetryCount > 0) {
                        pTxCtrl->Mp.pMacFrm->MacHeader.Tx.rsv_RetryCount = 0;
                        pTxCtrl->Mp.pMacFrm->MacHeader.Tx.Status2 = pTxCtrl->SetKeyMap;
                    }
                }
                
                u16 setmap, pollmap; // r0, r1 - :993
                WlMpKeyData* pKeyData; // r2 - :994
                
                pollmap = pTxCtrl->Mp.pMacFrm->MacHeader.Tx.Status2;
                setmap = pTxCtrl->SetKeyMap;
                pKeyData = pTxCtrl->pMpEndInd->mpKey.data;
                
                if (pollmap > 1 && wlMan->Config.Diversity) {
                    if ((W_RF_PINS & 1) == 0) {
                        W_X_290h = W_X_290h ^ 1;
                    }
                }
                
                if (wlMan->WlOperation & 0x40) {
                    while (pollmap > 1) {
                        pollmap = pollmap / 2;
                        setmap = setmap / 2;
                        
                        if (pollmap & 1) {
                            pKeyData->noResponse++;
                        }
                        if (setmap & 1) {
                            pKeyData = (WlMpKeyData*)((u32)pKeyData + pTxCtrl->pMpEndInd->mpKey.length);
                        }
                    }
                }
                
                break;
        }
        
        if (txFrm != 0x800 && (W_TXREQ_READ & 2) == 0) {
            if (pTxCtrl->Mp.Busy) {
                W_TXBUF_RESET = 2;
                W_POWER_unk = 0;
                
                if (W_IF & 0x1000) {
                    pTxCtrl->MpLastOk++;
                } else {
                    WlIntrMpEnd(2);
                }
                pTxCtrl->MpRstCnt++;
            }
            
            W_TXREQ_SET = 2;
        }
        
        AddTask(PRIORITY_HIGHEST, TASK_WL_INTR_TX_END);
    }
}

void WlIntrRxEnd() { // WlIntr.c:1101
    WORK_PARAM* pWork = &wlMan->Work; // r6 - :1103
    RX_CTRL* pRxCtrl = &wlMan->RxCtrl; // r7 - :1104
    RXFRM_MAC* pMFrm; // r0 - :1105
    u32 wlOperation = wlMan->WlOperation; // r4 - :1106
    u16 *pStatus, *pErrSts, *pTimeStamp, *pMPDU, *pDur; // r8, r0, r0, r0, None - :1107
    u32 bnry, curr, next_bnry, length, frameType; // r9, r10, r9, r1, r0 - :1108
    u16 keySts; // r0 - :1109
    u32 tm[4]; // not in nef, but probably exists

    W_IF = 1;
    if (pWork->Mode == 0) {
        W_RXBUF_READCSR = W_RXBUF_WRCSR;
    }

    for (;;) {
        bnry = pRxCtrl->wlCurr;
        curr = W_RXBUF_WRCSR;
        if (bnry == curr)
            break;

        tm[0] = W_US_COUNT0;
        tm[1] = W_US_COUNT1;
        tm[2] = W_US_COUNT0;
        tm[3] = W_US_COUNT1;

        if (tm[0] > tm[2])
            tm[0] = ((tm[2] >> 4) | (tm[3] << 12));
        else
            tm[0] = ((tm[0] >> 4) | (tm[1] << 12));

        if (bnry >= 2246 && bnry <= 2287)
            WUpdateCounter();
        
        pMFrm = (RXFRM_MAC*)&W_MACMEM(2 * bnry);
        pStatus = (u16 *)pMFrm;
        pErrSts = (u16 *)AdjustRingPointer(/*&pMFrm->MacHeader.Rx.NextBnry*/(u16*)((u32)pStatus+2));
        pTimeStamp = (u16 *)AdjustRingPointer(/*&pMFrm->MacHeader.Rx.TimeStamp*/(u16*)((u32)pErrSts+2));
        pMPDU = (u16 *)AdjustRingPointer(/*&pMFrm->MacHeader.Rx.MPDU*/(u16*)((u32)pTimeStamp+4));
        pDur = (u16 *)AdjustRingPointer(&pMFrm->Dot11Header.DurationID);
        *pStatus |= (2 * (*pErrSts)) & 0x400;
        *pTimeStamp = tm[0];

        length = *pMPDU;
        next_bnry = 2 * ((length + 2 * bnry + 15) / 4);
        if (next_bnry >= 0xFB0) {
            next_bnry -= pWork->Ofst.RxBuf.Size / 2;
        }
        if (length > 0x92C) {
            *pStatus = 0xFFFF;
            next_bnry = curr;
            pWork->CurrErrCount++;
            
        } else {
            if ((wlOperation & 1) != 0 && next_bnry != curr) {
                RXFRM_MAC* pNextMFrm = (RXFRM_MAC*)&W_MACMEM(2 * next_bnry); // r0 - :1247
                u16 nextStatus, nextRate; // r2, r0 - :1248

                nextStatus = pNextMFrm->MacHeader.Rx.Status;
                
                if ((u8*)pNextMFrm < (u8*)0x4805F5A)
                    nextRate = *(u16*)&pNextMFrm->MacHeader.Rx.Service_Rate;
                else
                    nextRate = *(u16*)((u32)pNextMFrm - (u32)wlMan->Work.Ofst.RxBuf.Size + 6);
                
                if ((nextStatus & 0x7C00) != 0 || (nextRate != 10 && nextRate != 20) || length > 0xFFF) {
                    pWork->CurrErrCount++;
                    *pStatus = 0xFFFF;
                    pRxCtrl->wlCurr = curr;
                    *pErrSts = curr;
                    break;
                }
            }
        }

        frameType = (*pStatus) & 0xF;
        
        if (frameType == 12) {
            u16 fc = *(u16*)AdjustRingPointer(&pMFrm->Dot11Header.FrameCtrl.Data); // None - :1309
            u16 seqCtrl = *(u16*)AdjustRingPointer(&pMFrm->Dot11Header.SeqCtrl.Data); // r10 - :1310
    
            if (pRxCtrl->LastMpSeq == seqCtrl && (fc & 0x800) != 0) {
                wlMan->Counter.rx.mpDuplicateErr++;
                *pStatus = 0xFFFF;
                
            } else if (wlMan->Config.NullKeyRes == 0 && pWork->STA == 0x40) {
                if (W_AID_LOW != 0 && ((W_TXBUF_REPLY2 & 0x8000) != 0 || (W_TXBUF_REPLY1 & 0x8000) == 0)) {
                    OS_CancelAlarm(&wlMan->KeyAlarm);
                    OS_SetAlarm(&wlMan->KeyAlarm, ((33514 * (u64)(*pDur))  >> 6) / 1000, (void *)WClearKSID, 0);
                    
                } else {
                    seqCtrl = 0xFFFF;
                    W_RX_UNK = 0xFFFF;
                    W_INTERNAL_11 = 0xFFFF;
                    W_INTERNAL_12 = 0xFFFF;
                    *pStatus = 0xFFFF;
                }
            }
            pRxCtrl->LastMpSeq = seqCtrl;
            keySts = CheckKeyTxEnd();
            if ((keySts & 1) != 0)
                wlMan->Counter.multiPoll.txNull++;
            
        } else if (frameType == 13) {
            if (wlMan->Config.NullKeyRes == 0 && pWork->STA == 0x40) {
                if (W_AID_LOW == 0 || (W_TXBUF_REPLY2 & 0x8000) == 0) {
                    *pStatus = 0xFFFF;
                }
            }
        }
        pRxCtrl->wlCurr = next_bnry;
        *pErrSts = next_bnry;
    }
    
    if (wlOperation & 1) {
        u16 bkcurr = W_RXBUF_WRCSR; // r0 - :1394
        u16 keySts = CheckKeyTxEnd(); // r0 - :1395

        if (keySts && bkcurr == W_RXBUF_WRCSR) {
            if (keySts & 2)
                SetFatalErr(0x80);
            else if (keySts & 1)
                SetFatalErr(0x100);
        }
    }

    if (W_RXBUF_READCSR != W_RXBUF_WRCSR)
        AddTask(0, 0xF);
}

static void WlIntrMpEnd(u32 bMacBugPatch) { // WlIntr.c:1449
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r3 - :1451
    u32 cnt; // r2 - :1452
    
    W_IF = 0x1000; // :1458
    
    if (pTxCtrl->Mp.Busy) { // :1465
        if (wlMan->WlOperation & 0x10 && bMacBugPatch) { // :1474, :1476?
            u16 pri = W_TXBUSY; // r1 - :1478
            u16 state = W_RF_STATUS; // r0 - :1479
            
            if ((state == 3 || state == 5) && !pri) {
                u16 map; // r0 - :1485
                
                map = pTxCtrl->Mp.pMacFrm->MacHeader.Tx.Status2; // :1488
                cnt = 0; // :1489
                while (map) { // :1490
                    cnt += map & 1; // :1492
                    map >>= 1; // :1493
                }
                
                SetupUsTimeOut( // :1496
                    // what the fuck??? TODO: fix this mess
                    (u16)(cnt * ((*(u16*)pTxCtrl->Mp.pMacFrm->Body) + 10) + 0xC0 + (4 * pTxCtrl->Mp.pMacFrm->MacHeader.Tx.MPDU)),
                    MacBugTxMp
                );
                wlMan->Work.MpEndErrCount++; // :1500
                return; // :1501
            }
        }
        
        AddTask(PRIORITY_HIGHEST, TASK_WL_INTR_MP_END); // :1528
    }
} // :1531

static void WlIntrStartTx() { // WlIntr.c:1548
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r0 - :1550
    u32 i, cnt; // None, r2 - :1552
    u16 sts; // r0 - :1564
    u16 adr; // r0 - :1565
    
    W_IF = 0x80;
    
    if (wlMan->WlOperation & 0x20) {
        sts = W_RF_STATUS & 0xFF;
        adr = W_RXTX_ADDR;
        
        if (sts >= 3 && sts <= 5 && adr >= (((u32)(pTxCtrl->Key[0].pMacFrm) >> 1) & 0xFFF) && adr <= (((u32)(pTxCtrl->Txq[2].pMacFrm) >> 1) & 0xFFF)) {
            W_X_244h |= 0x80;
            W_X_244h &= ~0x80;
        }
    }
    
    if (W_ID != 0x1440) {
        if ((W_RF_PINS & 0x42) == 0x42) {
            cnt = W_INTERNAL_14;
            if (cnt) {
                i = 0;
                while (cnt == W_INTERNAL_14) {
                    if (i++ > 0x3E8) {
                        SetFatalErr(0x40);
                        return;
                    }
                }
            }
        }
    }
}

static void WlIntrStartRx() { // WlIntr.c:1631
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :1633
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r5 - :1634
    
    W_IF = 0x40;
    
    if (wlMan->WlOperation & 0x20 && pTxCtrl->BkKeyOut == 0xFFFF && (W_RF_PINS & 3) == 3) {
        u32 i, tm; // r8, r6 - :1651
        u16* p; // r0 - :1652
        u16 curr, delt, tm_delt; // r0, r2, r0 - :1653
    
        if (W_RXTX_ADDR < (int)((W_RXBUF_BEGIN / 2) & 0xFFF))
            return;
        
        {
            curr = (u16)W_RXBUF_WRCSR;
            
            p = (u16*)&W_MACMEM(2 * curr);
            p = (u16*)AdjustRingPointer(p + 4);
            p = (u16*)AdjustRingPointer(p + 2);
            
            if ((*p & 0xE7FF) == 0x228) {
                p = (u16 *)AdjustRingPointer(p + 1);
                tm = W_US_COUNT0 - 0x10000; 
                
                for (;;) {
                    delt = W_RXTX_ADDR - curr;
                    if (delt & 0x8000)
                        delt += (u16)(pWork->Ofst.RxBuf.Size >> 1);
                    
                    if (delt > 0xE) 
                        break;

                    delt = W_US_COUNT0 - tm;

                    // somehow i couldn't do it without a goto
                    if (delt > 0x40)
                        goto exit;
                }
            
                p += 4;
                for (i = 0; i < 3; i++) {
                    p = (u16*)AdjustRingPointer(p);
                    if (*(p++) != pWork->BSSID[i])
                        return;
                    
                }
                
                p = (u16*)AdjustRingPointer(p + 5);
                    
                for (;;) {
                    delt = W_RXTX_ADDR - curr;
                    if (delt & 0x8000)
                        delt += (u16)(pWork->Ofst.RxBuf.Size >> 1);
                    
                    if (delt > 0x14) 
                        break;
                    
                    delt = W_US_COUNT0 - tm;
                    if (delt > 0x70)
                        goto exit;
                }
                
                if ((*p & (1 << W_AID_LOW)) == 0) {
                    pTxCtrl->BkKeyOut = W_TXBUF_REPLY2;
                    W_TXBUF_RESET = 0x40;
                    pWork->NotPollTxErrCount++;
                    while ((W_RF_PINS & 3) == 3);
                    
                    // fix variable?
                    W_X_244h = W_X_244h | 0x40;
                    W_X_244h = W_X_244h & 0xFFBF;
                    W_X_228h = 8;
                    W_X_228h = 0;
                    
                    MultiPollRevicedClearSeq();
                }
            }
        }
    }

    exit: ;
    
}

static void SetParentTbttTxq() { // WlIntr.c:1857
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r4 - :1859
    u32 bTask = 0; // r5 - :1860
    
    ResetTxqPri(2);
    ResetTxqPri(1);
    ResetTxqPri(0);
    
    if (pTxCtrl->Txq[2].Busy) {
        if (pTxCtrl->Txq[2].pMacFrm->MacHeader.Tx.Status) // Tx or Rx?
            bTask = 1;
        else
            pTxCtrl->Txq[2].Busy = 0;
    }
    
    if (pTxCtrl->Txq[1].Busy) {
        if (pTxCtrl->Txq[1].pMacFrm->MacHeader.Tx.Status) // Tx or Rx?
            bTask = 1;
        else
            pTxCtrl->Txq[1].Busy = 0;
    }
    
    if (pTxCtrl->Txq[0].Busy) {
        if (pTxCtrl->Txq[0].pMacFrm->MacHeader.Tx.Status) // Tx or Rx?
            bTask = 1;
        else
            pTxCtrl->Txq[0].Busy = 0;
    }
    
    if (bTask)
        AddTask(PRIORITY_HIGHEST, TASK_WL_INTR_TX_END);
    
    AddTask(PRIORITY_HIGHEST, TASK_SET_PARENT_TBTT_TXQ);
}

void MacBugTxMp() { // WlIntr.c:1934
    u32 x, i; // r4, r1 - :1938
    u16 seqCtrl, seqCtrl2; // r3, r0 - :1939

    x = OS_DisableIrqMask(0x1000000);
    seqCtrl = W_TX_SEQNO;
    
    W_X_244h = 0x1000;
    for (i = 100; i; --i) {
        seqCtrl2 = W_TX_SEQNO;
        if (seqCtrl != seqCtrl2)
            break;
    }
    
    W_X_244h = 0;
    WlIntrMpEnd(0);
    
    OS_EnableIrqMask(x);
}

void* AdjustRingPointer(void* p) { // WlIntr.c:1996
    if (p >= RING_END_PTR)
        return (char*)p - wlMan->Work.Ofst.RxBuf.Size;
    
    return p;
}

#pragma dont_inline on

static u32 CheckKeyTxEnd() { // WlIntr.c:2063
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r5 - :2065
    RX_CTRL* pRxCtrl = &wlMan->RxCtrl; // r6 - :2066
    u32 retVal = CheckKeyTxEndMain(&pTxCtrl->Key[0]) | CheckKeyTxEndMain(&pTxCtrl->Key[1]); // r0 - :2067
    
    if ((retVal & 0x1) != 0)
        pRxCtrl->TxKeyFrm |= 0x400;
    
    if ((retVal & 0x2) != 0)
        pRxCtrl->TxKeyFrm |= 0x8000;
    
    return retVal;
}

static u32 CheckKeyTxEndMain(TXQ* pTxq) { // WlIntr.c:2109
    u32 retVal = 0; // r4 - :2111
    
    if (pTxq->Busy == 2) {
        if ((GET_TX_BUF_ADDR(pTxq->pMacFrm) | 0x8000) != W_TXBUF_REPLY1) {
            pTxq->Busy = 1;
            retVal |= 1;
        }
    }
    
    if (pTxq->Busy == 1) {
        if (pTxq->pMacFrm->MacHeader.Tx.Status & 1) {
            TxEndKeyData(pTxq);
            retVal |= 2;
        }
    }
    
    return retVal;
}

#pragma dont_inline off

void InitializeIntr() { // WlIntr.c:2167
    OS_SetIrqFunction(0x1000000u, WlIntr);
    OS_EnableIrqMask(0x1000000u);
}

void ReleaseIntr() { // WlIntr.c:2190
    OS_DisableIrqMask(0x1000000u);
    OS_SetIrqFunction(0x1000000u, 0);
}