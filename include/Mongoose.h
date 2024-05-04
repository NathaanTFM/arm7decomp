#ifndef MONGOOSE_H
#define MONGOOSE_H

#define STATIC static

#ifdef WIFI_USE_SDK_H
#include <sdk.h>
#else
#include "PublicSdk.h"
#endif

#include "Registers.h"
#include "Marionea.h"
#include "Structures.h"
#include "Prototypes.h"

extern WMSPWork wmspW;

#define RSSI_UNK (*(u16 *)0x2FFFF98UL)

/* Functions defined in wmsp_private.h */

static inline u8 WMSP_GetRssi8(u8 rssi)
{ // wmsp_private.h:641
    if (rssi & 2)
    {
        return rssi >> 2;
    }
    else
    {
        return (rssi >> 2) + 25;
    }
}

static inline void WMSP_AddRssiToRandomPool(u8 rssi8)
{                             // wmsp_private.h:659
    u32 rssi_pool = RSSI_UNK; // r0 - :661
    rssi_pool = rssi8 ^ (rssi_pool << 1);
    rssi_pool = rssi_pool ^ (rssi_pool >> 16);
    RSSI_UNK = rssi_pool;

    // RSSI_UNK = rssi8 ^ (RSSI_UNK << 1) ^ ((rssi8 ^ ((u32)RSSI_UNK << 1)) >> 16);
}

#endif