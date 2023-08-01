#include "Mongoose.h"

extern u16 MP_ADRS[3];

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
    WlMaMpReq* pReq = (WlMaMpReq*)pReqt; // r0 - :365
    WlMaMpCfm* pCfm = (WlMaMpCfm*)pCfmt; // not in nef
    WlMpKeyData* pKeyData; // r1 - :366
    TXMPFRM_MAC* pFrm; // r5 - :367
    WORK_PARAM* pWork = &wlMan->Work; // r0 - :368
    CONFIG_PARAM* pConfig = &wlMan->Config; // r7 - :369
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r0 - :370
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r6 - :371
    u32 wlOperation = wlMan->WlOperation; // None - :372
    u16 i, resume; // r1, r7 - :373
    u32 oneLen, x, pollCnt, delt, aid; // r0, r0, r8, r11, r2 - :374
    u32 bDataUp = 1; // r9 - :375
    u32 mp_time; // r3 - :376

    pCfm->header.length = 1;
    
    if (pConfig->Mode != 1) return 11;
    if (pTxCtrl->Mp.Busy) return 8;
    
    pFrm = (TXMPFRM_MAC*)pTxCtrl->Mp.pMacFrm;
    
    if (pReq->resume & 0x8000) {
        if ((pReq->resume & 2) == 0)
            pReq->txop = pTxCtrl->TXOP;
        
        if ((pReq->resume & 4) == 0)
            pReq->pollBitmap = pTxCtrl->RestBitmap;
        
        if ((pReq->resume & 8) == 0)
            pReq->tmptt = pTxCtrl->TMPTT;
        
        if ((pReq->resume & 0x10) == 0) {
            bDataUp = 0;
            pReq->dataLength = pTxCtrl->DataLength;
        }
    }
    
    if (pReq->dataLength > 0x204)
        return 5;
    
    // okay, somehow, it's using the fact that it overflows to zero
    i = 2;
    pollCnt = 0;
    while (i != 0) {
        if (pReq->pollBitmap & i) // :456
            pollCnt++;

        i <<= 1;
    }
    
    pTxCtrl->TXOP = pReq->txop;
    
    if (pReq->txop & 0x8000) {
        pReq->txop &= 0x7FFF;
        oneLen = (u32)(pReq->txop - 0xD0) >> 2; // TODO: figure out which variable it's actually using
        pFrm->Dot11Header.DurationID = 0xEA;
        
        if (oneLen > 0x10000)
            return 5;
        
    } else {
        u32 macBug_byte, macBug_us, macBug_dur; // r3, r1, r2 - :484
        
        if (wlOperation & 2) {
            macBug_byte = 1;
            macBug_us = 2;
            macBug_dur = 6;
        } else {
            macBug_byte = 0;
            macBug_us = 0;
            macBug_dur = 0;
        }
        
        oneLen = pReq->txop + macBug_byte;
        if (oneLen > macBug_byte + 0x206)
            return 5;

        pReq->txop = 4 * oneLen + 0xD0 + macBug_us;
        pFrm->Dot11Header.DurationID = macBug_dur + 0xEA;
    }
    
    oneLen = (oneLen + 9) & ~1;

    pTxCtrl->pMpEndInd = (WlMaMpEndInd*)AllocateHeapBuf(&pHeapMan->TmpBuf, oneLen * pollCnt + 0x1A);
    if (!pTxCtrl->pMpEndInd)
        return 8;

    pTxCtrl->Mp.Busy = 1;
    pTxCtrl->Mp.InCount++;
    pTxCtrl->TMPTT = pReq->tmptt;
    pTxCtrl->SetKeyMap = pReq->pollBitmap;
    pTxCtrl->GetKeyMap = 0;
    pTxCtrl->DataLength = pReq->dataLength;
    pTxCtrl->RetryLimit = pReq->retryLimit;

    pFrm->MacHeader.Tx.Status = 0;
    pFrm->MacHeader.Tx.Status2 = pReq->pollBitmap;
    pFrm->MacHeader.Tx.rsv_RetryCount = 0;
    pFrm->MacHeader.Tx.Service_Rate = 0x14;
    pFrm->MacHeader.Tx.MPDU = pReq->dataLength + 0x22;
    
    pFrm->Dot11Header.FrameCtrl.Data = 0x228;
    pFrm->Dot11Header.DurationID += (pReq->txop + 10) * pollCnt;
    WSetMacAdrs3(pFrm->Dot11Header.Adrs1, MP_ADRS, pWork->BSSID, pConfig->MacAdrs);

    if (pReq->resume & 0x8000 && pFrm->Dot11Header.SeqCtrl.Data != 0xFFFF) {
        resume = 0x4000;
    } else {
        resume = 0;
        pFrm->Dot11Header.SeqCtrl.Data = 0xFFFF;
    }

    pFrm->TXOP = pReq->txop;
    pFrm->Bitmap = pReq->pollBitmap;
    *((u16*)pFrm->Body) = pReq->wmHeader; // TODO: ugly, is there a struct behind this?

    if (bDataUp && pReq->dataLength) {
        WUpdateCounter();
        DMA_Write(&pFrm->Body[2], pReq->datap, pReq->dataLength);
    }

    if (wlOperation & 4) {
        u16* pId = (u16*)(((u32)&pFrm->Body[2 + pReq->dataLength] + 3) & ~3); // r0 - :610
        pId[0] = 0xB6B8;
        pId[1] = 0x1D46;
    }

    pTxCtrl->pMpEndInd->header.code = 0x184;
    pTxCtrl->pMpEndInd->header.length = (oneLen * pollCnt + 11) / 2;
    pTxCtrl->pMpEndInd->mpKey.bitmap = pReq->pollBitmap;
    pTxCtrl->pMpEndInd->mpKey.count = pollCnt;
    pTxCtrl->pMpEndInd->mpKey.length = oneLen;
    pTxCtrl->pMpEndInd->mpKey.txCount = 0;

    // again with this overflow thing
    pKeyData = pTxCtrl->pMpEndInd->mpKey.data;

    aid = 1;
    i = 2;
    while (i != 0) {
        if (pReq->pollBitmap & i) {
            pKeyData->length = -1;
            *(u16*)(&pKeyData->rate) = 0; // u8?
            pKeyData->noResponse = 0;
            pKeyData->aid = aid;
            pKeyData = (WlMpKeyData*)((char*)pKeyData + oneLen);
        }
        i <<= 1;
        aid++;
    }

    W_CMD_REPLYTIME = pReq->txop;
    W_CMD_TOTALTIME = pFrm->Dot11Header.DurationID;
    delt = 0x10000 - pReq->currTsf;

    if (pReq->tmptt == 0) {
        mp_time = 4 * (pReq->dataLength + 0x22) + 0x60;
        if (W_ID != 0x1440) {
            mp_time += 0x3E8;
        }

        mp_time += (pReq->txop * pollCnt) + 0x388;
        mp_time = (mp_time + 0x32) / 10;

        x = OS_DisableInterrupts();
        W_CMD_COUNT = mp_time;
        W_TXBUF_CMD = (((u32)pFrm & 0x3FFF) >> 1) | 0x8000 | resume;
        OS_RestoreInterrupts(x);
        
    } else {
        x = OS_DisableInterrupts();
        mp_time = (u16)(delt + W_US_COUNT0) / 10;

        if (mp_time + 3 < pReq->tmptt) {
            W_CMD_COUNT = pReq->tmptt - mp_time - 1;
            W_TXBUF_CMD = (((u32)pFrm & 0x3FFF) >> 1) | 0x8000 | resume;
            OS_RestoreInterrupts(x);
            
        } else {
            OS_RestoreInterrupts(x);
            ReleaseHeapBuf(&pHeapMan->TmpBuf, (HEAPBUF_HEADER*)pTxCtrl->pMpEndInd);
            pTxCtrl->Mp.Busy = 0;
            pTxCtrl->Mp.InCount--;
            return 5;
        }
    }

    return 0;
}

u16 MA_TestDataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MA.c:775
    WlMaDataReq* pReq; // r0 - :782
    TXFRM* pFrm; // r0 - :783
}

u16 MA_ClrDataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt) { // MA.c:850
    WlMaClrDataReq* pReq = (WlMaClrDataReq*)pReqt; // r0 - :853
    WlMaClrDataCfm* pCfm = (WlMaClrDataCfm*)pCfmt;
    
    pCfm->header.length = 1;
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
