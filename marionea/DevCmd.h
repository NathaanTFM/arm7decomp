#include "Marionea.h"

#ifndef DEV_CMD_H
#define DEV_CMD_H

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlDevShutdownCfm;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlDevIdleCfm;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlDevClass1Cfm;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlDevRebootCfm;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlDevClrInfoCfm;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u8 wlVersion[8]; // offset 06
    u16 macVersion; // offset 0e
    u16 bbpVersion[2]; // offset 10
    u16 rfVersion; // offset 14
} WlDevGetVerInfoCfm;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 rsv1; // offset 06
    WlCounter counter; // offset 08
} WlDevGetInfoCfm;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 state; // offset 06
} WlDevGetStateCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 control; // offset 10
    u16 signal; // offset 12
    u16 rate; // offset 14
    u16 channel; // offset 16
} WlDevTestSignalReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlDevTestSignalCfm;

typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 control; // offset 10
    u16 channel; // offset 12
} WlDevTestRxReq;

typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlDevTestRxCfm;

u16 DEV_ShutdownReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 DEV_IdleReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 DEV_Class1ReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 DEV_RebootReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 DEV_ClearWlInfoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 DEV_GetVerInfoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 DEV_GetWlInfoReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 DEV_GetStateReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
u16 DEV_TestSignalReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);
void IntrCarrierSuppresionSignal();
u16 DEV_TestRxReqCmd(WlCmdReq* pReqt, WlCmdCfm* pCfmt);

#endif