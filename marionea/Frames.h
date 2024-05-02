#include "Marionea.h"

#ifndef FRAMES_H
#define FRAMES_H

#ifdef MARIONEA_INTERNAL
typedef struct
{
    u16 Basic;   // offset 00
    u16 Support; // offset 02
} RATE_SET;

typedef struct
{
    u16 FrameId;   // offset 00
    u16 CamAdrs;   // offset 02
    u16 FrameTime; // offset 04
    u16 Length;    // offset 06
} FIRM_HEADER;

typedef union
{
    struct
    {
        u16 Status;         // offset 00
        u16 Status2;        // offset 02
        u16 rsv_RetryCount; // offset 04
        u16 rsv_AppRate;    // offset 06
        u16 Service_Rate;   // offset 08
        u16 MPDU;           // offset 0a
    } Tx;                   // offset 00
    struct
    {
        u16 Status;       // offset 00
        u16 NextBnry;     // offset 02
        u16 TimeStamp;    // offset 04
        u16 Service_Rate; // offset 06
        u16 MPDU;         // offset 08
        u16 rsv_RSSI;     // offset 0a
    } Rx;                 // offset 00
} MAC_HEADER;

typedef union
{
    u16 Data; // offset 00
    struct
    {
        u16 Version : 2;  // offset 00
        u16 Type : 2;     // offset 00
        u16 SubType : 4;  // offset 00
        u16 ToDS : 1;     // offset 00
        u16 FromDS : 1;   // offset 00
        u16 MoreFrag : 1; // offset 00
        u16 Retry : 1;    // offset 00
        u16 PowerMan : 1; // offset 00
        u16 MoreData : 1; // offset 00
        u16 WEP : 1;      // offset 00
        u16 Order : 1;    // offset 00
    } Bit;                // offset 00
} FRAME_CTRL;

typedef union
{
    u16 Data; // offset 00
    struct
    {
        u16 FragNum : 4; // offset 00
        u16 SeqNum : 12; // offset 00
    } Bit;               // offset 00
} SEQ_CTRL;

typedef union
{
    u16 Data; // offset 00
    struct
    {
        u16 ESS : 1;           // offset 00
        u16 IBSS : 1;          // offset 00
        u16 CFPollable : 1;    // offset 00
        u16 CFPollRequest : 1; // offset 00
        u16 Privacy : 1;       // offset 00
        u16 ShortPreamble : 1; // offset 00
        u16 rsv6 : 1;          // offset 00
        u16 rsv7 : 1;          // offset 00
        u16 rsv8 : 1;          // offset 00
        u16 rsv9 : 1;          // offset 00
        u16 rsv10 : 1;         // offset 00
        u16 rsv11 : 1;         // offset 00
        u16 rsv12 : 1;         // offset 00
        u16 rsv13 : 1;         // offset 00
        u16 rsv14 : 1;         // offset 00
        u16 rsv15 : 1;         // offset 00
    } Bit;                     // offset 00
} CAPA_INFO;

typedef struct
{
    FRAME_CTRL FrameCtrl; // offset 00
    u16 DurationID;       // offset 02
    u16 Adrs1[3];         // offset 04
    u16 Adrs2[3];         // offset 0a
    u16 Adrs3[3];         // offset 10
    SEQ_CTRL SeqCtrl;     // offset 16
} DATA_HEADER;

typedef struct
{
    FRAME_CTRL FrameCtrl; // offset 00
    u16 Duration;         // offset 02
    u16 DA[3];            // offset 04
    u16 SA[3];            // offset 0a
    u16 BSSID[3];         // offset 10
    SEQ_CTRL SeqCtrl;     // offset 16
} MAN_HEADER;

typedef struct
{
    u8 ID;               // offset 00
    u8 Length;           // offset 01
    u8 DTIMCount;        // offset 02
    u8 DTIMPeriod;       // offset 03
    u8 BitmapCtrl;       // offset 04
    u8 VitrualBitmap[5]; // offset 05
} TIM_ELEMENT;

typedef struct
{
    u8 ID;          // offset 00
    u8 Length;      // offset 01
    u8 OUI[3];      // offset 02
    u8 SubType;     // offset 05
    u8 ActZone[2];  // offset 06
    u8 VTSF[2];     // offset 08
    u8 GameInfo[3]; // offset 0a
} GAME_INFO_ELEMENT;

typedef struct
{
    u8 ID;        // offset 00
    u8 Length;    // offset 01
    u8 Text[253]; // offset 02
} CHALLENGE_ELEMENT;

typedef struct
{
    u8 ID;               // offset 00
    u8 Length;           // offset 01
    u8 SupportedRate[2]; // offset 02
} SUP_RATE_ELEMENT;

