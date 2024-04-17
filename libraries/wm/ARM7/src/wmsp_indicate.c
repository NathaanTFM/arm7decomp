#include "Mongoose.h"

STATIC void WmspKickMPChild();

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
    struct WMStatus* status = wmspW.status;
    u32* buf; // r0 - :676
    int result; // r0 - :677
    struct WMIndCallback* cb; // r0 - :698
    
    buf = WMSP_GetInternalRequestBuf();
    if (buf) {
        buf[0] = 45;
        buf[1] = status->mp_prevPollBitmap;
        result = OS_SendMessage(&wmspW.requestQ, buf, 0);
        
    } else {
        result = 0;
    }
    
    if (result != 0) {
        status->mp_resumeFlag = 1;
        
    } else {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 128;
        cb->errcode = 8;
        cb->state = 22;
        cb->reason = 45;
        WMSP_ReturnResult2Wm9(cb);
    }
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

STATIC void WmspMPChildIntervalAlarmCallback() { // wmsp_indicate.c:1392
    WmspKickMPChild();
}

STATIC void WmspKickMPChild() { // wmsp_indicate.c:1407
    u32* buf = WMSP_GetInternalRequestBuf(); // r0 - :1409
    struct WMSPWork* sys = &wmspW; // r4 - :1410
    struct WMStatus* status = sys->status;
    int result = 0; // r0 - :1412
    
    status->mp_vsyncOrderedFlag = 0;
    status->mp_vsyncFlag = 0;
    status->mp_newFrameFlag = 0;
    
    if (buf) {
        buf[0] = 44;
        result = OS_SendMessage(&sys->requestQ, buf, 0);
    }
    
    if (result == 0) {
        if (sys->wm7buf) {
            struct WMIndCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :1437
            cb->apiid = 0x80;
            cb->errcode = 8;
            cb->state = 22;
            cb->reason = 44;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}

void WMSP_InitAlarm() { // wmsp_indicate.c:1951
    OS_CreateAlarm(&wmspMPIntervalAlarm);
    OS_CreateAlarm(&wmspMPAckAlarm);
}