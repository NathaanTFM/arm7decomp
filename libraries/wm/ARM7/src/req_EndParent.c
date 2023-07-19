#include "Mongoose.h"

void WMSP_EndParent() { // req_EndParent.c:131
    u32 wlBuf[128]; // None - :136
    struct WMCallback* cb; // r0 - :137
    WlCmdCfm* pConfirm; // r0 - :138
    struct WMCallback* callback; // r0 - :145
    u16 wMac[3]; // None - :157
    long i; // r8 - :158
    u32 e; // r0 - :159
    long auth_retry; // r9 - :175
}

static void WmspError(u16 wlCommand, u16 wlResult) { // req_EndParent.c:269
    struct WMCallback* cb; // r0 - :271
}