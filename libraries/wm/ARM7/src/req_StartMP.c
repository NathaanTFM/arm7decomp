#include "Mongoose.h"

void WMSP_StartMP(void* msg) { // req_StartMP.c:30
    u32 wlBuf[128]; // None - :32
    WMSPWork* sys = &wmspW; // r0 - :34
    WMStatus* status = wmspW.status; // :35
    WMErrCode err; // r7 - :36
    WMStartMPReq* req; // r0 - :38
    WMMpRecvBuf* recvBuf; // None - :39
    u32 recvBufSize; // r5 - :40
    u32* sendBuf; // None - :41
    u32 sendBufSize; // r6 - :42
    WMMPParam* mpParam; // r0 - :43
    WMMPTmpParam* mpTmpParam; // r0 - :44
    WMStartMPCallback* cb; // r0 - :82
    int fCleanQueue; // r7 - :91
    u32 e; // None - :92
    u64 now; // None - :158
    int i; // r7 - :159
    if (0) {
        WMStartMPCallback* cb; // r0 - :189
        WlCmdCfm* p_confirm; // r0 - :211
    }
    if (0) {
        WMCallback* cb; // r0 - :223
    }
    if (0) {
        WMStartMPCallback* cb; // r0 - :239
    }
}