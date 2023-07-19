#include "Mongoose.h"

void WMSP_StartConnectEx(void* msg) { // req_StartConnect.c:48
    u32 wlBuf[128]; // None - :51
    struct WMArm7Buf* p; // r8 - :54
    struct WMStartConnectReq* args; // r0 - :58
    WlMlmeAssociateCfm* assConfirm; // r9 - :59
    struct WMStartConnectCallback* callback; // r0 - :64
    struct WMStartConnectCallback* cb; // r0 - :88
    if (0) {
        struct WMStartConnectCallback* cb; // r0 - :107
        struct WMStartConnectCallback* callback; // r0 - :120
        WlCmdCfm* p_confirm; // r0 - :166
        u16 beaconLostTh; // r0 - :184
    }
    if (0) {
        WlParamSetCfm* p_confirm; // r0 - :185
    }
    if (0) {
        WlDevClass1Cfm* p_confirm; // r0 - :206
    }
    if (0) {
        WlMlmePowerMgtCfm* p_confirm; // r0 - :220
        u16 pwrMgtMode; // r11 - :222
    }
    if (0) {
        WlMlmeJoinCfm* p_confirm; // r0 - :240
        WlBssDesc bss_desc; // None - :243
    }
    if (0) {
        WlMlmeAuthCfm* p_confirm; // r0 - :304
        u16 mac[3]; // None - :308
        struct WMStartConnectCallback* callback; // r0 - :324
    }
    if (0) {
        u16 mac[3]; // None - :344
        u32 e; // r11 - :359
        struct WMStartConnectCallback* callback; // r0 - :366
    }if (0) {
        u16 linkLevel; // r0 - :396
        u32 e; // r6 - :405
        struct WMStartConnectCallback* callback; // r0 - :438
    }
}

static void WmspError(u16 wlCommand, u16 wlResult, u16 wlStatus) { // req_StartConnect.c:473
    struct WMStartConnectCallback* callback; // r0 - :475
}