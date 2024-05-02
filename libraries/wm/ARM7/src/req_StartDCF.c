#include "Mongoose.h"

void WMSP_StartDCF(void* msg) { // req_StartDCF.c:28
    u32* buf = (u32*)msg; // r0 - :30
    WMStatus* status = wmspW.status;
    WMDcfRecvBuf* recvBuf; // r6 - :34
    u16 recvSize; // r7 - :35
    u32 e; // r0 - :36
    WMStartDCFCallback* cb; // r0 - :67
    
    recvBuf = (WMDcfRecvBuf*)buf[1];
    recvSize = buf[2];
    
    e = OS_DisableInterrupts();
    status->dcf_recvBuf[0] = recvBuf;
    status->dcf_recvBufSize = recvSize;
    status->dcf_recvBuf[1] = (WMDcfRecvBuf*)((u32)recvBuf + recvSize);
    status->dcf_recvBufSel = 0;
    status->dcf_sendData = 0;
    status->dcf_sendSize = 0;
    status->dcf_sendFlag = 0;
    status->state = WM_STATE_DCF_CHILD;
    
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_START_DCF;
    cb->errcode = WM_ERRCODE_SUCCESS;
    cb->state = WM_STATECODE_DCF_START;
    WMSP_ReturnResult2Wm9(cb);
    
    status->dcf_flag = 1;
    OS_RestoreInterrupts(e);
}