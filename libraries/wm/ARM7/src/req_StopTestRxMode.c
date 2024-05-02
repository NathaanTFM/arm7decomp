#include "Mongoose.h"

void WMSP_StopTestRxMode()
{                                 // req_StopTestRxMode.c:34
    WMStopTestRxModeCallback *cb; // r0 - :38

    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_STOP_TESTRXMODE;
    cb->errcode = WM_ERRCODE_WM_DISABLE;
    WMSP_ReturnResult2Wm9(cb);
}