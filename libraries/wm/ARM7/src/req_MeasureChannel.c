#include "Mongoose.h"

void WMSP_MeasureChannel(void* msg) { // req_MeasureChannel.c:36
    u32 wlBuf[128]; // None - :43
    u16* buf; // r11 - :44
    u16 ccaMode; // r6 - :45
    u16 edThreshold; // r7 - :46
    u16 measureTime; // r8 - :47
    u16 channel; // r9 - :48
    u8 channelList[16]; // None - :49
    u16 current_state; // r0 - :50
    WlMlmeMeasureChannelCfm* pConfirm; // r0 - :51
    struct WMMeasureChannelCallback* callback; // r0 - :56
    WlDevGetStationStateCfm* p_confirm; // r0 - :66
    if (0) {
        WlDevClass1Cfm* p_confirm; // r0 - :88
    }
    if (0) {
        WlMlmePowerMgtCfm* p_confirm; // r0 - :100
        struct WMMeasureChannelReq* args; // r0 - :124
        u16 cb_channel; // r5 - :148
        u16 cb_ccaBusyRatio; // r6 - :149
        struct WMMeasureChannelCallback* cb; // r0 - :150
    }
    if (0) {
        WlDevIdleCfm* p_confirm; // r0 - :157
    }
}

STATIC void WmspError(u16 wlCommand, u16 wlResult) { // req_MeasureChannel.c:188
    struct WMCallback* cb; // r0 - :190
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = 30;
    cb->errcode = 1;
    cb->wlCmdID = wlCommand;
    cb->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(cb);
}