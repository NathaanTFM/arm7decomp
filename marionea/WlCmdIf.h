#include "Marionea.h"

#ifndef WL_CMD_IF_H
#define WL_CMD_IF_H

#define MLME_REQ_CMD                            (0x000)
#define MLME_RESET_REQ_CMD                      (MLME_REQ_CMD + 0) // MLME_ResetReqCmd
#define MLME_PWR_MGT_REQ_CMD                    (MLME_REQ_CMD + 1) // MLME_PwrMgtReqCmd
#define MLME_SCAN_REQ_CMD                       (MLME_REQ_CMD + 2) // MLME_ScanReqCmd
#define MLME_JOIN_REQ_CMD                       (MLME_REQ_CMD + 3) // MLME_JoinReqCmd
#define MLME_AUTH_REQ_CMD                       (MLME_REQ_CMD + 4) // MLME_AuthReqCmd
#define MLME_DE_AUTH_REQ_CMD                    (MLME_REQ_CMD + 5) // MLME_DeAuthReqCmd
#define MLME_ASS_REQ_CMD                        (MLME_REQ_CMD + 6) // MLME_AssReqCmd
#define MLME_RE_ASS_REQ_CMD                     (MLME_REQ_CMD + 7) // MLME_ReAssReqCmd
#define MLME_DIS_ASS_REQ_CMD                    (MLME_REQ_CMD + 8) // MLME_DisAssReqCmd
#define MLME_START_REQ_CMD                      (MLME_REQ_CMD + 9) // MLME_StartReqCmd
#define MLME_MEAS_CHAN_REQ_CMD                  (MLME_REQ_CMD + 10) // MLME_MeasChanReqCmd

#define MLME_IND_CMD                            (0x080)
#define MLME_AUTH_IND_CMD                       (MLME_IND_CMD + 4)
#define MLME_DE_AUTH_IND_CMD                    (MLME_IND_CMD + 5)
#define MLME_ASS_IND_CMD                        (MLME_IND_CMD + 6)
#define MLME_RE_ASS_IND_CMD                     (MLME_IND_CMD + 7)
#define MLME_DIS_ASS_IND_CMD                    (MLME_IND_CMD + 8)
#define MLME_BCN_LOST_IND_CMD                   (MLME_IND_CMD + 11)
#define MLME_BCN_SEND_IND_CMD                   (MLME_IND_CMD + 12)
#define MLME_BCN_RECV_IND_CMD                   (MLME_IND_CMD + 13)

#define MA_REQ_CMD                              (0x100)
#define MA_DATA_REQ_CMD                         (MA_REQ_CMD + 0) // MA_DataReqCmd
#define MA_KEY_DATA_REQ_CMD                     (MA_REQ_CMD + 1) // MA_KeyDataReqCmd
#define MA_MP_REQ_CMD                           (MA_REQ_CMD + 2) // MA_MpReqCmd
#define MA_TEST_DATA_REQ_CMD                    (MA_REQ_CMD + 3) // MA_TestDataReqCmd
#define MA_CLR_DATA_REQ_CMD                     (MA_REQ_CMD + 4) // MA_ClrDataReqCmd

#define MA_IND_CMD                              (0x180)
#define MA_DATA_IND_CMD                         (MA_IND_CMD + 0) // WmspIndicateMaData
#define MA_MP_IND_CMD                           (MA_IND_CMD + 2) // WmspIndicateMaMultiPoll
#define MA_MP_END_IND_CMD                       (MA_IND_CMD + 4) // WmspIndicateMaMultiPollEnd
#define MA_MP_ACK_IND_CMD                       (MA_IND_CMD + 5) // WmspIndicateMaMultiPollAck
#define MA_FATAL_ERR_IND_CMD                    (MA_IND_CMD + 6) // WmspIndicateMaFatalErr

