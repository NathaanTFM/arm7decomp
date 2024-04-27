#include "Mongoose.h"

static void WmspError(u16 wlCommand, u16 wlResult);

void WMSP_SetLifeTime(void* msg) { // req_SetLifeTime.c:36
    u32* buf = (u32*)msg; // r0 - :39
    u32 wlBuf[128]; // None - :40
    struct WMStatus* status = wmspW.status;
    WlParamSetCfm* pConfirm; // r0 - :43
    u16 mpLifeTime; // r6 - :45
    
    mpLifeTime = buf[4];
    pConfirm = WMSP_WL_ParamSetLifeTime((u16*)wlBuf, buf[1], buf[2], buf[3]);
    
    if (pConfirm->resultCode != 0) {
        WmspError(529, pConfirm->resultCode);
        return;
    }
    
    if (mpLifeTime != 0xFFFF) {
        status->mp_lifeTimeTick = mpLifeTime == 0 ? 1 : (33514 * (u64)(mpLifeTime * 100)) / 64;
        
    } else {
        status->mp_lifeTimeTick = 0;
    }
    
    u64 now = OS_GetTick() | 1; // None - :95
    int i; // r3 - :96
    for (i = 0; i < 16; i++) {
        status->mp_lastRecvTick[i] = now;
    }
    
    struct WMCallback* cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :105
    cb->apiid = 29;
    cb->errcode = 0;
    WMSP_ReturnResult2Wm9(cb);
}

static void WmspError(u16 wlCommand, u16 wlResult) { // req_EndMP.c:193
    struct WMCallback* cb; // r0 - :195
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = 29;
    cb->errcode = 1;
    cb->wlCmdID = wlCommand;
    cb->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(cb);
}