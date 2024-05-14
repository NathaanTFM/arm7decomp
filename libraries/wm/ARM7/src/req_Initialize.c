#include "Mongoose.h"

void WMSP_Initialize(void *msg)
{                             // req_Initialize.c:40
    u32 *reqBuf = (u32 *)msg; // r0 - :42
    WMSPWork *sys = &wmspW;   // :43
    WMArm7Buf *p;             // r4 - :44
    WMCallback *cb;           // r0 - :45

#ifdef TWL_MODE
    if (NWMSP_CheckInitialized())
    {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = 0;
        cb->errcode = 3;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }
#endif

    p = sys->wm7buf = (WMArm7Buf *)reqBuf[1];
    p->status = sys->status = (WMStatus *)reqBuf[2];
    p->fifo7to9 = (u32 *)reqBuf[3];

    WMSPi_CommonInit(reqBuf[4]);

#ifdef TWL_MODE
    if (OS_IsRunOnTwl() == 1)
    {
        WMSPi_SwitchModule(WMSP_MODULE_WM);
    }
#endif

    u16 wlcom; // None - :79
    u16 wlres; // None - :80

    if (!WMSPi_CommonWlIdle(&wlcom, &wlres))
    {
#ifdef TWL_MODE
        if (OS_IsRunOnTwl() == 1)
        {
            WMSPi_SwitchModule(WMSP_MODULE_NWM);
        }
#endif

        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_INITIALIZE;
        cb->errcode = WM_ERRCODE_FAILED;
        cb->wlCmdID = wlcom;
        cb->wlResult = wlres;
        WMSP_ReturnResult2Wm9(cb);
    }
    else
    {
        p->status->state = WM_STATE_IDLE;

        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_INITIALIZE;
        cb->errcode = WM_ERRCODE_SUCCESS;
        WMSP_ReturnResult2Wm9(cb);
    }
}