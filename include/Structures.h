#pragma pack(1)

typedef enum WMErrCode
{
    WM_ERRCODE_SUCCESS = 0,
    WM_ERRCODE_FAILED = 1,
    WM_ERRCODE_OPERATING = 2,
    WM_ERRCODE_ILLEGAL_STATE = 3,
    WM_ERRCODE_WM_DISABLE = 4,
    WM_ERRCODE_NO_KEYSET = 5,
    WM_ERRCODE_NO_DATASET = 5,
    WM_ERRCODE_INVALID_PARAM = 6,
    WM_ERRCODE_NO_CHILD = 7,
    WM_ERRCODE_FIFO_ERROR = 8,
    WM_ERRCODE_TIMEOUT = 9,
    WM_ERRCODE_SEND_QUEUE_FULL = 10,
    WM_ERRCODE_NO_ENTRY = 11,
    WM_ERRCODE_OVER_MAX_ENTRY = 12,
    WM_ERRCODE_INVALID_POLLBITMAP = 13,
    WM_ERRCODE_NO_DATA = 14,
    WM_ERRCODE_SEND_FAILED = 15,
    WM_ERRCODE_DCF_TEST = 16,
    WM_ERRCODE_WL_INVALID_PARAM = 17,
    WM_ERRCODE_WL_LENGTH_ERR = 18,
    WM_ERRCODE_FLASH_ERROR = 19,
    WM_ERRCODE_MAX = 20,
} WMErrCode;

typedef enum WMApiid
{
    WM_APIID_INITIALIZE = 0,
    WM_APIID_RESET = 1,
    WM_APIID_END = 2,
    WM_APIID_ENABLE = 3,
    WM_APIID_DISABLE = 4,
    WM_APIID_POWER_ON = 5,
    WM_APIID_POWER_OFF = 6,
    WM_APIID_SET_P_PARAM = 7,
    WM_APIID_START_PARENT = 8,
    WM_APIID_END_PARENT = 9,
    WM_APIID_START_SCAN = 10,
    WM_APIID_END_SCAN = 11,
    WM_APIID_START_CONNECT = 12,
    WM_APIID_DISCONNECT = 13,
    WM_APIID_START_MP = 14,
    WM_APIID_SET_MP_DATA = 15,
    WM_APIID_END_MP = 16,
    WM_APIID_START_DCF = 17,
    WM_APIID_SET_DCF_DATA = 18,
    WM_APIID_END_DCF = 19,
    WM_APIID_SET_WEPKEY = 20,
    WM_APIID_START_KS = 21,
    WM_APIID_END_KS = 22,
    WM_APIID_GET_KEYSET = 23,
    WM_APIID_SET_GAMEINFO = 24,
    WM_APIID_SET_BEACON_IND = 25,
    WM_APIID_START_TESTMODE = 26,
    WM_APIID_STOP_TESTMODE = 27,
    WM_APIID_VALARM_MP = 28,
    WM_APIID_SET_LIFETIME = 29,
    WM_APIID_MEASURE_CHANNEL = 30,
    WM_APIID_INIT_W_COUNTER = 31,
    WM_APIID_GET_W_COUNTER = 32,
    WM_APIID_SET_ENTRY = 33,
    WM_APIID_AUTO_DEAUTH = 34,
    WM_APIID_SET_MP_PARAMETER = 35,
    WM_APIID_SET_BEACON_PERIOD = 36,
    WM_APIID_AUTO_DISCONNECT = 37,
    WM_APIID_START_SCAN_EX = 38,
    WM_APIID_SET_WEPKEY_EX = 39,
    WM_APIID_SET_PS_MODE = 40,
    WM_APIID_START_TESTRXMODE = 41,
    WM_APIID_STOP_TESTRXMODE = 42,
    WM_APIID_KICK_MP_PARENT = 43,
    WM_APIID_KICK_MP_CHILD = 44,
    WM_APIID_KICK_MP_RESUME = 45,
    WM_APIID_ASYNC_KIND_MAX = 46,
    WM_APIID_INDICATION = 128,
    WM_APIID_PORT_SEND = 129,
    WM_APIID_PORT_RECV = 130,
    WM_APIID_READ_STATUS = 131,
    WM_APIID_UNKNOWN = 255
} WMApiid;

