#include "Mongoose.h"

void WMSP_StopTestMode() { // req_StopTestMode.c:36
    struct WMCallback* cb; // r0 - :40
    
    cb = (WMCallback*)WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = 27;
    cb->errcode = 4;
    WMSP_ReturnResult2Wm9(cb);
}