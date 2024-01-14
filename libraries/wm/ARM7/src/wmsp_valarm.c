#include "Mongoose.h"

static struct OSiVAlarm wmspVAlarm; // :22

static void WmspChildAdjustVSync1(void *unused);
static void WmspChildAdjustVSync2(void *unused);
static void WmspChildVAlarmMP(void *unused);
static void WmspParentAdjustVSync(void *unused);
static void WmspParentVAlarmMP(void *unused);
static void WmspFromVAlarmToWmspThread(void *unused);

// The following functions are known to exist, but are missing
// from the latest known version (inlined). They're not placed as they should be,
// but that should not cause any accuracy issue.

static void WmspSetVAlarm(s16 count, void (*handler)(void *), void *arg) {
    OS_SetVAlarm(&wmspVAlarm, count, 263, handler, arg);
}

static void WmspSetVTSF() {
    u16 vcount = REG_VCOUNT;
    u32 cur_tsf = global_vtsf_var; // optimized away
    u16 low0, low1, high;
    u32 v_tsf; // optimized away
    
    low0 = W_US_COUNT0;
    high = W_US_COUNT1;
    low1 = W_US_COUNT0;
    
    if (low0 > low1)
        high = W_US_COUNT1;
    
    v_tsf = (u32)(2 * (low1 | (high << 16)) - 127 * vcount) >> 7;
    global_vtsf_var = v_tsf;
}

static void WmspCalcVRemain() {
    struct WMStatus* status = wmspW.status; // unk
    u16 vcount; // unk
    u32 cur_tsf, next_0line_tsf;
    u16 low0, low1, high;
    int i;
    
    status->v_tsf <<= 6;
    
    low0 = W_US_COUNT0;
    high = W_US_COUNT1;
    low1 = W_US_COUNT0;
    
    if (low0 > low1)
        high = W_US_COUNT1;
    
    next_0line_tsf = (low1 | (high << 16));
    vcount = REG_VCOUNT;
    next_0line_tsf &= 0x3FFFC0;
    next_0line_tsf = ((2 * next_0line_tsf + 127 * (263 - vcount)) / 2) & 0x3FFFC0;
    
    if (status->v_tsf > next_0line_tsf) {
        status->v_remain = 0;
        
    } else {
        for (i = 1; i < 30; i++) {
            status->v_tsf += 16715;
            
            if (status->v_tsf > next_0line_tsf) {
                status->v_remain = status->v_tsf - next_0line_tsf;
                if (status->v_remain > 16398)
                    status->v_remain = 0;
                
                return;
            }
        }
        
        status->v_remain = 0;
    }
}

static void WmspExpendVRemain() {
    long i;
}

// Done with the missing functions

void WMSP_InitVAlarm() { // wmsp_valarm.c:72
    OS_CreateVAlarm(&wmspVAlarm);
}

void WMSP_CancelVAlarm() { // wmsp_valarm.c:92
    OS_CancelVAlarm(&wmspVAlarm);
}

void WMSP_SetVAlarm() { // wmsp_valarm.c:113
    struct WMStatus* status = wmspW.status;
    struct OSiVAlarm* pVAlarm = &wmspVAlarm; // r0 - :117
    
    if (status->mode == 1) {
        if (pVAlarm->handler)
            WMSP_CancelVAlarm();
        
        WmspSetVAlarm(203, WmspParentAdjustVSync, (void *)3);
        
    } else if (status->mode == 2) {
        status->VSyncFlag = 0;
        if (pVAlarm->handler)
            WMSP_CancelVAlarm();
        
        WmspSetVAlarm(200, WmspChildAdjustVSync1, (void *)1);
        status->v_remain = 0;
    }
}

static void WmspChildAdjustVSync1(void *unused) { // wmsp_valarm.c:360
    struct WMStatus* status = wmspW.status;
    status->v_tsf = global_vtsf_var;
    
    if (status->v_tsf_bak != status->v_tsf) {
        status->v_tsf_bak = status->v_tsf;
        WmspCalcVRemain();
    }
    
    if (status->v_remain > 0x7F) {
        WmspSetVAlarm(208, WmspChildAdjustVSync2, (void *)2);
        
    } else {
        status->VSyncFlag = 1;
        WmspSetVAlarm(status->mp_childVCount, WmspChildVAlarmMP, (void *)4);
    }
}

static void WmspChildAdjustVSync2(void *unused) { // wmsp_valarm.c:406
}

static void WmspChildVAlarmMP(void *unused) { // wmsp_valarm.c:435
    u64 now; // None - :449
    u64 last; // None - :450
    int result; // r5 - :451
    u32* buf; // r0 - :458
    struct WMIndCallback* cb; // r0 - :483
}

static void WmspParentAdjustVSync(void *unused) { // wmsp_valarm.c:509
    struct WMStatus* status = wmspW.status;
    
    if (status->valarm_counter >= 0x3C) {
        int tmp = REG_VCOUNT; // unknown var
        if (tmp >= 203 && tmp < 210) {
            REG_VCOUNT = tmp;
            status->valarm_counter = 0;
        }
        
    } else {
        status->valarm_counter++;
    }
    
    WmspSetVTSF();
    WmspSetVAlarm(status->mp_parentVCount, WmspParentVAlarmMP, (void *)5);
}

static void WmspParentVAlarmMP(void *unused) { // wmsp_valarm.c:551
}

static void WmspFromVAlarmToWmspThread(void *unused) { // wmsp_valarm.c:578
    int result; // r0 - :580
    u32* buf; // r0 - :581
    struct WMSPWork* wmspW; // r5 - :582
    u32 enabled; // r0 - :584
    struct WMIndCallback* cb; // r0 - :616
}