#include "Marionea.h"

#ifndef CAM_H
#define CAM_H

typedef struct {
    u16 state; // offset 00
    u16 aid; // offset 02
    u16 macAdrs[3]; // offset 04
    u16 rssi; // offset 0a
    u16 capaInfo; // offset 0c
    u16 authSeed; // offset 0e
    u16 supRateSet; // offset 10
    u16 rsv; // offset 12
    u16 lastSeqCtrl; // offset 14
    u16 frameCount; // offset 16
    u16 lifeTime; // offset 18
    u16 maxLifeTime; // offset 1a
} CAM_ELEMENT;

typedef struct {
    u16 Count; // offset 00
    u16 PowerMgtMode; // offset 02
    u16 PowerState; // offset 04
    u16 NotClass3; // offset 06
    u16 NotSetTIM; // offset 08
    u16 dmm; // offset 0a
    u16 ConnectSta; // offset 0c
    u16 UseAidMap; // offset 0e
} CAM_MAN;

u32 CAM_Search(u16* pMacAdrs);
u32 CAM_SearchAdd(u16* pMacAdrs);
void CAM_AddBcFrame(HEAPBUF_MAN* pBufMan, void* pBuf);
void CAM_IncFrameCount(TXFRM* pFrm);
void CAM_DecFrameCount(TXFRM* pFrm);
void CAM_SetStaState(u16 camAdrs, u16 state);
void CAM_SetRSSI(u16 camAdrs, u16 rssi);
void CAM_SetPowerMgtMode(u16 camAdrs, u16 pmtMode);
void CAM_SetDoze(u32 camAdrs);
void CAM_SetAwake(u32 camAdrs);
void CAM_SetCapaInfo(u32 camAdrs, u32 capInfo);
void CAM_SetSupRate(u32 camAdrs, u32 SupRate);
void CAM_SetLastSeqCtrl(u32 camAdrs, u32 seqCtrl);
void CAM_SetAuthSeed(u32 camAdrs, u32 seed);
void CAM_UpdateLifeTime(u32 camAdrs);
u32 CAM_AllocateAID(u16 camAdrs);
void CAM_ReleaseAID(u16 camAdrs);
u32 CAM_GetStaState(u32 camAdrs);
u32 CAM_IsActive(u32 camAdrs);
u32 CAM_GetPowerMgtMode(u32 camAdrs);
u16* CAM_GetMacAdrs(u32 camAdrs);
u32 CAM_GetAuthSeed(u32 camAdrs);
u32 CAM_GetLastSeqCtrl(u32 camAdrs);
u32 CAM_GetTxRate(u32 camAdrs);
u32 CAM_GetAID(u32 camAdrs);
u32 CAM_GetFrameCount(u32 camAdrs);
void CAM_SetTIMElementBitmap(u32 camAdrs);
void CAM_ClrTIMElementBitmap(u32 camAdrs);
void CAM_TimerTask();
void CAM_Delete(u16 camAdrs);
void InitializeCAM();
void InitCAM();

#endif