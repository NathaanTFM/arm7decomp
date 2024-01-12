#include "Registers.h"

#define NELEMS(a) (sizeof(a) / sizeof(*(a)))

// state
#define STATE_NONE 0

#define STATE_SCAN 0x10
#define STATE_SCAN_0 (STATE_SCAN + 0)
#define STATE_SCAN_1 (STATE_SCAN + 1)
#define STATE_SCAN_2 (STATE_SCAN + 2)
#define STATE_SCAN_3 (STATE_SCAN + 3)
#define STATE_SCAN_5 (STATE_SCAN + 5)

#define STATE_JOIN 0x20
#define STATE_JOIN_0 (STATE_JOIN + 0)
#define STATE_JOIN_1 (STATE_JOIN + 1)
#define STATE_JOIN_5 (STATE_JOIN + 5)

#define STATE_AUTH 0x30
#define STATE_AUTH_0 (STATE_AUTH + 0)
#define STATE_AUTH_1 (STATE_AUTH + 1)
#define STATE_AUTH_3 (STATE_AUTH + 3)
#define STATE_AUTH_5 (STATE_AUTH + 5)

#define STATE_DEAUTH 0x40
#define STATE_DEAUTH_1 (STATE_DEAUTH + 1)

#define STATE_ASS 0x50
#define STATE_ASS_0 (STATE_ASS + 0)
#define STATE_ASS_1 (STATE_ASS + 1)
#define STATE_ASS_3 (STATE_ASS + 3)

#define STATE_REASS 0x60
#define STATE_REASS_0 (STATE_REASS + 0)
#define STATE_REASS_1 (STATE_REASS + 1)
#define STATE_REASS_3 (STATE_REASS + 3)

#define STATE_DISASS 0x70
#define STATE_DISASS_1 (STATE_DISASS + 1)

#define STATE_MEASCHAN 0x80
#define STATE_MEASCHAN_0 (STATE_MEASCHAN + 0)
#define STATE_MEASCHAN_1 (STATE_MEASCHAN + 1)
#define STATE_MEASCHAN_2 (STATE_MEASCHAN + 2)
#define STATE_MEASCHAN_3 (STATE_MEASCHAN + 3)
#define STATE_MEASCHAN_4 (STATE_MEASCHAN + 4)


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