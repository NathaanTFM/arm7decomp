#include "Mongoose.h"

void WMSP_StartMP(void* msg) { // req_StartMP.c:30
    u32 wlBuf[128]; // None - :32
    struct WMSPWork* sys; // r0 - :34
    enum WMErrCode err; // r7 - :36
    struct WMStartMPReq* req; // r0 - :38
    struct WMMpRecvBuf* recvBuf; // None - :39
    u32 recvBufSize; // r5 - :40
    u32* sendBuf; // None - :41
    u32 sendBufSize; // r6 - :42
    struct WMMPParam* mpParam; // r0 - :43
    struct WMMPTmpParam* mpTmpParam; // r0 - :44
    struct WMStartMPCallback* cb; // r0 - :82
    int fCleanQueue; // r7 - :91
    u32 e; // None - :92
    u64 now; // None - :158
    int i; // r7 - :159
    if (0) {
        struct WMStartMPCallback* cb; // r0 - :189
        WlCmdCfm* p_confirm; // r0 - :211
    }
    if (0) {
        struct WMCallback* cb; // r0 - :223
    }
    if (0) {
        struct WMStartMPCallback* cb; // r0 - :239
    }
}