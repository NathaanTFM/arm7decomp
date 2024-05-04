#ifndef MARIONEA_H
#define MARIONEA_H

#ifdef MARIONEA_INTERNAL

// Wifi I/O
#define WIFI_MEM (*(volatile u16 **)0x4800000UL)

// general purpose registers
#define REG_WIFIWAITCNT (*(volatile u16 *)0x4000206UL)

#define W_WEPKEY_0 (*(volatile u16 *)0x4805F80UL)
#define W_WEPKEY_1 (*(volatile u16 *)0x4805FA0UL)
#define W_WEPKEY_2 (*(volatile u16 *)0x4805FC0UL)
#define W_WEPKEY_3 (*(volatile u16 *)0x4805FE0UL)

// dunno about those
#define W_RX_MAC_0 (*(volatile u16 *)0x4805F70UL)
#define W_RX_MAC_1 (*(volatile u16 *)0x4805F72UL)
#define W_RX_MAC_2 (*(volatile u16 *)0x4805F74UL)
#define W_RX_SEQ (*(volatile u16 *)0x4805F76UL)
#define W_RX_UNK (*(volatile u16 *)0x4805F7EUL)

// 4808--- is a mirror of 4800---
#define WIFI_REG(offset) (*(volatile u16 *)(0x4808000UL + (offset)))

#define W_MACMEM(offset) (*(volatile u16 *)(0x4804000UL + (offset)))

