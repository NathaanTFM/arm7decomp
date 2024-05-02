#include "Mongoose.h"

void WMSP_StopTestMode() { // req_StopTestMode.c:36
    WMCallback* cb; // r0 - :40
    
    cb = (WMCallback*)WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_STOP_TESTMODE;
    cb->errcode = WM_ERRCODE_WM_DISABLE;
    WMSP_ReturnResult2Wm9(cb);
}