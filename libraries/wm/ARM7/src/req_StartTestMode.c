#include "Mongoose.h"

void WMSP_StartTestMode() { // req_StartTestMode.c:40
    struct WMStartTestModeCallback* cb; // r0 - :45
    
    cb = (WMStartTestModeCallback*)WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = 26;
    cb->errcode = 4;
    WMSP_ReturnResult2Wm9(cb);
}