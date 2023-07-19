#include "Mongoose.h"

static struct OSiVAlarm wmspVAlarm; // :22

void WMSP_InitVAlarm() { // wmsp_valarm.c:72
}

void WMSP_CancelVAlarm() { // wmsp_valarm.c:92
}

void WMSP_SetVAlarm() { // wmsp_valarm.c:113
    struct OSiVAlarm* pVAlarm; // r0 - :117
}

static void WmspChildAdjustVSync1() { // wmsp_valarm.c:360
}

static void WmspChildAdjustVSync2() { // wmsp_valarm.c:406
}

static void WmspChildVAlarmMP() { // wmsp_valarm.c:435
    u64 now; // None - :449
    u64 last; // None - :450
    int result; // r5 - :451
    u32* buf; // r0 - :458
    struct WMIndCallback* cb; // r0 - :483
}

static void WmspParentAdjustVSync() { // wmsp_valarm.c:509
}

static void WmspParentVAlarmMP() { // wmsp_valarm.c:551
}

static void WmspFromVAlarmToWmspThread() { // wmsp_valarm.c:578
    int result; // r0 - :580
    u32* buf; // r0 - :581
    struct WMSPWork* wmspW; // r5 - :582
    u32 enabled; // r0 - :584
    struct WMIndCallback* cb; // r0 - :616
}