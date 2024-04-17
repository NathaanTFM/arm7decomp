#include "Mongoose.h"

static void WmspKickMPChild();

static struct OSiAlarm wmspMPIntervalAlarm; // :23
static struct OSiAlarm wmspMPAckAlarm; // :24

void WMSP_IndicateThread() { // wmsp_indicate.c:99
    void* msg; // None - :104
    u16 cmd_id; // r1 - :106
}

static void WmspFreeBufOfWL(WlCmdReq* buf) { // wmsp_indicate.c:227
    u32 enabled; // r0 - :229
    
    enabled = OS_DisableInterrupts();
    OS_FreeToHeap(wmspW.arenaId, wmspW.heapHandle, buf);
    OS_RestoreInterrupts(enabled);
}

void WMSP_RequestResumeMP() { // wmsp_indicate.c:673
    u32* buf; // r0 - :676
    int result; // r0 - :677
    struct WMIndCallback* cb; // r0 - :698
}

static void WmspMPParentIntervalAlarmCallback(void* arg) { // wmsp_indicate.c:724
}

static void WmspKickMPParent(u16 pollbmp) { // wmsp_indicate.c:739
    u32* buf; // r0 - :741
    struct WMSPWork* sys; // r4 - :742
    int result; // r0 - :743
    struct WMIndCallback* cb; // r0 - :765
}

static void WmspMaMultiPollAckAlarmCallback() { // wmsp_indicate.c:1119
    struct WMSPWork* sys; // r4 - :1122
    WlMaMpAckInd* pInd; // r5 - :1124
    int result; // r0 - :1125
    struct WMIndCallback* cb; // r0 - :1157
}

static void WmspMPChildIntervalAlarmCallback() { // wmsp_indicate.c:1392
    WmspKickMPChild();
}

static void WmspKickMPChild() { // wmsp_indicate.c:1407
    u32* buf; // r0 - :1409
    struct WMSPWork* sys; // r4 - :1410
    int result; // r0 - :1412
    struct WMIndCallback* cb; // r0 - :1437
}

void WMSP_InitAlarm() { // wmsp_indicate.c:1951
    OS_CreateAlarm(&wmspMPIntervalAlarm);
    OS_CreateAlarm(&wmspMPAckAlarm);
}