typedef enum WMStateCode
{
    WM_STATECODE_PARENT_START = 0,
    WM_STATECODE_BEACON_SENT = 2,
    WM_STATECODE_SCAN_START = 3,
    WM_STATECODE_PARENT_NOT_FOUND = 4,
    WM_STATECODE_PARENT_FOUND = 5,
    WM_STATECODE_CONNECT_START = 6,
    WM_STATECODE_CONNECTED = 7,
    WM_STATECODE_CHILD_CONNECTED = 7,
    WM_STATECODE_BEACON_LOST = 8,
    WM_STATECODE_DISCONNECTED = 9,
    WM_STATECODE_MP_START = 10,
    WM_STATECODE_MPEND_IND = 11,
    WM_STATECODE_MP_IND = 12,
    WM_STATECODE_MPACK_IND = 13,
    WM_STATECODE_DCF_START = 14,
    WM_STATECODE_DCF_IND = 15,
    WM_STATECODE_BEACON_RECV = 16,
    WM_STATECODE_DISASSOCIATE = 17,
    WM_STATECODE_REASSOCIATE = 18,
    WM_STATECODE_AUTHENTICATE = 19,
    WM_STATECODE_PORT_SEND = 20,
    WM_STATECODE_PORT_RECV = 21,
    WM_STATECODE_FIFO_ERROR = 22,
    WM_STATECODE_INFORMATION = 23,
    WM_STATECODE_UNKNOWN = 24,
    WM_STATECODE_PORT_INIT = 25,
    WM_STATECODE_DISCONNECTED_FROM_MYSELF = 26,
    WM_STATECODE_MAX = 27
} WMStateCode;

typedef enum WMState
{
    WM_STATE_READY = 0,
    WM_STATE_STOP = 1,
    WM_STATE_IDLE = 2,
    WM_STATE_CLASS1 = 3,
    WM_STATE_TESTMODE = 4,
    WM_STATE_SCAN = 5,
    WM_STATE_CONNECT = 6,
    WM_STATE_PARENT = 7,
    WM_STATE_CHILD = 8,
    WM_STATE_MP_PARENT = 9,
    WM_STATE_MP_CHILD = 10,
    WM_STATE_DCF_CHILD = 11,
    WM_STATE_TESTMODE_RX = 12,
    WM_STATE_MAX = 13
} WMState;

typedef struct WmInit
{
    u32 dmaNo;        // offset 00
    u32 indPrio_low;  // offset 04
    u32 indPrio_high; // offset 08
    u32 reqPrio_low;  // offset 0c
    u32 reqPrio_high; // offset 10
    u32 wlPrio_low;   // offset 14
    u32 wlPrio_high;  // offset 18
} WmInit;

typedef struct WMCallback
{
    u16 apiid;    // offset 00
    u16 errcode;  // offset 02
    u16 wlCmdID;  // offset 04
    u16 wlResult; // offset 06
} WMCallback;

typedef struct WMGameInfo
{
    u16 magicNumber;       // offset 00
    u8 ver;                // offset 02
    u8 platform;           // offset 03
    u32 ggid;              // offset 04
    u16 tgid;              // offset 08
    u8 userGameInfoLength; // offset 0a
    union
    {
        u8 gameNameCount_attribute; // offset 00
        u8 attribute;               // offset 00
    };                              // offset 0b
    u16 parentMaxSize;              // offset 0c
    u16 childMaxSize;               // offset 0e
    union
    {
        u16 userGameInfo[56]; // offset 00
        struct
        {
            u16 userName[4]; // offset 00
            u16 gameName[8]; // offset 08
            u16 padd1[44];   // offset 18
        } old_type;          // offset 00
    };                       // offset 10
} WMGameInfo;

