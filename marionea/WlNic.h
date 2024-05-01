#include "Marionea.h"

#ifndef WL_NIC_H
#define WL_NIC_H

typedef struct {
    struct {
        u32 success; // offset 00
        u32 failed; // offset 04
        u32 retry; // offset 08
        u32 ackErr; // offset 0c
        u32 unicast; // offset 10
        u32 multicast; // offset 14
        u32 wep; // offset 18
        u32 beacon; // offset 1c
    } tx; // offset 00
    struct {
        u32 rts; // offset 00
        u32 fragment; // offset 04
        u32 unicast; // offset 08
        u32 multicast; // offset 0c
        u32 wep; // offset 10
        u32 beacon; // offset 14
        u32 fcsErr; // offset 18
        u32 duplicateErr; // offset 1c
        u32 mpDuplicateErr; // offset 20
        u32 icvErr; // offset 24
        u32 fcErr; // offset 28
        u32 lengthErr; // offset 2c
        u32 plcpErr; // offset 30
        u32 bufOvfErr; // offset 34
        u32 pathErr; // offset 38
        u32 rateErr; // offset 3c
        u32 fcsOk; // offset 40
    } rx; // offset 20
    struct {
        u32 txMp; // offset 00
        u32 txKey; // offset 04
        u32 txNull; // offset 08
        u32 rxMp; // offset 0c
        u32 rxMpAck; // offset 10
        u32 keyResponseErr[15]; // offset 14
    } multiPoll; // offset 64
} WlCounter;

#ifdef MARIONEA_INTERNAL
typedef struct {
    u16 a; // offset 00
    u16 b; // offset 02
    u16 seed; // offset 04
    u16 rsv; // offset 06
} RAND_CTRL;

typedef struct {
    u16 Id; // offset 00
    u16 Bits; // offset 02
    u16 InitNum; // offset 04
    u16 ChanNum; // offset 06
    u16 BbpCnt; // offset 08
    u16 pad; // offset 0a
    u32 BkReg; // offset 0c
} RF_CONFIG;

typedef struct {
    CAM_ELEMENT* pCAM; // offset 00
    u16 CamMaxStaNum; // offset 04
    u16 MaxStaNum; // offset 06
    u16 MacAdrs[3]; // offset 08
    u16 RetryLimit; // offset 0e
    u16 EnableChannel; // offset 10
    u16 Mode; // offset 12
    u16 Rate; // offset 14
    u16 AuthAlgo; // offset 16
    u16 WepMode; // offset 18
    u16 WepKeyId; // offset 1a
    u16 FrameLifeTimePerBeacon; // offset 1c
    u16 BeaconType : 1; // offset 1e
    u16 BcSsidResponse : 1; // offset 1e
    u16 PreambleType : 1; // offset 1e
    u16 MainAntenna : 1; // offset 1e
    u16 Diversity : 1; // offset 1e
    u16 UseAntenna : 1; // offset 1e
    u16 BcnTxRxIndMsg : 1; // offset 1e
    u16 NullKeyRes : 1; // offset 1e
    u16 MulticastPass : 1; // offset 1e
    u16 dmm : 7; // offset 1e
    u16 ActiveZone; // offset 20
    u16 DiagResult; // offset 22
    u32 ParamFlag; // offset 24
} CONFIG_PARAM;

typedef struct {
    u64 NextTbttTsf; // offset 00
    u16 STA; // offset 08
    u16 RSSI; // offset 0a
    u16 Mode; // offset 0c
    u16 PowerMgtMode; // offset 0e
    u16 PowerState; // offset 10
    u16 bSynchro; // offset 12
    u16 RxDtims; // offset 14
    u16 PN15Rate; // offset 16
    u16 SigTest2; // offset 18
    u16 bFirstTbtt; // offset 1a
    u16 ManCtrlRetry; // offset 1c
    u16 SSIDLength; // offset 1e
    u8 SSID[32]; // offset 20
    u8 SSIDMask[32]; // offset 40
    RATE_SET RateSet; // offset 60
    u16 BSSID[3]; // offset 64
    u16 AID; // offset 6a
    u16 SeqNum; // offset 6c
    u16 BeaconPeriod; // offset 6e
    u16 ListenInterval; // offset 70
    u16 CurrListenInterval; // offset 72
    u16 DTIMPeriod; // offset 74
    u16 DTIMCount; // offset 76
    u16 rsvcc; // offset 78
    u16 CurrChannel; // offset 7a
    u16 CapaInfo; // offset 7c
    u16 BeaconLostTh; // offset 7e
    u16 BeaconLostCnt; // offset 80
    u16 LinkAdrs[3]; // offset 82
    u16 APCamAdrs; // offset 88
    u16 FrameCtrl; // offset 8a
    u16 FrameLifeTime; // offset 8c
    u16 bExistTIM; // offset 8e
    u16 TxPower; // offset 90
    struct {
        struct {
            u16 SSID; // offset 00
            u16 TIM; // offset 02
            u16 GameInfo; // offset 04
        } Beacon; // offset 00
        struct {
            u16 SSID; // offset 00
        } ProbeReq; // offset 06
        struct {
            u16 Size; // offset 00
        } RxBuf; // offset 08
    } Ofst; // offset 92
    void* GameInfoAdrs; // offset 9c
    u16 GameInfoLength; // offset a0
    u16 GameInfoAlign; // offset a2
    u16 bUpdateGameInfo; // offset a4
    u16 TmpttPs; // offset a6
    u32 IntervalCount; // offset a8
    u16 Scrambler; // offset ac
    u16 DbgSeqNum; // offset ae
    u16 FatalErr; // offset b0
    u16 rsv; // offset b2
    u16 CurrErrCount; // offset b4
    u16 TxBufErrCount; // offset b6
    u16 TxBufResCount; // offset b8
    u16 WepErrCount; // offset ba
    u16 MpEndErrCount; // offset bc
    u16 NotPollTxErrCount; // offset be
} WORK_PARAM;

