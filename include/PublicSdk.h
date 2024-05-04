#ifndef PUBLIC_SDK_H
#define PUBLIC_SDK_H

#define NELEMS(a) (sizeof(a) / sizeof(*(a)))

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

typedef enum OSArena
{
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

typedef enum OSThreadState
{
    OS_THREAD_STATE_WAITING = 0,
    OS_THREAD_STATE_READY = 1,
    OS_THREAD_STATE_TERMINATED = 2,
} OSThreadState;

typedef struct OSContext
{
    u32 cpsr;     // offset 00
    u32 r[13];    // offset 04
    u32 sp;       // offset 38
    u32 lr;       // offset 3c
    u32 pc_plus4; // offset 40
    u32 sp_svc;   // offset 44
} OSContext;

typedef struct _OSThreadQueue
{
    struct _OSThread *head; // offset 00
    struct _OSThread *tail; // offset 04
} OSThreadQueue;

typedef struct OSMessageQueue
{
    OSThreadQueue queueSend;    // offset 00
    OSThreadQueue queueReceive; // offset 08
    void **msgArray;            // offset 10
    long msgCount;              // offset 14
    long firstIndex;            // offset 18
    long usedCount;             // offset 1c
} OSMessageQueue;

typedef struct _OSThreadLink
{
    struct _OSThread *prev; // offset 00
    struct _OSThread *next; // offset 04
} OSThreadLink;

typedef struct _OSMutexLink
{
    struct OSMutex *next; // offset 00
    struct OSMutex *prev; // offset 04
} OSMutexLink;

typedef struct OSMutex
{
    OSThreadQueue queue;      // offset 00
    struct _OSThread *thread; // offset 08
    long count;               // offset 0c
    OSMutexLink link;         // offset 10
} OSMutex;

typedef struct _OSMutexQueue
{
    OSMutex *head; // offset 00
    OSMutex *tail; // offset 04
} OSMutexQueue;

typedef struct OSiAlarm
{
    void (*handler)(void *); // offset 00
    void *arg;               // offset 04
    u32 tag;                 // offset 08
    u64 fire;                // offset 0c
    struct OSiAlarm *prev;   // offset 14
    struct OSiAlarm *next;   // offset 18
    u64 period;              // offset 1c
    u64 start;               // offset 24
} OSAlarm;

typedef struct _OSThread
{
    OSContext context;          // offset 00
    OSThreadState state;        // offset 48
    struct _OSThread *next;     // offset 4c
    u32 id;                     // offset 50
    u32 priority;               // offset 54
    void *profiler;             // offset 58
    OSThreadQueue *queue;       // offset 5c
    OSThreadLink link;          // offset 60
    OSMutex *mutex;             // offset 68
    OSMutexQueue mutexQueue;    // offset 6c
    u32 stackTop;               // offset 74
    u32 stackBottom;            // offset 78
    u32 stackWarningOffset;     // offset 7c
    OSThreadQueue joinQueue;    // offset 80
    void *specific[3];          // offset 88
    OSAlarm *alarmForSleep;     // offset 94
    void (*destructor)(void *); // offset 98
    void *userParameter;        // offset 9c
    int systemErrno;            // offset a0
} OSThread;

typedef struct OSiVAlarm
{
    void (*handler)(void *); // offset 00
    void *arg;               // offset 04
    u32 tag;                 // offset 08
    u32 frame;               // offset 0c
    s16 fire;                // offset 10
    s16 delay;               // offset 12
    struct OSiVAlarm *prev;  // offset 14
    struct OSiVAlarm *next;  // offset 18
    int period;              // offset 1c
    int finish;              // offset 20
    int canceled;            // offset 24
} OSVAlarm;

typedef enum PMLEDPattern
{
    PM_LED_PATTERN_NONE = 0,
    PM_LED_PATTERN_ON = 1,
    PM_LED_PATTERN_BLINK_LOW = 2,
    PM_LED_PATTERN_BLINK_HIGH = 3,
    PM_LED_PATTERN_BLINK1 = 4,
    PM_LED_PATTERN_BLINK2 = 5,
    PM_LED_PATTERN_BLINK3 = 6,
    PM_LED_PATTERN_BLINK4 = 7,
    PM_LED_PATTERN_BLINK5 = 8,
    PM_LED_PATTERN_BLINK6 = 9,
    PM_LED_PATTERN_BLINK8 = 10,
    PM_LED_PATTERN_BLINK10 = 11,
    PM_LED_PATTERN_PATTERN1 = 12,
    PM_LED_PATTERN_PATTERN2 = 13,
    PM_LED_PATTERN_PATTERN3 = 14,
    PM_LED_PATTERN_WIRELESS = 15
} PMLEDPattern;

typedef enum PMLEDStatus
{
    PM_LED_NONE = 0,
    PM_LED_ON = 1,
    PM_LED_BLINK_LOW = 2,
    PM_LED_BLINK_HIGH = 3
} PMLEDStatus;

typedef enum PXIFifoTag
{
    PXI_FIFO_TAG_EX = 0,
    PXI_FIFO_TAG_USER_0 = 1,
    PXI_FIFO_TAG_USER_1 = 2,
    PXI_FIFO_TAG_SYSTEM = 3,
    PXI_FIFO_TAG_NVRAM = 4,
    PXI_FIFO_TAG_RTC = 5,
    PXI_FIFO_TAG_TOUCHPANEL = 6,
    PXI_FIFO_TAG_SOUND = 7,
    PXI_FIFO_TAG_PM = 8,
    PXI_FIFO_TAG_MIC = 9,
    PXI_FIFO_TAG_WM = 10,
    PXI_FIFO_TAG_FS = 11,
    PXI_FIFO_TAG_OS = 12,
    PXI_FIFO_TAG_CTRDG = 13,
    PXI_FIFO_TAG_CARD = 14,
    PXI_FIFO_TAG_WVR = 15,
    PXI_FIFO_TAG_CTRDG_Ex = 16,
    PXI_FIFO_TAG_CTRDG_PHI = 17,
    PXI_MAX_FIFO_TAG = 32,
} PXIFifoTag;

// Bunch of functions from an unknown outside API we don't have any access to.
extern void MI_CpuFill8(void *dstp, u8 data, u32 size);
extern void MIi_CpuClear16(u16 data, void *destp, u32 size);
extern void MIi_CpuClear32(u32 data, void *destp, u32 size);
extern void MIi_CpuClearFast(u32 data, void *destp, u32 size);
extern void MI_CpuCopy8(const void *srcp, void *dstp, u32 size);
extern void MIi_CpuCopy16(const void *srcp, void *destp, u32 size);
extern void MIi_CpuCopy32(const void *srcp, void *destp, u32 size);
extern void *OS_AllocFromHeap(OSArena id, int heap, u32 size);
extern void OS_FreeToHeap(OSArena id, int heap, void *ptr);
extern u32 OS_EnableIrqMask(u32 intr);
extern u32 OS_DisableIrqMask(u32 intr);
extern u32 OS_DisableInterrupts();
extern u32 OS_RestoreInterrupts(u32 state);
extern void OS_SetIrqFunction(u32 intrBit, void (*function)());
extern void OS_InitMutex(OSMutex *mutex);
extern void OS_LockMutex(OSMutex *mutex);
extern void OS_UnlockMutex(OSMutex *mutex);
extern void OS_TPrintf(const char *fmt, ...);
extern void OSi_TWarning(const char *file, int line, const char *fmt, ...);
extern void OS_Sleep(u32 msec);
extern void SPI_Lock(u32 id);
extern void SPI_Unlock(u32 id);
extern void OS_InitMessageQueue(OSMessageQueue *mq, void **msgArray, s32 msgCount);
extern int OS_SendMessage(OSMessageQueue *mq, void *msg, s32 flags);
extern int OS_ReceiveMessage(OSMessageQueue *mq, void **msg, s32 flags);
extern int OS_IsAlarmAvailable();
extern void OS_CreateAlarm(OSAlarm *alarm);
extern void OS_SetAlarm(OSAlarm *alarm, u64 tick, void (*handler)(void *), void *arg);
extern void OS_SetPeriodicAlarm(OSAlarm *alarm, u64 start, u64 period, void (*handler)(void *), void *arg);
extern void OS_CancelAlarm(OSAlarm *alarm);
extern u64 OS_GetTick();
extern void OS_CreateThread(OSThread *thread, void (*func)(void *), void *arg, void *stack, u32 stackSize, u32 prio);
extern void OS_ExitThread();
extern void OS_WakeupThreadDirect(OSThread *thread);
extern s32 OS_GetLockID();
extern void NVRAM_ReadStatusRegister(u8 *buf);
extern void NVRAM_ReadDataBytes(u32 address, u32 size, u8 *buf);
extern void NVRAM_SoftwareReset();
extern void MI_WaitDma(u32 dmaNo);

extern int OS_SetThreadPriority(OSThread *thread, u32 prio);
extern void OS_Terminate();
extern void OS_InitVAlarm();
extern int OS_IsVAlarmAvailable();
extern u32 OS_DisableScheduler();
extern u32 OS_EnableScheduler();

extern void OS_CreateVAlarm(OSVAlarm *alarm);
extern void OS_SetVAlarm(OSVAlarm *alarm, s16 count, s16 delay, void (*handler)(void *), void *arg);
extern void OS_CancelVAlarm(OSVAlarm *alarm);

extern void PM_SetLEDPattern(PMLEDPattern pattern);
extern void PMi_SetLED(PMLEDStatus status);

extern void PXI_Init();
extern void PXI_SetFifoRecvCallback(int fifotag, void (*callback)(PXIFifoTag, u32, int)); // p
extern int PXI_SendWordByFifo(int fifotag, u32 data, int err);                            // pxi_fifo.c:215

extern u8 MATH_CountPopulation(u32 x);
extern void SVC_WaitByLoop(u32 count);

extern void SND_BeginSleep();

#define REG_VCOUNT (*(volatile u16 *)0x4000006)
#define REG_IME (*(volatile u16 *)0x4000208)
#define REG_POWERCNT (*(volatile u16 *)0x4000304)

#pragma optimize_for_size off

static inline int OS_RestoreIrq(int enable)
{
    u16 prep = REG_IME;
    REG_IME = (u16)enable;
    return (int)prep;
}

static inline int OS_DisableIrq()
{
    u16 prep = REG_IME;
    REG_IME = 0;
    return (int)prep;
}

#pragma optimize_for_size on

#endif