typedef struct WMBssDesc
{
    u16 length;     // offset 00
    u16 rssi;       // offset 02
    u8 bssid[6];    // offset 04
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
    WMGameInfo gameInfo;   // offset 40
} WMBssDesc;

typedef struct WMParentParam
{
    u16 *userGameInfo;      // offset 00
    u16 userGameInfoLength; // offset 04
    u16 padding;            // offset 06
    u32 ggid;               // offset 08
    u16 tgid;               // offset 0c
    u16 entryFlag;          // offset 0e
    u16 maxEntry;           // offset 10
    u16 multiBootFlag;      // offset 12
    u16 KS_Flag;            // offset 14
    u16 CS_Flag;            // offset 16
    u16 beaconPeriod;       // offset 18
    u16 rsv1[4];            // offset 1a
    u16 rsv2[8];            // offset 22
    u16 channel;            // offset 32
    u16 parentMaxSize;      // offset 34
    u16 childMaxSize;       // offset 36
    u16 rsv[4];             // offset 38
} WMParentParam;

typedef struct WMMpRecvBuf
{
    u16 rsv1[3];      // offset 00
    u16 length;       // offset 06
    u16 rsv2[1];      // offset 08
    u16 ackTimeStamp; // offset 0a
    u16 timeStamp;    // offset 0c
    u16 rate_rssi;    // offset 0e
    u16 rsv3[2];      // offset 10
    u16 rsv4[2];      // offset 14
    u8 destAdrs[6];   // offset 18
    u8 srcAdrs[6];    // offset 1e
    u16 rsv5[3];      // offset 24
    u16 seqCtrl;      // offset 2a
    u16 txop;         // offset 2c
    u16 bitmap;       // offset 2e
    u16 wmHeader;     // offset 30
    u16 data[2];      // offset 32
} WMMpRecvBuf;

typedef struct WMPortSendQueue
{
    u16 head; // offset 00
    u16 tail; // offset 02
} WMPortSendQueue;

typedef struct WMPortSendQueueData
{
    u16 next;                 // offset 00
    u16 port;                 // offset 02
    u16 destBitmap;           // offset 04
    u16 restBitmap;           // offset 06
    u16 sentBitmap;           // offset 08
    u16 sendingBitmap;        // offset 0a
    u16 padding;              // offset 0c
    u16 size;                 // offset 0e
    u16 seqNo;                // offset 10
    u16 retryCount;           // offset 12
    u16 *data;                // offset 14
    void (*callback)(void *); // offset 18
    void *arg;                // offset 1c
} WMPortSendQueueData;

typedef struct WMDcfRecvBuf
{
    u16 frameID;    // offset 00
    u16 rsv1[2];    // offset 02
    u16 length;     // offset 06
    u16 rsv2[3];    // offset 08
    u16 rate_rssi;  // offset 0e
    u16 rsv3[4];    // offset 10
    u8 destAdrs[6]; // offset 18
    u8 srcAdrs[6];  // offset 1e
    u16 rsv4[4];    // offset 24
    u16 data[2];    // offset 2c
} WMDcfRecvBuf;

