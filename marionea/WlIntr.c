#include "Mongoose.h"

#define EXC_IRQ_IF (*(volatile u32*)0x0380FFF8)

static void WlIntrPreTbtt();
static void WlIntrTbtt();
static void WlIntrActEnd();
static void WlIntrRfWakeup();
static void WlIntrCntOvf();
static void WlIntrTxErr();
static void WlIntrRxCntup();
static void WlIntrTxEnd();
static void WlIntrRxEnd();
static void WlIntrMpEnd(u32 bMacBugPatch);
static void WlIntrStartTx();
static void WlIntrStartRx();
STATIC void SetParentTbttTxq();
STATIC u32 CheckKeyTxEnd();
static u32 CheckKeyTxEndMain(TXQ* pTxq);

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
                AddTask(1, 13);
            }
        }
    }
    
    pWork->PowerState = 1;
}

static void WlIntrTbtt() { // WlIntr.c:240
    WORK_PARAM* pWork; // r5 - :242
    CONFIG_PARAM* pConfig; // r7 - :243
    TX_CTRL* pTxCtrl; // r8 - :244
    u8* p; // r0 - :245
    u32 txq, i, vtsf; // r0, r7, r0 - :246
    u32 bWakeUp; // r1 - :247
    TXQ* pTxq; // r9 - :411
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
    TX_CTRL* pTxCtrl; // r7 - :859
    u32 txFrm; // r8 - :860
    u16 setmap, pollmap; // r0, r1 - :993
    WlMpKeyData* pKeyData; // r2 - :994
}

static void WlIntrRxEnd() { // WlIntr.c:1101
    WORK_PARAM* pWork; // r6 - :1103
    RX_CTRL* pRxCtrl; // r7 - :1104
    RXFRM_MAC* pMFrm; // r0 - :1105
    u32 wlOperation; // r4 - :1106
    u16 *pStatus, *pErrSts, *pTimeStamp, *pMPDU, *pDur; // r8, r0, r0, r0, None - :1107
    u32 bnry, curr, next_bnry, length, frameType; // r9, r10, r9, r1, r0 - :1108
    u16 keySts; // r0 - :1109
    
    if (0) {
        RXFRM_MAC* pNextMFrm; // r0 - :1247
        u16 nextStatus, nextRate; // r2, r0 - :1248
    }
    
    if (0) {
        u16 fc; // None - :1309
        u16 seqCtrl; // r10 - :1310
    }
    
    if (0) {
        u16 bkcurr; // r0 - :1394
        u16 keySts; // r0 - :1395
    }
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
        
        AddTask(0, 16); // :1528
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
    WORK_PARAM* pWork; // r4 - :1633
    TX_CTRL* pTxCtrl; // r5 - :1634
    
    u32 i, tm; // r8, r6 - :1651
    u16* p; // r0 - :1652
    u16 curr, delt, tm_delt; // r0, r2, r0 - :1653
}

STATIC void SetParentTbttTxq() { // WlIntr.c:1857
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
        AddTask(0, 0xE);
    
    AddTask(0, 0x14);
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

STATIC u32 CheckKeyTxEnd() { // WlIntr.c:2063
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
        if ((GetTxBufAddr(pTxq->pMacFrm) | 0x8000) != W_TXBUF_REPLY1) {
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

void InitializeIntr() { // WlIntr.c:2167
    OS_SetIrqFunction(0x1000000u, WlIntr);
    OS_EnableIrqMask(0x1000000u);
}

void ReleaseIntr() { // WlIntr.c:2190
    OS_DisableIrqMask(0x1000000u);
    OS_SetIrqFunction(0x1000000u, 0);
}