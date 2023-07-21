#include "Mongoose.h"

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

u32 RxMpFrame(RXFRM* pFrm) { // RxCtrl.c:324
    WORK_PARAM* pWork; // r5 - :326
    RX_CTRL* pRxCtrl; // r6 - :327
    WlMaMpInd* pInd; // r5 - :328
    u32 x; // r0 - :329
    u32 cnt; // r7 - :329
    u16 out; // r0 - :330
    u16 bitmap; // r2 - :330
}

void RxKeyDataFrame(RXFRM* pFrm) { // RxCtrl.c:450
    TX_CTRL* pTxCtrl; // r5 - :452
    WlMpKey* pMpKey; // r0 - :453
    WlMpKeyData* pKeyData; // r6 - :454
    u32 aid; // r0 - :455
    u32 camAdrs; // r6 - :455
    u16 bitmap; // r1 - :456
    DEAUTH_FRAME* pTxDeAuthFrm; // r0 - :519
}

u32 RxMpAckFrame(RXFRM* pFrm) { // RxCtrl.c:645
    WORK_PARAM* pWork; // r6 - :647
    WlMaMpAckInd* pInd; // r0 - :648
}

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

static void RxDisAssFrame(DISASS_FRAME* pFrm) { // RxCtrl.c:1146
    WORK_PARAM* pWork; // r1 - :1148
    DEAUTH_FRAME* pDeAuth; // r0 - :1149
    u32 cam_adrs; // r5 - :1150
    u32 st; // r0 - :1150
}

static void RxAssReqFrame(ASSREQ_FRAME* pFrm) { // RxCtrl.c:1228
    CONFIG_PARAM* pConfig; // r4 - :1230
    ASSREQ_BODY* pAssReq; // r0 - :1231
    DEAUTH_FRAME* pTxDeAuthFrm; // r0 - :1232
    ASSRES_FRAME* pTxFrm; // r0 - :1233
    u32 bodyLen; // r5 - :1234
    u16 cam_adrs; // r0 - :1235
    u16 stsCode; // r0 - :1235
    ELEMENT_CHECKER elementCheck; // None - :1236
}

static void RxProbeReqFrame(PRBREQ_FRAME* pFrm) { // RxCtrl.c:1703
    PRBRES_FRAME* pTxFrm; // r0 - :1705
    ELEMENT_CHECKER elementCheck; // None - :1706
}

static void RxProbeResFrame(PRBRES_FRAME* pFrm, ELEMENT_CHECKER* pChk) { // RxCtrl.c:1758
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

static void RxAuthFrame(AUTH_FRAME* pFrm) { // RxCtrl.c:1993
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

static void RxDeAuthFrame(DEAUTH_FRAME* pFrm) { // RxCtrl.c:2418
    WORK_PARAM* pWork; // r1 - :2420
    u32 cam_adrs; // r5 - :2422
}

static void ElementChecker(ELEMENT_CHECKER* p) { // RxCtrl.c:2564
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