#define W_ID WIFI_REG(0x000)
#define W_MODE_RST WIFI_REG(0x004)
#define W_MODE_WEP WIFI_REG(0x006)
#define W_TXSTATCNT WIFI_REG(0x008)
#define W_X_00Ah WIFI_REG(0x00A)
#define W_IF WIFI_REG(0x010)
#define W_IE WIFI_REG(0x012)
#define W_MACADDR_0 WIFI_REG(0x018)
#define W_MACADDR_1 WIFI_REG(0x01A)
#define W_MACADDR_2 WIFI_REG(0x01C)
#define W_BSSID_0 WIFI_REG(0x020)
#define W_BSSID_1 WIFI_REG(0x022)
#define W_BSSID_2 WIFI_REG(0x024)
#define W_AID_LOW WIFI_REG(0x028)
#define W_AID_FULL WIFI_REG(0x02A)
#define W_TX_RETRYLIMIT WIFI_REG(0x02C)
#define W_INTERNAL WIFI_REG(0x02E)
#define W_RXCNT WIFI_REG(0x030)
#define W_WEP_CNT WIFI_REG(0x032)
#define W_INTERNAL_0 WIFI_REG(0x034)
#define W_POWER_US WIFI_REG(0x036)
#define W_POWER_TX WIFI_REG(0x038)
#define W_POWERSTATE WIFI_REG(0x03C)
#define W_POWERFORCE WIFI_REG(0x040)
#define W_RANDOM WIFI_REG(0x044)
#define W_POWER_unk WIFI_REG(0x048)
#define W_RXBUF_BEGIN WIFI_REG(0x050)
#define W_RXBUF_END WIFI_REG(0x052)
#define W_RXBUF_WRCSR WIFI_REG(0x054)
#define W_RXBUF_WR_ADDR WIFI_REG(0x056)
#define W_RXBUF_RD_ADDR WIFI_REG(0x058)
#define W_RXBUF_READCSR WIFI_REG(0x05A)
#define W_RXBUF_COUNT WIFI_REG(0x05C)
#define W_RXBUF_RD_DATA WIFI_REG(0x060)
#define W_RXBUF_GAP WIFI_REG(0x062)
#define W_RXBUF_GAPDISP WIFI_REG(0x064)
#define W_TXBUF_WR_ADDR WIFI_REG(0x068)
#define W_TXBUF_COUNT WIFI_REG(0x06C)
#define W_TXBUF_WR_DATA WIFI_REG(0x070)
#define W_TXBUF_GAP WIFI_REG(0x074)
#define W_TXBUF_GAPDISP WIFI_REG(0x076)
#define W_INTERNAL_1 WIFI_REG(0x078)
#define W_TXBUF_BEACON WIFI_REG(0x080)
#define W_TXBUF_TIM WIFI_REG(0x084)
#define W_LISTENCOUNT WIFI_REG(0x088)
#define W_BEACONINT WIFI_REG(0x08C)
#define W_LISTENINT WIFI_REG(0x08E)
#define W_TXBUF_CMD WIFI_REG(0x090)
#define W_TXBUF_REPLY1 WIFI_REG(0x094)
#define W_TXBUF_REPLY2 WIFI_REG(0x098)
#define W_INTERNAL_2 WIFI_REG(0x09C)
#define W_TXBUF_LOC1 WIFI_REG(0x0A0)
#define W_TXBUF_LOC2 WIFI_REG(0x0A4)
#define W_TXBUF_LOC3 WIFI_REG(0x0A8)
#define W_TXREQ_RESET WIFI_REG(0x0AC)
#define W_TXREQ_SET WIFI_REG(0x0AE)
#define W_TXREQ_READ WIFI_REG(0x0B0)
#define W_TXBUF_RESET WIFI_REG(0x0B4)
#define W_TXBUSY WIFI_REG(0x0B6)
#define W_TXSTAT WIFI_REG(0x0B8)
#define W_INTERNAL_3 WIFI_REG(0x0BA)
#define W_PREAMBLE WIFI_REG(0x0BC)
#define W_CMD_TOTALTIME WIFI_REG(0x0C0)
#define W_CMD_REPLYTIME WIFI_REG(0x0C4)
#define W_INTERNAL_4 WIFI_REG(0x0C8)
#define W_RXFILTER WIFI_REG(0x0D0)
#define W_CONFIG_0D4h WIFI_REG(0x0D4)
#define W_CONFIG_0D8h WIFI_REG(0x0D8)
#define W_RX_LEN_CROP WIFI_REG(0x0DA)
#define W_RXFILTER2 WIFI_REG(0x0E0)
#define W_US_COUNTCNT WIFI_REG(0x0E8)
#define W_US_COMPARECNT WIFI_REG(0x0EA)
#define W_CONFIG_0ECh WIFI_REG(0x0EC)
#define W_CMD_COUNTCNT WIFI_REG(0x0EE)
#define W_US_COMPARE0 WIFI_REG(0x0F0)
#define W_US_COMPARE1 WIFI_REG(0x0F2)
#define W_US_COMPARE2 WIFI_REG(0x0F4)
#define W_US_COMPARE3 WIFI_REG(0x0F6)
#define W_US_COUNT0 WIFI_REG(0x0F8)
#define W_US_COUNT1 WIFI_REG(0x0FA)
#define W_US_COUNT2 WIFI_REG(0x0FC)
#define W_US_COUNT3 WIFI_REG(0x0FE)
#define W_INTERNAL_5 WIFI_REG(0x100)
#define W_INTERNAL_6 WIFI_REG(0x102)
#define W_INTERNAL_7 WIFI_REG(0x104)
#define W_INTERNAL_8 WIFI_REG(0x106)
#define W_CONTENTFREE WIFI_REG(0x10C)
#define W_PRE_BEACON WIFI_REG(0x110)
#define W_CMD_COUNT WIFI_REG(0x118)
#define W_BEACON_COUNT WIFI_REG(0x11C)
#define W_CONFIG_120h WIFI_REG(0x120)
#define W_CONFIG_122h WIFI_REG(0x122)
#define W_CONFIG_124h WIFI_REG(0x124)
#define W_INTERNAL_9 WIFI_REG(0x126)
#define W_CONFIG_128h WIFI_REG(0x128)
#define W_INTERNAL_10 WIFI_REG(0x12A)
#define W_CONFIG_130h WIFI_REG(0x130)
#define W_CONFIG_132h WIFI_REG(0x132)
#define W_POST_BEACON WIFI_REG(0x134)
#define W_CONFIG_140h WIFI_REG(0x140)
#define W_CONFIG_142h WIFI_REG(0x142)
#define W_CONFIG_144h WIFI_REG(0x144)
#define W_CONFIG_146h WIFI_REG(0x146)
#define W_CONFIG_148h WIFI_REG(0x148)
#define W_CONFIG_14Ah WIFI_REG(0x14A)
#define W_CONFIG_14Ch WIFI_REG(0x14C)
#define W_CONFIG_150h WIFI_REG(0x150)
#define W_CONFIG_154h WIFI_REG(0x154)
#define W_BB_CNT WIFI_REG(0x158)
#define W_BB_WRITE WIFI_REG(0x15A)
#define W_BB_READ WIFI_REG(0x15C)
#define W_BB_BUSY WIFI_REG(0x15E)
#define W_BB_MODE WIFI_REG(0x160)
#define W_BB_POWER WIFI_REG(0x168)
#define W_RF_DATA2 WIFI_REG(0x17C)
#define W_RF_DATA1 WIFI_REG(0x17E)
#define W_RF_BUSY WIFI_REG(0x180)
#define W_RF_CNT WIFI_REG(0x184)
#define W_TX_HDR_CNT WIFI_REG(0x194)
#define W_RF_PINS WIFI_REG(0x19C)
#define W_X_1A0h WIFI_REG(0x1A0)
#define W_X_1A2h WIFI_REG(0x1A2)
#define W_X_1A4h WIFI_REG(0x1A4)
#define W_RXSTAT_INC_IF WIFI_REG(0x1A8)
#define W_RXSTAT_INC_IE WIFI_REG(0x1AA)
#define W_RXSTAT_OVF_IF WIFI_REG(0x1AC)
#define W_RXSTAT_OVF_IE WIFI_REG(0x1AE)
#define W_RXSTAT WIFI_REG(0x1B0)
#define W_RXSTAT_0 WIFI_REG(0x1B2)
#define W_RXSTAT_1 WIFI_REG(0x1B4)
#define W_RXSTAT_2 WIFI_REG(0x1B6)
#define W_RXSTAT_3 WIFI_REG(0x1B8)
#define W_RXSTAT_4 WIFI_REG(0x1BA)
#define W_RXSTAT_5 WIFI_REG(0x1BC)
#define W_RXSTAT_6 WIFI_REG(0x1BE)
#define W_TX_ERR_COUNT WIFI_REG(0x1C0)
#define W_RX_COUNT WIFI_REG(0x1C4)
#define W_CMD_STAT WIFI_REG(0x1D0)
#define W_CMD_STAT_0 WIFI_REG(0x1D2)
#define W_CMD_STAT_1 WIFI_REG(0x1D4)
#define W_CMD_STAT_2 WIFI_REG(0x1D6)
#define W_CMD_STAT_3 WIFI_REG(0x1D8)
#define W_CMD_STAT_4 WIFI_REG(0x1DA)
#define W_CMD_STAT_5 WIFI_REG(0x1DC)
#define W_CMD_STAT_6 WIFI_REG(0x1DE)
#define W_TX_SEQNO WIFI_REG(0x210)
#define W_RF_STATUS WIFI_REG(0x214)
#define W_IF_SET WIFI_REG(0x21C)
#define W_RAM_DISABLE WIFI_REG(0x220)
#define W_X_228h WIFI_REG(0x228)
#define W_X_244h WIFI_REG(0x244)
#define W_INTERNAL_11 WIFI_REG(0x24C)
#define W_INTERNAL_12 WIFI_REG(0x24E)
#define W_INTERNAL_13 WIFI_REG(0x250)
#define W_CONFIG_254h WIFI_REG(0x254)
#define W_RXTX_ADDR WIFI_REG(0x268)
#define W_X_290h WIFI_REG(0x290)
#define W_INTERNAL_14 WIFI_REG(0x2B8)

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