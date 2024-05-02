#include "Mongoose.h"

static OSVAlarm wmspVAlarm; // :22

static void WmspChildAdjustVSync1();
static void WmspChildAdjustVSync2();
static void WmspChildVAlarmMP();
static void WmspParentAdjustVSync();
static void WmspParentVAlarmMP();
STATIC void WmspFromVAlarmToWmspThread();

// The following functions are known to exist, but are missing
// from the latest known version (inlined). They're not placed as they should be,
// but that should not cause any accuracy issue.

static void WmspSetVAlarm(s16 count, void (*handler)(void *), void *arg)
{
    OS_SetVAlarm(&wmspVAlarm, count, 263, handler, arg);
}

static void WmspSetVTSF()
{
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

static void WmspCalcVRemain()
{
    WMStatus *status = wmspW.status; // unk
    u16 vcount;                      // unk
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

    if (status->v_tsf > next_0line_tsf)
    {
        status->v_remain = 0;
    }
    else
    {
        for (i = 1; i < 30; i++)
        {
            status->v_tsf += 16715;

            if (status->v_tsf > next_0line_tsf)
            {
                status->v_remain = status->v_tsf - next_0line_tsf;
                if (status->v_remain > 16398)
                    status->v_remain = 0;

                return;
            }
        }

        status->v_remain = 0;
    }
}

static void WmspExpendVRemain()
{
    WMStatus *status = wmspW.status;
    int vcount = REG_VCOUNT;
    long i;

    if (vcount >= 208 && vcount < 210)
    {
        if (wmspW.status->v_remain >= 127)
        {
            for (i = 1; i < 7; i++)
            {
                if (status->v_remain < 63 * i + 127)
                {
                    break;
                }
            }

            REG_VCOUNT = vcount + (1 - i);
            status->v_remain -= 63 * i;
        }
    }
}

// Done with the missing functions

void WMSP_InitVAlarm()
{ // wmsp_valarm.c:72
    OS_CreateVAlarm(&wmspVAlarm);
}

void WMSP_CancelVAlarm()
{ // wmsp_valarm.c:92
    OS_CancelVAlarm(&wmspVAlarm);
}

void WMSP_SetVAlarm()
{ // wmsp_valarm.c:113
    WMStatus *status = wmspW.status;
    OSVAlarm *pVAlarm = &wmspVAlarm; // r0 - :117

    if (status->mode == 1)
    {
        if (pVAlarm->handler)
            WMSP_CancelVAlarm();

        WmspSetVAlarm(203, (void *)WmspParentAdjustVSync, (void *)3);
    }
    else if (status->mode == 2)
    {
        status->VSyncFlag = 0;
        if (pVAlarm->handler)
            WMSP_CancelVAlarm();

        WmspSetVAlarm(200, (void *)WmspChildAdjustVSync1, (void *)1);
        status->v_remain = 0;
    }
}

static void WmspChildAdjustVSync1()
{ // wmsp_valarm.c:360
    WMStatus *status = wmspW.status;
    status->v_tsf = global_vtsf_var;

    if (status->v_tsf_bak != status->v_tsf)
    {
        status->v_tsf_bak = status->v_tsf;
        WmspCalcVRemain();
    }

    if (status->v_remain > 0x7F)
    {
        WmspSetVAlarm(208, WmspChildAdjustVSync2, (void *)2);
    }
    else
    {
        status->VSyncFlag = 1;
        WmspSetVAlarm(status->mp_childVCount, (void *)WmspChildVAlarmMP, (void *)4);
    }
}

static void WmspChildAdjustVSync2()
{ // wmsp_valarm.c:406
    WMStatus *status = wmspW.status;
    WmspExpendVRemain();

    if (status->v_remain >= 0x7F)
    {
        status->VSyncFlag = 0;
    }

    WmspSetVAlarm(status->mp_childVCount, (void *)WmspChildVAlarmMP, (void *)4);
}

static void WmspChildVAlarmMP()
{                      // wmsp_valarm.c:435
    u64 now;           // None - :449
    u64 last;          // None - :450
    int result;        // r5 - :451
    u32 *buf;          // r0 - :458
    WMIndCallback *cb; // r0 - :483
}

static void WmspParentAdjustVSync()
{ // wmsp_valarm.c:509
    WMStatus *status = wmspW.status;

    if (status->valarm_counter >= 0x3C)
    {
        int tmp = REG_VCOUNT; // unknown var
        if (tmp >= 203 && tmp < 210)
        {
            REG_VCOUNT = tmp;
            status->valarm_counter = 0;
        }
    }
    else
    {
        status->valarm_counter++;
    }

    WmspSetVTSF();
    WmspSetVAlarm(status->mp_parentVCount, (void *)WmspParentVAlarmMP, (void *)5);
}

static void WmspParentVAlarmMP()
{ // wmsp_valarm.c:551
    if (wmspW.status->mp_flag == 1)
    {
        WmspSetVAlarm(203, (void *)WmspParentAdjustVSync, (void *)3);
        WmspFromVAlarmToWmspThread();
    }
}

STATIC void WmspFromVAlarmToWmspThread()
{                              // wmsp_valarm.c:578
    int result;                // r0 - :580
    u32 *buf;                  // r0 - :581
    WMSPWork *pWmspW = &wmspW; // r5 - :582
    WMStatus *status = pWmspW->status;
    u32 enabled; // r0 - :584

    enabled = OS_DisableInterrupts();

    if (status->valarm_queuedFlag == 1)
    {
        OS_RestoreInterrupts(enabled);
        return;
    }

    status->valarm_queuedFlag = 1;
    OS_RestoreInterrupts(enabled);

    buf = WMSP_GetInternalRequestBuf();
    if (buf)
    {
        buf[0] = 28;
        result = OS_SendMessage(&pWmspW->requestQ, buf, 0);
    }
    else
    {
        result = 0;
    }

    if (result == 0)
    {
        status->valarm_queuedFlag = 0;

        if (pWmspW->wm7buf)
        {
            WMIndCallback *cb; // r0 - :616
            cb = WMSP_GetBuffer4Callback2Wm9();
            cb->apiid = WM_APIID_INDICATION;
            cb->errcode = WM_ERRCODE_FIFO_ERROR;
            cb->state = WM_STATECODE_FIFO_ERROR;
            cb->reason = 28;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}