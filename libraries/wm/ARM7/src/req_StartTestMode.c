#include "Mongoose.h"

void WMSP_StartTestMode()
{                                // req_StartTestMode.c:40
    WMStartTestModeCallback *cb; // r0 - :45

    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_START_TESTMODE;
    cb->errcode = WM_ERRCODE_WM_DISABLE;
    WMSP_ReturnResult2Wm9(cb);
}