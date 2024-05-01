void UpdateApList(u16 channel, BEACON_FRAME* pFrm, SSID_ELEMENT* pSSID); // ApList.c:53
void InitApList(); // ApList.c:134
void UpdateApListTask(); // ApList.c:153
void* AllocateHeapBuf(HEAPBUF_MAN* pBufMan, u32 Length); // BufMan.c:71
u32 ReleaseHeapBuf(HEAPBUF_MAN* pBufMan, void* pBuf); // BufMan.c:162
u32 MoveHeapBuf(HEAPBUF_MAN* pFromMan, HEAPBUF_MAN* pToMan, void* pBuf); // BufMan.c:238
u32 NewHeapBuf(HEAPBUF_MAN* pBufMan, void* pBuf); // BufMan.c:287
u32 AddHeapBuf(HEAPBUF_MAN* pBufMan, void* pBuf); // BufMan.c:342
u32 DeleteHeapBuf(HEAPBUF_MAN* pBufMan, void* pBuf); // BufMan.c:408
void* GetHeapBufNextAdrs(void* pBuf); // BufMan.c:486
void InitHeapBufMan(HEAPBUF_MAN* pBufMan, u16 Flag); // BufMan.c:585
void InitializeHeapBuf(HEAP_INFO* pHeapInfo); // BufMan.c:611
void ReleaseAllWlHeapBuf(); // BufMan.c:661
void ReleaseAllHeapBuf(HEAPBUF_MAN* pHeapBufMan); // BufMan.c:692
u32 CAM_Search(u16* pMacAdrs); // CAM.c:63
u32 CAM_SearchAdd(u16* pMacAdrs); // CAM.c:109
void CAM_AddBcFrame(HEAPBUF_MAN* pBufMan, void* pBuf); // CAM.c:207
void CAM_IncFrameCount(TXFRM* pFrm); // CAM.c:291
void CAM_DecFrameCount(TXFRM* pFrm); // CAM.c:336
void CAM_SetStaState(u16 camAdrs, u16 state); // CAM.c:396
void CAM_SetRSSI(u16 camAdrs, u16 rssi); // CAM.c:448
void CAM_SetPowerMgtMode(u16 camAdrs, u16 pmtMode); // CAM.c:473
void CAM_SetDoze(u32 camAdrs); // CAM.c:505
void CAM_SetAwake(u32 camAdrs); // CAM.c:529
void CAM_SetCapaInfo(u32 camAdrs, u32 capInfo); // CAM.c:550
void CAM_SetSupRate(u32 camAdrs, u32 SupRate); // CAM.c:571
void CAM_SetLastSeqCtrl(u32 camAdrs, u32 seqCtrl); // CAM.c:592
void CAM_SetAuthSeed(u32 camAdrs, u32 seed); // CAM.c:613
void CAM_UpdateLifeTime(u32 camAdrs); // CAM.c:633
u32 CAM_AllocateAID(u16 camAdrs); // CAM.c:665
void CAM_ReleaseAID(u16 camAdrs); // CAM.c:736
u32 CAM_GetStaState(u32 camAdrs); // CAM.c:826
u32 CAM_IsActive(u32 camAdrs); // CAM.c:845
u32 CAM_GetPowerMgtMode(u32 camAdrs); // CAM.c:864
u16* CAM_GetMacAdrs(u32 camAdrs); // CAM.c:883
u32 CAM_GetAuthSeed(u32 camAdrs); // CAM.c:902
u32 CAM_GetLastSeqCtrl(u32 camAdrs); // CAM.c:921
u32 CAM_GetTxRate(u32 camAdrs); // CAM.c:940
u32 CAM_GetAID(u32 camAdrs); // CAM.c:961
u32 CAM_GetFrameCount(u32 camAdrs); // CAM.c:980
void CAM_SetTIMElementBitmap(u32 camAdrs); // CAM.c:1030
void CAM_ClrTIMElementBitmap(u32 camAdrs); // CAM.c:1089
void CAM_TimerTask(); // CAM.c:1158
void CAM_Delete(u16 camAdrs); // CAM.c:1267
void InitializeCAM(); // CAM.c:1307
void InitCAM(); // CAM.c:1346
u16 DEV_ShutdownReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // DevCmd.c:49
u16 DEV_IdleReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // DevCmd.c:84
u16 DEV_Class1ReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // DevCmd.c:127
u16 DEV_RebootReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // DevCmd.c:166
u16 DEV_ClearWlInfoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // DevCmd.c:201
u16 DEV_GetVerInfoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // DevCmd.c:239
u16 DEV_GetWlInfoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // DevCmd.c:293
u16 DEV_GetStateReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // DevCmd.c:330
u16 DEV_TestSignalReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // DevCmd.c:364
void IntrCarrierSuppresionSignal(); // DevCmd.c:644
u16 DEV_TestRxReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // DevCmd.c:690
void DiagMacRegister(); // Diag.c:101
void DiagMacMemory(); // Diag.c:229
void DiagBaseBand(); // Diag.c:843
u32 FLASH_VerifyCheckSum(u32* pCrc); // Flash.c:45
void FLASH_Wait(); // Flash.c:96
void FLASH_Read(u32 adrs, u32 size, u8* pBuf); // Flash.c:128
void FLASH_DirectRead(u32 adrs, u32 size, u8* pBuf); // Flash.c:183
u32 FLASH_MakeImage(); // Flash.c:206
u16 MA_DataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MA.c:48
u16 MA_KeyDataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MA.c:223
u16 MA_MpReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MA.c:363
u16 MA_TestDataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MA.c:775
u16 MA_ClrDataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MA.c:850
void IssueMaDataConfirm(HEAPBUF_MAN* pBufMan, void* pBuf); // MA.c:905
u16 MLME_ResetReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MLME.c:69
u16 MLME_PwrMgtReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MLME.c:105
u16 MLME_ScanReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MLME.c:166
u16 MLME_JoinReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MLME.c:245
u16 MLME_AuthReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MLME.c:336
u16 MLME_DeAuthReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MLME.c:403
u16 MLME_AssReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MLME.c:497
u16 MLME_ReAssReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MLME.c:566
u16 MLME_DisAssReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MLME.c:632
u16 MLME_StartReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MLME.c:719
u16 MLME_MeasChanReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // MLME.c:803
void MLME_ScanTask(); // MLME.c:886
void MLME_JoinTask(); // MLME.c:1173
void MLME_AuthTask(); // MLME.c:1296
void MLME_AssTask(); // MLME.c:1436
void MLME_ReAssTask(); // MLME.c:1570
void MLME_MeasChannelTask(); // MLME.c:1695
void MLME_BeaconLostTask(); // MLME.c:1903
void IssueMlmeConfirm(); // MLME.c:1934
u32 MLME_IssueAuthIndication(u16* pMacAdrs, u16 algorithm); // MLME.c:1981
u32 MLME_IssueDeAuthIndication(u16* pMacAdrs, u16 reason); // MLME.c:2022
u32 MLME_IssueAssIndication(u16* pMacAdrs, u16 aid, SSID_ELEMENT* pSSID); // MLME.c:2063
u32 MLME_IssueReAssIndication(u16* pMacAdrs, u16 aid, SSID_ELEMENT* pSSID); // MLME.c:2124
u32 MLME_IssueDisAssIndication(u16* pMacAdrs, u16 reason); // MLME.c:2185
u32 MLME_IssueBeaconLostIndication(u16* pMacAdrs); // MLME.c:2227
u32 MLME_IssueBeaconSendIndication(); // MLME.c:2267
u32 MLME_IssueBeaconRecvIndication(void* pRxFrm); // MLME.c:2304
void InitializeMLME(); // MLME.c:2382
u16 PARAMSET_AllReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:64
u16 PARAMSET_MacAdrsReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:109
u16 PARAMSET_RetryReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:138
u16 PARAMSET_EnableChannelReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:164
u16 PARAMSET_ModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:192
u16 PARAMSET_RateReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:221
u16 PARAMSET_WepModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:249
u16 PARAMSET_WepKeyIdReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:277
u16 PARAMSET_WepKeyReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:305
u16 PARAMSET_BeaconTypeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:333
u16 PARAMSET_ResBcSsidReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:362
u16 PARAMSET_BeaconLostThReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:388
u16 PARAMSET_ActiveZoneReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:414
u16 PARAMSET_SSIDMaskReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:440
u16 PARAMSET_PreambleTypeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:466
u16 PARAMSET_AuthAlgoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:492
u16 PARAMSET_CCAModeEDThReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:518
u16 PARAMSET_LifeTimeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:555
u16 PARAMSET_MaxConnReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:617
u16 PARAMSET_MainAntennaReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:651
u16 PARAMSET_DiversityReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:681
u16 PARAMSET_BcnSendRecvIndReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:710
u16 PARAMSET_NullKeyModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:738
u16 PARAMSET_McPassModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:766
u16 PARAMSET_BSSIDReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:809
u16 PARAMSET_SSIDReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:835
u16 PARAMSET_BeaconPeriodReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:861
u16 PARAMSET_DTIMPeriodReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:890
u16 PARAMSET_ListenIntervalReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:919
u16 PARAMSET_GameInfoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:948
u16 PARAMGET_AllReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:993
u16 PARAMGET_MacAdrsReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1037
u16 PARAMGET_RetryReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1069
u16 PARAMGET_EnableChannelReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1098
u16 PARAMGET_ModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1127
u16 PARAMGET_RateReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1156
u16 PARAMGET_WepModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1185
u16 PARAMGET_WepKeyIdReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1214
u16 PARAMGET_BeaconTypeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1244
u16 PARAMGET_ResBcSsidReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1274
u16 PARAMGET_BeaconLostThReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1304
u16 PARAMGET_ActiveZoneReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1334
u16 PARAMGET_SSIDMaskReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1365
u16 PARAMGET_PreambleTypeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1399
u16 PARAMGET_AuthAlgoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1429
u16 PARAMGET_CCAModeEDThReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1459
u16 PARAMGET_MaxConnReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1491
u16 PARAMGET_MainAntennaReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1520
u16 PARAMGET_DiversityReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1553
u16 PARAMGET_BcnSendRecvIndReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1587
u16 PARAMGET_NullKeyModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1620
u16 PARAMGET_McPassModeReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1653
u16 PARAMGET_BSSIDReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1698
u16 PARAMGET_SSIDReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1731
u16 PARAMGET_BeaconPeriodReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1767
u16 PARAMGET_DTIMPeriodReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1797
u16 PARAMGET_ListenIntervalReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1827
u16 PARAMGET_GameInfoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt); // ParamCmd.c:1857
void RxDataFrameTask(); // RxCtrl.c:66
u32 RxMpFrame(RXFRM* pFrm); // RxCtrl.c:324
void RxKeyDataFrame(RXFRM* pFrm); // RxCtrl.c:450
u32 RxMpAckFrame(RXFRM* pFrm); // RxCtrl.c:645
void RxBeaconFrame(BEACON_FRAME* pFrm); // RxCtrl.c:740
void RxManCtrlTask(); // RxCtrl.c:2717
void DefragTask(); // RxCtrl.c:3011
void DefragTimerTask(); // RxCtrl.c:3348
void InitRxCtrl(); // RxCtrl.c:3395
void InitializeTask(); // TaskMan.c:91
void MainTaskRoutine(); // TaskMan.c:136
void AddTask(s32 nPriority, u32 nTaskID); // TaskMan.c:291
u32 DeleteTask(u32 nPriority); // TaskMan.c:362
void LowestIdleTask(); // TaskMan.c:416
void ExecuteMessage(void** pMsg); // TaskMan.c:501
void TxqPri(u32 pri); // TxCtrl.c:64
void CopyTxFrmToMacBuf(TXFRM_MAC* pMacTxFrm, WlMaDataReq* pTxReq); // TxCtrl.c:298
u32 CheckFrameTimeout(TXFRM* pTxFrm); // TxCtrl.c:381
void TxqEndData(TXFRM* pFrm, u32 flag); // TxCtrl.c:442
void TxqEndManCtrl(TXFRM* pFrm, u32 flag); // TxCtrl.c:541
void TxqEndPsPoll(TXFRM* pFrm, u32 flag); // TxCtrl.c:887
void TxqEndBroadCast(TXFRM* pFrm, u32 flag); // TxCtrl.c:936
void TxEndKeyData(TXQ* pTxq); // TxCtrl.c:1014
void ClearTxKeyData(); // TxCtrl.c:1070
void ClearTxMp(); // TxCtrl.c:1109
void ClearTxData(); // TxCtrl.c:1144
void ClearQueuedPri(u32 pri); // TxCtrl.c:1202
void ResetTxqPri(u32 pri); // TxCtrl.c:1241
void DeleteTxFrames(u32 camAdrs); // TxCtrl.c:1298
void DeleteTxFrameByAdrs(u16* pMacAdrs); // TxCtrl.c:1367
void DeleteAllTxFrames(); // TxCtrl.c:1413
void MessageDeleteTx(u32 pri, u32 bMsg); // TxCtrl.c:1465
void TxManCtrlFrame(TXFRM* pFrm); // TxCtrl.c:1525
void SetManCtrlFrame(TXFRM* pFrm); // TxCtrl.c:1545
void TxPsPollFrame(); // TxCtrl.c:1579
void StartBeaconFrame(); // TxCtrl.c:1621
void StopBeaconFrame(); // TxCtrl.c:1652
void MakeBeaconFrame(); // TxCtrl.c:1692
void UpdateGameInfoElement(); // TxCtrl.c:1842
DISASS_FRAME* MakeDisAssFrame(u16* pDA, u16 reasonCode); // TxCtrl.c:1923
ASSREQ_FRAME* MakeAssReqFrame(u16* pDA); // TxCtrl.c:1969
REASSREQ_FRAME* MakeReAssReqFrame(u16* pDA); // TxCtrl.c:2021
ASSRES_FRAME* MakeAssResFrame(u16 camAdrs, u16 statusCode, SSID_ELEMENT* pSSID); // TxCtrl.c:2074
REASSRES_FRAME* MakeReAssResFrame(u16 camAdrs, u16 statusCode, SSID_ELEMENT* pSSID); // TxCtrl.c:2157
PRBREQ_FRAME* MakeProbeReqFrame(u16* pDA); // TxCtrl.c:2245
PRBRES_FRAME* MakeProbeResFrame(u16* pDA); // TxCtrl.c:2294
AUTH_FRAME* MakeAuthFrame(u16* pDA, u16 txtLen, u32 bCheck); // TxCtrl.c:2355
DEAUTH_FRAME* MakeDeAuthFrame(u16* pDA, u16 reasonCode, u32 bCheck); // TxCtrl.c:2416
void MakePsPollFrame(u16 aid); // TxCtrl.c:2469
void InitManHeader(TXFRM* pFrm, u16* pDA); // TxCtrl.c:2503
u32 IsExistManFrame(u16* pDA, u16 frameCtrl); // TxCtrl.c:2526
void InitTxCtrl(); // TxCtrl.c:2825
void WaitLoop_Rxpe(); // WaitLoop.c:44
void WaitLoop_Waitus(u32 us, void (*TimeoutFunc)(void*)); // WaitLoop.c:73
void WaitLoop_ClrAid(); // WaitLoop.c:98
u32 WaitLoop_BbpAccess(); // WaitLoop.c:141
u32 WaitLoop_RfAccess(); // WaitLoop.c:169
void RequestCmdTask(); // WlCmdIf.c:267
u16 CMD_ReservedReqCmd(); // WlCmdIf.c:548
void SendMessageToWmDirect(HEAPBUF_MAN* pBufMan, void* pMsg); // WlCmdIf.c:568
void SendMessageToWmTask(); // WlCmdIf.c:620
void InitializeCmdIf(); // WlCmdIf.c:657
void WlIntr(); // WlIntr.c:80
void MacBugTxMp(); // WlIntr.c:1934
void* AdjustRingPointer(void* p); // WlIntr.c:1996
void InitializeIntr(); // WlIntr.c:2167
void ReleaseIntr(); // WlIntr.c:2190
void WlIntrTxBeaconTask(); // WlIntrTask.c:77
void WlIntrTxEndTask(); // WlIntrTask.c:106
void WlIntrRxEndTask(); // WlIntrTask.c:240
void WlIntrMpEndTask(); // WlIntrTask.c:615
void SetParentTbttTxqTask(); // WlIntrTask.c:770
void InitializeParam(CAM_ELEMENT* pCam, u32 staNum); // WlNic.c:330
u16 WSetMacAdrs(u16* pMacAdrs); // WlNic.c:369
u16 WSetRetryLimit(u16 retry); // WlNic.c:400
u16 WSetEnableChannel(u16 enableChannel); // WlNic.c:427
u16 WSetMode(u16 mode); // WlNic.c:454
u16 WSetRate(u16 rate); // WlNic.c:488
u16 WSetWepMode(u16 mode); // WlNic.c:516
u16 WSetWepKeyId(u16 keyId); // WlNic.c:565
u16 WSetWepKey(const u16* pKey); // WlNic.c:589
u16 WSetBeaconType(u16 type); // WlNic.c:614
u16 WSetBcSsidResponse(u16 response); // WlNic.c:638
u16 WSetBeaconLostThreshold(u16 threshold); // WlNic.c:663
u16 WSetActiveZoneTime(u16 time, u32 update); // WlNic.c:690
u16 WSetSsidMask(const u16* pMask); // WlNic.c:741
u16 WSetPreambleType(u16 type); // WlNic.c:769
u16 WSetAuthAlgo(u16 type); // WlNic.c:825
u16 WSetCCA_ED(u32 ccaMode, u32 edThreshold); // WlNic.c:849
u16 WSetMainAntenna(u32 mainAntenna); // WlNic.c:877
u16 WSetDiversity(u32 diversity, u32 useAntenna); // WlNic.c:902
u16 WSetBeaconSendRecvIndicate(u32 mode); // WlNic.c:940
u16 WSetNullKeyMode(u32 mode); // WlNic.c:963
u16 WSetMcPassMode(u32 mode); // WlNic.c:991
u16 WSetBssid(u16* pBssid); // WlNic.c:1029
u16 WSetSsid(u16 length, u8* pSsid); // WlNic.c:1065
u16 WSetBeaconPeriod(u16 period); // WlNic.c:1121
u16 WSetDTIMPeriod(u16 period); // WlNic.c:1152
u16 WSetListenInterval(u16 interval); // WlNic.c:1182
void WSetDefaultParameters(); // WlNic.c:1223
u16 WSetChannel(u16 channel, u32 bDirect); // WlNic.c:1312
u16 WSetRateSet(RATE_SET* pRateSet); // WlNic.c:1487
void WSetTxTimeStampOffset(); // WlNic.c:1514
u16 WSetPowerMgtMode(u32 mode); // WlNic.c:1574
u16 WSetPowerState(u32 state); // WlNic.c:1610
u16 WSetForcePowerState(u32 state); // WlNic.c:1661
void WShutdown(); // WlNic.c:1706
void WWakeUp(); // WlNic.c:1760
u16 WSetFrameLifeTime(u32 lifeTimePerBeacon); // WlNic.c:1830
void WDisableTmpttPowerSave(); // WlNic.c:1881
void WEnableTmpttPowerSave(); // WlNic.c:1907
u16 WInitGameInfo(u32 length, u8* pGameInfo); // WlNic.c:1929
u16 WSetGameInfo(u32 length, u8* pGameInfo); // WlNic.c:1958
void WSetAids(u16 aid); // WlNic.c:2009
void WClearAids(); // WlNic.c:2036
void WSetKSID(); // WlNic.c:2067
void WClearKSID(void* unused); // WlNic.c:2087
void WSetStaState(u32 state); // WlNic.c:2111
void WSetMacAdrs1(u16* dst, u16* src1); // WlNic.c:2157
void WSetMacAdrs2(u16* dst, u16* src1, u16* src2); // WlNic.c:2178
void WSetMacAdrs3(u16* dst, u16* src1, u16* src2, u16* src3); // WlNic.c:2202
void WInitCounter(); // WlNic.c:2243
void WUpdateCounter(); // WlNic.c:2277
u32 WCheckSSID(u16 len, u8* pSSID); // WlNic.c:2494
u32 MatchMacAdrs(u16* pAdrs1, u16* pAdrs2); // WlNic.c:2552
u32 CheckEnableChannel(u32 ch); // WlNic.c:2571
void WElement2RateSet(SUP_RATE_ELEMENT* pSup, RATE_SET* pRateSet); // WlNic.c:2602
u32 WCalcManRate(); // WlNic.c:2658
void WStart(); // WlNic.c:2700
void WStop(); // WlNic.c:2919
u32 BBP_Read(u32 adrs); // WlNic.c:3073
u32 BBP_Write(u32 adrs, u32 data); // WlNic.c:3100
void RF_Write(u32 data); // WlNic.c:3140
u16 CalcBbpCRC(); // WlNic.c:3198
void WConfigDevice(); // WlNic.c:3322
void InitMac(); // WlNic.c:3357
void InitBaseBand(); // WlNic.c:3412
void InitRF(); // WlNic.c:3455
void InitializeAlarm(); // WlNic.c:3672
void WWaitus(u32 us); // WlNic.c:3729
void SetupPeriodicTimeOut(u32 ms, void (*pFunc)(void*)); // WlNic.c:3748
void ClearPeriodicTimeOut(); // WlNic.c:3776
void WIntervalTimer(); // WlNic.c:3797
void SetupTimeOut(u32 ms, void (*pFunc)(void*)); // WlNic.c:3876
void SetupUsTimeOut(u32 us, void (*pFunc)(void*)); // WlNic.c:3898
void ClearTimeOut(); // WlNic.c:3921
void DMA_Read(void* destAdrs, void* srcAdrs, u32 length); // WlNic.c:3960
void DMA_Write(void* destAdrs, void* srcAdrs, u32 length); // WlNic.c:4101
void DMA_WriteHeaderData(TXFRM_MAC* destAdrs, MAC_HEADER* header, u8* data, u32 length); // WlNic.c:4120
void DMA_WepWriteHeaderData(TXFRM_MAC* destAdrs, MAC_HEADER* header, u8* data, u32 length); // WlNic.c:4148
void WL_WriteByte(void* p, u8 data); // WlNic.c:4341
u8 WL_ReadByte(void* p); // WlNic.c:4372
void RND_init(u32 a, u32 b); // WlNic.c:4512
void RND_seed(u32 seed); // WlNic.c:4534
u16 RND_rand(); // WlNic.c:4553
u16 calc_NextCRC(u8 data, u16 total); // WlNic.c:4609
void WCheckTxBuf(); // WlNic.c:4819
void SetFatalErr(u32 errCode); // WlNic.c:4992
void SendFatalErrMsgTask(); // WlNic.c:5026
void TerminateWlTask(); // WlNic.c:5209
void ReleaseWlTask(); // WlNic.c:5253
void WlessLibReboot(); // WlSys.c:70
u32 WL_InitDriver(WlInit* pInit); // WlSys.c:120
struct _OSThread* WL_GetThreadStruct(); // WlSys.c:310
void CARD_Init(); // card_api.c:60
u32 CARD_SetThreadPriority(u32 prior); // card_api.c:223
u8* CARD_GetRomHeader(); // card_api.c:265
void CARDi_LockMutexForARM7(); // card_command.c:46
void CARDi_OnFifoRecv(enum _enum_31522 tag, u32 data, int err); // card_command.c:178
void CARDi_InitResourceLock(); // card_common.c:186
void CARDi_InitCommand(); // card_common.c:200
u32 CARDi_ReadRomIDCore(); // card_rom.c:219
u32 CARDi_ReadRomID(); // card_rom.c:671
void CARDi_InitRom(); // card_rom.c:821
void CARD_InitPulledOutCallback(); // card_sp_pullOut.c:54
int CARD_IsPulledOut(); // card_sp_pullOut.c:169
int CARD_CompareCardID(); // card_sp_pullOut.c:198
int CARD_IsCardIreqLo(); // card_sp_pullOut.c:240
void CARD_CheckPullOut_Polling(); // card_sp_pullOut.c:271
u8 CARDi_CommandReadStatus(); // card_spi.c:228
void CARDi_InitStatusRegister(); // card_spi.c:478
void CARDi_ReadBackupCore(u32 src, void* dst, u32 len); // card_spi.c:518
void CARDi_ProgramBackupCore(u32 dst, void* src, u32 len); // card_spi.c:542
void CARDi_WriteBackupCore(u32 dst, void* src, u32 len); // card_spi.c:584
void CARDi_VerifyBackupCore(u32 dst, void* src, u32 len); // card_spi.c:626
void CARDi_EraseBackupSectorCore(u32 dst, u32 len); // card_spi.c:654
void CARDi_EraseBackupSubSectorCore(u32 dst, u32 len); // card_spi.c:692
void CARDi_EraseChipCore(); // card_spi.c:729
void CARDi_SetWriteProtectCore(u16 stat); // card_spi.c:751
void CARDi_InitTaskQueue(struct CARDTaskQueue* queue); // card_task.c:37
void CARDi_InitTask(struct CARDTask* task, u32 priority, void* userdata, void (*function)(struct CARDTask*), void (*callback)(struct CARDTask*)); // card_task.c:74
void CARDi_ProcessTask(struct CARDTaskQueue* queue, struct CARDTask* task, int blocking, int changePriority); // card_task.c:97
struct CARDTask* CARDi_ReceiveTask(struct CARDTaskQueue* queue, int blocking); // card_task.c:153
void CARDi_TaskWorkerProcedure(void* arg); // card_task.c:183
void RTC_Init(u32 priority); // control.c:87
int RTC_IsAvailablePxiCommand(u16 command); // control.c:227
void _start(); // crt0.c:78
void _start_AutoloadDoneCallback(void** argv); // crt0.c:285
void NitroSpStartUp(); // crt0.c:445
void CTRDGi_InitCommon(); // ctrdg.c:59
int CTRDG_IsPulledOut(); // ctrdg.c:296
int CTRDG_IsExisting(); // ctrdg.c:356
void CTRDGi_ChangeLatestAccessCycle(struct CTRDGRomCycle* r); // ctrdg.c:430
int CTRDGi_LockByProcessor(u16 lockID, struct CTRDGLockByProc* info); // ctrdg.c:471
void CTRDGi_UnlockByProcessor(u16 lockID, struct CTRDGLockByProc* info); // ctrdg.c:509
void CTRDGi_SendtoPxi(u32 data); // ctrdg.c:528
void CTRDG_Init(); // ctrdg_sp.c:61
void CTRDGi_InitModuleInfo(); // ctrdg_sp.c:125
void CTRDG_VibPulseEdgeUpdate(void* ctrdg_ex_data); // ctrdg_sp.c:262
void CTRDG_CheckPullOut_Polling(); // ctrdg_sp.c:515
void FS_Init(); // fs_api.c:54
void EXIi_SetBitRcnt0L(u16 mask, u16 data); // gpio.c:32
void RTCi_GpioStart(); // gpio.c:38
void RTCi_GpioEnd(); // gpio.c:81
void EXIi_SelectRcnt(enum _enum_35466 type); // gpio.c:83
void RTCi_GpioSendCommand(u16 command, u16 parameter); // gpio.c:119
void RTCi_GpioSendData(void* pData, u32 size); // gpio.c:181
void RTCi_GpioReceiveData(void* pData, u32 size); // gpio.c:259
void RTC_Reset(); // instruction.c:43
void RTC_SetHourFormat(u16 format); // instruction.c:65
void RTC_ReadDateTime(union RTCRawData* data); // instruction.c:135
void RTC_WriteDateTime(union RTCRawData* data); // instruction.c:153
void RTC_ReadDate(struct RTCRawDate* date); // instruction.c:171
void RTC_ReadTime(struct RTCRawTime* time); // instruction.c:189
void RTC_WriteTime(struct RTCRawTime* time); // instruction.c:207
int RTC_ReadPulse(struct RTCRawPulse* pulse); // instruction.c:226
int RTC_WritePulse(struct RTCRawPulse* pulse); // instruction.c:253
int RTC_ReadAlarm1(struct RTCRawAlarm* alarm); // instruction.c:281
int RTC_WriteAlarm1(struct RTCRawAlarm* alarm); // instruction.c:309
int RTC_ReadAlarm2(struct RTCRawAlarm* alarm); // instruction.c:337
int RTC_WriteAlarm2(struct RTCRawAlarm* alarm); // instruction.c:364
void RTC_ReadStatus1(struct RTCRawStatus1* stat); // instruction.c:389
void RTC_WriteStatus1(struct RTCRawStatus1* stat); // instruction.c:407
void RTC_ReadStatus2(struct RTCRawStatus2* stat); // instruction.c:425
void RTC_WriteStatus2(struct RTCRawStatus2* stat); // instruction.c:443
void RTC_ReadAdjust(struct RTCRawAdjust* adjust); // instruction.c:461
void RTC_WriteAdjust(struct RTCRawAdjust* adjust); // instruction.c:479
void RTC_ReadFree(struct RTCRawFree* free); // instruction.c:497
void RTC_WriteFree(struct RTCRawFree* free); // instruction.c:515
void NitroSpMain(); // main.c:81
int InitializeAllocateSystem(); // main.c:185
u8 MATH_CountPopulation(u32 x); // math.c:101
void MI_WaitDma(u32 dmaNo); // mi_dma.c:1222
void MI_StopDma(u32 dmaNo); // mi_dma.c:1253
void MI_StopAllDma(); // mi_dma.c:1292
void MIi_CpuClear16(u16 data, void* destp, u32 size); // mi_memory.c:103
void MIi_CpuCopy16(const void* srcp, void* destp, u32 size); // mi_memory.c:128
void MIi_CpuClear32(u32 data, void* destp, u32 size); // mi_memory.c:344
void MIi_CpuCopy32(const void* srcp, void* destp, u32 size); // mi_memory.c:367
void MIi_CpuClearFast(u32 data, void* destp, u32 size); // mi_memory.c:568
void MI_CpuFill8(void* dstp, u8 data, u32 size); // mi_memory.c:946
void MI_CpuCopy8(const void* srcp, void* dstp, u32 size); // mi_memory.c:1144
u32 MI_SwapWord(u32 setData, u32* destp); // mi_swap.c:43
void MIC_SetIrqFunction(u32 intrBit, void (*function)()); // mic_irq.c:112
void MIC_EnableMultipleInterrupt(); // mic_irq.c:137
void MIC_DisableMultipleInterrupt(); // mic_irq.c:190
void MIC_IrqHandler(); // mic_irq.c:483
u16 MIC_ExecSampling8(); // mic_sampling.c:57
u16 MIC_ExecSampling12(); // mic_sampling.c:108
void MIC_Init(); // mic_sp.c:55
void MIC_AnalyzeCommand(u32 data); // mic_sp.c:82
void MIC_ExecuteProcess(struct SPIEntry* entry); // mic_sp.c:322
void NVRAM_WriteEnable(); // nvram_instruction.c:32
void NVRAM_WriteDisable(); // nvram_instruction.c:48
void NVRAM_ReadStatusRegister(u8* buf); // nvram_instruction.c:64
void NVRAM_ReadDataBytes(u32 address, u32 size, u8* buf); // nvram_instruction.c:111
void NVRAM_ReadDataBytesAtHigherSpeed(u32 address, u32 size, u8* buf); // nvram_instruction.c:257
void NVRAM_PageWrite(u32 address, u16 size, u8* buf); // nvram_instruction.c:406
void NVRAM_PageProgram(u32 address, u16 size, u8* buf); // nvram_instruction.c:508
void NVRAM_PageErase(u32 address); // nvram_instruction.c:608
void NVRAM_SectorErase(u32 address); // nvram_instruction.c:638
void NVRAM_DeepPowerDown(); // nvram_instruction.c:665
void NVRAM_ReleaseFromDeepPowerDown(); // nvram_instruction.c:682
void NVRAM_ChipErase(); // nvram_instruction.c:701
void NVRAM_ReadSiliconId(u8* buf); // nvram_instruction.c:720
void NVRAM_SoftwareReset(); // nvram_instruction.c:777
void NVRAM_Init(); // nvram_sp.c:49
void NVRAM_AnalyzeCommand(u32 data); // nvram_sp.c:71
void NVRAM_ExecuteProcess(struct SPIEntry* entry); // nvram_sp.c:218
void OS_InitAlarm(); // os_alarm.c:102
int OS_IsAlarmAvailable(); // os_alarm.c:167
void OS_CreateAlarm(struct OSiAlarm* alarm); // os_alarm.c:182
void OS_SetAlarm(struct OSiAlarm* alarm, u64 tick, void (*handler)(void*), void* arg); // os_alarm.c:278
void OS_SetPeriodicAlarm(struct OSiAlarm* alarm, u64 start, u64 period, void (*handler)(void*), void* arg); // os_alarm.c:322
void OS_CancelAlarm(struct OSiAlarm* alarm); // os_alarm.c:365
void* OS_AllocFromHeap(enum _enum_16263 id, int heap, u32 size); // os_alloc.c:360
void OS_FreeToHeap(enum _enum_16263 id, int heap, void* ptr); // os_alloc.c:673
int OS_SetCurrentHeap(enum _enum_16263 id, int heap); // os_alloc.c:793
void* OS_InitAlloc(enum _enum_16263 id, void* arenaStart, void* arenaEnd, int maxHeaps); // os_alloc.c:830
int OS_CreateHeap(enum _enum_16263 id, void* start, void* end); // os_alloc.c:911
s32 OS_CheckHeap(enum _enum_16263 id, int heap); // os_alloc.c:1265
void OS_InitArena(); // os_arena.c:205
void* OS_GetArenaHi(enum _enum_16263 id); // os_arena.c:356
void* OS_GetArenaLo(enum _enum_16263 id); // os_arena.c:376
void* OS_GetInitArenaHi(enum _enum_16263 id); // os_arena.c:396
void* OS_GetInitArenaLo(enum _enum_16263 id); // os_arena.c:527
void OS_SetArenaLo(enum _enum_16263 id, void* newLo); // os_arena.c:639
void OS_InitContext(struct OSContext* context, u32 newpc, u32 newsp); // os_context.c:36
int OS_SaveContext(struct OSContext* context); // os_context.c:94
void OS_LoadContext(struct OSContext* context); // os_context.c:140
u32 OS_GetConsoleType(); // os_emulator.c:52
u32 OS_GetRunningConsoleType(); // os_emulator.c:86
u32 OSi_DetectEmulator(); // os_emulator.c:155
u32 OSi_DetectDebugger(); // os_emulator.c:295
int OSi_IsNitroModeOnTwl(); // os_emulator.c:410
void OS_InitException(); // os_exception.c:114
void OS_Init(); // os_init.c:82
void OSi_InitCommon(); // os_init.c:105
void OS_InitIrqTable(); // os_interrupt.c:39
void OS_SetIrqFunction(u32 intrBit, void (*function)()); // os_interrupt.c:60
void OSi_EnterTimerCallback(u32 timerNo, void (*callback)(void*), void* arg); // os_interrupt.c:279
u32 OS_SetIrqMask(u32 intr); // os_interrupt.c:307
u32 OS_EnableIrqMask(u32 intr); // os_interrupt.c:337
u32 OS_DisableIrqMask(u32 intr); // os_interrupt.c:366
u32 OS_ResetRequestIrqMask(u32 intr); // os_interrupt.c:396
void OS_IrqHandler(); // os_irqHandler.c:63
void OS_IrqHandler_ThreadSwitch(); // os_irqHandler.c:140
void OS_IrqDummy(); // os_irqTable.c:246
void OSi_IrqCallback(int index); // os_irqTable.c:261
void OSi_IrqDma0(); // os_irqTable.c:294
void OSi_IrqDma1(); // os_irqTable.c:300
void OSi_IrqDma2(); // os_irqTable.c:306
void OSi_IrqDma3(); // os_irqTable.c:312
void OSi_IrqTimer0(); // os_irqTable.c:327
void OSi_IrqTimer1(); // os_irqTable.c:333
void OSi_IrqTimer2(); // os_irqTable.c:339
void OSi_IrqTimer3(); // os_irqTable.c:345
void OSi_IrqVBlank(); // os_irqTable.c:365
void OS_InitMessageQueue(struct OSMessageQueue* mq, void** msgArray, s32 msgCount); // os_message.c:33
int OS_SendMessage(struct OSMessageQueue* mq, void* msg, s32 flags); // os_message.c:55
int OS_ReceiveMessage(struct OSMessageQueue* mq, void** msg, s32 flags); // os_message.c:100
int OS_ReadMessage(struct OSMessageQueue* mq, void** msg, s32 flags); // os_message.c:194
void OS_InitMutex(struct OSMutex* mutex); // os_mutex.c:37
void OS_LockMutex(struct OSMutex* mutex); // os_mutex.c:56
void OS_UnlockMutex(struct OSMutex* mutex); // os_mutex.c:86
void OSi_UnlockAllMutex(struct _OSThread* thread); // os_mutex.c:100
int OS_TryLockMutex(struct OSMutex* mutex); // os_mutex.c:140
void OSi_UnlockMutexCore(struct OSMutex* mutex, u32 type); // os_mutex.c:331
void OSi_EnqueueTail(struct _OSThread* thread, struct OSMutex* mutex); // os_mutex.c:557
void OSi_DequeueItem(struct _OSThread* thread, struct OSMutex* mutex); // os_mutex.c:607
void OS_TPrintf(char* fmt, ...); // os_printf.c:390
void OSi_TWarning(char* file, int line, char* fmt, ...); // os_printf.c:453
void OSi_TPanic(char* file, int line, char* fmt, ...); // os_printf.c:490
int OS_IsResetOccurred(); // os_pxi.c:36
void OSi_CommonCallback(u32 data); // os_pxi.c:85
void OSi_SendToPxi(u16 data); // os_pxi.c:148
void OS_InitReset(); // os_reset.c:65
void OS_ResetSystem(); // os_reset.c:171
void OSi_DoResetSystem(); // os_reset.c:483
void OS_InitLock(); // os_spinLock.c:163
s32 OS_LockByWord(u16 lockID, struct OSLockWord* lockp, void (*ctrlFuncp)()); // os_spinLock.c:369
s32 OS_LockCartridge(u16 lockID); // os_spinLock.c:583
s32 OS_UnlockCartridge(u16 lockID); // os_spinLock.c:619
s32 OS_UnLockCartridge(u16 lockID); // os_spinLock.c:646
s32 OS_TryLockCartridge(u16 lockID); // os_spinLock.c:662
s32 OS_LockCard(u16 lockID); // os_spinLock.c:716
s32 OS_UnlockCard(u16 lockID); // os_spinLock.c:733
s32 OS_TryLockCard(u16 lockID); // os_spinLock.c:757
u16 OS_ReadOwnerOfLockWord(struct OSLockWord* lockp); // os_spinLock.c:805
s32 OS_GetLockID(); // os_spinLock.c:833
void OS_ReleaseLockID(u16 lockID); // os_spinLock.c:910
u32 OS_EnableInterrupts(); // os_system.c:39
u32 OS_DisableInterrupts(); // os_system.c:59
u32 OS_RestoreInterrupts(u32 state); // os_system.c:79
u32 OS_DisableInterrupts_IrqAndFiq(); // os_system.c:123
u32 OS_RestoreInterrupts_IrqAndFiq(u32 state); // os_system.c:143
enum _enum_24784 OS_GetProcMode(); // os_system.c:188
void OS_SpinWaitSysCycles(u32 cycle); // os_system.c:254
u16 OS_GetBootType(); // os_systemWork.c:33
void OS_Terminate(); // os_terminate_sp.c:36
void OSi_TerminateCore(); // os_terminate_sp.c:108
struct OSMutex* OSi_RemoveMutexLinkFromQueue(struct _OSMutexQueue* queue); // os_thread.c:300
void OS_InitThread(); // os_thread.c:495
void OS_CreateThread(struct _OSThread* thread, void (*func)(void*), void* arg, void* stack, u32 stackSize, u32 prio); // os_thread.c:618
void OS_ExitThread(); // os_thread.c:721
void OS_SleepThread(struct _OSThreadQueue* queue); // os_thread.c:1176
void OS_WakeupThread(struct _OSThreadQueue* queue); // os_thread.c:1224
void OS_WakeupThreadDirect(struct _OSThread* thread); // os_thread.c:1284
struct _OSThread* OS_SelectThread(); // os_thread.c:1310
int OS_SetThreadPriority(struct _OSThread* thread, u32 prio); // os_thread.c:1639
u32 OS_GetThreadPriority(struct _OSThread* thread); // os_thread.c:1698
void OS_Sleep(u32 msec); // os_thread.c:1715
// TODO FIXME void (*)(struct _OSThread*, struct _OSThread*) OS_SetSwitchThreadCallback(void (*callback)(struct _OSThread*, struct _OSThread*)); // os_thread.c:1764
u32 OS_DisableScheduler(); // os_thread.c:1825
u32 OS_EnableScheduler(); // os_thread.c:1849
void OS_InitTick(); // os_tick.c:55
int OS_IsTickAvailable(); // os_tick.c:91
u64 OS_GetTick(); // os_tick.c:138
u16 OS_GetTickLo(); // os_tick.c:203
void OSi_SetTimerReserved(int timerNum); // os_timer.c:68
void OS_InitVAlarm(); // os_valarm.c:86
int OS_IsVAlarmAvailable(); // os_valarm.c:141
void OS_CreateVAlarm(struct OSiVAlarm* alarm); // os_valarm.c:289
void OS_SetVAlarm(struct OSiVAlarm* alarm, s16 count, s16 delay, void (*handler)(void*), void* arg); // os_valarm.c:313
void OS_SetPeriodicVAlarm(struct OSiVAlarm* alarm, s16 count, s16 delay, void (*handler)(void*), void* arg); // os_valarm.c:371
void OS_SetVAlarmTag(struct OSiVAlarm* alarm, u32 tag); // os_valarm.c:453
void OS_CancelVAlarm(struct OSiVAlarm* alarm); // os_valarm.c:484
void OS_CancelVAlarms(u32 tag); // os_valarm.c:544
int PAD_InitXYButton(); // pad_xyButton.c:41
void PMi_SetRegister(u16 reg, u8 data); // pm_pmic.c:62
u8 PMi_GetRegister(u16 reg); // pm_pmic.c:90
void PMi_SetControl(u8 sw); // pm_pmic.c:121
void PMi_ResetControl(u8 sw); // pm_pmic.c:139
void PM_SelfBlinkProc(); // pm_selfBlink.c:78
void PM_SetLEDPattern(enum _enum_64984 pattern); // pm_selfBlink.c:137
enum _enum_64984 PM_GetLEDPattern(); // pm_selfBlink.c:157
void PMi_SendPxiCommand(u16 command, u16 data); // pm_send.c:50
void PMi_PreSleepForDma(); // pm_sleep.c:52
void PMi_DoSleep(u32 trigger, u32 keyPattern, u32 backlight); // pm_sleep.c:150
void PM_Init(); // pm_sp.c:62
void PM_AnalyzeCommand(u32 data); // pm_sp.c:94
void PM_ExecuteProcess(struct SPIEntry* entry); // pm_sp.c:172
u16 PMi_SwitchUtilityProc(u32 procNumber, u16 parameter); // pm_utility.c:56
void PMi_SetLED(enum _enum_63738 status); // pm_utility.c:372
void PMi_SetSoundPower(int bool); // pm_utility.c:429
void PXI_InitFifo(); // pxi_fifo.c:40
void PXI_SetFifoRecvCallback(int fifotag, void (*callback)(enum _enum_31522, u32, int)); // pxi_fifo.c:139
int PXI_IsCallbackReady(int fifotag, enum _enum_32007 proc); // pxi_fifo.c:173
int PXI_SendWordByFifo(int fifotag, u32 data, int err); // pxi_fifo.c:215
void PXIi_HandlerRecvFifoNotEmpty(); // pxi_fifo.c:298
void PXI_Init(); // pxi_init.c:41
void WMSP_AutoDeAuth(void* msg); // req_AutoDeAuth.c:42
void WMSP_AutoDisconnect(void* msg); // req_AutoDisconnect.c:49
void WMSP_Disable(); // req_Disable.c:38
void WMSP_Disconnect(void* msg); // req_Disconnect.c:189
int WMSP_DisconnectCore(u32* args, int indicateFlag, u16* disconnected); // req_Disconnect.c:225
void WMSP_IndicateDisconnectionFromMyself(int parent, u16 aid, void* mac); // req_Disconnect.c:605
void WMSP_Enable(void* msg); // req_Enable.c:41
void WMSPi_CommonInit(u32 miscFlags); // req_Enable.c:87
void WMSP_End(); // req_End.c:73
void WMSP_EndDCF(); // req_EndDCF.c:59
void WMSP_EndMP(); // req_EndMP.c:94
void WMSP_EndParent(); // req_EndParent.c:131
void WMSP_EndScan(); // req_EndScan.c:74
void WMSP_GetWirelessCounter(); // req_GetWirelessCounter.c:36
void WMSP_InitWirelessCounter(); // req_InitWirelessCounter.c:36
void WMSP_Initialize(void* msg); // req_Initialize.c:40
void WMSP_MeasureChannel(void* msg); // req_MeasureChannel.c:36
void WMSP_PowerOff(); // req_PowerOff.c:32
void WMSP_PowerOn(); // req_PowerOn.c:47
int WMSPi_CommonWlIdle(u16* pWlCommand, u16* pWlResult); // req_PowerOn.c:120
void WMSP_Reset(); // req_Reset.c:36
void WMSP_SetBeaconPeriod(void* msg); // req_SetBeaconPeriod.c:36
void WMSP_SetBeaconTxRxInd(void* msg); // req_SetBeaconTxRxInd.c:37
void WMSP_SetDCFData(void* msg); // req_SetDCFData.c:28
void WMSP_SetEntry(void* msg); // req_SetEntry.c:28
void WMSP_SetGameInfo(void* msg); // req_SetGameInfo.c:28
void WMSP_SetLifeTime(void* msg); // req_SetLifeTime.c:36
void WMSP_SetMPData(void* msg); // req_SetMPData.c:28
void WMSP_SetMPParameter(void* msg); // req_SetMPParameter.c:28
enum WMErrCode WMSP_SetMPParameterCore(struct WMMPParam* param, struct WMMPParam* old_param); // req_SetMPParameter.c:65
void WMSP_SetParentParam(void* msg); // req_SetParentParam.c:35
void WMSP_SetPowerSaveMode(void* msg); // req_SetPowerSaveMode.c:29
void WMSP_SetWEPKey(void* msg); // req_SetWEPKey.c:34
void WMSP_SetWEPKeyEx(void* msg); // req_SetWEPKey.c:90
void WMSP_StartConnectEx(void* msg); // req_StartConnect.c:48
void WMSP_StartDCF(void* msg); // req_StartDCF.c:28
void WMSP_StartMP(void* msg); // req_StartMP.c:30
void WMSP_StartParent(void* msg); // req_StartParent.c:51
void WMSP_StartScan(void* msg); // req_StartScan.c:44
void WMSP_StartScanEx(void* msg); // req_StartScan.c:307
void WMSP_StartTestMode(); // req_StartTestMode.c:40
void WMSP_StartTestRxMode(); // req_StartTestRxMode.c:34
void WMSP_StopTestMode(); // req_StopTestMode.c:36
void WMSP_StopTestRxMode(); // req_StopTestRxMode.c:34
void SND_AlarmInit(); // snd_alarm.c:85
void SND_SetupAlarm(int alarmNo, u64 tick, u64 period, int id); // snd_alarm.c:96
void SND_StartAlarm(int alarmNo); // snd_alarm.c:117
void SND_StopAlarm(int alarmNo); // snd_alarm.c:152
int SND_ReadInstData(struct SNDBankData* bank, int prgNo, int key, struct SNDInstData* inst); // snd_bank.c:244
struct SNDWaveData* SND_GetWaveDataAddress(struct SNDWaveArc* waveArc, int index); // snd_bank.c:585
int SND_NoteOn(struct SNDExChannel* ch_p, int key, int velocity, s32 length, struct SNDBankData* bank, struct SNDInstData* inst); // snd_bank.c:628
void SND_SetupCapture(enum _enum_40436 capture, enum _enum_40489 format, void* buffer_addr, int length, int repeat, enum _enum_40569 in, enum _enum_40756 out); // snd_capture.c:37
int SND_IsCaptureActive(enum _enum_40436 capture); // snd_capture.c:65
void SND_SetupChannelPcm(int chNo, void* dataaddr, enum _enum_36756 format, enum _enum_36906 loop, int loopStart, int loopLen, int volume, enum _enum_37008 shift, int timer, int pan); // snd_channel.c:54
void SND_SetupChannelPsg(int chNo, enum _enum_37153 duty, int volume, enum _enum_37008 shift, int timer, int pan); // snd_channel.c:105
void SND_SetupChannelNoise(int chNo, int volume, enum _enum_37008 shift, int timer, int pan); // snd_channel.c:145
void SND_StopChannel(int chNo, s32 flags); // snd_channel.c:186
void SND_SetChannelVolume(int chNo, int volume, enum _enum_37008 shift); // snd_channel.c:215
void SND_SetChannelTimer(int chNo, int timer); // snd_channel.c:248
void SND_SetChannelPan(int chNo, int pan); // snd_channel.c:266
int SND_IsChannelActive(int chNo); // snd_channel.c:300
void SND_SetMasterPan(int pan); // snd_channel.c:321
u32 SND_GetChannelControl(int chNo); // snd_channel.c:355
void SNDi_SetSurroundDecay(int decay); // snd_channel.c:375
int CalcSurroundDecay(int volume, int pan); // snd_channel.c:412
void SND_CommandInit(); // snd_command.c:110
void SND_CommandProc(); // snd_command.c:659
void SND_ExChannelInit(); // snd_exchannel.c:92
void SND_UpdateExChannel(); // snd_exchannel.c:119
void SND_ExChannelMain(int doPeriodicProc); // snd_exchannel.c:212
int SND_StartExChannelPcm(struct SNDExChannel* ch_p, struct SNDWaveParam* wave, void* data, s32 length); // snd_exchannel.c:396
int SND_StartExChannelPsg(struct SNDExChannel* ch_p, enum _enum_37153 duty, s32 length); // snd_exchannel.c:425
int SND_StartExChannelNoise(struct SNDExChannel* ch_p, s32 length); // snd_exchannel.c:452
s32 SND_UpdateExChannelEnvelope(struct SNDExChannel* ch_p, int doPeriodicProc); // snd_exchannel.c:477
void SND_SetExChannelAttack(struct SNDExChannel* ch_p, int attack); // snd_exchannel.c:537
void SND_SetExChannelDecay(struct SNDExChannel* ch_p, int decay); // snd_exchannel.c:566
void SND_SetExChannelSustain(struct SNDExChannel* ch_p, int sustain); // snd_exchannel.c:583
void SND_SetExChannelRelease(struct SNDExChannel* ch_p, int release); // snd_exchannel.c:600
void SND_ReleaseExChannel(struct SNDExChannel* ch_p); // snd_exchannel.c:616
int SND_IsExChannelActive(struct SNDExChannel* ch_p); // snd_exchannel.c:630
struct SNDExChannel* SND_AllocExChannel(u32 chBitMask, int prio, int strongRequest, void (*callback)(struct SNDExChannel*, enum SNDExChannelCallbackStatus, void*), void* callbackData); // snd_exchannel.c:651
void SND_FreeExChannel(struct SNDExChannel* ch_p); // snd_exchannel.c:730
void SND_StopUnlockedChannel(u32 chBitMask); // snd_exchannel.c:749
void SND_LockChannel(u32 chBitMask, u32 flags); // snd_exchannel.c:793
void SND_UnlockChannel(u32 chBitMask, u32 flags); // snd_exchannel.c:844
u32 SND_GetLockedChannel(u32 flags); // snd_exchannel.c:865
void SND_InvalidateWave(void* start, void* end); // snd_exchannel.c:887
void SND_InitLfoParam(struct SNDLfoParam* lfo); // snd_exchannel.c:919
void SND_UpdateLfo(struct SNDLfo* lfo); // snd_exchannel.c:934
s32 SND_GetLfoValue(struct SNDLfo* lfo); // snd_exchannel.c:959
void SND_Enable(); // snd_global.c:59
void SND_Shutdown(); // snd_global.c:87
void SND_BeginSleep(); // snd_global.c:110
void SND_EndSleep(); // snd_global.c:135
void SND_SetMasterVolume(int volume); // snd_global.c:162
void SND_SetOutputSelector(enum SNDOutput left, enum SNDOutput right, enum SNDChannelOut channel1, enum SNDChannelOut channel3); // snd_global.c:181
void SND_Init(u32 threadPrio); // snd_main.c:118
void SND_StartIntervalTimer(); // snd_main.c:190
void SND_StopIntervalTimer(); // snd_main.c:206
int SND_SendWakeupMessage(); // snd_main.c:238
void SNDi_LockMutex(); // snd_main.c:258
void SNDi_UnlockMutex(); // snd_main.c:274
void SND_SeqInit(); // snd_seq.c:223
void SND_SeqMain(int doPeriodicProc); // snd_seq.c:255
void SND_PrepareSeq(int playerNo, void* seqBase, u32 seqOffset, struct SNDBankData* bank_p); // snd_seq.c:301
void SND_StartPreparedSeq(int playerNo); // snd_seq.c:392
void SND_StartSeq(int playerNo, void* seqBase, u32 seqOffset, struct SNDBankData* bank_p); // snd_seq.c:415
void SND_StopSeq(int playerNo); // snd_seq.c:430
void SND_PauseSeq(int playerNo, int flag); // snd_seq.c:459
void SND_SkipSeq(int playerNo, u32 tick); // snd_seq.c:497
void SND_SetTrackMute(int playerNo, u32 trackBitMask, enum _enum_48280 mute); // snd_seq.c:551
void SND_SetTrackAllocatableChannel(int playerNo, u32 trackBitMask, u32 chBitMask); // snd_seq.c:585
void SND_InvalidateSeq(void* start, void* end); // snd_seq.c:620
void SND_InvalidateBank(void* start, void* end); // snd_seq.c:659
void SNDi_SetPlayerParam(int playerNo, u32 offset, u32 data, int size); // snd_seq.c:708
void SNDi_SetTrackParam(int playerNo, u32 trackBitMask, u32 offset, u32 data, int size); // snd_seq.c:743
u16 SND_CalcTimer(int base_timer, int pitch_); // snd_util.c:312
u16 SND_CalcChannelVolume(int dB); // snd_util.c:383
s8 SND_SinIdx(int index); // snd_util.c:434
u16 SND_CalcRandom(); // snd_util.c:462
void SND_SetPlayerLocalVariable(int playerNo, int varNo, s16 var); // snd_work.c:446
void SND_SetPlayerGlobalVariable(int varNo, s16 var); // snd_work.c:455
void SND_UpdateSharedWork(); // snd_work.c:463
void SPI_Init(u32 prio); // spi_sp.c:61
void SPI_Lock(u32 id); // spi_sp.c:168
void SPI_Unlock(u32 id); // spi_sp.c:211
void SPIi_ReturnResult(u16 command, u16 result); // spi_sp.c:271
int SPIi_CheckException(); // spi_sp.c:327
void SPIi_GetException(enum SPIDeviceType type); // spi_sp.c:352
void SPIi_ReleaseException(enum SPIDeviceType type); // spi_sp.c:367
int SPIi_SetEntry(enum SPIDeviceType type, u32 process, u16 args); // spi_sp.c:389
int SPIi_CheckEntry(); // spi_sp.c:431
int STD_TVSNPrintf(char* dst, u32 len, char* fmt, char* vlist); // std_sprintf.c:157
void ISTDPrintEx(); // stubs_istd.c:64
void _ISTDbgLib_Initialize(); // stubs_istd.c:76
void _ISTDbgLib_AllocateEmualtor(); // stubs_istd.c:79
void _ISTDbgLib_FreeEmulator(); // stubs_istd.c:83
void TP_ExecSampling(union SPITpData* data, s32 range, u16* density); // tp_sampling.c:239
void TP_Init(); // tp_sp.c:50
void TP_AnalyzeCommand(u32 data); // tp_sp.c:108
void TP_ExecuteProcess(struct SPIEntry* entry); // tp_sp.c:290
void WMSP_VAlarmSetMPData(); // valarm_SetMPData.c:29
void WMSP_KickNextMP_Parent(void* msg); // valarm_SetMPData.c:173
void WMSP_KickNextMP_Child(); // valarm_SetMPData.c:201
void WMSP_KickNextMP_Resume(void* msg); // valarm_SetMPData.c:230
void WMSP_IndicateThread(); // wmsp_indicate.c:99
void WMSP_RequestResumeMP(); // wmsp_indicate.c:673
void WMSP_InitAlarm(); // wmsp_indicate.c:1951
void WMSP_InitSendQueue(); // wmsp_port.c:38
void WMSP_SendMaKeyData(); // wmsp_port.c:79
void WMSP_SendMaMP(u16 pollBitmap); // wmsp_port.c:144
void WMSP_ResumeMaMP(u16 pollBitmap); // wmsp_port.c:294
int WMSP_PutSendQueue(u32 childBitmap, u16 priority, u16 port, u32 destBitmap, u16* sendData, u16 sendDataSize, void (*callback)(void*), void* arg); // wmsp_port.c:748
int WMSP_FlushSendQueue(int timeout, u16 pollBitmap); // wmsp_port.c:869
void WMSP_CleanSendQueue(u16 aidBitmap); // wmsp_port.c:1134
void WMSP_ParsePortPacket(u16 aid, u16 wmHeader, u16* data, u8 rssi, u16 length, struct WMMpRecvBuf* recvBuf); // wmsp_port.c:1290
void WMSP_RequestThread(); // wmsp_request.c:89
void WM_sp_init(WlInit* wlInit, struct WmInit* wmInit); // wmsp_system.c:55
void WMSP_ReturnResult2Wm9(void* ptr); // wmsp_system.c:192
void* WMSP_GetBuffer4Callback2Wm9(); // wmsp_system.c:237
u16* WMSP_WlRequest(u16* request); // wmsp_system.c:264
int WMSP_CheckMacAddress(u8* macAdr); // wmsp_system.c:348
void WMSP_CopyParentParam(struct WMGameInfo* gameInfop, struct WMParentParam* pparamp); // wmsp_system.c:422
int WMSP_SetAllParams(u16 wmApiID, u16* buf); // wmsp_system.c:461
u16 WMSP_GetAllowedChannel(u16 bitField); // wmsp_system.c:576
void WMSP_AddRssiToList(u8 rssi8); // wmsp_system.c:650
void WMSP_FillRssiIntoList(u8 rssi8); // wmsp_system.c:666
u16 WMSP_GetAverageLinkLevel(); // wmsp_system.c:685
u16 WMSP_GetLinkLevel(u32 rssi); // wmsp_system.c:715
void WMSP_SetThreadPriorityLow(); // wmsp_system.c:756
void WMSP_SetThreadPriorityHigh(); // wmsp_system.c:792
u32* WMSP_GetInternalRequestBuf(); // wmsp_system.c:914
void WMSP_ResetSizeVars(); // wmsp_system.c:943
void WMSP_SetParentMaxSize(u16 parentMaxSize); // wmsp_system.c:962
void WMSP_SetChildMaxSize(u16 childMaxSize); // wmsp_system.c:994
void WMSP_SetParentSize(u16 parentSize); // wmsp_system.c:1026
void WMSP_SetChildSize(u16 childSize); // wmsp_system.c:1051
void WMSP_InitVAlarm(); // wmsp_valarm.c:72
void WMSP_CancelVAlarm(); // wmsp_valarm.c:92
void WMSP_SetVAlarm(); // wmsp_valarm.c:113
WlMlmeResetCfm* WMSP_WL_MlmeReset(u16* buf, u16 mib); // wmsp_wl_control.c:48
WlMlmePowerMgtCfm* WMSP_WL_MlmePowerManagement(u16* buf, u16 pwrMgtMode, u16 wakeUp, u16 recieveDtims); // wmsp_wl_control.c:85
WlMlmeScanCfm* WMSP_WL_MlmeScan(u16* buf, u32 bufSize, u16* bssid, u16 ssidLength, u8* ssid, u16 scanType, u8* channelList, u16 maxChannelTime); // wmsp_wl_control.c:131
WlMlmeJoinCfm* WMSP_WL_MlmeJoin(u16* buf, u16 timeOut, WlBssDesc* bssDesc); // wmsp_wl_control.c:181
WlMlmeAuthCfm* WMSP_WL_MlmeAuthenticate(u16* buf, u16* peerMacAdrs, u16 algorithm, u16 timeOut); // wmsp_wl_control.c:223
WlMlmeDeAuthenticateCfm* WMSP_WL_MlmeDeAuthenticate(u16* buf, u16* peerMacAdrs, u16 reasonCode); // wmsp_wl_control.c:265
WlMlmeAssociateCfm* WMSP_WL_MlmeAssociate(u16* buf, u16* peerMacAdrs, u16 listenInterval, u16 timeOut); // wmsp_wl_control.c:305
WlMlmeStartCfm* WMSP_WL_MlmeStart(u16* buf, u16 ssidLength, u8* ssid, u16 beaconPeriod, u16 dtimPeriod, u16 channel, u16 basicRateSet, u16 supportRateSet, u16 gameInfoLength, struct WMGameInfo* gameInfo); // wmsp_wl_control.c:430
WlMlmeMeasureChannelCfm* WMSP_WL_MlmeMeasureChannel(u16* buf, u16 ccaMode, u16 edThreshold, u16 measureTime, u8* channelList); // wmsp_wl_control.c:481
WlMaDataCfm* WMSP_WL_MaData(u16* buf, WlTxFrame* frame); // wmsp_wl_control.c:526
WlMaKeyDataCfm* WMSP_WL_MaKeyData(u16* buf, u16 length, u16 wmHeader, u16* keyDatap); // wmsp_wl_control.c:580
WlMaMpCfm* WMSP_WL_MaMp(u16* buf, u16 resume, u16 retryLimit, u16 txop, u16 pollBitmap, u16 tmptt, u16 currTsf, u16 dataLength, u16 wmHeader, u16* datap); // wmsp_wl_control.c:625
WlMaClrDataCfm* WMSP_WL_MaClearData(u16* buf, u16 flag); // wmsp_wl_control.c:712
WlParamSetCfm* WMSP_WL_ParamSetAll(WlParamSetAllReq* req); // wmsp_wl_control.c:750
WlParamSetCfm* WMSP_WL_ParamSetWepKeyId(u16* buf, u16 wepKeyId); // wmsp_wl_control.c:990
WlParamSetCfm* WMSP_WL_ParamSetBeaconLostThreshold(u16* buf, u16 beaconLostTh); // wmsp_wl_control.c:1130
WlParamSetCfm* WMSP_WL_ParamSetSsidMask(u16* buf, u8* mask); // wmsp_wl_control.c:1200
WlParamSetCfm* WMSP_WL_ParamSetPreambleType(u16* buf, u16 type); // wmsp_wl_control.c:1235
WlParamSetCfm* WMSP_WL_ParamSetLifeTime(u16* buf, u16 tableNumber, u16 camLifeTime, u16 frameLifeTime); // wmsp_wl_control.c:1348
WlParamSetCfm* WMSP_WL_ParamSetMaxConnectableChild(u16* buf, u16 count); // wmsp_wl_control.c:1386
WlParamSetCfm* WMSP_WL_ParamSetBeaconSendRecvInd(u16* buf, u16 enableMessage); // wmsp_wl_control.c:1493
WlParamSetCfm* WMSP_WL_ParamSetNullKeyResponseMode(u16* buf, u16 mode); // wmsp_wl_control.c:1528
WlParamSetCfm* WMSP_WL_ParamSetBeaconPeriod(u16* buf, u16 beaconPeriod); // wmsp_wl_control.c:1635
WlParamSetCfm* WMSP_WL_ParamSetGameInfo(u16* buf, u16 gameInfoLength, u16* gameInfo); // wmsp_wl_control.c:1741
WlParamGetMacAddressCfm* WMSP_WL_ParamGetMacAddress(u16* buf); // wmsp_wl_control.c:1811
WlParamGetEnableChannelCfm* WMSP_WL_ParamGetEnableChannel(u16* buf); // wmsp_wl_control.c:1844
WlParamGetModeCfm* WMSP_WL_ParamGetMode(u16* buf); // wmsp_wl_control.c:1860
WlDevShutdownCfm* WMSP_WL_DevShutdown(u16* buf); // wmsp_wl_control.c:2241
WlDevIdleCfm* WMSP_WL_DevIdle(u16* buf); // wmsp_wl_control.c:2256
WlDevClass1Cfm* WMSP_WL_DevClass1(u16* buf); // wmsp_wl_control.c:2271
WlDevRestartCfm* WMSP_WL_DevRestart(u16* buf); // wmsp_wl_control.c:2286
WlDevClrInfoCfm* WMSP_WL_DevSetInitializeWirelessCounter(u16* buf); // wmsp_wl_control.c:2301
WlDevGetVersionCfm* WMSP_WL_DevGetVersion(u16* buf); // wmsp_wl_control.c:2321
WlDevGetInfoCfm* WMSP_WL_DevGetWirelessCounter(u16* buf); // wmsp_wl_control.c:2338
WlDevGetStationStateCfm* WMSP_WL_DevGetStationState(u16* buf); // wmsp_wl_control.c:2354
void WVR_Begin(int handle); // wvr_sp.c:113
void WVR_Shutdown(); // wvr_sp.c:229