typedef struct
{
    u8 ID;        // offset 00
    u8 Length;    // offset 01
    u8 CFPCount;  // offset 02
    u8 CFPPeriod; // offset 03
    union
    {
        u16 u16;      // offset 00
        u8 u8[2];     // offset 00
    } CFPMaxDuration; // offset 04
    union
    {
        u16 u16;    // offset 00
        u8 u8[2];   // offset 00
    } CFPDurRemain; // offset 06
} CF_PARAM_ELEMENT;

typedef struct
{
    u8 ID;       // offset 00
    u8 Length;   // offset 01
    u8 SSID[32]; // offset 02
} SSID_ELEMENT;

typedef struct
{
    u16 ReasonCode; // offset 00
} DEAUTH_BODY;

typedef struct
{
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader;   // offset 08
    MAN_HEADER Dot11Header; // offset 14
    DEAUTH_BODY Body;       // offset 2c
} DEAUTH_FRAME;

typedef struct
{
    u16 AlgoType;                    // offset 00
    u16 SeqNum;                      // offset 02
    u16 StatusCode;                  // offset 04
    CHALLENGE_ELEMENT ChallengeText; // offset 06
    u8 pad[1];                       // offset 105
} AUTH_BODY;

typedef struct
{
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader;   // offset 08
    MAN_HEADER Dot11Header; // offset 14
    AUTH_BODY Body;         // offset 2c
} AUTH_FRAME;

typedef struct
{
    CAPA_INFO CapaInfo; // offset 00
    u16 StatusCode;     // offset 02
    u16 AID;            // offset 04
    u8 Buf[4];          // offset 06
} ASSRES_BODY;

typedef struct
{
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader;   // offset 08
    MAN_HEADER Dot11Header; // offset 14
    ASSRES_BODY Body;       // offset 2c
} ASSRES_FRAME;

typedef struct
{
    u16 ReasonCode; // offset 00
} DISASS_BODY;

typedef struct
{
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader;   // offset 08
    MAN_HEADER Dot11Header; // offset 14
    DISASS_BODY Body;       // offset 2c
} DISASS_FRAME;

typedef struct
{
    CAPA_INFO CapaInfo; // offset 00
    u16 ListenInterval; // offset 02
    u8 Buf[38];         // offset 04
} ASSREQ_BODY;

typedef struct
{
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader;   // offset 08
    MAN_HEADER Dot11Header; // offset 14
    ASSREQ_BODY Body;       // offset 2c
} ASSREQ_FRAME;

typedef struct
{
    CAPA_INFO CapaInfo;   // offset 00
    u16 ListenInterval;   // offset 02
    u16 CurrAPMacAdrs[3]; // offset 04
    u8 Buf[38];           // offset 0a
} REASSREQ_BODY;

typedef struct
{
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader;   // offset 08
    MAN_HEADER Dot11Header; // offset 14
    REASSREQ_BODY Body;     // offset 2c
} REASSREQ_FRAME;

typedef struct
{
    CAPA_INFO CapaInfo; // offset 00
    u16 StatusCode;     // offset 02
    u16 AID;            // offset 04
    u8 Buf[4];          // offset 06
} REASSRES_BODY;

typedef struct
{
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader;   // offset 08
    MAN_HEADER Dot11Header; // offset 14
    REASSRES_BODY Body;     // offset 2c
} REASSRES_FRAME;

typedef struct
{
    u8 Buf[38]; // offset 00
} PRBREQ_BODY;

typedef struct
{
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader;   // offset 08
    MAN_HEADER Dot11Header; // offset 14
    PRBREQ_BODY Body;       // offset 2c
} PRBREQ_FRAME;

typedef struct
{
    u32 TimeStamp[2];   // offset 00
    u16 BeaconInterval; // offset 08
    CAPA_INFO CapaInfo; // offset 0a
    u8 Buf[54];         // offset 0c
    u8 pad[2];          // offset 42
} PRBRES_BODY;

typedef struct
{
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader;   // offset 08
    MAN_HEADER Dot11Header; // offset 14
    PRBRES_BODY Body;       // offset 2c
} PRBRES_FRAME;

typedef struct
{
    u32 TimeStamp[2];   // offset 00
    u16 BeaconInterval; // offset 08
    CAPA_INFO CapaInfo; // offset 0a
    u8 Buf[54];         // offset 0c
    u8 pad[2];          // offset 42
} BEACON_BODY;

typedef struct
{
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader;   // offset 08
    MAN_HEADER Dot11Header; // offset 14
    BEACON_BODY Body;       // offset 2c
} BEACON_FRAME;

typedef struct
{
    FRAME_CTRL FrameCtrl;      // offset 00
    unsigned short DurationID; // offset 02
    unsigned short BSSID[3];   // offset 04
    unsigned short TA[3];      // offset 0a
} PSPOLL_HEADER;

typedef struct
{
    FIRM_HEADER FirmHeader;    // offset 00
    MAC_HEADER MacHeader;      // offset 08
    PSPOLL_HEADER Dot11Header; // offset 14
} PSPOLL_FRAME;
#endif

#endif