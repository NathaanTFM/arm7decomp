#include "Mongoose.h"

void WMSP_SetLifeTime(void* msg) { // req_SetLifeTime.c:36
    u32* buf; // r0 - :39
    u32 wlBuf[128]; // None - :40
    WlParamSetCfm* pConfirm; // r0 - :43
    u16 mpLifeTime; // r6 - :45
    u64 now; // None - :95
    int i; // r3 - :96
    struct WMCallback* cb; // r0 - :105
}