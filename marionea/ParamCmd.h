#include "Marionea.h"

#ifndef PARAM_CMD_H
#define PARAM_CMD_H

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 staMacAdrs[3]; // offset 10
    u16 retryLimit; // offset 16
    u16 enableChannel; // offset 18
    u16 rsv; // offset 1a
    u16 mode; // offset 1c
    u16 rate; // offset 1e
    u16 wepMode; // offset 20
    u16 wepKeyId; // offset 22
    u16 wepKey[4][10]; // offset 24
    u16 beaconType; // offset 74
    u16 probeRes; // offset 76
    u16 beaconLostTh; // offset 78
    u16 activeZoneTime; // offset 7a
    u8 ssidMask[32]; // offset 7c
    u16 preambleType; // offset 9c
    u16 authAlgo; // offset 9e
} WlParamSetAllReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlParamSetCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 staMacAdrs[3]; // offset 10
} WlParamSetMacAdrsReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 staMacAdrs[3]; // offset 10
} WlParamSetMacAddressReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 retryLimit; // offset 10
} WlParamSetRetryLimitReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 enableChannel; // offset 10
} WlParamSetEnableChannelReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 mode; // offset 10
} WlParamSetModeReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 rate; // offset 10
} WlParamSetRateReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 wepMode; // offset 10
} WlParamSetWepModeReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 wepKeyId; // offset 10
} WlParamSetWepKeyIdReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 wepKey[4][10]; // offset 10
} WlParamSetWepKeyReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 beaconType; // offset 10
} WlParamSetBeaconTypeReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 probeRes; // offset 10
} WlParamSetProbeResReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 probeRes; // offset 10
} WlParamSetProbeResponseReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 beaconLostTh; // offset 10
} WlParamSetBeaconLostThresholdReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 beaconLostTh; // offset 10
} WlParamSetBeaconLostThReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 activeZoneTime; // offset 10
} WlParamSetActiveZoneReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u8 mask[32]; // offset 10
} WlParamSetSsidMaskReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 type; // offset 10
} WlParamSetPreambleTypeReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 type; // offset 10
} WlParamSetAuthenticationAlgorithmReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 type; // offset 10
} WlParamSetAuthAlgoReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 ccaMode; // offset 10
    u16 edThreshold; // offset 12
    u16 agcLimit; // offset 14
} WlParamSetCCAModeEDThReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 ccaMode; // offset 10
    u16 edThreshold; // offset 12
    u16 agcLimit; // offset 14
} WlParamSetCCAModeEDThresholdReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 tableNumber; // offset 10
    u16 camLifeTime; // offset 12
    u16 frameLifeTime; // offset 14
} WlParamSetLifeTimeReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 count; // offset 10
} WlParamSetMaxConnReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 count; // offset 10
} WlParamSetMaxConnectableChildReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 mainAntenna; // offset 10
} WlParamSetMainAntennaReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 diversity; // offset 10
    u16 useAntenna; // offset 12
} WlParamSetDiversityReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 enableMessage; // offset 10
} WlParamSetBeaconSendRecvIndReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 mode; // offset 10
} WlParamSetNullKeyModeReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 bssid[3]; // offset 10
} WlParamSetBssidReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 ssidLength; // offset 10
    u8 ssid[32]; // offset 12
} WlParamSetSsidReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 beaconPeriod; // offset 10
} WlParamSetBeaconPeriodReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 dtimPeriod; // offset 10
} WlParamSetDtimPeriodReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 listenInterval; // offset 10
} WlParamSetIntervalReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 gameInfoLength; // offset 10
    u16 gameInfo[1]; // offset 12
} WlParamSetGameInfoReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 staMacAdrs[3]; // offset 06
    u16 retryLimit; // offset 0c
    u16 enableChannel; // offset 0e
    u16 channel; // offset 10
    u16 mode; // offset 12
    u16 rate; // offset 14
    u16 wepMode; // offset 16
    u16 wepKeyId; // offset 18
    u16 beaconType; // offset 1a
    u16 probeRes; // offset 1c
    u16 beaconLostTh; // offset 1e
    u16 activeZoneTime; // offset 20
    u8 ssidMask[32]; // offset 22
    u16 preambleType; // offset 42
    u16 authAlgo; // offset 44
} WlParamGetAllCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 staMacAdrs[3]; // offset 06
} WlParamGetMacAdrsCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 staMacAdrs[3]; // offset 06
} WlParamGetMacAddressCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 retryLimit; // offset 06
} WlParamGetRetryLimitCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 enableChannel; // offset 06
    u16 channel; // offset 08
} WlParamGetEnableChannelCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 mode; // offset 06
} WlParamGetModeCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 rate; // offset 06
} WlParamGetRateCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 wepMode; // offset 06
} WlParamGetWepModeCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 wepKeyId; // offset 06
} WlParamGetWepKeyIdCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 beaconType; // offset 06
} WlParamGetBeaconTypeCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 probe; // offset 06
} WlParamGetProbeResCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 probe; // offset 06
} WlParamGetProbeResponseCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 beaconLostTh; // offset 06
} WlParamGetBeaconLostThresholdCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 beaconLostTh; // offset 06
} WlParamGetBeaconLostThCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 activeZoneTime; // offset 06
} WlParamGetActiveZoneCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u8 mask[32]; // offset 06
} WlParamGetSsidMaskCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 type; // offset 06
} WlParamGetPreambleTypeCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 type; // offset 06
} WlParamGetAuthenticationAlgorithmCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 type; // offset 06
} WlParamGetAuthAlgoCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 ccaMode; // offset 06
    u16 edThreshold; // offset 08
    u16 agcLimit; // offset 0a
} WlParamGetCCAModeEDThCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 ccaMode; // offset 06
    u16 edThreshold; // offset 08
    u16 agcLimit; // offset 0a
} WlParamGetCCAModeEDThresholdCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 count; // offset 06
} WlParamGetMaxConnectableChildCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 count; // offset 06
} WlParamGetMaxConnCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 mainAntenna; // offset 06
} WlParamGetMainAntennaCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 diversity; // offset 06
    u16 useAntenna; // offset 08
} WlParamGetDiversityCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 enableMessage; // offset 06
} WlParamGetBeaconSendRecvIndCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 mode; // offset 06
} WlParamGetNullKeyModeCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 bssid[3]; // offset 06
} WlParamGetBssidCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 ssidLength; // offset 06
    u8 ssid[32]; // offset 08
} WlParamGetSsidCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 beaconPeriod; // offset 06
} WlParamGetBeaconPeriodCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 dtimPeriond; // offset 06
} WlParamGetDtimPeriodCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 listenInterval; // offset 06
} WlParamGetIntervalCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 gameInfoLength; // offset 06
    u16 gameInfo[1]; // offset 08
} WlParamGetGameInfoCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 mode; // offset 10
} WlParamSetMcPassModeReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 mode; // offset 06
} WlParamGetMcPassModeCfm;