typedef struct WMStatus
{
    u16 state;                             // offset 00
    u16 BusyApiid;                         // offset 02
    int apiBusy;                           // offset 04
    int scan_continue;                     // offset 08
    int mp_flag;                           // offset 0c
    int dcf_flag;                          // offset 10
    int ks_flag;                           // offset 14
    int dcf_sendFlag;                      // offset 18
    int VSyncFlag;                         // offset 1c
    u8 wlVersion[8];                       // offset 20
    u16 macVersion;                        // offset 28
    u16 rfVersion;                         // offset 2a
    u16 bbpVersion[2];                     // offset 2c
    u16 mp_parentSize;                     // offset 30
    u16 mp_childSize;                      // offset 32
    u16 mp_parentMaxSize;                  // offset 34
    u16 mp_childMaxSize;                   // offset 36
    u16 mp_sendSize;                       // offset 38
    u16 mp_recvSize;                       // offset 3a
    u16 mp_maxSendSize;                    // offset 3c
    u16 mp_maxRecvSize;                    // offset 3e
    u16 mp_parentVCount;                   // offset 40
    u16 mp_childVCount;                    // offset 42
    u16 mp_parentInterval;                 // offset 44
    u16 mp_childInterval;                  // offset 46
    u64 mp_parentIntervalTick;             // offset 48
    u64 mp_childIntervalTick;              // offset 50
    u16 mp_minFreq;                        // offset 58
    u16 mp_freq;                           // offset 5a
    u16 mp_maxFreq;                        // offset 5c
    u16 mp_vsyncOrderedFlag;               // offset 5e
    u16 mp_vsyncFlag;                      // offset 60
    s16 mp_count;                          // offset 62
    s16 mp_limitCount;                     // offset 64
    u16 mp_resumeFlag;                     // offset 66
    u16 mp_prevPollBitmap;                 // offset 68
    u16 mp_prevWmHeader;                   // offset 6a
    u16 mp_prevTxop;                       // offset 6c
    u16 mp_prevDataLength;                 // offset 6e
    u16 mp_recvBufSel;                     // offset 70
    u16 mp_recvBufSize;                    // offset 72
    WMMpRecvBuf *mp_recvBuf[2];            // offset 74
    u32 *mp_sendBuf;                       // offset 7c
    u16 mp_sendBufSize;                    // offset 80
    u16 mp_ackTime;                        // offset 82
    u16 mp_waitAckFlag;                    // offset 84
    u16 mp_readyBitmap;                    // offset 86
    u16 mp_newFrameFlag;                   // offset 88
    u8 reserved_b[2];                      // offset 8a
    u16 mp_sentDataFlag;                   // offset 8c
    u16 mp_bufferEmptyFlag;                // offset 8e
    u16 mp_isPolledFlag;                   // offset 90
    u16 mp_minPollBmpMode;                 // offset 92
    u16 mp_singlePacketMode;               // offset 94
    u8 reserved_c[2];                      // offset 96
    u16 mp_defaultRetryCount;              // offset 98
    u16 mp_ignoreFatalErrorMode;           // offset 9a
    u16 mp_ignoreSizePrecheckMode;         // offset 9c
    u16 mp_pingFlag;                       // offset 9e
    u16 mp_pingCounter;                    // offset a0
    u8 dcf_destAdr[6];                     // offset a2
    u16 *dcf_sendData;                     // offset a8
    u16 dcf_sendSize;                      // offset ac
    u16 dcf_recvBufSel;                    // offset ae
    WMDcfRecvBuf *dcf_recvBuf[2];          // offset b0
    u16 dcf_recvBufSize;                   // offset b8
    u16 curr_tgid;                         // offset ba
    u16 linkLevel;                         // offset bc
    u16 minRssi;                           // offset be
    u16 rssiCounter;                       // offset c0
    u16 beaconIndicateFlag;                // offset c2
    u16 wepKeyId;                          // offset c4
    u16 pwrMgtMode;                        // offset c6
    u32 miscFlags;                         // offset c8
    u16 VSyncBitmap;                       // offset cc
    u16 valarm_queuedFlag;                 // offset ce
    u32 v_tsf;                             // offset d0
    u32 v_tsf_bak;                         // offset d4
    u32 v_remain;                          // offset d8
    u16 valarm_counter;                    // offset dc
    u8 reserved_e[2];                      // offset de
    u8 MacAddress[6];                      // offset e0
    u16 mode;                              // offset e6
    WMParentParam pparam;                  // offset e8
    u8 childMacAddress[15][6];             // offset 128
    u16 child_bitmap;                      // offset 182
    WMBssDesc *pInfoBuf;                   // offset 184
    u16 aid;                               // offset 188
    u8 parentMacAddress[6];                // offset 18a
    u16 scan_channel;                      // offset 190
    u8 reserved_f[4];                      // offset 192
    u16 wepMode;                           // offset 196
    int wep_flag;                          // offset 198
    u16 wepKey[40];                        // offset 19c
    u16 rate;                              // offset 1ec
    u16 preamble;                          // offset 1ee
    u16 tmptt;                             // offset 1f0
    u16 retryLimit;                        // offset 1f2
    u16 enableChannel;                     // offset 1f4
    u16 allowedChannel;                    // offset 1f6
    u16 portSeqNo[16][8];                  // offset 1f8
    WMPortSendQueueData sendQueueData[32]; // offset 2f8
    WMPortSendQueue sendQueueFreeList;     // offset 6f8
    WMPortSendQueue sendQueue[4];          // offset 6fc
    WMPortSendQueue readyQueue[4];         // offset 70c
    OSMutex sendQueueMutex;                // offset 71c
    int sendQueueInUse;                    // offset 734
    u64 mp_lastRecvTick[16];               // offset 738
    u64 mp_lifeTimeTick;                   // offset 7b8
    u16 mp_current_minFreq;                // offset 7c0
    u16 mp_current_freq;                   // offset 7c2
    u16 mp_current_maxFreq;                // offset 7c4
    u16 mp_current_minPollBmpMode;         // offset 7c6
    u16 mp_current_singlePacketMode;       // offset 7c8
    u16 mp_current_defaultRetryCount;      // offset 7ca
    u16 mp_current_ignoreFatalErrorMode;   // offset 7cc
    u8 reserved_g[2];                      // offset 7ce
} WMStatus;

