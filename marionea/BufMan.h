#include "Marionea.h"

#ifndef BUF_MAN_H
#define BUF_MAN_H

#ifdef MARIONEA_INTERNAL
typedef struct {
    u32 heapType; // offset 00
    union {
        struct {
            OSArena id; // offset 00
            int heapHandle; // offset 04
        } os; // offset 00
        struct {
            u32 (*alloc)(u32); // offset 00
            u32 (*free)(void*); // offset 04
        } ext; // offset 00
    } func; // offset 04
} HEAP_INFO;

typedef struct {
    u32 Prev; // offset 00
    u32 Next; // offset 04
    u16 Flag; // offset 08
    u16 BufID; // offset 0a
} HEAPBUF_HEADER;

typedef struct {
    HEAPBUF_HEADER* Head; // offset 00
    HEAPBUF_HEADER* Tail; // offset 04
    u16 Count; // offset 08
    u16 Flag; // offset 0a
} HEAPBUF_MAN;

typedef struct {
    HEAP_INFO HeapInfo; // offset 00
    HEAPBUF_MAN TmpBuf; // offset 0c
    HEAPBUF_MAN TxPri[3]; // offset 18
    HEAPBUF_MAN MaDataCfm; // offset 3c
    HEAPBUF_MAN RxData; // offset 48
    HEAPBUF_MAN RxBeacon; // offset 54
    HEAPBUF_MAN RxManCtrl; // offset 60
    HEAPBUF_MAN Defrag; // offset 6c
    HEAPBUF_MAN ToWM; // offset 78
    HEAPBUF_MAN RequestCmd; // offset 84
    HEAPBUF_MAN man_rsv[4]; // offset 90
} HEAP_MAN;

void* AllocateHeapBuf(HEAPBUF_MAN* pBufMan, u32 Length);
u32 ReleaseHeapBuf(HEAPBUF_MAN* pBufMan, void* pBuf);
u32 MoveHeapBuf(HEAPBUF_MAN* pFromMan, HEAPBUF_MAN* pToMan, void* pBuf);
u32 NewHeapBuf(HEAPBUF_MAN* pBufMan, void* pBuf);
u32 AddHeapBuf(HEAPBUF_MAN* pBufMan, void* pBuf);
u32 DeleteHeapBuf(HEAPBUF_MAN* pBufMan, void* pBuf);
void* GetHeapBufNextAdrs(void* pBuf);
void InitHeapBufMan(HEAPBUF_MAN* pBufMan, u16 Flag);
void InitializeHeapBuf(HEAP_INFO* pHeapInfo);
void ReleaseAllWlHeapBuf();
void ReleaseAllHeapBuf(HEAPBUF_MAN* pHeapBufMan);
#endif

#endif