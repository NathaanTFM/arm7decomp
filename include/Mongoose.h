#include "Registers.h"

#define NELEMS(a) (sizeof(a) / sizeof(*(a)))

// priority
#define PRIORITY_LOWEST 3
#define PRIORITY_LOW 2
#define PRIORITY_HIGH 1
#define PRIORITY_HIGHEST 0

// tasks
#define TASK_SCAN   0
#define TASK_JOIN   1
#define TASK_AUTH   2
#define TASK_ASS    3
#define TASK_RE_ASS  4
#define TASK_MEAS_CHANNEL 5
#define TASK_RX_DATA_FRAME    6
#define TASK_RX_MAN_CTRL      7
#define TASK_WL_INTR_TX_BEACON  8
#define TASK_DEFRAG 9
#define TASK_TIMER  10
#define TASK_REQUEST_CMD    11
#define TASK_LOWEST_IDLE 12
#define TASK_BEACON_LOST 13
#define TASK_WL_INTR_TX_END 14
#define TASK_WL_INTR_RX_END 15
#define TASK_WL_INTR_MP_END 16
#define TASK_DEFRAG_TIMER   17
#define TASK_UPDATE_AP_LIST 18
#define TASK_SEND_MESSAGE_TO_WM 19
#define TASK_SET_PARENT_TBTT_TXQ    20
#define TASK_SEND_FATAL_ERR_MSG 21
#define TASK_TERMINATE_WL   22
#define TASK_RELEASE    23

#include "Structures.h"
#include "Prototypes.h"

#define GET_TX_BUF_ADDR(macFrm) (((u32)(macFrm) & 0x3FFF) / 2)

// tick
#define HW_SYSTEM_CLOCK 33514000

#define OS_MicroSecondsToTicks(usec) ((u64)( ((HW_SYSTEM_CLOCK/1000) * (u64)(usec)) / 64 / 1000 ))
#define OS_MicroSecondsToTicks32(usec) ((u64)( ((HW_SYSTEM_CLOCK/1000) * (u32)(usec)) / 64 / 1000 ))

#define OS_MilliSecondsToTicks(msec) ((u64)( ((HW_SYSTEM_CLOCK/1000) * (u64)(msec)) / 64 ))
#define OS_MilliSecondsToTicks32(msec) ((u64)( ((HW_SYSTEM_CLOCK/1000) * (u32)(msec)) / 64 ))

#define OS_SecondsToTicks(sec) ((u64)( (HW_SYSTEM_CLOCK * (u64)(sec)) / 64 ))
#define OS_SecondsToTicks32(sec) ((u64)( (HW_SYSTEM_CLOCK * (u32)(sec)) / 64 ))

#define OS_TicksToMicroSeconds(tick) ( ((u64)(tick) * 64 * 1000) / (HW_SYSTEM_CLOCK/1000) )
#define OS_TicksToMicroSeconds32(tick) ( ((u32)(tick) * 64 * 1000) / (HW_SYSTEM_CLOCK/1000) )

#define OS_TicksToMilliSeconds(tick) ( ((u64)(tick) * 64) / (HW_SYSTEM_CLOCK/1000) )
#define OS_TicksToMilliSeconds32(tick) ( ((u32)(tick) * 64) / (HW_SYSTEM_CLOCK/1000) )

#define OS_TicksToSeconds(tick) ( ((u64)(tick) * 64) / HW_SYSTEM_CLOCK )
#define OS_TicksToSeconds32(tick) ( ((u32)(tick) * 64) / HW_SYSTEM_CLOCK )

// 0x10 is sizeof(WlCmdHeader) + sizeof(HEAPBUF_HEADER)
#define GET_CFM(req) ((void*)((u8*)(req) + ((req)->header.length * 2) + 0x10))
#define GET_HEADER(ptr) (void*)((u8*)(ptr) - (0x10))

// TODO: Decided to make some static functions not static for now so they compile
#ifndef STATIC
#warning STATIC is not static!
#define STATIC
#endif

// this one has no name
#define global_vtsf_var (*(u16*)0x0380FFF0)
#define wlMan (*(WL_MAN**)0x0380FFF4)

#define REG_VCOUNT (*(volatile u16*)0x4000006)
#define REG_IME (*(volatile u16*)0x4000208)
#define REG_POWERCNT (*(volatile u16*)0x4000304)

#define RING_END_PTR ((void*)0x04805F60)

#pragma optimize_for_size off

static inline BOOL OS_RestoreIrq(BOOL enable) {
    u16 prep = REG_IME;
    REG_IME = (u16)enable;
    return (BOOL)prep;
}

static inline BOOL OS_DisableIrq() {
    u16 prep = REG_IME;
    REG_IME = 0;
    return (BOOL)prep;
}

extern struct WMSPWork wmspW;

#pragma optimize_for_size on