typedef struct WMArm7Buf
{
    WMStatus *status;       // offset 00
    u8 reserved_a[4];       // offset 04
    u32 *fifo7to9;          // offset 08
    u8 reserved_b[4];       // offset 0c
    WMBssDesc connectPInfo; // offset 10
    u32 requestBuf[128];    // offset d0
} WMArm7Buf;

typedef struct WMSPWork
{
    OSMessageQueue toWLmsgQ;   // offset 00
    void *toWLmsg[2];          // offset 20
    OSMessageQueue fromWLmsgQ; // offset 28
    void *fromWLmsg[4];        // offset 48
    OSMessageQueue confirmQ;   // offset 58
    void *confirm[4];          // offset 78
    OSMessageQueue requestQ;   // offset 88
    void *request[32];         // offset a8
    u32 requestStack[1024];    // offset 128
    u32 indicateStack[256];    // offset 1128
    OSMutex fifoExclusive;     // offset 1528
    u32 dmaNo;                 // offset 1540
    OSArena arenaId;           // offset 1544
    int heapHandle;            // offset 1548
    WMArm7Buf *wm7buf;         // offset 154c
    WMStatus *status;          // offset 1550
    u8 rssiHistory[32];        // offset 1554
    u32 rssiIndex;             // offset 1574
    u32 indPrio_high;          // offset 1578
    u32 wlPrio_high;           // offset 157c
    u32 reqPrio_high;          // offset 1580
    u32 indPrio_low;           // offset 1584
    u32 wlPrio_low;            // offset 1588
    u32 reqPrio_low;           // offset 158c
} WMSPWork;

typedef struct WMIndCallback
{
    u16 apiid;   // offset 00
    u16 errcode; // offset 02
    u16 state;   // offset 04
    u16 reason;  // offset 06
} WMIndCallback;

typedef struct WMMpRecvData
{
    u16 length;     // offset 00
    u16 rate_rssi;  // offset 02
    u16 aid;        // offset 04
    u16 noResponse; // offset 06
    u16 wmHeader;   // offset 08
    u16 cdata[1];   // offset 0a
} WMMpRecvData;