typedef struct {
    u16 adrs; // offset 00
    u16 value; // offset 02
} MAC_INIT_REGS;

extern u16 NULL_ADRS[3];
extern u16 BC_ADRS[3];
extern u16 MP_ADRS[3];
extern u16 MPKEY_ADRS[3];

extern const u16 RateBit2Element[16];
extern const u16 RateElement2Bit[120];

void InitializeParam(CAM_ELEMENT* pCam, u32 staNum);
u16 WSetMacAdrs(u16* pMacAdrs);
u16 WSetRetryLimit(u16 retry) ;
u16 WSetEnableChannel(u16 enableChannel);
u16 WSetMode(u16 mode);
u16 WSetRate(u16 rate);
u16 WSetWepMode(u16 mode);
u16 WSetWepKeyId(u16 keyId);
u16 WSetWepKey(const u16* pKey);
u16 WSetBeaconType(u16 type);
u16 WSetBcSsidResponse(u16 response);
u16 WSetBeaconLostThreshold(u16 threshold);
u16 WSetActiveZoneTime(u16 time, u32 update);
u16 WSetSsidMask(const u16* pMask);
u16 WSetPreambleType(u16 type);
u16 WSetAuthAlgo(u16 type);
u16 WSetCCA_ED(u32 ccaMode, u32 edThreshold);
u16 WSetMainAntenna(u32 mainAntenna);
u16 WSetDiversity(u32 diversity, u32 useAntenna);
u16 WSetBeaconSendRecvIndicate(u32 mode);
u16 WSetNullKeyMode(u32 mode);
u16 WSetMcPassMode(u32 mode);
u16 WSetBssid(u16* pBssid);
u16 WSetSsid(u16 length, u8* pSsid);
u16 WSetBeaconPeriod(u16 period);
u16 WSetDTIMPeriod(u16 period);
u16 WSetListenInterval(u16 interval);
void WSetDefaultParameters();
u16 WSetChannel(u16 channel, u32 bDirect);
u16 WSetRateSet(RATE_SET* pRateSet);
void WSetTxTimeStampOffset();
u16 WSetPowerMgtMode(u32 mode);
u16 WSetPowerState(u32 state);
u16 WSetForcePowerState(u32 state);
void WShutdown();
void WWakeUp();
u16 WSetFrameLifeTime(u32 lifeTimePerBeacon);
void WDisableTmpttPowerSave();
void WEnableTmpttPowerSave();
u16 WInitGameInfo(u32 length, u8* pGameInfo);
u16 WSetGameInfo(u32 length, u8* pGameInfo);
void WSetAids(u16 aid);
void WClearAids();
void WSetKSID();
void WClearKSID(void* unused);
void WSetStaState(u32 state);
void WSetMacAdrs1(u16* dst, u16* src1);
void WSetMacAdrs2(u16* dst, u16* src1, u16* src2);
void WSetMacAdrs3(u16* dst, u16* src1, u16* src2, u16* src3);
void WInitCounter();
void WUpdateCounter();
u32 WCheckSSID(u16 len, u8* pSSID);
u32 MatchMacAdrs(u16* pAdrs1, u16* pAdrs2);
u32 CheckEnableChannel(u32 ch);
void WElement2RateSet(SUP_RATE_ELEMENT* pSup, RATE_SET* pRateSet);
u32 WCalcManRate();
void WStart();
void WStop();
u32 BBP_Read(u32 adrs);
u32 BBP_Write(u32 adrs, u32 data);
void RF_Write(u32 data);
u16 CalcBbpCRC();
void WConfigDevice();
void InitMac();
void InitBaseBand();
void InitRF();
void InitializeAlarm();
void WWaitus(u32 us);
void SetupPeriodicTimeOut(u32 ms, void (*pFunc)(void*));
void ClearPeriodicTimeOut();
void WIntervalTimer();
void SetupTimeOut(u32 ms, void (*pFunc)(void*));
void SetupUsTimeOut(u32 us, void (*pFunc)(void*));
void ClearTimeOut();
void DMA_Read(void* destAdrs, void* srcAdrs, u32 length);
void DMA_Write(void* destAdrs, void* srcAdrs, u32 length);
void DMA_WriteHeaderData(TXFRM_MAC* destAdrs, MAC_HEADER* header, u8* data, u32 length);
void DMA_WepWriteHeaderData(TXFRM_MAC* destAdrs, MAC_HEADER* header, u8* data, u32 length);
void WL_WriteByte(void* p, u8 data);
u8 WL_ReadByte(void* p);
void RND_init(u32 a, u32 b);
void RND_seed(u32 seed);
u16 RND_rand();
u16 calc_NextCRC(u8 data, u16 total);
void WCheckTxBuf();
void SetFatalErr(u32 errCode);
void SendFatalErrMsgTask();
void TerminateWlTask();
void ReleaseWlTask();
#endif

#endif