#ifdef MARIONEA_INTERNAL
u16 PARAMSET_AllReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_MacAdrsReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_RetryReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_EnableChannelReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_ModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_RateReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_WepModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_WepKeyIdReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_WepKeyReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_BeaconTypeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_ResBcSsidReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_BeaconLostThReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_ActiveZoneReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_SSIDMaskReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_PreambleTypeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_AuthAlgoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_CCAModeEDThReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_LifeTimeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_MaxConnReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_MainAntennaReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_DiversityReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_BcnSendRecvIndReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_NullKeyModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_McPassModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_BSSIDReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_SSIDReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_BeaconPeriodReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_DTIMPeriodReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_ListenIntervalReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMSET_GameInfoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_AllReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_MacAdrsReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_RetryReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_EnableChannelReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_ModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_RateReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_WepModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_WepKeyIdReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_BeaconTypeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_ResBcSsidReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_BeaconLostThReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_ActiveZoneReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_SSIDMaskReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_PreambleTypeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_AuthAlgoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_CCAModeEDThReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_MaxConnReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_MainAntennaReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_DiversityReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_BcnSendRecvIndReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_NullKeyModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_McPassModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_BSSIDReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_SSIDReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_BeaconPeriodReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_DTIMPeriodReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_ListenIntervalReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 PARAMGET_GameInfoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
#endif

#endif