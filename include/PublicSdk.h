#ifndef PUBLIC_SDK_H
#define PUBLIC_SDK_H

typedef enum OSArena {
    OS_ARENA_MAIN = 0,
    OS_ARENA_MAIN_SUBPRIV = 1,
    OS_ARENA_MAINEX = 2,
    OS_ARENA_ITCM = 3,
    OS_ARENA_DTCM = 4,
    OS_ARENA_SHARED = 5,
    OS_ARENA_WRAM_MAIN = 6,
    OS_ARENA_WRAM_SUB = 7,
    OS_ARENA_WRAM_SUBPRIV = 8,
    OS_ARENA_MAX = 9,
} OSArena;

enum OSThreadState {
    OS_THREAD_STATE_WAITING = 0,
    OS_THREAD_STATE_READY = 1,
    OS_THREAD_STATE_TERMINATED = 2,
};

struct OSContext {
    u32 cpsr; // offset 00
    u32 r[13]; // offset 04
    u32 sp; // offset 38
    u32 lr; // offset 3c
    u32 pc_plus4; // offset 40
    u32 sp_svc; // offset 44
};

struct _OSThreadQueue {
    struct _OSThread* head; // offset 00
    struct _OSThread* tail; // offset 04
};

typedef struct OSMessageQueue {
    struct _OSThreadQueue queueSend; // offset 00
    struct _OSThreadQueue queueReceive; // offset 08
    void** msgArray; // offset 10
    long msgCount; // offset 14
    long firstIndex; // offset 18
    long usedCount; // offset 1c
} OSMessageQueue;

struct _OSThreadLink {
    struct _OSThread* prev; // offset 00
    struct _OSThread* next; // offset 04
};

struct _OSMutexLink {
    struct OSMutex* next; // offset 00
    struct OSMutex* prev; // offset 04
};

struct OSMutex {
    struct _OSThreadQueue queue; // offset 00
    struct _OSThread* thread; // offset 08
    long count; // offset 0c
    struct _OSMutexLink link; // offset 10
};

struct _OSMutexQueue {
    struct OSMutex* head; // offset 00
    struct OSMutex* tail; // offset 04
};

typedef struct OSiAlarm {
    void (*handler)(void*); // offset 00
    void* arg; // offset 04
    u32 tag; // offset 08
    u64 fire; // offset 0c
    struct OSiAlarm* prev; // offset 14
    struct OSiAlarm* next; // offset 18
    u64 period; // offset 1c
    u64 start; // offset 24
} OSAlarm;

typedef struct _OSThread {
    struct OSContext context; // offset 00
    enum OSThreadState state; // offset 48
    struct _OSThread* next; // offset 4c
    u32 id; // offset 50
    u32 priority; // offset 54
    void* profiler; // offset 58
    struct _OSThreadQueue* queue; // offset 5c
    struct _OSThreadLink link; // offset 60
    struct OSMutex* mutex; // offset 68
    struct _OSMutexQueue mutexQueue; // offset 6c
    u32 stackTop; // offset 74
    u32 stackBottom; // offset 78
    u32 stackWarningOffset; // offset 7c
    struct _OSThreadQueue joinQueue; // offset 80
    void* specific[3]; // offset 88
    struct OSiAlarm* alarmForSleep; // offset 94
    void (*destructor)(void*); // offset 98
    void* userParameter; // offset 9c
    int systemErrno; // offset a0
} OSThread;

// Bunch of functions from an unknown outside API we don't have any access to.
extern void MIi_CpuClear16(u16 data, void* destp, u32 size);
extern void MIi_CpuClear32(u32 data, void* destp, u32 size);
extern void MIi_CpuClearFast(u32 data, void* destp, u32 size);
extern void MIi_CpuCopy16(const void* srcp, void* destp, u32 size);
extern void MIi_CpuCopy32(const void* srcp, void* destp, u32 size);
extern void* OS_AllocFromHeap(OSArena id, int heap, u32 size);
extern void OS_FreeToHeap(OSArena id, int heap, void* ptr);
extern u32 OS_EnableIrqMask(u32 intr);
extern u32 OS_DisableIrqMask(u32 intr);
extern u32 OS_DisableInterrupts();
extern u32 OS_RestoreInterrupts(u32 state);
extern void OS_SetIrqFunction(u32 intrBit, void (*function)());
extern void SPI_Lock(u32 id);
extern void SPI_Unlock(u32 id);
extern int OS_SendMessage(OSMessageQueue* mq, void* msg, s32 flags);
extern int OS_ReceiveMessage(OSMessageQueue* mq, void** msg, s32 flags);
extern int OS_IsAlarmAvailable(); 
extern void OS_CreateAlarm(OSAlarm* alarm); 
extern void OS_SetAlarm(OSAlarm* alarm, u64 tick, void (*handler)(void*), void* arg); 
extern void OS_SetPeriodicAlarm(OSAlarm* alarm, u64 start, u64 period, void (*handler)(void*), void* arg); 
extern void OS_CancelAlarm(OSAlarm* alarm); 
extern u64 OS_GetTick();
extern void OS_CreateThread(OSThread* thread, void (*func)(void*), void* arg, void* stack, u32 stackSize, u32 prio);
extern void OS_ExitThread();
extern void OS_WakeupThreadDirect(OSThread* thread);
extern s32 OS_GetLockID();
extern void NVRAM_ReadStatusRegister(u8* buf);
extern void NVRAM_ReadDataBytes(u32 address, u32 size, u8* buf);
extern void NVRAM_SoftwareReset();
extern void MI_WaitDma(u32 dmaNo);

// Home made inlined functions that probably exist somewhere (I think they were in the older version of the driver)
#define REG_IME (*(volatile u16*)0x4000208)

static inline int OS_RestoreIrq(int enable) {
    u16 prep = REG_IME;
    REG_IME = (u16)enable;
    return (int)prep;
}

static inline int OS_DisableIrq() {
    u16 prep = REG_IME;
    REG_IME = 0;
    return (int)prep;
}

#endif