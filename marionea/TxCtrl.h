#include "Marionea.h"

#ifndef TX_CTRL_H
#define TX_CTRL_H

typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    DATA_HEADER Dot11Header; // offset 14
    union {
        u8 Body[4]; // offset 00
        u8* Pointer; // offset 00
    } Data; // offset 2c
} TXFRM;

typedef struct {
    MAC_HEADER MacHeader; // offset 00
    DATA_HEADER Dot11Header; // offset 0c
    u8 Body[4]; // offset 24
} TXFRM_MAC;

typedef struct {
    MAC_HEADER MacHeader; // offset 00
    DATA_HEADER Dot11Header; // offset 0c
    u16 TXOP; // offset 24
    u16 Bitmap; // offset 26
    u8 Body[4]; // offset 28
} TXMPFRM_MAC;

typedef struct {
    u16 Busy; // offset 00
    u16 InCount; // offset 02
    u16 OutCount; // offset 04
    u16 pad; // offset 06
    TXFRM_MAC* pMacFrm; // offset 08
    TXFRM* pFrm; // offset 0c
    void (*pEndFunc)(TXFRM*, u32); // offset 10
} TXQ;

typedef struct {
    TXQ Txq[3]; // offset 00
    TXQ Mp; // offset 3c
    TXQ Key[2]; // offset 50
    TXQ Beacon; // offset 78
    u32 Flag; // offset 8c
    WlMaMpEndInd* pMpEndInd; // offset 90
    u16 TXOP; // offset 94
    u16 TMPTT; // offset 96
    u16 SetKeyMap; // offset 98
    u16 GetKeyMap; // offset 9a
    u16 DataLength; // offset 9c
    u16 RetryLimit; // offset 9e
    u16 RestBitmap; // offset a0
    u16 BkKeyIn; // offset a2
    u16 BkKeyOut; // offset a4
    u16 BkSeqNum; // offset a6
    u16 MpBlkCnt; // offset a8
    u16 MpRstCnt; // offset aa
    u16 MpLastOk; // offset ac
    u16 TimeOutFrm; // offset ae
} TX_CTRL;

void TxqPri(u32 pri);
void CopyTxFrmToMacBuf(TXFRM_MAC* pMacTxFrm, WlMaDataReq* pTxReq);
u32 CheckFrameTimeout(TXFRM* pTxFrm);
void TxqEndData(TXFRM* pFrm, u32 flag);
void TxqEndManCtrl(TXFRM* pFrm, u32 flag);
void TxqEndPsPoll(TXFRM* pFrm, u32 flag);
void TxqEndBroadCast(TXFRM* pFrm, u32 flag);
void TxEndKeyData(TXQ* pTxq);
void ClearTxKeyData();
void ClearTxMp();
void ClearTxData();
void ClearQueuedPri(u32 pri);
void ResetTxqPri(u32 pri);
void DeleteTxFrames(u32 camAdrs);
void DeleteTxFrameByAdrs(u16* pMacAdrs);
void DeleteAllTxFrames();
void MessageDeleteTx(u32 pri, u32 bMsg);
void TxManCtrlFrame(TXFRM* pFrm);
void SetManCtrlFrame(TXFRM* pFrm);
void TxPsPollFrame();
void StartBeaconFrame();
void StopBeaconFrame();
void MakeBeaconFrame();
void UpdateGameInfoElement();
DISASS_FRAME* MakeDisAssFrame(u16* pDA, u16 reasonCode);
ASSREQ_FRAME* MakeAssReqFrame(u16* pDA);
REASSREQ_FRAME* MakeReAssReqFrame(u16* pDA);
ASSRES_FRAME* MakeAssResFrame(u16 camAdrs, u16 statusCode, SSID_ELEMENT* pSSID);
REASSRES_FRAME* MakeReAssResFrame(u16 camAdrs, u16 statusCode, SSID_ELEMENT* pSSID);
PRBREQ_FRAME* MakeProbeReqFrame(u16* pDA);
PRBRES_FRAME* MakeProbeResFrame(u16* pDA);
AUTH_FRAME* MakeAuthFrame(u16* pDA, u16 txtLen, u32 bCheck);
DEAUTH_FRAME* MakeDeAuthFrame(u16* pDA, u16 reasonCode, u32 bCheck);
void MakePsPollFrame(u16 aid);
void InitManHeader(TXFRM* pFrm, u16* pDA);
u32 IsExistManFrame(u16* pDA, u16 frameCtrl);
void InitTxCtrl();

#endif