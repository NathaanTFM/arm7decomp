#include "Mongoose.h"

void WMSP_AutoDeAuth(void* msg) { // req_AutoDeAuth.c:42
    u32 wlBuf[128]; // None - :44
    u16 wMac[3]; // None - :46
    WlCmdCfm* pConfirm; // r4 - :47
    long auth_retry; // r8 - :60
    struct WMCallback* cb; // r0 - :86
}