#include "Mongoose.h"

void WMSP_SetWEPKey(void* msg) { // req_SetWEPKey.c:34
    u32* buf; // r0 - :36
    struct WMCallback* cb; // r0 - :70
}

void WMSP_SetWEPKeyEx(void* msg) { // req_SetWEPKey.c:90
    u32* msgbuf; // r0 - :92
    WlCmdCfm* pConfirm; // r0 - :127
    u32 wlBuf[128]; // None - :128
    struct WMCallback* cb; // r0 - :139
}