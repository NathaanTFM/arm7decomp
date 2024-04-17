#include "Mongoose.h"

void WMSP_StopTestRxMode() { // req_StopTestRxMode.c:34
    struct WMStopTestRxModeCallback* cb; // r0 - :38
    
    cb = (WMStopTestRxModeCallback*)WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = 42;
    cb->errcode = 4;
    WMSP_ReturnResult2Wm9(cb);
}