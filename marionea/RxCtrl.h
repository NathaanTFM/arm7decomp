#include "Marionea.h"

#ifndef RX_CTRL_H
#define RX_CTRL_H

#ifdef MARIONEA_INTERNAL
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    DATA_HEADER Dot11Header; // offset 14
    u8 Body[4]; // offset 2c
} RXFRM;

typedef struct {
    MAC_HEADER MacHeader; // offset 00
    DATA_HEADER Dot11Header; // offset 0c
    u8 Body[4]; // offset 24
} RXFRM_MAC;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    RXFRM frame; // offset 10
} RXPACKET;

typedef struct {
    u16 DA[3]; // offset 00
    u16 SA[3]; // offset 06
    SEQ_CTRL SeqCtrl; // offset 0c
    u16 pad; // offset 0e
} DEFRAG_TBL;

typedef struct {
    u16 RestTime; // offset 00
    u16 UnitLength; // offset 02
    DEFRAG_TBL DefragTbl; // offset 04
    RXPACKET* pPacket; // offset 14
} DEFRAG_LIST;

typedef struct {
    u16 LastMpSeq; // offset 00
    u16 IcvOkCntFlag; // offset 02
    u16 wlCurr; // offset 04
    u16 TxKeyReg; // offset 06
    u16 TxKeyFrm; // offset 08
    u16 rsv; // offset 0a
    DEFRAG_LIST DefragList[3]; // offset 0c
} RX_CTRL;

typedef struct {
    u8* pElement; // offset 00
    u16 rxStatus; // offset 04
    u16 capability; // offset 06
    u16 bodyLength; // offset 08
    u16 matchFlag; // offset 0a
    u16 foundFlag; // offset 0c
    u16 activeZone; // offset 0e
    u16 vtsf; // offset 10
    u16 channel; // offset 12
    RATE_SET rateSet; // offset 14
    u16 otherElementCount; // offset 18
    u16 otherElementLength; // offset 1a
    SSID_ELEMENT* pSSID; // offset 1c
    CF_PARAM_ELEMENT* pCFP; // offset 20
    TIM_ELEMENT* pTIM; // offset 24
    GAME_INFO_ELEMENT* pGMIF; // offset 28
} ELEMENT_CHECKER;

void RxDataFrameTask();
u32 RxMpFrame(RXFRM* pFrm);
void RxKeyDataFrame(RXFRM* pFrm);
u32 RxMpAckFrame(RXFRM* pFrm);
void RxBeaconFrame(BEACON_FRAME* pFrm);
void RxManCtrlTask();
void DefragTask();
void DefragTimerTask();
void InitRxCtrl();
#endif

#endif