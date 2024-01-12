#include "Marionea.h"

#ifndef MA_H
#define MA_H

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 errCode; // offset 10
} WlMaFatalErrInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    WlRxFrame frame; // offset 10
} WlMaDataInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    WlRxMpFrame frame; // offset 10
} WlMaMpInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    WlMpKey mpKey; // offset 10
} WlMaMpEndInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    WlRxMpAckFrame ack; // offset 10
} WlMaMpAckInd;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    WlTxFrame frame; // offset 10
} WlMaDataReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 txStatus; // offset 06
} WlMaDataCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 length; // offset 10
    u16 wmHeader; // offset 12
    u16* keyDatap; // offset 14
} WlMaKeyDataReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMaKeyDataCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 resume; // offset 10
    u16 retryLimit; // offset 12
    u16 txop; // offset 14
    u16 pollBitmap; // offset 16
    u16 tmptt; // offset 18
    u16 currTsf; // offset 1a
    u16 dataLength; // offset 1c
    u16 wmHeader; // offset 1e
    u16* datap; // offset 20
} WlMaMpReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMaMpCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 flag; // offset 10
} WlMaClrDataReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMaClrDataCfm;

u16 MA_DataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MA_KeyDataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MA_MpReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MA_TestDataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 MA_ClrDataReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
void IssueMaDataConfirm(HEAPBUF_MAN* pBufMan, void* pBuf);

#endif