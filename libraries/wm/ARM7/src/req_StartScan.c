#include "Mongoose.h"

STATIC void WmspError(u16 wlCommand, u16 wlResult, int exFlag);

void WMSP_StartScan(void *msg)
{                                  // req_StartScan.c:44
    u32 wlBuf[128];                // None - :47
    u16 scanChannel;               // r8 - :54
    u16 scanBssid[3];              // None - :55
    u16 scanMaxChannelTime;        // r9 - :56
    WMStartScanReq *args;          // r0 - :59
    WMStartScanCallback *callback; // r0 - :65
    if (0)
    {
        WMStartScanCallback *callback;      // r0 - :96
        WMStartScanCallback *cb;            // r0 - :107
        WlDevGetStationStateCfm *p_confirm; // r0 - :120
    }
    if (0)
    {
        WlDevClass1Cfm *p_confirm; // r0 - :139
    }
    if (0)
    {
        WlMlmePowerMgtCfm *p_confirm; // r0 - :151
    }
    if (0)
    {
        WlMlmeScanCfm *p_confirm;      // r0 - :178
        u16 ssid[16];                  // None - :183
        u8 channelList[16];            // None - :185
        WMStartScanCallback *callback; // r9 - :237
        u8 tempRssi;                   // r0 - :261
    }
}

void WMSP_StartScanEx(void *msg)
{                                    // req_StartScan.c:307
    u32 wlBuf[278];                  // None - :310
    u16 scanChannelList;             // r0 - :318
    u16 scanBssid[3];                // None - :319
    u16 scanMaxChannelTime;          // None - :320
    u16 scanType;                    // r5 - :321
    u16 ssidMatchLength;             // r11 - :323
    int ssidMaskCustomize;           // r6 - :324
    u16 scanSsidLength;              // r4 - :326
    u8 scanSsid[32];                 // None - :327
    u16 scanBufSize;                 // r8 - :328
    WMStartScanExReq *args;          // r0 - :331
    WMStartScanExCallback *callback; // r0 - :337
    if (0)
    {
        WMStartScanExCallback *callback;    // r0 - :397
        WlDevGetStationStateCfm *p_confirm; // r0 - :410
    }
    if (0)
    {
        WlDevClass1Cfm *p_confirm; // r0 - :429
    }
    if (0)
    {
        WlMlmePowerMgtCfm *p_confirm; // r0 - :441
    }
    if (0)
    {
        WlParamSetCfm *p_confirm; // r0 - :469
    }
    if (0)
    {
        WlParamSetCfm *p_confirm; // r0 - :485
        u8 mask[32];              // None - :501
    }
    if (0)
    {
        WlParamSetCfm *p_confirm; // r0 - :502
    }
    if (0)
    {
        WlMlmeScanCfm *p_confirm;        // r5 - :523
        u8 channelList[16];              // None - :527
        u16 j;                           // r9 - :528
        u16 i;                           // r1 - :528
        WMStartScanExCallback *callback; // r6 - :561
    }
    if (0)
    {
        int i;           // r7 - :575
        WMBssDesc *dest; // r9 - :576
        WMBssDesc *src;  // r8 - :576
        u16 length_byte; // r11 - :586
        u8 tempRssi;     // r0 - :608
    }
}

STATIC void WmspError(u16 wlCommand, u16 wlResult, int exFlag)
{                                  // req_StartScan.c:713
    WMStartScanCallback *callback; // r0 - :715
    callback = WMSP_GetBuffer4Callback2Wm9();
    if (exFlag)
    {
        callback->apiid = WM_APIID_START_SCAN_EX;
    }
    else
    {
        callback->apiid = WM_APIID_START_SCAN;
    }
    callback->errcode = WM_ERRCODE_FAILED;
    callback->state = WM_STATECODE_PARENT_NOT_FOUND;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}