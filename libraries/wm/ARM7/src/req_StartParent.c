#include "Mongoose.h"

void WMSP_StartParent(void* msg) { // req_StartParent.c:51
    struct WMStatus* status; // r7 - :53
    u32 wlBuf[128]; // None - :54
    u16* buf; // r11 - :55
    struct WMStartParentCallback* cb; // r0 - :56
    WlCmdCfm* pConfirm; // r0 - :57
    u16 ssidLength; // r0 - :58
    int powerSave; // r8 - :59
    struct WMStartParentCallback* callback; // r0 - :64
    u32 e; // r0 - :94
    u16 pwrMgtMode; // r8 - :117
    u16 ssid[16]; // None - :136
    struct WMParentParam* pparam; // r9 - :139
    struct WMGameInfo GameInfo; // None - :140
}

static void WmspError(u16 wlCommand, u16 wlResult) { // req_StartParent.c:227
    struct WMStartParentCallback* cb; // r0 - :229
}