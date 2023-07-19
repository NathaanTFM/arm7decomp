#include "Mongoose.h"

static void (*WmspRequestFuncTable[46])(void*); // :30

void WMSP_RequestThread() { // wmsp_request.c:89
    struct WMSPWork* p; // r0 - :93
    struct WMStatus* status; // r9 - :94
    void* msg; // None - :95
    u16 apiid; // r10 - :96
}

static void WmspRequestFuncDummy() { // wmsp_request.c:145
}