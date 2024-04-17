#include "Mongoose.h"

void WMSP_StartDCF(void* msg) { // req_StartDCF.c:28
    u32* buf = (u32*)msg; // r0 - :30
    struct WMStatus* status = wmspW.status;
    struct WMDcfRecvBuf* recvBuf; // r6 - :34
    u16 recvSize; // r7 - :35
    u32 e; // r0 - :36
    struct WMStartDCFCallback* cb; // r0 - :67
    
    recvBuf = (struct WMDcfRecvBuf*)buf[1];
    recvSize = buf[2];
    
    e = OS_DisableInterrupts();
    status->dcf_recvBuf[0] = recvBuf;
    status->dcf_recvBufSize = recvSize;
    status->dcf_recvBuf[1] = (struct WMDcfRecvBuf*)((u32)recvBuf + recvSize);
    status->dcf_recvBufSel = 0;
    status->dcf_sendData = 0;
    status->dcf_sendSize = 0;
    status->dcf_sendFlag = 0;
    status->state = 11;
    
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = 17;
    cb->errcode = 0;
    cb->state = 14;
    WMSP_ReturnResult2Wm9(cb);
    
    status->dcf_flag = 1;
    OS_RestoreInterrupts(e);
}