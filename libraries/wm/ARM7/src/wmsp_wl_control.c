#include "Mongoose.h"

#define CLEAR_WL_RSV(req) \
    do { (req)->wlRsv[0] = 0; \
         (req)->wlRsv[1] = 0; \
         (req)->wlRsv[2] = 0; \
         (req)->wlRsv[3] = 0; \
         (req)->wlRsv[4] = 0; \
         (req)->wlRsv[5] = 0; \
    } while (0)

static WlCmdCfm* WMSPi_WL_NoArg(u16* buf, u16 code, u16 cfmLength);

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
    
    req = (WlMlmeAuthReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 4;
    req->header.length = 5;
    MIi_CpuCopy16(peerMacAdrs, &req->peerMacAdrs, sizeof(req->peerMacAdrs));
    req->algorithm = algorithm;
    req->timeOut = timeOut;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 6;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlMlmeDeAuthenticateCfm* WMSP_WL_MlmeDeAuthenticate(u16* buf, u16* peerMacAdrs, u16 reasonCode) { // wmsp_wl_control.c:265
    WlMlmeDeAuthenticateReq* req; // r0 - :267
    WlMlmeDeAuthenticateCfm* cfm; // r0 - :268
    
    req = (WlMlmeDeAuthenticateReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 5;
    req->header.length = 4;
    MIi_CpuCopy16(peerMacAdrs, &req->peerMacAdrs, sizeof(req->peerMacAdrs));
    req->reasonCode = reasonCode;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 4;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlMlmeAssociateCfm* WMSP_WL_MlmeAssociate(u16* buf, u16* peerMacAdrs, u16 listenInterval, u16 timeOut) { // wmsp_wl_control.c:305
    WlMlmeAssociateReq* req; // r0 - :308
    WlMlmeAssociateCfm* cfm; // r0 - :309
    
    req = (WlMlmeAssociateReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 6;
    req->header.length = 5;
    MIi_CpuCopy16(peerMacAdrs, &req->peerMacAdrs, sizeof(req->peerMacAdrs));
    req->listenInterval = listenInterval;
    req->timeOut = timeOut;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 3;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlMlmeStartCfm* WMSP_WL_MlmeStart(u16* buf, u16 ssidLength, u8* ssid, u16 beaconPeriod, u16 dtimPeriod, u16 channel, u16 basicRateSet, u16 supportRateSet, u16 gameInfoLength, struct WMGameInfo* gameInfo) { // wmsp_wl_control.c:430
    WlMlmeStartReq* req; // r0 - :434
    WlMlmeStartCfm* cfm; // r0 - :435
    
    req = (WlMlmeStartReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 9;
    req->header.length = (gameInfoLength + 1) / 2 + 0x17;
    req->ssidLength = ssidLength;
    MIi_CpuCopy16(ssid, &req->ssid, sizeof(req->ssid));
    req->beaconPeriod = beaconPeriod;
    req->dtimPeriod = dtimPeriod;
    req->channel = channel;
    req->basicRateSet = basicRateSet;
    req->supportRateSet = supportRateSet;
    req->gameInfoLength = gameInfoLength;
    MIi_CpuCopy16(gameInfo, req->gameInfo, gameInfoLength);
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlMlmeMeasureChannelCfm* WMSP_WL_MlmeMeasureChannel(u16* buf, u16 ccaMode, u16 edThreshold, u16 measureTime, u8* channelList) { // wmsp_wl_control.c:481
    WlMlmeMeasureChannelReq* req; // r0 - :484
    WlMlmeMeasureChannelCfm* cfm; // r0 - :485
    
    req = (WlMlmeMeasureChannelReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 10;
    req->header.length = 12;
    req->rsv = 0;
    req->ccaMode = ccaMode;
    req->edThreshold = edThreshold;
    req->measureTime = measureTime;
    MIi_CpuCopy16(channelList, req->channelList, sizeof(req->channelList));
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 18;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlMaDataCfm* WMSP_WL_MaData(u16* buf, WlTxFrame* frame) { // wmsp_wl_control.c:526
    WlMaDataReq* req; // r0 - :528
    WlMaDataCfm* cfm; // r0 - :529
    struct WMDcfRecvBuf* pFrame; // r0 - :541
}

WlMaKeyDataCfm* WMSP_WL_MaKeyData(u16* buf, u16 length, u16 wmHeader, u16* keyDatap) { // wmsp_wl_control.c:580
    WlMaKeyDataReq* req; // r0 - :582
    WlMaKeyDataCfm* cfm; // r0 - :583
    
    req = (WlMaKeyDataReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x101;
    req->header.length = 4;
    req->length = length;
    req->wmHeader = wmHeader;
    req->keyDatap = keyDatap;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlMaMpCfm* WMSP_WL_MaMp(u16* buf, u16 resume, u16 retryLimit, u16 txop, u16 pollBitmap, u16 tmptt, u16 currTsf, u16 dataLength, u16 wmHeader, u16* datap) { // wmsp_wl_control.c:625
    WlMaMpReq* req; // r0 - :628
    WlMaMpCfm* cfm; // r0 - :629
    
    req = (WlMaMpReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x102;
    req->header.length = 10;
    req->resume = resume;
    req->retryLimit = retryLimit;
    req->txop = txop;
    req->pollBitmap = pollBitmap;
    req->tmptt = tmptt;
    req->currTsf = currTsf;
    req->dataLength = dataLength;
    req->wmHeader = wmHeader;
    req->datap = datap;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlMaClrDataCfm* WMSP_WL_MaClearData(u16* buf, u16 flag) { // wmsp_wl_control.c:712
    WlMaClrDataReq* req; // r0 - :714
    WlMaClrDataCfm* cfm; // r0 - :715
    
    req = (WlMaClrDataReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 260;
    req->header.length = 1;
    req->flag = flag;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlParamSetCfm* WMSP_WL_ParamSetAll(WlParamSetAllReq* req) { // wmsp_wl_control.c:750
    WlParamSetCfm* cfm; // r0 - :752
    
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x200;
    req->header.length = 72;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlParamSetCfm* WMSP_WL_ParamSetWepKeyId(u16* buf, u16 wepKeyId) { // wmsp_wl_control.c:990
    WlParamSetWepKeyIdReq* req; // r0 - :992
    WlParamSetCfm* cfm; // r0 - :993
    
    req = (WlParamSetWepKeyIdReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x207;
    req->header.length = 1;
    req->wepKeyId = wepKeyId;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlParamSetCfm* WMSP_WL_ParamSetBeaconLostThreshold(u16* buf, u16 beaconLostTh) { // wmsp_wl_control.c:1130
    WlParamSetBeaconLostThReq* req; // r0 - :1132
    WlParamSetCfm* cfm; // r0 - :1133
    
    req = (WlParamSetBeaconLostThReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x20B;
    req->header.length = 1;
    req->beaconLostTh = beaconLostTh;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlParamSetCfm* WMSP_WL_ParamSetSsidMask(u16* buf, u8* mask) { // wmsp_wl_control.c:1200
    WlParamSetSsidMaskReq* req; // r0 - :1202
    WlParamSetCfm* cfm; // r0 - :1203
    
    req = (WlParamSetSsidMaskReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x20D;
    req->header.length = 16;
    MIi_CpuCopy16(mask, req->mask, 0x20u);
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlParamSetCfm* WMSP_WL_ParamSetPreambleType(u16* buf, u16 type) { // wmsp_wl_control.c:1235
    WlParamSetPreambleTypeReq* req; // r0 - :1237
    WlParamSetCfm* cfm; // r0 - :1238
    
    req = (WlParamSetPreambleTypeReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x20E;
    req->header.length = 1;
    req->type = type;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlParamSetCfm* WMSP_WL_ParamSetLifeTime(u16* buf, u16 tableNumber, u16 camLifeTime, u16 frameLifeTime) { // wmsp_wl_control.c:1348
    WlParamSetLifeTimeReq* req; // r0 - :1351
    WlParamSetCfm* cfm; // r0 - :1352
    
    req = (WlParamSetLifeTimeReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x211;
    req->header.length = 3;
    req->tableNumber = tableNumber;
    req->camLifeTime = camLifeTime;
    req->frameLifeTime = frameLifeTime;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlParamSetCfm* WMSP_WL_ParamSetMaxConnectableChild(u16* buf, u16 count) { // wmsp_wl_control.c:1386
    WlParamSetMaxConnectableChildReq* req; // r0 - :1388
    WlParamSetCfm* cfm; // r0 - :1389
    
    req = (WlParamSetMaxConnectableChildReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x212;
    req->header.length = 1;
    req->count = count;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlParamSetCfm* WMSP_WL_ParamSetBeaconSendRecvInd(u16* buf, u16 enableMessage) { // wmsp_wl_control.c:1493
    WlParamSetBeaconSendRecvIndReq* req; // r0 - :1495
    WlParamSetCfm* cfm; // r0 - :1496
    
    req = (WlParamSetBeaconSendRecvIndReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x215;
    req->header.length = 1;
    req->enableMessage = enableMessage;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlParamSetCfm* WMSP_WL_ParamSetNullKeyResponseMode(u16* buf, u16 mode) { // wmsp_wl_control.c:1528
    WlParamSetNullKeyModeReq* req; // r0 - :1530
    WlParamSetCfm* cfm; // r0 - :1531
    
    req = (WlParamSetNullKeyModeReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x216;
    req->header.length = 1;
    req->mode = mode;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlParamSetCfm* WMSP_WL_ParamSetBeaconPeriod(u16* buf, u16 beaconPeriod) { // wmsp_wl_control.c:1635
    WlParamSetBeaconPeriodReq* req; // r0 - :1637
    WlParamSetCfm* cfm; // r0 - :1638
    
    req = (WlParamSetBeaconPeriodReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x242;
    req->header.length = 1;
    req->beaconPeriod = beaconPeriod;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlParamSetCfm* WMSP_WL_ParamSetGameInfo(u16* buf, u16 gameInfoLength, u16* gameInfo) { // wmsp_wl_control.c:1741
    WlParamSetGameInfoReq* req; // r0 - :1743
    WlParamSetCfm* cfm; // r0 - :1744
    
    req = (WlParamSetGameInfoReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = 0x245;
    req->header.length = (gameInfoLength + 1) / 2 + 1;
    req->gameInfoLength = gameInfoLength;
    MIi_CpuCopy16(gameInfo, req->gameInfo, gameInfoLength);
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = 1;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}

WlParamGetMacAddressCfm* WMSP_WL_ParamGetMacAddress(u16* buf) { // wmsp_wl_control.c:1811
    return (WlParamGetMacAddressCfm*)WMSPi_WL_NoArg(buf, 0x281, 4);
}

WlParamGetEnableChannelCfm* WMSP_WL_ParamGetEnableChannel(u16* buf) { // wmsp_wl_control.c:1844
    return (WlParamGetEnableChannelCfm*)WMSPi_WL_NoArg(buf, 0x283, 3);
}

WlParamGetModeCfm* WMSP_WL_ParamGetMode(u16* buf) { // wmsp_wl_control.c:1860
    return (WlParamGetModeCfm*)WMSPi_WL_NoArg(buf, 0x284, 2);
}

WlDevShutdownCfm* WMSP_WL_DevShutdown(u16* buf) { // wmsp_wl_control.c:2241
    return (WlDevShutdownCfm*)WMSPi_WL_NoArg(buf, 0x301, 1);
}

WlDevIdleCfm* WMSP_WL_DevIdle(u16* buf) { // wmsp_wl_control.c:2256
    return (WlDevIdleCfm*)WMSPi_WL_NoArg(buf, 0x302, 1);
}

WlDevClass1Cfm* WMSP_WL_DevClass1(u16* buf) { // wmsp_wl_control.c:2271
    return (WlDevClass1Cfm*)WMSPi_WL_NoArg(buf, 0x303, 1);
}

WlDevRestartCfm* WMSP_WL_DevRestart(u16* buf) { // wmsp_wl_control.c:2286
    return (WlDevRestartCfm*)WMSPi_WL_NoArg(buf, 0x304, 1);
}

WlDevClrInfoCfm* WMSP_WL_DevSetInitializeWirelessCounter(u16* buf) { // wmsp_wl_control.c:2301
    return (WlDevClrInfoCfm*)WMSPi_WL_NoArg(buf, 0x305, 1);
}

WlDevGetVersionCfm* WMSP_WL_DevGetVersion(u16* buf) { // wmsp_wl_control.c:2321
    return (WlDevGetVersionCfm*)WMSPi_WL_NoArg(buf, 0x306, 9);
}

WlDevGetInfoCfm* WMSP_WL_DevGetWirelessCounter(u16* buf) { // wmsp_wl_control.c:2338
    return (WlDevGetInfoCfm*)WMSPi_WL_NoArg(buf, 0x307, 0x5C);
}

WlDevGetStationStateCfm* WMSP_WL_DevGetStationState(u16* buf) { // wmsp_wl_control.c:2354
    return (WlDevGetStationStateCfm*)WMSPi_WL_NoArg(buf, 0x308, 2);
}

static WlCmdCfm* WMSPi_WL_NoArg(u16* buf, u16 code, u16 cfmLength) { // wmsp_wl_control.c:2446
    WlCmdReq* req; // r0 - :2448
    WlCmdCfm* cfm; // r0 - :2449
    
    req = (WlCmdReq*)buf;
    CLEAR_WL_RSV(req);
    
    req->header.code = code;
    req->header.length = 0;
    
    cfm = GET_CFM(req);
    cfm->header.code = req->header.code;
    cfm->header.length = cfmLength;
    
    WMSP_WlRequest((u16 *)req);
    return cfm;
}