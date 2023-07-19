#include "Mongoose.h"

void WMSP_SetEntry(void* msg) { // req_SetEntry.c:28
    u32* buf; // r0 - :30
    struct WMStatus* status; // r5 - :31
    WlParamSetCfm* wlResult; // r0 - :32
    u32 wlBuf[128]; // None - :33
    struct WMGameInfo GameInfo; // None - :40
    u16 length; // r0 - :41
    struct WMCallback* cb; // r0 - :50
}