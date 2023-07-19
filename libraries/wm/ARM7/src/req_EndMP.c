#include "Mongoose.h"

void WMSP_EndMP() { // req_EndMP.c:94
    u32 wlBuf[128]; // None - :100
    WlCmdCfm* pConfirm; // r0 - :101
    int fCleanQueue; // r5 - :103
    struct WMCallback* callback; // r0 - :108
    u32 e; // r6 - :116
    struct WMCallback* cb; // r0 - :172
}

static void WmspError(u16 wlCommand, u16 wlResult) { // req_EndMP.c:193
    struct WMCallback* cb; // r0 - :195
}