#include "Mongoose.h"

void WMSP_SetPowerSaveMode(void* msg) { // req_SetPowerSaveMode.c:29
    u32 wlBuf[128]; // None - :32
    u16* buf; // r6 - :33
    struct WMCallback* callback; // r7 - :36
    WlMlmePowerMgtCfm* p_confirm; // r0 - :61
    if (0) {
        WlMaDataCfm* p_confirm; // r0 - :81
        WlTxFrame wlTxFrame; // None - :82
    }
}