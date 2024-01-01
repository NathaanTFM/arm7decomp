#include "Mongoose.h"

void WMSP_Disable() { // req_Disable.c:38
    struct WMCallback* cb; // r0 - :42
    struct WMStatus* status = wmspW.status; // :43, not in nef
    
    if (status->state != 1) {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 4;
        cb->errcode = 3;
        
    } else {
        PM_SetLEDPattern(PM_LED_PATTERN_ON);
        status->state = 0;
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 4;
        cb->errcode = 0;
    }
    
    WMSP_ReturnResult2Wm9(cb);
}