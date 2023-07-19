#include "Mongoose.h"

u16 MA_DataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MA.c:48
    WlMaDataReq* pReq; // r0 - :51
    WORK_PARAM* pWork; // r6 - :52
    CONFIG_PARAM* pConfig; // r7 - :53
    HEAP_MAN* pHeapMan; // r0 - :54
    TXFRM* pFrm; // r0 - :55
    u32 camAdrs; // r8 - :56
}

u16 MA_KeyDataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MA.c:223
    WlMaKeyDataReq* pReq; // r0 - :225
    WORK_PARAM* pWork; // r0 - :226
    CONFIG_PARAM* pConfig; // r4 - :227
    TX_CTRL* pTxCtrl; // r5 - :228
    u32 wlOperation; // r6 - :229
    TXFRM_MAC* pFrm; // r7 - :230
    u32 x; // r4 - :231
    u32 pos; // r0 - :231
    u16* pId; // r0 - :319
}

u16 MA_MpReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MA.c:363
    WlMaMpReq* pReq; // r0 - :365
    WlMpKeyData* pKeyData; // r1 - :366
    TXMPFRM_MAC* pFrm; // r5 - :367
    WORK_PARAM* pWork; // r0 - :368
    CONFIG_PARAM* pConfig; // r7 - :369
    HEAP_MAN* pHeapMan; // r0 - :370
    TX_CTRL* pTxCtrl; // r6 - :371
    u32 wlOperation; // None - :372
    u16 resume; // r7 - :373
    u16 i; // r1 - :373
    u32 aid; // r2 - :374
    u32 delt; // r11 - :374
    u32 pollCnt; // r8 - :374
    u32 x; // r0 - :374
    u32 oneLen; // r0 - :374
    u32 bDataUp; // r9 - :375
    u32 mp_time; // r3 - :376
    u32 macBug_dur; // r2 - :484
    u32 macBug_us; // r1 - :484
    u32 macBug_byte; // r3 - :484
    u16* pId; // r0 - :610
}

u16 MA_TestDataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MA.c:775
    WlMaDataReq* pReq; // r0 - :782
    TXFRM* pFrm; // r0 - :783
}

u16 MA_ClrDataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MA.c:850
    WlMaClrDataReq* pReq = (WlMaClrDataReq*)pReqt; // r0 - :853
    
    pCfmt->header.length = 1;
    if (pReq->flag & 1)
        ClearTxKeyData();
    if (pReq->flag & 2)
        ClearTxMp();
    if (pReq->flag & 4)
        ClearTxData();
    
    return 0;
}

void IssueMaDataConfirm(HEAPBUF_MAN* pBufMan, void* pBuf) { // MA.c:905
    WlMaDataReq* pReq = (WlMaDataReq*)pBuf; // r0 - :907
    WlMaDataCfm* pCfm = (WlMaDataCfm*)GetCfm(pReq); // r0 - :908
    
    pReq->header.code = pCfm->header.code;
    pCfm->header.length = 2;
    pCfm->resultCode = 0;
    pCfm->txStatus = pReq->frame.status;
    SendMessageToWmDirect(pBufMan, pBuf);
}
