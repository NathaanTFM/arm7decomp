#include "Marionea.h"

#ifndef MLME_H
#define MLME_H

#ifdef MARIONEA_INTERNAL

// MLME States
#define STATE_NONE 0

#define STATE_SCAN 0x10
#define STATE_SCAN_0 (STATE_SCAN + 0)
#define STATE_SCAN_1 (STATE_SCAN + 1)
#define STATE_SCAN_2 (STATE_SCAN + 2)
#define STATE_SCAN_3 (STATE_SCAN + 3)
#define STATE_SCAN_5 (STATE_SCAN + 5)

#define STATE_JOIN 0x20
#define STATE_JOIN_0 (STATE_JOIN + 0)
#define STATE_JOIN_1 (STATE_JOIN + 1)
#define STATE_JOIN_5 (STATE_JOIN + 5)

#define STATE_AUTH 0x30
#define STATE_AUTH_0 (STATE_AUTH + 0)
#define STATE_AUTH_1 (STATE_AUTH + 1)
#define STATE_AUTH_3 (STATE_AUTH + 3)
#define STATE_AUTH_5 (STATE_AUTH + 5)

#define STATE_DEAUTH 0x40
#define STATE_DEAUTH_1 (STATE_DEAUTH + 1)

#define STATE_ASS 0x50
#define STATE_ASS_0 (STATE_ASS + 0)
#define STATE_ASS_1 (STATE_ASS + 1)
#define STATE_ASS_3 (STATE_ASS + 3)

#define STATE_REASS 0x60
#define STATE_REASS_0 (STATE_REASS + 0)
#define STATE_REASS_1 (STATE_REASS + 1)
#define STATE_REASS_3 (STATE_REASS + 3)

#define STATE_DISASS 0x70
#define STATE_DISASS_1 (STATE_DISASS + 1)

#define STATE_MEASCHAN 0x80
#define STATE_MEASCHAN_0 (STATE_MEASCHAN + 0)
#define STATE_MEASCHAN_1 (STATE_MEASCHAN + 1)
#define STATE_MEASCHAN_2 (STATE_MEASCHAN + 2)
#define STATE_MEASCHAN_3 (STATE_MEASCHAN + 3)
#define STATE_MEASCHAN_4 (STATE_MEASCHAN + 4)

#endif

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 algorithm; // offset 16
} WlMlmeAuthInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 reasonCode; // offset 16
} WlMlmeDeAuthInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 aid; // offset 16
    u16 ssidLength; // offset 18
    u8 ssid[32]; // offset 1a
} WlMlmeAssInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 aid; // offset 16
    u16 ssidLength; // offset 18
    u8 ssid[32]; // offset 1a
} WlMlmeReAssInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 reasonCode; // offset 16
} WlMlmeDisAssInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 apMacAdrs[3]; // offset 10
} WlMlmeBeaconLostInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
} WlMlmeBeaconSendInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 reserved1[3]; // offset 10
    u16 gameInfoLength; // offset 16
    u16 reserved2[3]; // offset 18
    u8 rate; // offset 1e
    u8 rssi; // offset 1f
    u16 reserved3[4]; // offset 20
    u16 reserved4[3]; // offset 28
    u16 srcMacAdrs[3]; // offset 2e
    u16 reserved5[4]; // offset 34
    u16 gameInfo[1]; // offset 3c
} WlMlmeBeaconRecvInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 mib; // offset 10
} WlMlmeResetReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMlmeResetCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 pwrMgtMode; // offset 10
    u16 wakeUp; // offset 12
    u16 recieveDtims; // offset 14
} WlMlmePowerMgtReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMlmePowerMgtCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 bssid[3]; // offset 10
    u16 ssidLength; // offset 16
    u8 ssid[32]; // offset 18
    u16 scanType; // offset 38
    u8 channelList[16]; // offset 3a
    u16 maxChannelTime; // offset 4a
    u16 bssidMaskCount; // offset 4c
    u16 bssidMask[3]; // offset 4e
} WlMlmeScanReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 foundMap; // offset 06
    u16 bssDescCount; // offset 08
    WlBssDesc bssDescList[1]; // offset 0a
} WlMlmeScanCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 timeOut; // offset 10
    u16 rsv; // offset 12
    WlBssDesc bssDesc; // offset 14
} WlMlmeJoinReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 statusCode; // offset 06
    u16 peerMacAdrs[3]; // offset 08
} WlMlmeJoinCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 algorithm; // offset 16
    u16 timeOut; // offset 18
} WlMlmeAuthReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 statusCode; // offset 06
    u16 peerMacAdrs[3]; // offset 08
    u16 algorithm; // offset 0e
} WlMlmeAuthCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 reasonCode; // offset 16
} WlMlmeDeAuthReq, WlMlmeDeAuthenticateReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 peerMacAdrs[3]; // offset 06
} WlMlmeDeAuthCfm, WlMlmeDeAuthenticateCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 listenInterval; // offset 16
    u16 timeOut; // offset 18
} WlMlmeAssReq, WlMlmeAssociateReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 statusCode; // offset 06
    u16 aid; // offset 08
} WlMlmeAssCfm, WlMlmeAssociateCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 newApMacAdrs[3]; // offset 10
    u16 listenInterval; // offset 16
    u16 timeOut; // offset 18
} WlMlmeReAssReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 statusCode; // offset 06
    u16 aid; // offset 08
} WlMlmeReAssCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 reasonCode; // offset 16
} WlMlmeDisAssReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMlmeDisAssCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 ssidLength; // offset 10
    u8 ssid[32]; // offset 12
    u16 beaconPeriod; // offset 32
    u16 dtimPeriod; // offset 34
    u16 channel; // offset 36
    u16 basicRateSet; // offset 38
    u16 supportRateSet; // offset 3a
    u16 gameInfoLength; // offset 3c
    u8 gameInfo[2]; // offset 3e
} WlMlmeStartReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMlmeStartCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 rsv; // offset 10
    u16 ccaMode; // offset 12
    u16 edThreshold; // offset 14
    u16 measureTime; // offset 16
    u8 channelList[16]; // offset 18
} WlMlmeMeasChanReq, WlMlmeMeasureChannelReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 reserved; // offset 06
    u16 ccaBusyInfo[16]; // offset 08
} WlMlmeMeasChanCfm, WlMlmeMeasureChannelCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 reasonCode; // offset 16
} WlMlmeDeAuthenticateInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 aid; // offset 16
    u16 ssidLength; // offset 18
    u8 ssid[32]; // offset 1a
} WlMlmeAssociateInd;

