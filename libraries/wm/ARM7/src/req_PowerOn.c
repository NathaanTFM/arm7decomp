#include "Mongoose.h"

void WMSP_PowerOn() { // req_PowerOn.c:47
    struct WMCallback* cb; // r0 - :51
    u16 wlcom; // None - :74
    u16 wlres; // None - :75
}

int WMSPi_CommonWlIdle(u16* pWlCommand, u16* pWlResult) { // req_PowerOn.c:120
    WlCmdCfm* pConfirm; // r0 - :123
    u32 wlBuf[128]; // None - :124
    u16 enableChannel; // r0 - :125
    WlParamGetEnableChannelCfm* pECConfirm; // r0 - :153
    WlDevGetVersionCfm* pVConfirm; // r0 - :203
    WlParamGetMacAddressCfm* pMConfirm; // r0 - :222
}