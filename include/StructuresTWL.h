typedef enum WmspModule
{
    WMSP_MODULE_WM = 0,
    WMSP_MODULE_NWM = 1,
    WMSP_MODULE_NUM = 2,
} WmspModule;

typedef enum NWMPowerMode
{
    NWM_POWERMODE_ACTIVE = 0,
    NWM_POWERMODE_STANDARD = 1,
    NWM_POWERMODE_UAPSD = 2,
} NWMPowerMode;

typedef struct NwmspInit
{
    u32 dmaNo;    // offset 00
    u32 cmdPrio;  // offset 04
    u32 evtPrio;  // offset 08
    u32 sdioPrio; // offset 0c
    struct
    {
        OSArena id; // offset 00
        int handle; // offset 04
    } drvHeap;      // offset 10
    u32 wpaPrio;    // offset 18
    struct
    {
        OSArena id; // offset 00
        int handle; // offset 04
    } wpaHeap;      // offset 1c
} NwmspInit;

typedef struct NWMSPThread
{
    OSThread commandThread; // offset 00
    u32 commandStack[1024]; // offset a4
    OSThread eventThread;   // offset 10a4
    u32 eventStack[1024];   // offset 1148
} NWMSPThread;

typedef struct NWMSPQueue
{
    OSMessageQueue commandQ; // offset 00
    void *command[32];       // offset 20
    OSMessageQueue eventQ;   // offset a0
    void *event[32];         // offset c0
} NWMSPQueue;

typedef struct NWMFirmSection
{
    u8 *addr;         // offset 00
    u32 length;       // offset 04
    u8 *targetaddr;   // offset 08
    u32 isCompressed; // offset 0c
} NWMFirmSection;

typedef struct NWMStatus
{
    u8 apiActiveFlag[36]; // offset 00
    u16 state;            // offset 24
    u8 pad1[2];           // offset 26
    void *pDevice;        // offset 28
    u32 regDomain;        // offset 2c
    u32 hostVersion;      // offset 30
    u32 chipID;           // offset 34
    u32 romID;            // offset 38
    u32 fwVersion;        // offset 3c
    u8 macAddress[6];     // offset 40
    u16 wepKeySize;       // offset 46
    int wep_flag;         // offset 48
    u16 wepKeyId;         // offset 4c
    u16 wepKey[4][10];    // offset 4e
    u16 allowedChannel;   // offset 9e
    int roaming_flag;     // offset a0
    NWMPowerMode psMode;  // offset a4
} NWMStatus;

typedef struct NWMBinaryData
{
    NWMFirmSection firmware;    // offset 00
    NWMFirmSection datapatch;   // offset 10
    NWMFirmSection dataseg;     // offset 20
    NWMFirmSection eeptext;     // offset 30
    NWMFirmSection eepdata;     // offset 40
    NWMFirmSection rompatch[8]; // offset 50
    u32 hwIdNum;                // offset d0
    struct
    {
        u32 chipId;     // offset 00
        u32 romId;      // offset 04
    } hwDescriptor[16]; // offset d4
} NWMBinaryData;

typedef struct NWMArm7Buf
{
    u8 callbackSyncFlag; // offset 00
    u8 reserved_a[3];    // offset 01
    u32 *fifo7to9;       // offset 04
    u8 reserved_b[4];    // offset 08
    NWMStatus *status;   // offset 0c
} NWMArm7Buf;

typedef struct WPAConfig
{
    char ssid[32];  // offset 00
    int proto;      // offset 20
    int key_mgmt;   // offset 24
    int pairwise;   // offset 28
    int group;      // offset 2c
    int keymode;    // offset 30
    char key[63];   // offset 34
    char rsv[1];    // offset 73
    int wps_enable; // offset 74
    int wps_pbc;    // offset 78
} WPAConfig;

typedef struct NWMBssDesc
{
    u16 length;     // offset 00
    s16 rssi;       // offset 02
    u8 bssid[6];    // offset 04
    u16 ssidLength; // offset 0a
    u8 ssid[32];    // offset 0c
    u16 capaInfo;   // offset 2c
    struct
    {
        u16 basic;      // offset 00
        u16 support;    // offset 02
    } rateSet;          // offset 2e
    u16 beaconPeriod;   // offset 32
    u16 dtimPeriod;     // offset 34
    u16 channel;        // offset 36
    u16 cfpPeriod;      // offset 38
    u16 cfpMaxDuration; // offset 3a
    u16 reserved;       // offset 3c
    u16 elementCount;   // offset 3e
    u16 elements[1];    // offset 40
} NWMBssDesc;

typedef struct NWMScanInfo
{
    u16 bDscCount;           // offset 00
    u16 bDscSize;            // offset 02
    struct NWMBssDesc *bDsc; // offset 04
    u32 channelList;         // offset 08
    u8 scanType;             // offset 0c
    u8 isBssAny;             // offset 0d
    u8 ssid[32];             // offset 0e
    u16 ssidLength;          // offset 2e
    u32 ssidMatchLength;     // offset 30
} NWMScanInfo;

typedef struct NWMMsgQueue
{
    OSMessageQueue q; // offset 00
    void *fifo[64];   // offset 20
} NWMMsgQueue;

typedef struct NWMSPWork
{
    OSArena arenaId;                     // offset 00
    int heapHandle;                      // offset 04
    u32 sdioTskPrio;                     // offset 08
    u32 drvInitPrio;                     // offset 0c
    NWMSPThread th;                      // offset 10
    NWMSPQueue q;                        // offset 2158
    OSMutex fifoMtx;                     // offset 2298
    OSMutex apiFlagMtx;                  // offset 22b0
    u32 dmaNo;                           // offset 22c8
    NWMBinaryData *pBinData;             // offset 22cc
    NWMArm7Buf *nwm7buf;                 // offset 22d0
    NWMStatus *status;                   // offset 22d4
    void (*wpaCallbackTable[6])(void *); // offset 22d8
    int wpaEnable;                       // offset 22f0
    WPAConfig wpaCfg;                    // offset 22f4
    u64 tkipMicErrTime;                  // offset 2370
    u16 disconnectReason;                // offset 2378
    u8 apsdEnable;                       // offset 237a
    u8 pad1[1];                          // offset 237b
    u16 apsdServiceInt;                  // offset 237c
    u16 apsdTrgTimeout;                  // offset 237e
    NWMScanInfo scInfo;                  // offset 2380
    NWMMsgQueue sendCbQ;                 // offset 23b4
    NWMMsgQueue iCmdbufQ;                // offset 24d4
    u32 iCmdbuf[16][64];                 // offset 25f4
    NWMMsgQueue rxMsgbufQ;               // offset 35f4
    u32 rxMsgbuf[32][4];                 // offset 3714
    u32 nwmInitialized;                  // offset 3914
    NWMMsgQueue receiveBufQ[3];          // offset 3918
    u32 receivedCount;                   // offset 3c78
    u32 discardCount;                    // offset 3c7c
    u8 dummy[288];                       // offset 3c80
    signed char txLimit;                 // offset 3da0
    u8 pad2[3];                          // offset 3da1
} NWMSPWork;

typedef struct NWMCallback
{
    u16 apiid;   // offset 00
    u16 retcode; // offset 02
} NWMCallback;