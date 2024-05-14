#include "Mongoose.h"

void WMSP_Disable()
{                                    // req_Disable.c:38
    WMCallback *cb;                  // r0 - :42
    WMStatus *status = wmspW.status; // :43, not in nef
    WMSPWork *sys = &wmspW;          // :46

    if (status->state != WM_STATE_STOP)
    {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_DISABLE;
        cb->errcode = WM_ERRCODE_ILLEGAL_STATE;
    }
    else
    {
        PM_SetLEDPattern(PM_LED_PATTERN_ON);
        status->state = WM_STATE_READY;
#ifdef TWL_MODE
        sys->wmInitialized = 0;
#endif
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_DISABLE;
        cb->errcode = WM_ERRCODE_SUCCESS;
    }

    WMSP_ReturnResult2Wm9(cb);
}