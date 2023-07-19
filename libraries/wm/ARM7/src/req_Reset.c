#include "Mongoose.h"

void WMSP_Reset() { // req_Reset.c:36
    u32 wlBuf[128]; // None - :41
    struct WMCallback* cb; // r0 - :43
    WlCmdCfm* pConfirm; // r0 - :44
    u16 station; // r4 - :45
    u16 mode; // r1 - :46
    u16 child_bitmap; // r0 - :47
    int fparent; // r0 - :48
    int fCleanQueue; // r4 - :52
    u32 e; // r5 - :53
    int i; // r7 - :110
    u16 wMac[3]; // None - :165
    long auth_retry; // r5 - :176
    if (0) {
        u16 wMac[3]; // None - :214
        WlParamSetCfm* p_confirm; // r0 - :261
    }
}

static void WmspError(u16 wlCommand, u16 wlResult) { // req_Reset.c:337
    struct WMCallback* cb; // r0 - :339
}