#ifdef MARIONEA_INTERNAL

typedef struct {
    u16 State; // offset 00
    u16 pad; // offset 02
    union {
        struct {
            u16 MaxConfirmLength; // offset 00
            u16 ChannelCount; // offset 02
            u16 bFound; // offset 04
            u16 ElapseTime; // offset 06
            u16 TxPeriod; // offset 08
        } Scan; // offset 00
        struct {
            u16 Result; // offset 00
            u16 Status; // offset 02
        } Join; // offset 00
        struct {
            TXFRM* pTxFrm; // offset 00
        } DeAuth; // offset 00
        struct {
            TXFRM* pTxFrm; // offset 00
        } DisAss; // offset 00
        struct {
            u32 Counter; // offset 00
            u32 CCA; // offset 04
            u16 bkPowerMode; // offset 08
            u16 bkCCAMode; // offset 0a
            u16 bkEdTh; // offset 0c
            u16 bkMode; // offset 0e
            u16 Channel; // offset 10
            u16 sts; // offset 12
        } Measure; // offset 00
    } Work; // offset 04
    union {
        WlMlmeResetReq* Reset; // offset 00
        WlMlmePowerMgtReq* PwrMgt; // offset 00
        WlMlmeScanReq* Scan; // offset 00
        WlMlmeJoinReq* Join; // offset 00
        WlMlmeAuthReq* Auth; // offset 00
        WlMlmeDeAuthReq* DeAuth; // offset 00
        WlMlmeAssReq* Ass; // offset 00
        WlMlmeReAssReq* ReAss; // offset 00
        WlMlmeDisAssReq* DisAss; // offset 00
        WlMlmeStartReq* Start; // offset 00
        WlMlmeMeasChanReq* MeasChannel; // offset 00
    } pReq; // offset 18
    union {
        WlCmdCfm* Cfm; // offset 00
        WlMlmeResetCfm* Reset; // offset 00
        WlMlmePowerMgtCfm* PwrMgt; // offset 00
        WlMlmeScanCfm* Scan; // offset 00
        WlMlmeJoinCfm* Join; // offset 00
        WlMlmeAuthCfm* Auth; // offset 00
        WlMlmeDeAuthCfm* DeAuth; // offset 00
        WlMlmeAssCfm* Ass; // offset 00
        WlMlmeReAssCfm* ReAss; // offset 00
        WlMlmeDisAssCfm* DisAss; // offset 00
        WlMlmeStartCfm* Start; // offset 00
        WlMlmeMeasChanCfm* MeasChannel; // offset 00
    } pCfm; // offset 1c
} MLME_MAN;

u16 MLME_ResetReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MLME_PwrMgtReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MLME_ScanReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MLME_JoinReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MLME_AuthReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MLME_DeAuthReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MLME_AssReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MLME_ReAssReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MLME_DisAssReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MLME_StartReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MLME_MeasChanReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
void MLME_ScanTask();
void MLME_JoinTask();
void MLME_AuthTask();
void MLME_AssTask();
void MLME_ReAssTask();
void MLME_MeasChannelTask();
void MLME_BeaconLostTask();
void IssueMlmeConfirm();
u32 MLME_IssueAuthIndication(u16* pMacAdrs, u16 algorithm);
u32 MLME_IssueDeAuthIndication(u16* pMacAdrs, u16 reason);
u32 MLME_IssueAssIndication(u16* pMacAdrs, u16 aid, SSID_ELEMENT* pSSID);
u32 MLME_IssueReAssIndication(u16* pMacAdrs, u16 aid, SSID_ELEMENT* pSSID);
u32 MLME_IssueDisAssIndication(u16* pMacAdrs, u16 reason);
u32 MLME_IssueBeaconLostIndication(u16* pMacAdrs);
u32 MLME_IssueBeaconSendIndication();
u32 MLME_IssueBeaconRecvIndication(void* pRxFrm);
void InitializeMLME();

#endif

#endif