typedef struct WMMpRecvHeader
{
    u16 bitmap;           // offset 00
    u16 errBitmap;        // offset 02
    u16 count;            // offset 04
    u16 length;           // offset 06
    u16 txCount;          // offset 08
    WMMpRecvData data[1]; // offset 0a
} WMMpRecvHeader;

typedef struct WMStartParentCallback
{
    u16 apiid;        // offset 00
    u16 errcode;      // offset 02
    u16 wlCmdID;      // offset 04
    u16 wlResult;     // offset 06
    u16 state;        // offset 08
    u8 macAddress[6]; // offset 0a
    u16 aid;          // offset 10
    u16 reason;       // offset 12
    u8 ssid[24];      // offset 14
    u16 parentSize;   // offset 2c
    u16 childSize;    // offset 2e
} WMStartParentCallback;

typedef struct WMStartScanCallback
{
    u16 apiid;           // offset 00
    u16 errcode;         // offset 02
    u16 wlCmdID;         // offset 04
    u16 wlResult;        // offset 06
    u16 state;           // offset 08
    u8 macAddress[6];    // offset 0a
    u16 channel;         // offset 10
    u16 linkLevel;       // offset 12
    u16 ssidLength;      // offset 14
    u16 ssid[16];        // offset 16
    u16 gameInfoLength;  // offset 36
    WMGameInfo gameInfo; // offset 38
} WMStartScanCallback;

typedef struct WMStartScanReq
{
    u16 apiid;          // offset 00
    u16 channel;        // offset 02
    WMBssDesc *scanBuf; // offset 04
    u16 maxChannelTime; // offset 08
    u8 bssid[6];        // offset 0a
} WMStartScanReq;

typedef struct WMStartScanExCallback
{
    u16 apiid;              // offset 00
    u16 errcode;            // offset 02
    u16 wlCmdID;            // offset 04
    u16 wlResult;           // offset 06
    u16 state;              // offset 08
    u16 channelList;        // offset 0a
    u8 reserved[2];         // offset 0c
    u16 bssDescCount;       // offset 0e
    WMBssDesc *bssDesc[16]; // offset 10
    u16 linkLevel[16];      // offset 50
} WMStartScanExCallback;

typedef struct WMStartScanExReq
{
    u16 apiid;           // offset 00
    u16 channelList;     // offset 02
    WMBssDesc *scanBuf;  // offset 04
    u16 scanBufSize;     // offset 08
    u16 maxChannelTime;  // offset 0a
    u8 bssid[6];         // offset 0c
    u16 scanType;        // offset 12
    u16 ssidLength;      // offset 14
    u8 ssid[32];         // offset 16
    u16 ssidMatchLength; // offset 36
    u16 rsv[2];          // offset 38
} WMStartScanExReq;

typedef struct WMStartConnectCallback
{
    u16 apiid;        // offset 00
    u16 errcode;      // offset 02
    u16 wlCmdID;      // offset 04
    u16 wlResult;     // offset 06
    u16 state;        // offset 08
    u16 aid;          // offset 0a
    u16 reason;       // offset 0c
    u16 wlStatus;     // offset 0e
    u8 macAddress[6]; // offset 10
    u16 parentSize;   // offset 16
    u16 childSize;    // offset 18
} WMStartConnectCallback;

typedef struct WMStartConnectReq
{
    u16 apiid;        // offset 00
    u16 reserved;     // offset 02
    WMBssDesc *pInfo; // offset 04
    u8 ssid[24];      // offset 08
    int powerSave;    // offset 20
    u16 reserved2;    // offset 24
    u16 authMode;     // offset 26
} WMStartConnectReq;

typedef struct WMDisconnectCallback
{
    u16 apiid;              // offset 00
    u16 errcode;            // offset 02
    u16 wlCmdID;            // offset 04
    u16 wlResult;           // offset 06
    u16 tryBitmap;          // offset 08
    u16 disconnectedBitmap; // offset 0a
} WMDisconnectCallback;