#define PARAMSET_REQ_CMD                        (0x200)
#define PARAMSET_ALL_REQ_CMD                    (PARAMSET_REQ_CMD + 0) // PARAMSET_AllReqCmd
#define PARAMSET_MAC_ADRS_REQ_CMD               (PARAMSET_REQ_CMD + 1) // PARAMSET_MacAdrsReqCmd
#define PARAMSET_RETRY_REQ_CMD                  (PARAMSET_REQ_CMD + 2) // PARAMSET_RetryReqCmd
#define PARAMSET_ENABLE_CHANNEL_REQ_CMD         (PARAMSET_REQ_CMD + 3) // PARAMSET_EnableChannelReqCmd
#define PARAMSET_MODE_REQ_CMD                   (PARAMSET_REQ_CMD + 4) // PARAMSET_ModeReqCmd
#define PARAMSET_RATE_REQ_CMD                   (PARAMSET_REQ_CMD + 5) // PARAMSET_RateReqCmd
#define PARAMSET_WEP_MODE_REQ_CMD               (PARAMSET_REQ_CMD + 6) // PARAMSET_WepModeReqCmd
#define PARAMSET_WEP_KEY_ID_REQ_CMD             (PARAMSET_REQ_CMD + 7) // PARAMSET_WepKeyIdReqCmd
#define PARAMSET_WEP_KEY_REQ_CMD                (PARAMSET_REQ_CMD + 8) // PARAMSET_WepKeyReqCmd
#define PARAMSET_BEACON_TYPE_REQ_CMD            (PARAMSET_REQ_CMD + 9) // PARAMSET_BeaconTypeReqCmd
#define PARAMSET_RES_BC_SSID_REQ_CMD            (PARAMSET_REQ_CMD + 10) // PARAMSET_ResBcSsidReqCmd
#define PARAMSET_BEACON_LOST_TH_REQ_CMD         (PARAMSET_REQ_CMD + 11) // PARAMSET_BeaconLostThReqCmd
#define PARAMSET_ACTIVE_ZONE_REQ_CMD            (PARAMSET_REQ_CMD + 12) // PARAMSET_ActiveZoneReqCmd
#define PARAMSET_SSID_MASK_REQ_CMD              (PARAMSET_REQ_CMD + 13) // PARAMSET_SSIDMaskReqCmd
#define PARAMSET_PREAMBLE_TYPE_REQ_CMD          (PARAMSET_REQ_CMD + 14) // PARAMSET_PreambleTypeReqCmd
#define PARAMSET_AUTH_ALGO_REQ_CMD              (PARAMSET_REQ_CMD + 15) // PARAMSET_AuthAlgoReqCmd
#define PARAMSET_CCA_MODE_ED_TH_REQ_CMD         (PARAMSET_REQ_CMD + 16) // PARAMSET_CCAModeEDThReqCmd
#define PARAMSET_LIFE_TIME_REQ_CMD              (PARAMSET_REQ_CMD + 17) // PARAMSET_LifeTimeReqCmd
#define PARAMSET_MAX_CONN_REQ_CMD               (PARAMSET_REQ_CMD + 18) // PARAMSET_MaxConnReqCmd
#define PARAMSET_MAIN_ANTENNA_REQ_CMD           (PARAMSET_REQ_CMD + 19) // PARAMSET_MainAntennaReqCmd
#define PARAMSET_DIVERSITY_REQ_CMD              (PARAMSET_REQ_CMD + 20) // PARAMSET_DiversityReqCmd
#define PARAMSET_BCN_SEND_RECV_IND_REQ_CMD      (PARAMSET_REQ_CMD + 21) // PARAMSET_BcnSendRecvIndReqCmd
#define PARAMSET_NULL_KEY_MODE_REQ_CMD          (PARAMSET_REQ_CMD + 22) // PARAMSET_NullKeyModeReqCmd
#define PARAMSET_MC_PASS_MODE_REQ_CMD           (PARAMSET_REQ_CMD + 23) // PARAMSET_McPassModeReqCmd

