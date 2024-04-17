#include "Mongoose.h"

void WMSP_StartTestRxMode() { // req_StartTestRxMode.c:34
    struct WMCallback* cb; // r0 - :39
    
    cb = (WMCallback*)WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = 41;
    cb->errcode = 4;
    WMSP_ReturnResult2Wm9(cb);
}