typedef struct WMStartMPCallback
{
    u16 apiid;            // offset 00
    u16 errcode;          // offset 02
    u16 state;            // offset 04
    u8 reserved[2];       // offset 06
    WMMpRecvBuf *recvBuf; // offset 08
    u16 timeStamp;        // offset 0c
    u16 rate_rssi;        // offset 0e
    u8 destAdrs[6];       // offset 10
    u8 srcAdrs[6];        // offset 16
    u16 seqNum;           // offset 1c
    u16 tmptt;            // offset 1e
    u16 pollbmp;          // offset 20
    u16 reserved2;        // offset 22
} WMStartMPCallback;

typedef struct WMMPTmpParam
{
    u32 mask;                // offset 00
    u16 minFrequency;        // offset 04
    u16 frequency;           // offset 06
    u16 maxFrequency;        // offset 08
    u16 defaultRetryCount;   // offset 0a
    u8 minPollBmpMode;       // offset 0c
    u8 singlePacketMode;     // offset 0d
    u8 ignoreFatalErrorMode; // offset 0e
    u8 reserved[1];          // offset 0f
} WMMPTmpParam;

typedef struct WMMPParam
{
    u32 mask;                  // offset 00
    u16 minFrequency;          // offset 04
    u16 frequency;             // offset 06
    u16 maxFrequency;          // offset 08
    u16 parentSize;            // offset 0a
    u16 childSize;             // offset 0c
    u16 parentInterval;        // offset 0e
    u16 childInterval;         // offset 10
    u16 parentVCount;          // offset 12
    u16 childVCount;           // offset 14
    u16 defaultRetryCount;     // offset 16
    u8 minPollBmpMode;         // offset 18
    u8 singlePacketMode;       // offset 19
    u8 ignoreFatalErrorMode;   // offset 1a
    u8 ignoreSizePrecheckMode; // offset 1b
} WMMPParam;

typedef struct WMStartMPReq
{
    u16 apiid;             // offset 00
    u16 rsv1;              // offset 02
    u32 *recvBuf;          // offset 04
    u32 recvBufSize;       // offset 08
    u32 *sendBuf;          // offset 0c
    u32 sendBufSize;       // offset 10
    WMMPParam param;       // offset 14
    WMMPTmpParam tmpParam; // offset 30
} WMStartMPReq;

typedef struct WMPortSendCallback
{
    u16 apiid;      // offset 00
    u16 errcode;    // offset 02
    u16 wlCmdID;    // offset 04
    u16 wlResult;   // offset 06
    u16 state;      // offset 08
    u16 port;       // offset 0a
    u16 destBitmap; // offset 0c
    u16 restBitmap; // offset 0e
    u16 sentBitmap; // offset 10
    u16 rsv;        // offset 12
    u16 *data;      // offset 14
    union
    {
        u16 size;             // offset 00
        u16 length;           // offset 00
    };                        // offset 18
    u16 seqNo;                // offset 1a
    void (*callback)(void *); // offset 1c
    void *arg;                // offset 20
    u16 maxSendDataSize;      // offset 24
    u16 maxRecvDataSize;      // offset 26
} WMPortSendCallback;

typedef struct WMStartDCFCallback
{
    u16 apiid;             // offset 00
    u16 errcode;           // offset 02
    u16 state;             // offset 04
    u8 reserved[2];        // offset 06
    WMDcfRecvBuf *recvBuf; // offset 08
} WMStartDCFCallback;

typedef struct WMStartTestModeCallback
{
    u16 apiid;        // offset 00
    u16 errcode;      // offset 02
    u32 RFadr5;       // offset 04
    u32 RFadr6;       // offset 08
    u16 PllLockCheck; // offset 0c
    u16 RFMDflag;     // offset 0e
} WMStartTestModeCallback;

typedef struct WMMeasureChannelCallback
{
    u16 apiid;        // offset 00
    u16 errcode;      // offset 02
    u16 wlCmdID;      // offset 04
    u16 wlResult;     // offset 06
    u16 channel;      // offset 08
    u16 ccaBusyRatio; // offset 0a
} WMMeasureChannelCallback;

