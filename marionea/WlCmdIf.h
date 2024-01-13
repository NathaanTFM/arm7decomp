#include "Marionea.h"

#ifndef WL_CMD_IF_H
#define WL_CMD_IF_H

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