#include "Mongoose.h"

static void WmspStabilizeBeacon();

void WMSP_PowerOn()
{ // req_PowerOn.c:47
    WMStatus *status = wmspW.status;
    WMCallback *cb; // r0 - :51

    if (status->state != WM_STATE_STOP)
    {
        cb = WMSP_GetBuffer4Callback2Wm9();
        cb->apiid = WM_APIID_POWER_ON;
        cb->errcode = WM_ERRCODE_ILLEGAL_STATE;
        WMSP_ReturnResult2Wm9(cb);
    }
    else
    {
        u16 wlcom; // None - :74
        u16 wlres; // None - :75

        if (!WMSPi_CommonWlIdle(&wlcom, &wlres))
        {
            cb = WMSP_GetBuffer4Callback2Wm9();
            cb->apiid = WM_APIID_POWER_ON;
            cb->errcode = WM_ERRCODE_FAILED;
            cb->wlCmdID = wlcom;
            cb->wlResult = wlres;
            WMSP_ReturnResult2Wm9(cb);
        }
        else
        {
            status->state = WM_STATE_IDLE;

            cb = WMSP_GetBuffer4Callback2Wm9();
            cb->apiid = WM_APIID_POWER_ON;
            cb->errcode = WM_ERRCODE_SUCCESS;
            WMSP_ReturnResult2Wm9(cb);
        }
    }
}

int WMSPi_CommonWlIdle(u16 *pWlCommand, u16 *pWlResult)
{ // req_PowerOn.c:120
    WMStatus *status = wmspW.status;
    WlCmdCfm *pConfirm; // r0 - :123
    u32 wlBuf[128];     // None - :124
    u16 enableChannel;  // r0 - :125

    pConfirm = (WlCmdCfm *)WMSP_WL_DevRestart((u16 *)wlBuf);

    if (pConfirm->resultCode != 0)
    {
        *pWlCommand = 772;
        *pWlResult = pConfirm->resultCode;
        return 0;
    }

    pConfirm = (WlCmdCfm *)WMSP_WL_DevIdle((u16 *)wlBuf);

    if (pConfirm->resultCode != 0)
    {
        *pWlCommand = 770;
        *pWlResult = pConfirm->resultCode;
        return 0;
    }

    WmspStabilizeBeacon();

    WlParamGetEnableChannelCfm *pECConfirm; // r0 - :153
    pECConfirm = WMSP_WL_ParamGetEnableChannel((u16 *)wlBuf);

    if (pECConfirm->resultCode != 0)
    {
        *pWlCommand = 643;
        *pWlResult = pECConfirm->resultCode;
        return 0;
    }

    status->enableChannel = pECConfirm->enableChannel;
    status->allowedChannel = WMSP_GetAllowedChannel(status->enableChannel >> 1);
    WMSP_WL_ParamSetLifeTime((u16 *)wlBuf, 0xFFFF, 40, 5);
    status->mp_lifeTimeTick = 2094625;
    status->rate = 2;
    status->preamble = 1;

    WlDevGetVersionCfm *pVConfirm; // r0 - :203
    pVConfirm = WMSP_WL_DevGetVersion((u16 *)wlBuf);

    if (pVConfirm->resultCode != 0)
    {
        *pWlCommand = 774;
        *pWlResult = pVConfirm->resultCode;
        return 0;
    }

    MIi_CpuCopy16(pVConfirm->wlVersion, status->wlVersion, 8);
    status->macVersion = pVConfirm->macVersion;
    status->bbpVersion[0] = pVConfirm->bbpVersion[0];
    status->bbpVersion[1] = pVConfirm->bbpVersion[1];
    status->rfVersion = pVConfirm->rfVersion;

    WlParamGetMacAddressCfm *pMConfirm; // r0 - :222
    pMConfirm = WMSP_WL_ParamGetMacAddress((u16 *)wlBuf);

    if (pMConfirm->resultCode != 0)
    {
        *pWlCommand = 641;
        *pWlResult = pMConfirm->resultCode;
        return 0;
    }

    MI_CpuCopy8(pMConfirm->staMacAdrs, status->MacAddress, 6);

    pConfirm = (WlCmdCfm *)WMSP_WL_ParamSetBeaconSendRecvInd((u16 *)wlBuf, 1);
    if (pConfirm->resultCode != 0)
    {
        *pWlCommand = 533;
        *pWlResult = pConfirm->resultCode;
        return 0;
    }

    return 1;
}

static void WmspStabilizeBeacon()
{
    W_CONFIG_124h = 200;
    W_CONFIG_128h = 2000;
    W_CONFIG_150h = 0x202;
}