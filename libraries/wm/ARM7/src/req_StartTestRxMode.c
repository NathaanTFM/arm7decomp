#include "Mongoose.h"

void WMSP_StartTestRxMode()
{                   // req_StartTestRxMode.c:34
    WMCallback *cb; // r0 - :39

    cb = (WMCallback *)WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = WM_APIID_START_TESTRXMODE;
    cb->errcode = WM_ERRCODE_WM_DISABLE;
    WMSP_ReturnResult2Wm9(cb);
}