#define PARAMSET2_REQ_CMD                       (0x240)
#define PARAMSET2_BSSID_REQ_CMD                 (PARAMSET2_REQ_CMD + 0) // PARAMSET_BSSIDReqCmd
#define PARAMSET2_SSID_REQ_CMD                  (PARAMSET2_REQ_CMD + 1) // PARAMSET_SSIDReqCmd
#define PARAMSET2_BEACON_PERIOD_REQ_CMD         (PARAMSET2_REQ_CMD + 2) // PARAMSET_BeaconPeriodReqCmd
#define PARAMSET2_DTIM_PERIOD_REQ_CMD           (PARAMSET2_REQ_CMD + 3) // PARAMSET_DTIMPeriodReqCmd
#define PARAMSET2_LISTEN_INTERVAL_REQ_CMD       (PARAMSET2_REQ_CMD + 4) // PARAMSET_ListenIntervalReqCmd
#define PARAMSET2_GAME_INFO_REQ_CMD             (PARAMSET2_REQ_CMD + 5) // PARAMSET_GameInfoReqCmd

#define PARAMGET_REQ_CMD                        (0x280)
#define PARAMGET_ALL_REQ_CMD                    (PARAMGET_REQ_CMD + 0) // PARAMGET_AllReqCmd
#define PARAMGET_MAC_ADRS_REQ_CMD               (PARAMGET_REQ_CMD + 1) // PARAMGET_MacAdrsReqCmd
#define PARAMGET_RETRY_REQ_CMD                  (PARAMGET_REQ_CMD + 2) // PARAMGET_RetryReqCmd
#define PARAMGET_ENABLE_CHANNEL_REQ_CMD         (PARAMGET_REQ_CMD + 3) // PARAMGET_EnableChannelReqCmd
#define PARAMGET_MODE_REQ_CMD                   (PARAMGET_REQ_CMD + 4) // PARAMGET_ModeReqCmd
#define PARAMGET_RATE_REQ_CMD                   (PARAMGET_REQ_CMD + 5) // PARAMGET_RateReqCmd
#define PARAMGET_WEP_MODE_REQ_CMD               (PARAMGET_REQ_CMD + 6) // PARAMGET_WepModeReqCmd
#define PARAMGET_WEP_KEY_ID_REQ_CMD             (PARAMGET_REQ_CMD + 7) // PARAMGET_WepKeyIdReqCmd
#define PARAMGET_BEACON_TYPE_REQ_CMD            (PARAMGET_REQ_CMD + 9) // PARAMGET_BeaconTypeReqCmd
#define PARAMGET_RES_BC_SSID_REQ_CMD            (PARAMGET_REQ_CMD + 10) // PARAMGET_ResBcSsidReqCmd
#define PARAMGET_BEACON_LOST_TH_REQ_CMD         (PARAMGET_REQ_CMD + 11) // PARAMGET_BeaconLostThReqCmd
#define PARAMGET_ACTIVE_ZONE_REQ_CMD            (PARAMGET_REQ_CMD + 12) // PARAMGET_ActiveZoneReqCmd
#define PARAMGET_SSID_MASK_REQ_CMD              (PARAMGET_REQ_CMD + 13) // PARAMGET_SSIDMaskReqCmd
#define PARAMGET_PREAMBLE_TYPE_REQ_CMD          (PARAMGET_REQ_CMD + 14) // PARAMGET_PreambleTypeReqCmd
#define PARAMGET_AUTH_ALGO_REQ_CMD              (PARAMGET_REQ_CMD + 15) // PARAMGET_AuthAlgoReqCmd
#define PARAMGET_CCA_MODE_ED_TH_REQ_CMD         (PARAMGET_REQ_CMD + 16) // PARAMGET_CCAModeEDThReqCmd
#define PARAMGET_MAX_CONN_REQ_CMD               (PARAMGET_REQ_CMD + 18) // PARAMGET_MaxConnReqCmd
#define PARAMGET_MAIN_ANTENNA_REQ_CMD           (PARAMGET_REQ_CMD + 19) // PARAMGET_MainAntennaReqCmd
#define PARAMGET_DIVERSITY_REQ_CMD              (PARAMGET_REQ_CMD + 20) // PARAMGET_DiversityReqCmd
#define PARAMGET_BCN_SEND_RECV_IND_REQ_CMD      (PARAMGET_REQ_CMD + 21) // PARAMGET_BcnSendRecvIndReqCmd
#define PARAMGET_NULL_KEY_MODE_REQ_CMD          (PARAMGET_REQ_CMD + 22) // PARAMGET_NullKeyModeReqCmd
#define PARAMGET_MC_PASS_MODE_REQ_CMD           (PARAMGET_REQ_CMD + 23) // PARAMGET_McPassModeReqCmd

