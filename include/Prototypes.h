extern void WMSP_AutoDeAuth(void* msg); // req_AutoDeAuth.c:42
extern void WMSP_AutoDisconnect(void* msg); // req_AutoDisconnect.c:49
extern void WMSP_Disable(); // req_Disable.c:38
extern void WMSP_Disconnect(void* msg); // req_Disconnect.c:189
extern int WMSP_DisconnectCore(u32* args, int indicateFlag, u16* disconnected); // req_Disconnect.c:225
extern void WMSP_IndicateDisconnectionFromMyself(int parent, u16 aid, void* mac); // req_Disconnect.c:605
extern void WMSP_Enable(void* msg); // req_Enable.c:41
extern void WMSPi_CommonInit(u32 miscFlags); // req_Enable.c:87
extern void WMSP_End(); // req_End.c:73
extern void WMSP_EndDCF(); // req_EndDCF.c:59
extern void WMSP_EndMP(); // req_EndMP.c:94
extern void WMSP_EndParent(); // req_EndParent.c:131
extern void WMSP_EndScan(); // req_EndScan.c:74
extern void WMSP_GetWirelessCounter(); // req_GetWirelessCounter.c:36
extern void WMSP_InitWirelessCounter(); // req_InitWirelessCounter.c:36
extern void WMSP_Initialize(void* msg); // req_Initialize.c:40
extern void WMSP_MeasureChannel(void* msg); // req_MeasureChannel.c:36
extern void WMSP_PowerOff(); // req_PowerOff.c:32
extern void WMSP_PowerOn(); // req_PowerOn.c:47
extern int WMSPi_CommonWlIdle(u16* pWlCommand, u16* pWlResult); // req_PowerOn.c:120
extern void WMSP_Reset(); // req_Reset.c:36
extern void WMSP_SetBeaconPeriod(void* msg); // req_SetBeaconPeriod.c:36
extern void WMSP_SetBeaconTxRxInd(void* msg); // req_SetBeaconTxRxInd.c:37
extern void WMSP_SetDCFData(void* msg); // req_SetDCFData.c:28
extern void WMSP_SetEntry(void* msg); // req_SetEntry.c:28
extern void WMSP_SetGameInfo(void* msg); // req_SetGameInfo.c:28
extern void WMSP_SetLifeTime(void* msg); // req_SetLifeTime.c:36
extern void WMSP_SetMPData(void* msg); // req_SetMPData.c:28
extern void WMSP_SetMPParameter(void* msg); // req_SetMPParameter.c:28
extern WMErrCode WMSP_SetMPParameterCore(WMMPParam* param, WMMPParam* old_param); // req_SetMPParameter.c:65
extern void WMSP_SetParentParam(void* msg); // req_SetParentParam.c:35
extern void WMSP_SetPowerSaveMode(void* msg); // req_SetPowerSaveMode.c:29
extern void WMSP_SetWEPKey(void* msg); // req_SetWEPKey.c:34
extern void WMSP_SetWEPKeyEx(void* msg); // req_SetWEPKey.c:90
extern void WMSP_StartConnectEx(void* msg); // req_StartConnect.c:48
extern void WMSP_StartDCF(void* msg); // req_StartDCF.c:28
extern void WMSP_StartMP(void* msg); // req_StartMP.c:30
extern void WMSP_StartParent(void* msg); // req_StartParent.c:51
extern void WMSP_StartScan(void* msg); // req_StartScan.c:44
extern void WMSP_StartScanEx(void* msg); // req_StartScan.c:307
extern void WMSP_StartTestMode(); // req_StartTestMode.c:40
extern void WMSP_StartTestRxMode(); // req_StartTestRxMode.c:34
extern void WMSP_StopTestMode(); // req_StopTestMode.c:36
extern void WMSP_StopTestRxMode(); // req_StopTestRxMode.c:34
extern void WMSP_VAlarmSetMPData(); // valarm_SetMPData.c:29
extern void WMSP_KickNextMP_Parent(void* msg); // valarm_SetMPData.c:173
extern void WMSP_KickNextMP_Child(); // valarm_SetMPData.c:201
extern void WMSP_KickNextMP_Resume(void* msg); // valarm_SetMPData.c:230
extern void WMSP_IndicateThread(); // wmsp_indicate.c:99
extern void WMSP_RequestResumeMP(); // wmsp_indicate.c:673
extern void WMSP_InitAlarm(); // wmsp_indicate.c:1951
extern void WMSP_InitSendQueue(); // wmsp_port.c:38
extern void WMSP_SendMaKeyData(); // wmsp_port.c:79
extern void WMSP_SendMaMP(u16 pollBitmap); // wmsp_port.c:144
extern void WMSP_ResumeMaMP(u16 pollBitmap); // wmsp_port.c:294
extern int WMSP_PutSendQueue(u32 childBitmap, u16 priority, u16 port, u32 destBitmap, u16* sendData, u16 sendDataSize, void (*callback)(void*), void* arg); // wmsp_port.c:748
extern int WMSP_FlushSendQueue(int timeout, u16 pollBitmap); // wmsp_port.c:869
extern void WMSP_CleanSendQueue(u16 aidBitmap); // wmsp_port.c:1134
extern void WMSP_ParsePortPacket(u16 aid, u16 wmHeader, u16* data, u8 rssi, u16 length, WMMpRecvBuf* recvBuf); // wmsp_port.c:1290
extern void WMSP_RequestThread(); // wmsp_request.c:89
extern void WM_sp_init(WlInit* wlInit, WmInit* wmInit); // wmsp_system.c:55
extern void WMSP_ReturnResult2Wm9(void* ptr); // wmsp_system.c:192
extern void* WMSP_GetBuffer4Callback2Wm9(); // wmsp_system.c:237
extern u16* WMSP_WlRequest(u16* request); // wmsp_system.c:264
extern int WMSP_CheckMacAddress(u8* macAdr); // wmsp_system.c:348
extern void WMSP_CopyParentParam(WMGameInfo* gameInfop, WMParentParam* pparamp); // wmsp_system.c:422
extern int WMSP_SetAllParams(u16 wmApiID, u16* buf); // wmsp_system.c:461
extern u16 WMSP_GetAllowedChannel(u16 bitField); // wmsp_system.c:576
extern void WMSP_AddRssiToList(u8 rssi8); // wmsp_system.c:650
extern void WMSP_FillRssiIntoList(u8 rssi8); // wmsp_system.c:666
extern u16 WMSP_GetAverageLinkLevel(); // wmsp_system.c:685
extern u16 WMSP_GetLinkLevel(u32 rssi); // wmsp_system.c:715
extern void WMSP_SetThreadPriorityLow(); // wmsp_system.c:756
extern void WMSP_SetThreadPriorityHigh(); // wmsp_system.c:792
extern u32* WMSP_GetInternalRequestBuf(); // wmsp_system.c:914
extern void WMSP_ResetSizeVars(); // wmsp_system.c:943
extern void WMSP_SetParentMaxSize(u16 parentMaxSize); // wmsp_system.c:962
extern void WMSP_SetChildMaxSize(u16 childMaxSize); // wmsp_system.c:994
extern void WMSP_SetParentSize(u16 parentSize); // wmsp_system.c:1026
extern void WMSP_SetChildSize(u16 childSize); // wmsp_system.c:1051
extern void WMSP_InitVAlarm(); // wmsp_valarm.c:72
extern void WMSP_CancelVAlarm(); // wmsp_valarm.c:92
extern void WMSP_SetVAlarm(); // wmsp_valarm.c:113
extern WlMlmeResetCfm* WMSP_WL_MlmeReset(u16* buf, u16 mib); // wmsp_wl_control.c:48
extern WlMlmePowerMgtCfm* WMSP_WL_MlmePowerManagement(u16* buf, u16 pwrMgtMode, u16 wakeUp, u16 recieveDtims); // wmsp_wl_control.c:85
extern WlMlmeScanCfm* WMSP_WL_MlmeScan(u16* buf, u32 bufSize, u16* bssid, u16 ssidLength, u8* ssid, u16 scanType, u8* channelList, u16 maxChannelTime); // wmsp_wl_control.c:131
extern WlMlmeJoinCfm* WMSP_WL_MlmeJoin(u16* buf, u16 timeOut, WlBssDesc* bssDesc); // wmsp_wl_control.c:181
extern WlMlmeAuthCfm* WMSP_WL_MlmeAuthenticate(u16* buf, u16* peerMacAdrs, u16 algorithm, u16 timeOut); // wmsp_wl_control.c:223
extern WlMlmeDeAuthenticateCfm* WMSP_WL_MlmeDeAuthenticate(u16* buf, u16* peerMacAdrs, u16 reasonCode); // wmsp_wl_control.c:265
extern WlMlmeAssociateCfm* WMSP_WL_MlmeAssociate(u16* buf, u16* peerMacAdrs, u16 listenInterval, u16 timeOut); // wmsp_wl_control.c:305
extern WlMlmeStartCfm* WMSP_WL_MlmeStart(u16* buf, u16 ssidLength, u8* ssid, u16 beaconPeriod, u16 dtimPeriod, u16 channel, u16 basicRateSet, u16 supportRateSet, u16 gameInfoLength, WMGameInfo* gameInfo); // wmsp_wl_control.c:430
extern WlMlmeMeasureChannelCfm* WMSP_WL_MlmeMeasureChannel(u16* buf, u16 ccaMode, u16 edThreshold, u16 measureTime, u8* channelList); // wmsp_wl_control.c:481
extern WlMaDataCfm* WMSP_WL_MaData(u16* buf, WlTxFrame* frame); // wmsp_wl_control.c:526
extern WlMaKeyDataCfm* WMSP_WL_MaKeyData(u16* buf, u16 length, u16 wmHeader, u16* keyDatap); // wmsp_wl_control.c:580
extern WlMaMpCfm* WMSP_WL_MaMp(u16* buf, u16 resume, u16 retryLimit, u16 txop, u16 pollBitmap, u16 tmptt, u16 currTsf, u16 dataLength, u16 wmHeader, u16* datap); // wmsp_wl_control.c:625
extern WlMaClrDataCfm* WMSP_WL_MaClearData(u16* buf, u16 flag); // wmsp_wl_control.c:712
extern WlParamSetCfm* WMSP_WL_ParamSetAll(WlParamSetAllReq* req); // wmsp_wl_control.c:750
extern WlParamSetCfm* WMSP_WL_ParamSetWepKeyId(u16* buf, u16 wepKeyId); // wmsp_wl_control.c:990
extern WlParamSetCfm* WMSP_WL_ParamSetBeaconLostThreshold(u16* buf, u16 beaconLostTh); // wmsp_wl_control.c:1130
extern WlParamSetCfm* WMSP_WL_ParamSetSsidMask(u16* buf, u8* mask); // wmsp_wl_control.c:1200
extern WlParamSetCfm* WMSP_WL_ParamSetPreambleType(u16* buf, u16 type); // wmsp_wl_control.c:1235
extern WlParamSetCfm* WMSP_WL_ParamSetLifeTime(u16* buf, u16 tableNumber, u16 camLifeTime, u16 frameLifeTime); // wmsp_wl_control.c:1348
extern WlParamSetCfm* WMSP_WL_ParamSetMaxConnectableChild(u16* buf, u16 count); // wmsp_wl_control.c:1386
extern WlParamSetCfm* WMSP_WL_ParamSetBeaconSendRecvInd(u16* buf, u16 enableMessage); // wmsp_wl_control.c:1493
extern WlParamSetCfm* WMSP_WL_ParamSetNullKeyResponseMode(u16* buf, u16 mode); // wmsp_wl_control.c:1528
extern WlParamSetCfm* WMSP_WL_ParamSetBeaconPeriod(u16* buf, u16 beaconPeriod); // wmsp_wl_control.c:1635
extern WlParamSetCfm* WMSP_WL_ParamSetGameInfo(u16* buf, u16 gameInfoLength, u16* gameInfo); // wmsp_wl_control.c:1741
extern WlParamGetMacAddressCfm* WMSP_WL_ParamGetMacAddress(u16* buf); // wmsp_wl_control.c:1811
extern WlParamGetEnableChannelCfm* WMSP_WL_ParamGetEnableChannel(u16* buf); // wmsp_wl_control.c:1844
extern WlParamGetModeCfm* WMSP_WL_ParamGetMode(u16* buf); // wmsp_wl_control.c:1860
extern WlDevShutdownCfm* WMSP_WL_DevShutdown(u16* buf); // wmsp_wl_control.c:2241
extern WlDevIdleCfm* WMSP_WL_DevIdle(u16* buf); // wmsp_wl_control.c:2256
extern WlDevClass1Cfm* WMSP_WL_DevClass1(u16* buf); // wmsp_wl_control.c:2271
extern WlDevRestartCfm* WMSP_WL_DevRestart(u16* buf); // wmsp_wl_control.c:2286
extern WlDevClrInfoCfm* WMSP_WL_DevSetInitializeWirelessCounter(u16* buf); // wmsp_wl_control.c:2301
extern WlDevGetVersionCfm* WMSP_WL_DevGetVersion(u16* buf); // wmsp_wl_control.c:2321
extern WlDevGetInfoCfm* WMSP_WL_DevGetWirelessCounter(u16* buf); // wmsp_wl_control.c:2338
extern WlDevGetStationStateCfm* WMSP_WL_DevGetStationState(u16* buf); // wmsp_wl_control.c:2354
extern void WVR_Begin(int handle); // wvr_sp.c:113
extern void WVR_Shutdown(); // wvr_sp.c:229