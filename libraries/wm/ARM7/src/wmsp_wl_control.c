#include "Mongoose.h"

#define CLEAR_WL_RSV(req) \
    do { (req)->wlRsv[0] = 0; \
         (req)->wlRsv[1] = 0; \
         (req)->wlRsv[2] = 0; \
         (req)->wlRsv[3] = 0; \
         (req)->wlRsv[4] = 0; \
         (req)->wlRsv[5] = 0; \
    } while (0)

WlMlmeResetCfm* WMSP_WL_MlmeReset(u16* buf, u16 mib) { // wmsp_wl_control.c:48
    WlMlmeResetReq* req; // r0 - :50
    WlMlmeResetCfm* cfm; // r0 - :51
    
    req = (WlMlmeResetReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0;
    req->header.length = 1;
    req->mib = mib;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlMlmePowerMgtCfm* WMSP_WL_MlmePowerManagement(u16* buf, u16 pwrMgtMode, u16 wakeUp, u16 recieveDtims) { // wmsp_wl_control.c:85
    WlMlmePowerMgtReq* req; // r0 - :88
    WlMlmePowerMgtCfm* cfm; // r0 - :89
    
    req = (WlMlmePowerMgtReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 1;
    req->header.length = 3;
    req->pwrMgtMode = pwrMgtMode;
    req->wakeUp = wakeUp;
    req->recieveDtims = recieveDtims;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlMlmeScanCfm* WMSP_WL_MlmeScan(u16* buf, u32 bufSize, u16* bssid, u16 ssidLength, u8* ssid, u16 scanType, u8* channelList, u16 maxChannelTime) { // wmsp_wl_control.c:131
    WlMlmeScanReq* req; // r0 - :134
    WlMlmeScanCfm* cfm; // r0 - :135
    
    req = (WlMlmeScanReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 2;
    req->header.length = 31;
    MIi_CpuCopy16(bssid, req->bssid, sizeof(req->bssid));
    req->ssidLength = ssidLength;
    MIi_CpuCopy16(ssid, req->ssid, sizeof(req->ssid));
    req->scanType = scanType;
    MIi_CpuCopy16(channelList, req->channelList, sizeof(req->channelList));
    req->bssidMaskCount = 0;
    req->maxChannelTime = maxChannelTime;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = bufSize / 2 - 44;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlMlmeJoinCfm* WMSP_WL_MlmeJoin(u16* buf, u16 timeOut, WlBssDesc* bssDesc) { // wmsp_wl_control.c:181
    WlMlmeJoinReq* req; // r0 - :183
    WlMlmeJoinCfm* cfm; // r0 - :184
    
    req = (WlMlmeJoinReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 3;
    req->header.length = 34;
    req->timeOut = timeOut;
    req->rsv = 0;
    MIi_CpuCopy16(bssDesc, &req->bssDesc, sizeof(req->bssDesc));
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 5;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlMlmeAuthCfm* WMSP_WL_MlmeAuthenticate(u16* buf, u16* peerMacAdrs, u16 algorithm, u16 timeOut) { // wmsp_wl_control.c:223
    WlMlmeAuthReq* req; // r0 - :226
    WlMlmeAuthCfm* cfm; // r0 - :227
}

WlMlmeDeAuthenticateCfm* WMSP_WL_MlmeDeAuthenticate(u16* buf, u16* peerMacAdrs, u16 reasonCode) { // wmsp_wl_control.c:265
    WlMlmeDeAuthenticateReq* req; // r0 - :267
    WlMlmeDeAuthenticateCfm* cfm; // r0 - :268
}

WlMlmeAssociateCfm* WMSP_WL_MlmeAssociate(u16* buf, u16* peerMacAdrs, u16 listenInterval, u16 timeOut) { // wmsp_wl_control.c:305
    WlMlmeAssociateReq* req; // r0 - :308
    WlMlmeAssociateCfm* cfm; // r0 - :309
}

WlMlmeStartCfm* WMSP_WL_MlmeStart(u16* buf, u16 ssidLength, u8* ssid, u16 beaconPeriod, u16 dtimPeriod, u16 channel, u16 basicRateSet, u16 supportRateSet, u16 gameInfoLength, struct WMGameInfo* gameInfo) { // wmsp_wl_control.c:430
    WlMlmeStartReq* req; // r0 - :434
    WlMlmeStartCfm* cfm; // r0 - :435
}

WlMlmeMeasureChannelCfm* WMSP_WL_MlmeMeasureChannel(u16* buf, u16 ccaMode, u16 edThreshold, u16 measureTime, u8* channelList) { // wmsp_wl_control.c:481
    WlMlmeMeasureChannelReq* req; // r0 - :484
    WlMlmeMeasureChannelCfm* cfm; // r0 - :485
}

WlMaDataCfm* WMSP_WL_MaData(u16* buf, WlTxFrame* frame) { // wmsp_wl_control.c:526
    WlMaDataReq* req; // r0 - :528
    WlMaDataCfm* cfm; // r0 - :529
    struct WMDcfRecvBuf* pFrame; // r0 - :541
}

WlMaKeyDataCfm* WMSP_WL_MaKeyData(u16* buf, u16 length, u16 wmHeader, u16* keyDatap) { // wmsp_wl_control.c:580
    WlMaKeyDataReq* req; // r0 - :582
    WlMaKeyDataCfm* cfm; // r0 - :583
}

WlMaMpCfm* WMSP_WL_MaMp(u16* buf, u16 resume, u16 retryLimit, u16 txop, u16 pollBitmap, u16 tmptt, u16 currTsf, u16 dataLength, u16 wmHeader, u16* datap) { // wmsp_wl_control.c:625
    WlMaMpReq* req; // r0 - :628
    WlMaMpCfm* cfm; // r0 - :629
}

WlMaClrDataCfm* WMSP_WL_MaClearData(u16* buf, u16 flag) { // wmsp_wl_control.c:712
    WlMaClrDataReq* req; // r0 - :714
    WlMaClrDataCfm* cfm; // r0 - :715
}

WlParamSetCfm* WMSP_WL_ParamSetAll(WlParamSetAllReq* req) { // wmsp_wl_control.c:750
    WlParamSetCfm* cfm; // r0 - :752
}

WlParamSetCfm* WMSP_WL_ParamSetWepKeyId(u16* buf, u16 wepKeyId) { // wmsp_wl_control.c:990
    WlParamSetWepKeyIdReq* req; // r0 - :992
    WlParamSetCfm* cfm; // r0 - :993
}

WlParamSetCfm* WMSP_WL_ParamSetBeaconLostThreshold(u16* buf, u16 beaconLostTh) { // wmsp_wl_control.c:1130
    WlParamSetBeaconLostThReq* req; // r0 - :1132
    WlParamSetCfm* cfm; // r0 - :1133
}

WlParamSetCfm* WMSP_WL_ParamSetSsidMask(u16* buf, u8* mask) { // wmsp_wl_control.c:1200
    WlParamSetSsidMaskReq* req; // r0 - :1202
    WlParamSetCfm* cfm; // r0 - :1203
}

WlParamSetCfm* WMSP_WL_ParamSetPreambleType(u16* buf, u16 type) { // wmsp_wl_control.c:1235
    WlParamSetPreambleTypeReq* req; // r0 - :1237
    WlParamSetCfm* cfm; // r0 - :1238
}

WlParamSetCfm* WMSP_WL_ParamSetLifeTime(u16* buf, u16 tableNumber, u16 camLifeTime, u16 frameLifeTime) { // wmsp_wl_control.c:1348
    WlParamSetLifeTimeReq* req; // r0 - :1351
    WlParamSetCfm* cfm; // r0 - :1352
}

WlParamSetCfm* WMSP_WL_ParamSetMaxConnectableChild(u16* buf, u16 count) { // wmsp_wl_control.c:1386
    WlParamSetMaxConnectableChildReq* req; // r0 - :1388
    WlParamSetCfm* cfm; // r0 - :1389
}

WlParamSetCfm* WMSP_WL_ParamSetBeaconSendRecvInd(u16* buf, u16 enableMessage) { // wmsp_wl_control.c:1493
    WlParamSetBeaconSendRecvIndReq* req; // r0 - :1495
    WlParamSetCfm* cfm; // r0 - :1496
}

WlParamSetCfm* WMSP_WL_ParamSetNullKeyResponseMode(u16* buf, u16 mode) { // wmsp_wl_control.c:1528
    WlParamSetNullKeyModeReq* req; // r0 - :1530
    WlParamSetCfm* cfm; // r0 - :1531
}

WlParamSetCfm* WMSP_WL_ParamSetBeaconPeriod(u16* buf, u16 beaconPeriod) { // wmsp_wl_control.c:1635
    WlParamSetBeaconPeriodReq* req; // r0 - :1637
    WlParamSetCfm* cfm; // r0 - :1638
}

WlParamSetCfm* WMSP_WL_ParamSetGameInfo(u16* buf, u16 gameInfoLength, u16* gameInfo) { // wmsp_wl_control.c:1741
    WlParamSetGameInfoReq* req; // r0 - :1743
    WlParamSetCfm* cfm; // r0 - :1744
}

WlParamGetMacAddressCfm* WMSP_WL_ParamGetMacAddress(u16* buf) { // wmsp_wl_control.c:1811
}

WlParamGetEnableChannelCfm* WMSP_WL_ParamGetEnableChannel(u16* buf) { // wmsp_wl_control.c:1844
}

WlParamGetModeCfm* WMSP_WL_ParamGetMode(u16* buf) { // wmsp_wl_control.c:1860
}

WlDevShutdownCfm* WMSP_WL_DevShutdown(u16* buf) { // wmsp_wl_control.c:2241
}

WlDevIdleCfm* WMSP_WL_DevIdle(u16* buf) { // wmsp_wl_control.c:2256
}

WlDevClass1Cfm* WMSP_WL_DevClass1(u16* buf) { // wmsp_wl_control.c:2271
}

WlDevRestartCfm* WMSP_WL_DevRestart(u16* buf) { // wmsp_wl_control.c:2286
}

WlDevClrInfoCfm* WMSP_WL_DevSetInitializeWirelessCounter(u16* buf) { // wmsp_wl_control.c:2301
}

WlDevGetVersionCfm* WMSP_WL_DevGetVersion(u16* buf) { // wmsp_wl_control.c:2321
}

WlDevGetInfoCfm* WMSP_WL_DevGetWirelessCounter(u16* buf) { // wmsp_wl_control.c:2338
}

WlDevGetStationStateCfm* WMSP_WL_DevGetStationState(u16* buf) { // wmsp_wl_control.c:2354
}

static WlCmdCfm* WMSPi_WL_NoArg(u16* buf, u16 code, u16 cfmLength) { // wmsp_wl_control.c:2446
    WlCmdReq* req; // r0 - :2448
    WlCmdCfm* cfm; // r0 - :2449
}