#define PARAMGET2_REQ_CMD                       (0x2C0)
#define PARAMGET2_BSSID_REQ_CMD                 (PARAMGET2_REQ_CMD + 0) // PARAMGET_BSSIDReqCmd
#define PARAMGET2_SSID_REQ_CMD                  (PARAMGET2_REQ_CMD + 1) // PARAMGET_SSIDReqCmd
#define PARAMGET2_BEACON_PERIOD_REQ_CMD         (PARAMGET2_REQ_CMD + 2) // PARAMGET_BeaconPeriodReqCmd
#define PARAMGET2_DTIM_PERIOD_REQ_CMD           (PARAMGET2_REQ_CMD + 3) // PARAMGET_DTIMPeriodReqCmd
#define PARAMGET2_LISTEN_INTERVAL_REQ_CMD       (PARAMGET2_REQ_CMD + 4) // PARAMGET_ListenIntervalReqCmd
#define PARAMGET2_GAME_INFO_REQ_CMD             (PARAMGET2_REQ_CMD + 5) // PARAMGET_GameInfoReqCmd

#define DEV_REQ_CMD                             (0x300)
#define DEV_SHUTDOWN_REQ_CMD                    (DEV_REQ_CMD + 1) // DEV_ShutdownReqCmd
#define DEV_IDLE_REQ_CMD                        (DEV_REQ_CMD + 2) // DEV_IdleReqCmd
#define DEV_CLASS1_REQ_CMD                      (DEV_REQ_CMD + 3) // DEV_Class1ReqCmd
#define DEV_REBOOT_REQ_CMD                      (DEV_REQ_CMD + 4) // DEV_RebootReqCmd
#define DEV_CLEAR_WL_INFO_REQ_CMD               (DEV_REQ_CMD + 5) // DEV_ClearWlInfoReqCmd
#define DEV_GET_VER_INFO_REQ_CMD                (DEV_REQ_CMD + 6) // DEV_GetVerInfoReqCmd
#define DEV_GET_WL_INFO_REQ_CMD                 (DEV_REQ_CMD + 7) // DEV_GetWlInfoReqCmd
#define DEV_GET_STATE_REQ_CMD                   (DEV_REQ_CMD + 8) // DEV_GetStateReqCmd
#define DEV_TEST_SIGNAL_REQ_CMD                 (DEV_REQ_CMD + 9) // DEV_TestSignalReqCmd
#define DEV_TEST_RX_REQ_CMD                     (DEV_REQ_CMD + 10) // DEV_TestRxReqCmd

typedef struct {
    u16 code; // offset 00
    u16 length; // offset 02
} WlCmdHeader;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 buf[2]; // offset 10
} WlCmdReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 buf[2]; // offset 06
} WlCmdCfm;

#ifdef MARIONEA_INTERNAL
typedef struct {
    u8* pCmd; // offset 00
    u16 Busy; // offset 04
    u16 pad; // offset 06
} CMDIF_MAN;

typedef struct {
    u16 RequestMinLength; // offset 00
    u16 ConfirmMinLength; // offset 02
    u16 (*pCmdFunc)(WlCmdReq*, WlCmdCfm*); // offset 04
} WLLIB_CMD_TBL;

void RequestCmdTask();
u16 CMD_ReservedReqCmd();
void SendMessageToWmDirect(HEAPBUF_MAN* pBufMan, void* pMsg);
void SendMessageToWmTask();
void InitializeCmdIf();
#endif

#endif