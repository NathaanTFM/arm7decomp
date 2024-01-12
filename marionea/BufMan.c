#include "BufMan.h"

void* AllocateHeapBuf(HEAPBUF_MAN* pBufMan, u32 Length) { // BufMan.c:71
    //u32 AllocPages; // r0 - :74   // is it used?
    HEAPBUF_HEADER* pAllocBuf; // r6 - :75
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r0 - :76
    
    if (!Length)
        return 0;
    
    switch (pHeapMan->HeapInfo.heapType) {
        case 0:
            pAllocBuf = (HEAPBUF_HEADER*)OS_AllocFromHeap(
                pHeapMan->HeapInfo.func.os.id,
                pHeapMan->HeapInfo.func.os.heapHandle,
                Length + sizeof(HEAPBUF_HEADER)
            );
            break;
            
        case 1:
            pAllocBuf = (HEAPBUF_HEADER*)(wlMan->HeapMan.HeapInfo.func.ext.alloc(Length + 12));
            break;
    }
    
    if (!pAllocBuf)
        return 0;
    
    pAllocBuf->BufID = 0xBF1D; // nice
    pAllocBuf->Flag = 0;
    AddHeapBuf(pBufMan, pAllocBuf);
    return pAllocBuf;
}

u32 ReleaseHeapBuf(HEAPBUF_MAN* pBufMan, void* pBuf) { // BufMan.c:162
    HEAPBUF_HEADER* pBufHeader = (HEAPBUF_HEADER*)pBuf; // r0 - :165
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r0 - :166
    u32 sts; // r6 - :167
    
    if (pBufHeader->BufID != 0xBF1D)
        return 1;
    
    sts = DeleteHeapBuf(pBufMan, pBuf);
    if (sts == 0) {
        switch (pHeapMan->HeapInfo.heapType) {
            case 0:
                OS_FreeToHeap(pHeapMan->HeapInfo.func.os.id, pHeapMan->HeapInfo.func.os.heapHandle, pBuf);
                break;
                
            case 1:
                pHeapMan->HeapInfo.func.ext.free(pBuf);
                break;
        }
    }
    
    return sts;
}

u32 MoveHeapBuf(HEAPBUF_MAN* pFromMan, HEAPBUF_MAN* pToMan, void* pBuf) { // BufMan.c:238
    HEAPBUF_HEADER* pBufHeader = (HEAPBUF_HEADER*)pBuf; // r0 - :241
    u32 sts; // r4 - :242
    u32 x; // r5 - :243
    
    if (pBufHeader->BufID != 0xBF1D)
        return 1;
    
    x = OS_DisableIrqMask(0x1000000);
    
    sts = DeleteHeapBuf(pFromMan, pBuf);
    if (sts == 0) {
        sts = AddHeapBuf(pToMan, pBuf);
    }
    
    OS_EnableIrqMask(x);
    return sts;
}

u32 NewHeapBuf(HEAPBUF_MAN* pBufMan, void* pBuf) { // BufMan.c:287
    HEAPBUF_HEADER* pBufHeader = (HEAPBUF_HEADER*)pBuf; // r0 - :289
    HEAPBUF_HEADER* pTailBuf; // r0 - :290
    u32 x; // r0 - :291
    
    x = OS_DisableIrqMask(0x1000000);
    
    if (pBufMan->Count == 0) {
        //pTailBuf = (HEAPBUF_HEADER*)-1;
        pBufHeader->Prev = -1;
        pBufMan->Head = pBufHeader;
        
    } else {
        pTailBuf = pBufMan->Tail;
        pBufHeader->Prev = (u32)pTailBuf;
        pTailBuf->Next = (u32)pBuf;
    }
    
    pBufHeader->Next = -1;
    pBufHeader->Flag = pBufMan->Flag;
    pBufHeader->BufID = 0xBF1D;
    
    pBufMan->Tail = pBufHeader;
    pBufMan->Count++;
    
    OS_EnableIrqMask(x);
    return 0;
}

u32 AddHeapBuf(HEAPBUF_MAN* pBufMan, void* pBuf) { // BufMan.c:342
    HEAPBUF_HEADER* pBufHeader = (HEAPBUF_HEADER*)pBuf; // r0 - :345
    HEAPBUF_HEADER* pTailBuf; // r0 - :346
    u32 x; // r0 - :347
    
    if (pBufHeader->BufID != 0xBF1D)
        return 1;
    
    if (pBufHeader->Flag != 0)
        return 2;
    
    x = OS_DisableIrqMask(0x1000000);
    
    if (pBufMan->Count == 0) {
        pTailBuf = (HEAPBUF_HEADER*)-1;
        pBufHeader->Prev = -1;
        
        pBufMan->Head = pBuf;
        
    } else {
        pTailBuf = (HEAPBUF_HEADER*)pBufMan->Tail;
        
        pBufHeader->Prev = (u32)pTailBuf;
        pTailBuf->Next = (u32)pBuf;
    }
    
    pBufHeader->Next = -1;
    pBufHeader->Flag = pBufMan->Flag;
    
    pBufMan->Tail = pBufHeader;
    pBufMan->Count++;
    
    OS_EnableIrqMask(x);
    return 0;
}