typedef struct WMMeasureChannelReq
{
    u16 apiid;       // offset 00
    u16 ccaMode;     // offset 02
    u16 edThreshold; // offset 04
    u16 channel;     // offset 06
    u16 measureTime; // offset 08
} WMMeasureChannelReq;

typedef struct WMGetWirelessCounterCallback
{
    u16 apiid;                  // offset 00
    u16 errcode;                // offset 02
    u16 wlCmdID;                // offset 04
    u16 wlResult;               // offset 06
    u32 TX_Success;             // offset 08
    u32 TX_Failed;              // offset 0c
    u32 TX_Retry;               // offset 10
    u32 TX_AckError;            // offset 14
    u32 TX_Unicast;             // offset 18
    u32 TX_Multicast;           // offset 1c
    u32 TX_WEP;                 // offset 20
    u32 TX_Beacon;              // offset 24
    u32 RX_RTS;                 // offset 28
    u32 RX_Fragment;            // offset 2c
    u32 RX_Unicast;             // offset 30
    u32 RX_Multicast;           // offset 34
    u32 RX_WEP;                 // offset 38
    u32 RX_Beacon;              // offset 3c
    u32 RX_FCSError;            // offset 40
    u32 RX_DuplicateError;      // offset 44
    u32 RX_MPDuplicateError;    // offset 48
    u32 RX_ICVError;            // offset 4c
    u32 RX_FrameCtrlError;      // offset 50
    u32 RX_LengthError;         // offset 54
    u32 RX_PLCPError;           // offset 58
    u32 RX_BufferOverflowError; // offset 5c
    u32 RX_PathError;           // offset 60
    u32 RX_RateError;           // offset 64
    u32 RX_FCSOK;               // offset 68
    u32 TX_MP;                  // offset 6c
    u32 TX_KeyData;             // offset 70
    u32 TX_NullKey;             // offset 74
    u32 RX_MP;                  // offset 78
    u32 RX_MPACK;               // offset 7c
    u32 MPKeyResponseError[15]; // offset 80
} WMGetWirelessCounterCallback;

typedef struct WMPortRecvCallback
{
    u16 apiid;              // offset 00
    u16 errcode;            // offset 02
    u16 state;              // offset 04
    u16 port;               // offset 06
    WMMpRecvBuf *recvBuf;   // offset 08
    u16 *data;              // offset 0c
    u16 length;             // offset 10
    u16 aid;                // offset 12
    u8 macAddress[6];       // offset 14
    u16 seqNo;              // offset 1a
    void *arg;              // offset 1c
    u16 myAid;              // offset 20
    u16 connectedAidBitmap; // offset 22
    u8 ssid[24];            // offset 24
    u16 reason;             // offset 3c
    u16 rssi;               // offset 3e
    u16 maxSendDataSize;    // offset 40
    u16 maxRecvDataSize;    // offset 42
} WMPortRecvCallback;

typedef struct WMSetMPParameterCallback
{
    u16 apiid;          // offset 00
    u16 errcode;        // offset 02
    u32 mask;           // offset 04
    WMMPParam oldParam; // offset 08
} WMSetMPParameterCallback;

typedef struct WMStopTestRxModeCallback
{
    u16 apiid;   // offset 00
    u16 errcode; // offset 02
    u32 fcsOk;   // offset 04
    u32 fcsErr;  // offset 08
} WMStopTestRxModeCallback;

typedef struct WMBeaconRecvIndCallback
{
    u16 apiid;           // offset 00
    u16 errcode;         // offset 02
    u16 state;           // offset 04
    u16 tgid;            // offset 06
    u16 wmstate;         // offset 08
    u16 gameInfoLength;  // offset 0a
    WMGameInfo gameInfo; // offset 0c
} WMBeaconRecvIndCallback;