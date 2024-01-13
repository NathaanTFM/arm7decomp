#include "Marionea.h"

#ifndef WL_SYS_H
#define WL_SYS_H

#ifdef MARIONEA_INTERNAL
typedef struct {
    TASK_MAN TaskMan; // offset 00
    HEAP_MAN HeapMan; // offset 17c
    WlApElement ApList[4]; // offset 23c
    struct OSMessageQueue* pSendMsgQueue; // offset 304
    struct OSMessageQueue* pRecvMsgQueue; // offset 308
    u32 DmaChannel; // offset 30c
    u32 DmaMaxCount; // offset 310
    long lockID; // offset 314
    void* pFlashImg; // offset 318
    CONFIG_PARAM Config; // offset 31c
    WORK_PARAM Work; // offset 344
    MLME_MAN MLME; // offset 404
    CMDIF_MAN CmdIf; // offset 424
    TX_CTRL TxCtrl; // offset 42c
    RX_CTRL RxCtrl; // offset 4dc
    CAM_MAN CamMan; // offset 530
    WlCounter Counter; // offset 540
    RAND_CTRL Rand; // offset 5f4
    RF_CONFIG Rf; // offset 5fc
    struct OSiAlarm PeriodicAlarm; // offset 60c
    struct OSiAlarm Alarm; // offset 638
    struct OSiAlarm KeyAlarm; // offset 664
    u32 WlDbgLevel; // offset 690
    u16 WlOperation; // offset 694
    u16 EnableChannel; // offset 696
} WL_MAN;
#endif

typedef struct {
    u32 workingMemAdrs; // offset 00
    void* stack; // offset 04
    u32 stacksize; // offset 08
    u32 priority; // offset 0c
    struct OSMessageQueue* sendMsgQueuep; // offset 10
    struct OSMessageQueue* recvMsgQueuep; // offset 14
    u32 dmaChannel; // offset 18
    u32 dmaMaxSize; // offset 1c
    u32 heapType; // offset 20
    union {
        struct {
            enum OSArena id; // offset 00
            int heapHandle; // offset 04
        } os; // offset 00
        struct {
            u32 (*alloc)(u32); // offset 00
            u32 (*free)(void*); // offset 04
        } ext; // offset 00
    } heapFunc; // offset 24
    void* camAdrs; // offset 2c
    u32 camSize; // offset 30
} WlInit;

#ifdef MARIONEA_INTERNAL
void WlessLibReboot();
#endif

u32 WL_InitDriver(WlInit* pInit);
struct _OSThread* WL_GetThreadStruct();

#endif