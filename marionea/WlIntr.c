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
static void SetParentTbttTxq();
static u32 CheckKeyTxEnd();
static u32 CheckKeyTxEndMain(TXQ* pTxq);

#pragma dont_inline on

// WlIntr.c:80
void WlIntr() {
    while (1) {
        u32 isr = W_IF & W_IE;
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

// WlIntr.c:175
static void WlIntrPreTbtt() {
    WORK_PARAM* pWork; // r4
    pWork = &wlMan->Work;
    
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

// WlIntr.c:240
static void WlIntrTbtt() {
    TXQ* pTxq; // r9
    u32 bWakeUp; // r1
    u32 vtsf; // r0
    u32 i; // r7
    u32 txq; // r0
    u8* p; // r0
    TX_CTRL* pTxCtrl; // r8
    CONFIG_PARAM* pConfig; // r7
    WORK_PARAM* pWork; // r5
}

// WlIntr.c:457
static void WlIntrActEnd() {
    WORK_PARAM* pWork; // r0
    pWork = &wlMan->Work;
    
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

// WlIntr.c:559
static void WlIntrRfWakeup() {
    W_IF = 0x800;
}

// WlIntr.c:592
static void WlIntrCntOvf() {
    WUpdateCounter();
    W_RXSTAT_OVF_IF = 0xFFFF;
    W_IF = 0x30;
}

// WlIntr.c:625
static void WlIntrTxErr() {
    u16* pIcv; // r0
    TX_CTRL* pTxCtrl; // r3
    WORK_PARAM* pWork; // r0
    u32 i; // r2
    TXFRM_MAC* pMFrm; // r0
}

// WlIntr.c:705
static void WlIntrRxCntup() {
    u16 curr; // r0
    u16 map; // r0
    u32 isr; // r2
    TXQ* pTxq; // r12
    RX_CTRL* pRxCtrl; // r1
}

// WlIntr.c:857
static void WlIntrTxEnd() {
    WlMpKeyData* pKeyData; // r2
    u16 pollmap; // r1
    u16 setmap; // r0
    u32 txFrm; // r8
    TX_CTRL* pTxCtrl; // r7
}

// WlIntr.c:1101
static void WlIntrRxEnd() {
    u16 keySts; // r0
    u16 bkcurr; // r0
    u16 seqCtrl; // r10
    u16 fc; // None
    u16 nextRate; // r0
    u16 nextStatus; // r2
    RXFRM_MAC* pNextMFrm; // r0
    u32 frameType; // r0
    u32 length; // r1
    u32 next_bnry; // r9
    u32 curr; // r10
    u32 bnry; // r9
    u16* pDur; // None
    u16* pMPDU; // r0
    u16* pTimeStamp; // r0
    u16* pErrSts; // r0
    u16* pStatus; // r8
    u32 wlOperation; // r4
    RXFRM_MAC* pMFrm; // r0
    RX_CTRL* pRxCtrl; // r7
    WORK_PARAM* pWork; // r6
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

// WlIntr.c:1548
static void WlIntrStartTx() {
    u16 adr; // r0
    u16 sts; // r0
    u32 cnt; // r2
    u32 i; // None
    TX_CTRL* pTxCtrl; // r0
}

// WlIntr.c:1631
static void WlIntrStartRx() {
    u16 tm_delt; // r0
    u16 delt; // r2
    u16 curr; // r0
    u16* p; // r0
    u32 tm; // r6
    u32 i; // r8
    TX_CTRL* pTxCtrl; // r5
    WORK_PARAM* pWork; // r4
}

// WlIntr.c:1857
static void SetParentTbttTxq() {
    u32 bTask; // r5
    TX_CTRL* pTxCtrl; // r4
}

// WlIntr.c:1934
void MacBugTxMp() {
    u32 i, x; // r1, r4
    u16 seqCtrl, seqCtrl2; // r3, r0
    
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

// WlIntr.c:1996
void* AdjustRingPointer(void* p) {
    if (p >= RING_END_PTR)
        return (char*)p - wlMan->Work.Ofst.RxBuf.Size;
    
    return p;
}

// WlIntr.c:2063
static u32 CheckKeyTxEnd() {
    TX_CTRL* pTxCtrl; // r5
    RX_CTRL* pRxCtrl; // r6
    u32 retVal; // r0
    
    pTxCtrl = &wlMan->TxCtrl;
    pRxCtrl = &wlMan->RxCtrl;
    
    retVal = CheckKeyTxEndMain(&pTxCtrl->Key[0]) | CheckKeyTxEndMain(&pTxCtrl->Key[1]);
    if ((retVal & 0x1) != 0)
        pRxCtrl->TxKeyFrm |= 0x400;
    
    if ((retVal & 0x2) != 0)
        pRxCtrl->TxKeyFrm |= 0x8000;
    
    return retVal;
}

// WlIntr.c:2109
static u32 CheckKeyTxEndMain(TXQ* pTxq) {
    u32 retVal; // r4
    
    retVal = 0;
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

// WlIntr.c:2167
void InitializeIntr() {
    OS_SetIrqFunction(0x1000000u, WlIntr);
    OS_EnableIrqMask(0x1000000u);
}

// WlIntr.c:2190
void ReleaseIntr() {
    OS_DisableIrqMask(0x1000000u);
    OS_SetIrqFunction(0x1000000u, 0);
}