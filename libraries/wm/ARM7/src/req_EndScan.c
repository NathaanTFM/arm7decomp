#include "Mongoose.h"

void WMSP_EndScan() { // req_EndScan.c:74
    u32 wlBuf[128]; // None - :79
    u16* buf; // r0 - :80
    struct WMCallback* callback; // r0 - :87
    WlDevIdleCfm* p_confirm; // r0 - :95
    if (0) {
        WlParamSetCfm* p_confirm; // r0 - :109
        struct WMCallback* callback; // r0 - :122
    }
}

static void WmspError(u16 wlCommand, u16 wlResult) { // req_EndScan.c:143
    struct WMCallback* callback; // r0 - :145
}
