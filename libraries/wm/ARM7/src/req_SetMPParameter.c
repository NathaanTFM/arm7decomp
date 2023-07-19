#include "Mongoose.h"

void WMSP_SetMPParameter(void* msg) { // req_SetMPParameter.c:28
    struct WMMPParam* param; // r0 - :33
    struct WMMPParam old_param; // None - :34
    enum WMErrCode result; // r0 - :36
    struct WMSetMPParameterCallback* cb; // r0 - :45
}

enum WMErrCode WMSP_SetMPParameterCore(struct WMMPParam* param, struct WMMPParam* old_param) { // req_SetMPParameter.c:65
    u32 enabled; // r4 - :68
    enum WMErrCode result; // r5 - :70
    u32 mask; // r6 - :71
    u16 freq; // r1 - :117
    if (0) {
        u16 freq; // r1 - :131
    }
    if (0) {
        u16 freq; // r1 - :149
    }
}