u32 DeleteHeapBuf(HEAPBUF_MAN* pBufMan, void* pBuf) { // BufMan.c:408
    HEAPBUF_HEADER* pBufHeader = (HEAPBUF_HEADER*)pBuf; // r0 - :411
    u32 x; // r0 - :412
    
    if (pBufHeader->BufID != 0xBF1D)
        return 1;
    
    if (pBufHeader->Flag != pBufMan->Flag)
        return 2;
    
    x = OS_DisableIrqMask(0x1000000);
    
    pBufMan->Count--;
    
    if (pBufMan->Count == 0) {
        pBufMan->Head = (HEAPBUF_HEADER*)-1;
        pBufMan->Tail = (HEAPBUF_HEADER*)-1;
        
    } else {
        if (pBuf == pBufMan->Head) {
            pBufMan->Head = (HEAPBUF_HEADER*)pBufHeader->Next;
            pBufMan->Head->Prev = -1;
            
        } else if (pBuf == pBufMan->Tail) {
            pBufMan->Tail = (HEAPBUF_HEADER*)pBufHeader->Prev;
            pBufMan->Tail->Next = -1;
            
        } else {
            // this doesn't feel right
            ((HEAPBUF_HEADER*)pBufHeader->Next)->Prev = pBufHeader->Prev;
            ((HEAPBUF_HEADER*)pBufHeader->Prev)->Next = pBufHeader->Next;
        }
    }
    
    pBufHeader->Flag = 0;
    
    OS_EnableIrqMask(x);
    return 0;
}

void* GetHeapBufNextAdrs(void* pBuf) { // BufMan.c:486
    return (void*)((HEAPBUF_HEADER*)pBuf)->Next;
}

void InitHeapBufMan(HEAPBUF_MAN* pBufMan, u16 Flag) { // BufMan.c:585
    pBufMan->Head = (HEAPBUF_HEADER*)-1;
    pBufMan->Tail = (HEAPBUF_HEADER*)-1;
    pBufMan->Count = 0;
    pBufMan->Flag = Flag;
}

void InitializeHeapBuf(HEAP_INFO* pHeapInfo) { // BufMan.c:611
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r0 - :613
    WORK_PARAM* pWork = &wlMan->Work; // r5 - :614
    
    // TODO: should we be using func.ext or func.os?
    pHeapMan->HeapInfo.heapType = pHeapInfo->heapType;
    pHeapMan->HeapInfo.func.ext.alloc = pHeapInfo->func.ext.alloc;
    pHeapMan->HeapInfo.func.ext.free = pHeapInfo->func.ext.free;
    
    InitHeapBufMan(&pHeapMan->TmpBuf, 2);
    InitHeapBufMan(&pHeapMan->TxPri[0], 3);
    InitHeapBufMan(&pHeapMan->TxPri[1], 4);
    InitHeapBufMan(&pHeapMan->TxPri[2], 5);
    InitHeapBufMan(&pHeapMan->MaDataCfm, 6);
    InitHeapBufMan(&pHeapMan->RxData, 7);
    InitHeapBufMan(&pHeapMan->RxBeacon, 8);
    InitHeapBufMan(&pHeapMan->RxManCtrl, 9);
    InitHeapBufMan(&pHeapMan->Defrag, 10);
    InitHeapBufMan(&pHeapMan->ToWM, 11);
    InitHeapBufMan(&pHeapMan->RequestCmd, 12);
    
    pWork->GameInfoAdrs = (char*)AllocateHeapBuf(&pHeapMan->TmpBuf, 0x81) + sizeof(HEAPBUF_HEADER);
    pWork->GameInfoLength = 0;
    pWork->bUpdateGameInfo = 0;
}

void ReleaseAllWlHeapBuf() { // BufMan.c:661
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r0 - :663
    
    ReleaseAllHeapBuf(&pHeapMan->TxPri[0]);
    ReleaseAllHeapBuf(&pHeapMan->TxPri[1]);
    ReleaseAllHeapBuf(&pHeapMan->TxPri[2]);
    ReleaseAllHeapBuf(&pHeapMan->MaDataCfm);
    ReleaseAllHeapBuf(&pHeapMan->RxData);
    ReleaseAllHeapBuf(&pHeapMan->RxBeacon);
    ReleaseAllHeapBuf(&pHeapMan->RxManCtrl);
    ReleaseAllHeapBuf(&pHeapMan->Defrag);
}

void ReleaseAllHeapBuf(HEAPBUF_MAN* pHeapBufMan) { // BufMan.c:692
    u32* pBuf; // r0 - :695
    u32* pNext; // ** THIS VARIABLE IS DEFINED IN SDK 3.0 NEF **
    
    pBuf = (u32*)pHeapBufMan->Head;
    
    if (pHeapBufMan->Count) {
        while (pBuf != (u32*)-1) {
            // This variable is unknown
            pNext = (u32*)GetHeapBufNextAdrs(pBuf);
            ReleaseHeapBuf(pHeapBufMan, pBuf);
            pBuf = pNext;
            
        }
    }
}