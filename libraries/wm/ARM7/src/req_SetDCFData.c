#include "Mongoose.h"

void WMSP_SetDCFData(void* msg) { // req_SetDCFData.c:28
    u32* buf; // r0 - :30
    u32 wlBuf[128]; // None - :33
    WlTxFrame wlTxFrame; // None - :34
    WlMaDataCfm* wlResult; // r4 - :35
    struct WMCallback* cb; // r0 - :60
}