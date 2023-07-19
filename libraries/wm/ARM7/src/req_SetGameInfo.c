#include "Mongoose.h"

void WMSP_SetGameInfo(void* msg) { // req_SetGameInfo.c:28
    u32* buf; // r0 - :30
    struct WMSPWork* sys; // r0 - :31
    struct WMStatus* status; // r5 - :32
    u8 attribute; // r0 - :33
    u16 length; // r0 - :34
    u32 wlBuf[128]; // None - :36
    WlParamSetCfm* wlResult; // r4 - :37
    struct WMGameInfo GameInfo; // None - :54
    struct WMCallback* cb; // r0 - :63
}