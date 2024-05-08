#ifndef MARIONEA_H
#define MARIONEA_H

#ifdef MARIONEA_INTERNAL

#include "Registers.h"

// Useful macros
#define GET_TX_BUF_ADDR(macFrm) (((u32)(macFrm) & 0x3FFF) / 2)
#define GET_HEADER(ptr) (void *)((u8 *)(ptr) - (0x10))

#define wlMan (*(WL_MAN **)0x0380FFF4)

#define RING_END_PTR ((void *)0x04805F60)
#endif /* MARIONEA_INTERNAL */

#define GET_CFM(req) ((void *)((u32)req + ((u16 *)req)[7] * 2 + 0x10))

#define global_vtsf_var (*(u16 *)0x0380FFF0)

// Don't pack this one
#ifdef WIFI_USE_SDK_H
#include <sdk.h>
#else
#include "PublicSdk.h"
#endif

typedef struct
{
    u16 rssi;           // offset 00
    u16 channel;        // offset 02
    u16 capaInfo;       // offset 04
    u16 bssid[3];       // offset 06
    u16 ssidLength;     // offset 0c
    u8 ssid[32];        // offset 0e
    u16 beaconInterval; // offset 2e
    u16 lifeTime;       // offset 30
} WlApElement;

typedef struct
{
    u16 length;     // offset 00
    u8 rate;        // offset 02
    u8 rssi;        // offset 03
    u16 aid;        // offset 04
    u16 noResponse; // offset 06
    u8 cdata[4];    // offset 08
} WlMpKeyData;

typedef struct
{
    u16 bitmap;          // offset 00
    u16 errBitmap;       // offset 02
    u16 count;           // offset 04
    u16 length;          // offset 06
    u16 txCount;         // offset 08
    WlMpKeyData data[1]; // offset 0a
} WlMpKey;

typedef struct
{
    u16 length;     // offset 00
    u16 rssi;       // offset 02
    u16 bssid[3];   // offset 04
    u16 ssidLength; // offset 0a
    u8 ssid[32];    // offset 0c
    u16 capaInfo;   // offset 2c
    struct
    {
        u16 basic;         // offset 00
        u16 support;       // offset 02
    } rateSet;             // offset 2e
    u16 beaconPeriod;      // offset 32
    u16 dtimPeriod;        // offset 34
    u16 channel;           // offset 36
    u16 cfpPeriod;         // offset 38
    u16 cfpMaxDuration;    // offset 3a
    u16 gameInfoLength;    // offset 3c
    u16 otherElementCount; // offset 3e
    union
    {
        u8 gameInfo[4];     // offset 00
        u8 otherElement[4]; // offset 00
    };                      // offset 40
} WlBssDesc;

typedef struct
{
    u16 frameId;     // offset 00
    u8 rsv1[4];      // offset 02
    u16 length;      // offset 06
    u16 status;      // offset 08
    u16 rsvm2;       // offset 0a
    u16 timeStamp;   // offset 0c
    u8 rate;         // offset 0e
    u8 rssi;         // offset 0f
    u32 rsvm3;       // offset 10
    u8 rsv4[4];      // offset 14
    u16 destAdrs[3]; // offset 18
    u16 srcAdrs[3];  // offset 1e
    u8 rsv5[8];      // offset 24
    u8 data[4];      // offset 2c
} WlRxFrame;

typedef struct
{
    u8 rsv1[6];       // offset 00
    u16 length;       // offset 06
    u16 txKeySts;     // offset 08
    u16 ackTimeStamp; // offset 0a
    u16 timeStamp;    // offset 0c
    u8 rate;          // offset 0e
    u8 rssi;          // offset 0f
    u8 rsv3[4];       // offset 10
    u8 rsv4[4];       // offset 14
    u16 destAdrs[3];  // offset 18
    u16 srcAdrs[3];   // offset 1e
    u8 rsv5[6];       // offset 24
    u16 seqCtrl;      // offset 2a
    u16 txop;         // offset 2c
    u16 bitmap;       // offset 2e
    u16 data[1];      // offset 30
} WlRxMpFrame;

typedef struct
{
    u8 rsv1[6];      // offset 00
    u16 length;      // offset 06
    u16 txKeySts;    // offset 08
    u16 rsv3;        // offset 0a
    u16 timeStamp;   // offset 0c
    u8 rate;         // offset 0e
    u8 rssi;         // offset 0f
    u8 rsv4[4];      // offset 10
    u8 rsv5[4];      // offset 14
    u16 destAdrs[3]; // offset 18
    u16 srcAdrs[3];  // offset 1e
    u8 rsv6[6];      // offset 24
    u16 seqCtrl;     // offset 2a
    u16 tmptt;       // offset 2c
    u16 bitmap;      // offset 2e
} WlRxMpAckFrame;

typedef struct
{
    u16 frameId;     // offset 00
    u8 rsv1[4];      // offset 02
    u16 length;      // offset 06
    u16 status;      // offset 08
    u16 rsvm1;       // offset 0a
    u16 rsvm2;       // offset 0c
    u8 rate;         // offset 0e
    u8 rssi;         // offset 0f
    u32 rsvm3;       // offset 10
    u8 rsv4[4];      // offset 14
    u16 destAdrs[3]; // offset 18
    u16 srcAdrs[3];  // offset 1e
    u8 rsv5[8];      // offset 24
    u16 *datap;      // offset 2c
} WlTxFrame;

typedef struct
{
    u16 state;       // offset 00
    u16 aid;         // offset 02
    u16 macAdrs[3];  // offset 04
    u16 rssi;        // offset 0a
    u16 capaInfo;    // offset 0c
    u16 authSeed;    // offset 0e
    u16 supRateSet;  // offset 10
    u16 rsv;         // offset 12
    u16 lastSeqCtrl; // offset 14
    u16 frameCount;  // offset 16
    u16 lifeTime;    // offset 18
    u16 maxLifeTime; // offset 1a
} WlStaElement;

// Some other includes we need
#include "Frames.h"
#include "BufMan.h"
#include "WlCmdIf.h"
#include "MA.h"
#include "RxCtrl.h"
#include "TxCtrl.h"
#include "MLME.h"
#include "ApList.h"
#include "BufMan.h"
#include "CAM.h"
#include "Diag.h"
#include "Flash.h"
#include "ParamCmd.h"
#include "TaskMan.h"
#include "WaitLoop.h"
#include "WlIntr.h"
#include "WlIntrTask.h"
#include "WlNic.h"
#include "DevCmd.h"
#include "WlSys.h"

#endif