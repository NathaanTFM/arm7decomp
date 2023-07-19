#pragma pack(1)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;

typedef int BOOL;
typedef int OSHeapHandle;
typedef int mbstate_t;
typedef u32 MATHCRC32Context;
typedef u32 REGType32;
typedef u32 OSIrqMask;
typedef u32 FSCommandType;
typedef u32 CARDEvent;
typedef u32 __file_handle;
typedef u32 size_t;
typedef u32 OSIntrMode;
typedef u32 fpos_t;
typedef u32 REGType32v;
typedef u32 FSEvent;
typedef u32 PMWakeUpTrigger;
typedef u32 PMLogic;
typedef u32 FSOverlayID;
typedef u32 VecFx10;
struct _OSThreadQueue {
    struct _OSThread* head; // offset 00
    struct _OSThread* tail; // offset 04
};
typedef struct _OSThreadQueue OSThreadQueue;
struct OSContext {
    u32 cpsr; // offset 00
    u32 r[13]; // offset 04
    u32 sp; // offset 38
    u32 lr; // offset 3c
    u32 pc_plus4; // offset 40
    u32 sp_svc; // offset 44
};
struct _OSMutexQueue {
    struct OSMutex* head; // offset 00
    struct OSMutex* tail; // offset 04
};
enum _enum_2150 {
    OS_THREAD_STATE_WAITING = 0,
    OS_THREAD_STATE_READY = 1,
    OS_THREAD_STATE_TERMINATED = 2,
};
struct _OSThreadLink {
    struct _OSThread* prev; // offset 00
    struct _OSThread* next; // offset 04
};
struct _OSThread {
    struct OSContext context; // offset 00
    enum _enum_2150 state; // offset 48
    struct _OSThread* next; // offset 4c
    u32 id; // offset 50
    u32 priority; // offset 54
    void* profiler; // offset 58
    struct _OSThreadQueue* queue; // offset 5c
    struct _OSThreadLink link; // offset 60
    struct OSMutex* mutex; // offset 68
    struct _OSMutexQueue mutexQueue; // offset 6c
    u32 stackTop; // offset 74
    u32 stackBottom; // offset 78
    u32 stackWarningOffset; // offset 7c
    struct _OSThreadQueue joinQueue; // offset 80
    void* specific[3]; // offset 88
    struct OSiAlarm* alarmForSleep; // offset 94
    void (*destructor)(void*); // offset 98
    void* userParameter; // offset 9c
    int systemErrno; // offset a0
};
typedef struct _OSThread OSThread;
typedef struct OSContext OSContext;
typedef struct _OSThreadLink OSThreadLink;
struct _OSMutexLink {
    struct OSMutex* next; // offset 00
    struct OSMutex* prev; // offset 04
};
struct OSMutex {
    struct _OSThreadQueue queue; // offset 00
    struct _OSThread* thread; // offset 08
    long count; // offset 0c
    struct _OSMutexLink link; // offset 10
};
typedef struct OSMutex OSMutex;
typedef struct _OSMutexLink OSMutexLink;
typedef struct _OSMutexQueue OSMutexQueue;
struct OSiAlarm {
    void (*handler)(void*); // offset 00
    void* arg; // offset 04
    u32 tag; // offset 08
    u64 fire; // offset 0c
    struct OSiAlarm* prev; // offset 14
    struct OSiAlarm* next; // offset 18
    u64 period; // offset 1c
    u64 start; // offset 24
};
typedef struct OSiAlarm OSAlarm;
typedef u64 REGType64;
typedef u64 REGType64v;
typedef u64 OSTick;
typedef u16 MATHCRC16Context;
typedef u16 MATHChecksum8Context;
typedef u16 wctype_t;
typedef u16 MATHChecksum16Context;
typedef u16 GXRgb;
typedef u16 REGType16;
typedef u16 PMBatteryLevel;
typedef u16 wint_t;
typedef u16 REGType16v;
typedef u16 GXRgba;
typedef u16 Wint_t;
typedef u16 OSBootType;
typedef u16 wchar_t;
typedef struct {
    void (*func)(void*); // offset 00
    u32 enable; // offset 04
    void* arg; // offset 08
} OSIrqCallbackInfo;
struct OSLockWord {
    u32 lockFlag; // offset 00
    u16 ownerID; // offset 04
    u16 extension; // offset 06
};
typedef struct OSLockWord OSLockWord;
typedef union {
    u16 s; // offset 00
    char c[2]; // offset 00
} OSPrintWChar;
typedef struct {
    u32 in; // offset 00
    u32 out; // offset 04
    OSPrintWChar buffer[512]; // offset 08
} OSPrintServerBuffer;
struct OSThreadInfo {
    u16 isNeedRescheduling; // offset 00
    u16 irqDepth; // offset 02
    struct _OSThread* current; // offset 04
    struct _OSThread* list; // offset 08
    void* switchCallback; // offset 0c
};
typedef struct OSThreadInfo OSThreadInfo;
struct OSThreadResource {
    int num; // offset 00
};
typedef struct OSThreadResource OSThreadResource;
struct OSMessageQueue {
    struct _OSThreadQueue queueSend; // offset 00
    struct _OSThreadQueue queueReceive; // offset 08
    void** msgArray; // offset 10
    long msgCount; // offset 14
    long firstIndex; // offset 18
    long usedCount; // offset 1c
};
typedef struct OSMessageQueue OSMessageQueue;
enum _enum_16263 {
    OS_ARENA_MAIN = 0,
    OS_ARENA_MAIN_SUBPRIV = 1,
    OS_ARENA_MAINEX = 2,
    OS_ARENA_ITCM = 3,
    OS_ARENA_DTCM = 4,
    OS_ARENA_SHARED = 5,
    OS_ARENA_WRAM_MAIN = 6,
    OS_ARENA_WRAM_SUB = 7,
    OS_ARENA_WRAM_SUBPRIV = 8,
    OS_ARENA_MAX = 9,
};
typedef u8 REGType8;
typedef u8 REGType8v;
typedef u8 MATHCRC8Context;
typedef struct {
    void* lo[9]; // offset 00
    void* hi[9]; // offset 24
} OSArenaInfo;
struct OSArenaResource {
    OSArenaInfo info; // offset 00
};
typedef struct OSArenaResource OSArenaResource;
struct Cell {
    struct Cell* prev; // offset 00
    struct Cell* next; // offset 04
    long size; // offset 08
};
typedef struct Cell Cell;
struct HeapDesc {
    long size; // offset 00
    struct Cell* free; // offset 04
    struct Cell* allocated; // offset 08
};
typedef struct HeapDesc HeapDesc;
typedef struct {
    int currentHeap; // offset 00
    int numHeaps; // offset 04
    void* arenaStart; // offset 08
    void* arenaEnd; // offset 0c
    struct HeapDesc* heapArray; // offset 10
} OSHeapInfo;
typedef struct {
    struct OSContext context; // offset 00
    u32 cp15; // offset 48
    u32 spsr; // offset 4c
    u32 exinfo; // offset 50
} OSiExContext;
struct OSiAlarmQueue {
    struct OSiAlarm* head; // offset 00
    struct OSiAlarm* tail; // offset 04
};
struct OSAlarmResource {
    int num; // offset 00
};
typedef struct OSAlarmResource OSAlarmResource;
struct OSiVAlarmQueue {
    struct OSiVAlarm* head; // offset 00
    struct OSiVAlarm* tail; // offset 04
};
struct OSiVAlarm {
    void (*handler)(void*); // offset 00
    void* arg; // offset 04
    u32 tag; // offset 08
    u32 frame; // offset 0c
    s16 fire; // offset 10
    s16 delay; // offset 12
    struct OSiVAlarm* prev; // offset 14
    struct OSiVAlarm* next; // offset 18
    int period; // offset 1c
    int finish; // offset 20
    int canceled; // offset 24
};
struct OSVAlarmResource {
    int num; // offset 00
};
typedef struct OSVAlarmResource OSVAlarmResource;
enum _enum_24784 {
    OS_PROCMODE_USER = 16,
    OS_PROCMODE_FIQ = 17,
    OS_PROCMODE_IRQ = 18,
    OS_PROCMODE_SVC = 19,
    OS_PROCMODE_ABORT = 23,
    OS_PROCMODE_UNDEF = 27,
    OS_PROCMODE_SYS = 31,
};
struct OSBootInfo {
    u16 boot_type; // offset 00
    u16 length; // offset 02
    u16 rssi; // offset 04
    u16 bssid[3]; // offset 06
    u16 ssidLength; // offset 0c
    u8 ssid[32]; // offset 0e
    u16 capaInfo; // offset 2e
    struct {
        u16 basic; // offset 00
        u16 support; // offset 02
    } rateSet; // offset 30
    u16 beaconPeriod; // offset 34
    u16 dtimPeriod; // offset 36
    u16 channel; // offset 38
    u16 cfpPeriod; // offset 3a
    u16 cfpMaxDuration; // offset 3c
    u16 rsv1; // offset 3e
};
typedef struct OSBootInfo OSBootInfo;
struct OSBirthday {
    u8 month; // offset 00
    u8 day; // offset 01
};
struct OSOwnerInfo {
    u8 language; // offset 00
    u8 favoriteColor; // offset 01
    struct OSBirthday birthday; // offset 02
    u16 nickName[11]; // offset 04
    u16 nickNameLength; // offset 1a
    u16 comment[27]; // offset 1c
    u16 commentLength; // offset 52
};
typedef struct OSOwnerInfo OSOwnerInfo;
typedef struct OSBirthday OSBirthday;
typedef struct {
    u32 flag; // offset 00
    struct _OSThreadQueue queue; // offset 04
} OSEvent;
typedef union {
    u32 b32; // offset 00
    u16 b16; // offset 00
} MIiDmaClearSrc;
typedef struct {
    u16 srcNum; // offset 00
    u16 srcBitNum : 8; // offset 02
    u16 destBitNum : 8; // offset 02
    u32 destOffset : 31; // offset 04
    u32 destOffset0_on : 1; // offset 04
} MIUnpackBitsParam;
typedef struct {
    long (*initStream)(u8*, void*, void*); // offset 00
    long (*terminateStream)(u8*); // offset 04
    u8 (*readByteStream)(u8*); // offset 08
    u16 (*readShortStream)(u8*); // offset 0c
    u32 (*readWordStream)(u8*); // offset 10
} MIReadStreamCallbacks;
enum _enum_29898 {
    MI_CTRDG_ROMCYCLE1_10 = 0,
    MI_CTRDG_ROMCYCLE1_8 = 1,
    MI_CTRDG_ROMCYCLE1_6 = 2,
    MI_CTRDG_ROMCYCLE1_18 = 3,
};
enum _enum_30021 {
    MI_CTRDG_ROMCYCLE2_6 = 0,
    MI_CTRDG_ROMCYCLE2_4 = 1,
};
struct MICache {
    u32 pagewidth; // offset 00
    struct MICachePage* valid; // offset 04
    struct MICachePage* invalid; // offset 08
    struct MICachePage* loading; // offset 0c
    int valid_total; // offset 10
    int invalid_total; // offset 14
    int loading_total; // offset 18
};
typedef struct MICache MICache;
struct MICachePage {
    struct MICachePage* next; // offset 00
    u32 offset; // offset 04
    u8* buffer; // offset 08
};
typedef struct MICachePage MICachePage;
struct MIDevice {
    void* userdata; // offset 00
    int (*Read)(void*, void*, u32, u32); // offset 04
    int (*Write)(void*, void*, u32, u32); // offset 08
};
typedef struct MIDevice MIDevice;
enum _enum_31522 {
    PXI_FIFO_TAG_EX = 0,
    PXI_FIFO_TAG_USER_0 = 1,
    PXI_FIFO_TAG_USER_1 = 2,
    PXI_FIFO_TAG_SYSTEM = 3,
    PXI_FIFO_TAG_NVRAM = 4,
    PXI_FIFO_TAG_RTC = 5,
    PXI_FIFO_TAG_TOUCHPANEL = 6,
    PXI_FIFO_TAG_SOUND = 7,
    PXI_FIFO_TAG_PM = 8,
    PXI_FIFO_TAG_MIC = 9,
    PXI_FIFO_TAG_WM = 10,
    PXI_FIFO_TAG_FS = 11,
    PXI_FIFO_TAG_OS = 12,
    PXI_FIFO_TAG_CTRDG = 13,
    PXI_FIFO_TAG_CARD = 14,
    PXI_FIFO_TAG_WVR = 15,
    PXI_FIFO_TAG_CTRDG_Ex = 16,
    PXI_FIFO_TAG_CTRDG_PHI = 17,
    PXI_MAX_FIFO_TAG = 32,
};
enum _enum_32007 {
    PXI_PROC_ARM9 = 0,
    PXI_PROC_ARM7 = 1,
};
typedef union {
    struct {
        u32 tag : 5; // offset 00
        u32 err : 1; // offset 00
        u32 data : 26; // offset 00
    } e; // offset 00
    u32 raw; // offset 00
} PXIFifoMessage;
enum _enum_32197 {
    PXI_FIFO_SUCCESS = 0,
    PXI_FIFO_FAIL_SEND_ERR = 4294967295,
    PXI_FIFO_FAIL_SEND_FULL = 4294967294,
    PXI_FIFO_FAIL_RECV_ERR = 4294967293,
    PXI_FIFO_FAIL_RECV_EMPTY = 4294967292,
    PXI_FIFO_NO_CALLBACK_ENTRY = 4294967291,
};
struct MATHMD5Context {
    union {
        struct {
            u32 a; // offset 00
            u32 b; // offset 04
            u32 c; // offset 08
            u32 d; // offset 0c
        }; // offset 00
        u32 state[4]; // offset 00
    }; // offset 00
    u64 length; // offset 10
    union {
        u32 buffer32[16]; // offset 00
        u8 buffer8[64]; // offset 00
    }; // offset 18
};
typedef struct MATHMD5Context MATHMD5Context;
struct MATHiHMACFuncs {
    u32 dlength; // offset 00
    u32 blength; // offset 04
    void* context; // offset 08
    u8* hash_buf; // offset 0c
    void (*HashReset)(void*); // offset 10
    void (*HashSetSource)(void*, void*, u32); // offset 14
    void (*HashGetDigest)(void*, void*); // offset 18
};
typedef struct MATHiHMACFuncs MATHiHMACFuncs;
struct MATHSHA1Context {
    u32 h[5]; // offset 00
    u8 block[64]; // offset 14
    u32 pool; // offset 54
    u32 blocks_low; // offset 58
    u32 blocks_high; // offset 5c
};
typedef struct MATHSHA1Context MATHSHA1Context;
struct MATHCRC8Table {
    u8 table[256]; // offset 00
};
typedef struct MATHCRC8Table MATHCRC8Table;
struct MATHCRC16Table {
    u16 table[256]; // offset 00
};
typedef struct MATHCRC16Table MATHCRC16Table;
struct MATHCRC32Table {
    u32 table[256]; // offset 00
};
typedef struct MATHCRC32Table MATHCRC32Table;
struct MATHSHA256Context {
    u32 h[8]; // offset 00
    u32 Nl; // offset 20
    u32 Nh; // offset 24
    u8 data[64]; // offset 28
    int num; // offset 68
};
typedef struct MATHSHA256Context MATHSHA256Context;
enum _enum_35466 {
    EXI_GPIOIF_SERIAL = 0,
    EXI_GPIOIF_UNDEF = 16384,
    EXI_GPIOIF_GPIO = 32768,
    EXI_GPIOIF_JOY = 49152,
};
enum SNDOutput {
    SND_OUTPUT_MIXER = 0,
    SND_OUTPUT_CHANNEL1 = 1,
    SND_OUTPUT_CHANNEL3 = 2,
    SND_OUTPUT_CHANNEL1_3 = 3,
};
enum SNDChannelOut {
    SND_CHANNEL_OUT_MIXER = 0,
    SND_CHANNEL_OUT_BYPASS = 1,
};
enum _enum_36756 {
    SND_WAVE_FORMAT_PCM8 = 0,
    SND_WAVE_FORMAT_PCM16 = 1,
    SND_WAVE_FORMAT_ADPCM = 2,
    SND_WAVE_FORMAT_PSG = 3,
    SND_WAVE_FORMAT_NOISE = 3,
};
enum _enum_36906 {
    SND_CHANNEL_LOOP_MANUAL = 0,
    SND_CHANNEL_LOOP_REPEAT = 1,
    SND_CHANNEL_LOOP_1SHOT = 2,
};
enum _enum_37008 {
    SND_CHANNEL_DATASHIFT_NONE = 0,
    SND_CHANNEL_DATASHIFT_1BIT = 1,
    SND_CHANNEL_DATASHIFT_2BIT = 2,
    SND_CHANNEL_DATASHIFT_4BIT = 3,
};
enum _enum_37153 {
    SND_DUTY_1_8 = 0,
    SND_DUTY_2_8 = 1,
    SND_DUTY_3_8 = 2,
    SND_DUTY_4_8 = 3,
    SND_DUTY_5_8 = 4,
    SND_DUTY_6_8 = 5,
    SND_DUTY_7_8 = 6,
};
enum _enum_40436 {
    SND_CAPTURE_0 = 0,
    SND_CAPTURE_1 = 1,
};
enum _enum_40489 {
    SND_CAPTURE_FORMAT_PCM16 = 0,
    SND_CAPTURE_FORMAT_PCM8 = 1,
};
enum _enum_40569 {
    SND_CAPTURE_IN_MIXER = 0,
    SND_CAPTURE_IN_CHANNEL = 1,
    SND_CAPTURE_IN_MIXER_L = 0,
    SND_CAPTURE_IN_MIXER_R = 0,
    SND_CAPTURE_IN_CHANNEL0 = 1,
    SND_CAPTURE_IN_CHANNEL2 = 1,
};
enum _enum_40756 {
    SND_CAPTURE_OUT_NORMAL = 0,
    SND_CAPTURE_OUT_CHANNEL_MIX = 1,
    SND_CAPTURE_OUT_CHANNEL0_MIX = 1,
    SND_CAPTURE_OUT_CHANNEL2_MIX = 1,
};
enum SNDExChannelCallbackStatus {
    SND_EX_CHANNEL_CALLBACK_DROP = 0,
    SND_EX_CHANNEL_CALLBACK_FINISH = 1,
};
struct SNDLfoParam {
    u8 target; // offset 00
    u8 speed; // offset 01
    u8 depth; // offset 02
    u8 range; // offset 03
    u16 delay; // offset 04
};
struct SNDLfo {
    struct SNDLfoParam param; // offset 00
    u16 delay_counter; // offset 06
    u16 counter; // offset 08
};
struct SNDWaveParam {
    u8 format; // offset 00
    u8 loopflag; // offset 01
    u16 rate; // offset 02
    u16 timer; // offset 04
    u16 loopstart; // offset 06
    u32 looplen; // offset 08
};
struct SNDExChannel {
    u8 myNo; // offset 00
    u8 type; // offset 01
    u8 env_status; // offset 02
    u8 active_flag : 1; // offset 03
    u8 start_flag : 1; // offset 03
    u8 auto_sweep : 1; // offset 03
    u8 sync_flag : 5; // offset 03
    u8 pan_range; // offset 04
    u8 original_key; // offset 05
    s16 user_decay2; // offset 06
    u8 key; // offset 08
    u8 velocity; // offset 09
    s8 init_pan; // offset 0a
    s8 user_pan; // offset 0b
    s16 user_decay; // offset 0c
    s16 user_pitch; // offset 0e
    long env_decay; // offset 10
    long sweep_counter; // offset 14
    long sweep_length; // offset 18
    u8 attack; // offset 1c
    u8 sustain; // offset 1d
    u16 decay; // offset 1e
    u16 release; // offset 20
    u8 prio; // offset 22
    u8 pan; // offset 23
    u16 volume; // offset 24
    u16 timer; // offset 26
    struct SNDLfo lfo; // offset 28
    s16 sweep_pitch; // offset 32
    long length; // offset 34
    struct SNDWaveParam wave; // offset 38
    union {
        void* data; // offset 00
        enum _enum_37153 duty; // offset 00
    }; // offset 44
    void (*callback)(struct SNDExChannel*, enum SNDExChannelCallbackStatus, void*); // offset 48
    void* callback_data; // offset 4c
    struct SNDExChannel* nextLink; // offset 50
};
typedef struct SNDExChannel SNDExChannel;
typedef struct SNDLfo SNDLfo;
typedef struct SNDLfoParam SNDLfoParam;
typedef struct SNDWaveParam SNDWaveParam;
struct SNDSeqCache {
    u32* base; // offset 00
    u32* endp; // offset 04
    u32 buffer[4]; // offset 08
};
typedef struct SNDSeqCache SNDSeqCache;
struct SNDPlayer {
    u8 active_flag : 1; // offset 00
    u8 prepared_flag : 1; // offset 00
    u8 pause_flag : 1; // offset 00
    u8 pad_ : 5; // offset 00
    u8 myNo; // offset 01
    u8 pad2_; // offset 02
    u8 pad3_; // offset 03
    u8 prio; // offset 04
    u8 volume; // offset 05
    s16 extFader; // offset 06
    u8 tracks[16]; // offset 08
    u16 tempo; // offset 18
    u16 tempo_ratio; // offset 1a
    u16 tempo_counter; // offset 1c
    u16 pad__; // offset 1e
    struct SNDBankData* bank; // offset 20
};
typedef struct SNDPlayer SNDPlayer;
struct SNDWaveArcLink {
    struct SNDWaveArc* waveArc; // offset 00
    struct SNDWaveArcLink* next; // offset 04
};
struct SNDBinaryFileHeader {
    char signature[4]; // offset 00
    u16 byteOrder; // offset 04
    u16 version; // offset 06
    u32 fileSize; // offset 08
    u16 headerSize; // offset 0c
    u16 dataBlocks; // offset 0e
};
struct SNDBinaryBlockHeader {
    u32 kind; // offset 00
    u32 size; // offset 04
};
struct SNDBankData {
    struct SNDBinaryFileHeader fileHeader; // offset 00
    struct SNDBinaryBlockHeader blockHeader; // offset 10
    struct SNDWaveArcLink waveArcLink[4]; // offset 18
    u32 instCount; // offset 38
    u32 instOffset[0]; // offset 3c
};
typedef struct SNDBankData SNDBankData;
typedef struct SNDBinaryFileHeader SNDBinaryFileHeader;
typedef struct SNDBinaryBlockHeader SNDBinaryBlockHeader;
typedef struct SNDWaveArcLink SNDWaveArcLink;
struct SNDWaveArc {
    struct SNDBinaryFileHeader fileHeader; // offset 00
    struct SNDBinaryBlockHeader blockHeader; // offset 10
    struct SNDWaveArcLink* topLink; // offset 18
    u32 reserved[7]; // offset 1c
    u32 waveCount; // offset 38
    u32 waveOffset[0]; // offset 3c
};
typedef struct SNDWaveArc SNDWaveArc;
struct SNDTrack {
    u8 active_flag : 1; // offset 00
    u8 note_wait : 1; // offset 00
    u8 mute_flag : 1; // offset 00
    u8 tie_flag : 1; // offset 00
    u8 note_finish_wait : 1; // offset 00
    u8 porta_flag : 1; // offset 00
    u8 cmp_flag : 1; // offset 00
    u8 channel_mask_flag : 1; // offset 00
    u8 pan_range; // offset 01
    u16 prgNo; // offset 02
    u8 volume; // offset 04
    u8 volume2; // offset 05
    s8 pitch_bend; // offset 06
    u8 bend_range; // offset 07
    s8 pan; // offset 08
    s8 ext_pan; // offset 09
    s16 extFader; // offset 0a
    s16 ext_pitch; // offset 0c
    u8 attack; // offset 0e
    u8 decay; // offset 0f
    u8 sustain; // offset 10
    u8 release; // offset 11
    u8 prio; // offset 12
    s8 transpose; // offset 13
    u8 porta_key; // offset 14
    u8 porta_time; // offset 15
    s16 sweep_pitch; // offset 16
    struct SNDLfoParam mod; // offset 18
    u16 channel_mask; // offset 1e
    long wait; // offset 20
    u8* base; // offset 24
    u8* cur; // offset 28
    u8* call_stack[3]; // offset 2c
    u8 loop_count[3]; // offset 38
    u8 call_stack_depth; // offset 3b
    struct SNDExChannel* channel_list; // offset 3c
};
typedef struct SNDTrack SNDTrack;
enum _enum_48280 {
    SND_SEQ_MUTE_OFF = 0,
    SND_SEQ_MUTE_NO_STOP = 1,
    SND_SEQ_MUTE_RELEASE = 2,
    SND_SEQ_MUTE_STOP = 3,
};
enum _enum_48508 {
    SEQ_ARG_U8 = 0,
    SEQ_ARG_S16 = 1,
    SEQ_ARG_VMIDI = 2,
    SEQ_ARG_RANDOM = 3,
    SEQ_ARG_VARIABLE = 4,
};
struct SNDMidiChannel {
    struct SNDExChannel* chp; // offset 00
    u8 key; // offset 04
    u8 pad1; // offset 05
    u16 pad2; // offset 06
};
struct SNDMidiTrack {
    struct SNDMidiChannel channels[16]; // offset 00
    struct SNDLfoParam mod; // offset 80
    s16 sweep_pitch; // offset 86
    u16 prgNo; // offset 88
    s8 pitchbend; // offset 8a
    u8 porta_time; // offset 8b
    u8 volume; // offset 8c
    s8 pan; // offset 8d
    u8 expression; // offset 8e
    s8 transpose; // offset 8f
    u8 prio; // offset 90
    u8 bendrange; // offset 91
    u8 porta_flag; // offset 92
    u8 porta_key; // offset 93
    u8 attack; // offset 94
    u8 decay; // offset 95
    u8 sustain; // offset 96
    u8 release; // offset 97
    u8 rpnLSB; // offset 98
    u8 rpnMSB; // offset 99
    u8 nrpnLSB; // offset 9a
    u8 nrpnMSB; // offset 9b
    u8 rpn_flag; // offset 9c
    u8 pad1; // offset 9d
    u16 pad2; // offset 9e
};
struct SNDMidiPlayer {
    struct SNDBankData* bank; // offset 00
    struct SNDMidiTrack track[16]; // offset 04
    u8 main_volume; // offset a04
    u8 prio; // offset a05
    u16 pad; // offset a06
};
typedef struct SNDMidiPlayer SNDMidiPlayer;
typedef struct SNDMidiTrack SNDMidiTrack;
typedef struct SNDMidiChannel SNDMidiChannel;
struct SNDInstParam {
    u16 wave[2]; // offset 00
    u8 original_key; // offset 04
    u8 attack; // offset 05
    u8 decay; // offset 06
    u8 sustain; // offset 07
    u8 release; // offset 08
    u8 pan; // offset 09
};
struct SNDInstData {
    u8 type; // offset 00
    u8 padding_; // offset 01
    struct SNDInstParam param; // offset 02
};
typedef struct SNDInstData SNDInstData;
typedef struct SNDInstParam SNDInstParam;
struct SNDKeySplit {
    u8 key[8]; // offset 00
    struct SNDInstData instOffset[0]; // offset 08
};
typedef struct SNDKeySplit SNDKeySplit;
struct SNDDrumSet {
    u8 min; // offset 00
    u8 max; // offset 01
    struct SNDInstData instOffset[0]; // offset 02
};
typedef struct SNDDrumSet SNDDrumSet;
struct SNDInstPos {
    u32 prgNo; // offset 00
    u32 index; // offset 04
};
typedef struct SNDInstPos SNDInstPos;
struct SNDWaveData {
    struct SNDWaveParam param; // offset 00
    u8 samples[0]; // offset 0c
};
typedef struct SNDWaveData SNDWaveData;
struct SNDSharedWork {
    u32 finishCommandTag; // offset 00
    u32 playerStatus; // offset 04
    u16 channelStatus; // offset 08
    u16 captureStatus; // offset 0a
    u32 padding[5]; // offset 0c
    struct {
        s16 variable[16]; // offset 00
        u32 tickCounter; // offset 20
    } player[16]; // offset 20
    s16 globalVariable[16]; // offset 260
};
typedef struct SNDSharedWork SNDSharedWork;
struct SNDAlarm {
    u8 enable; // offset 00
    u8 id; // offset 01
    u8 count; // offset 02
    u8 padding; // offset 03
    struct {
        u64 tick; // offset 00
        u64 period; // offset 08
    } setting; // offset 04
    struct OSiAlarm alarm; // offset 14
};
struct SNDWork {
    struct SNDExChannel channel[16]; // offset 00
    struct SNDPlayer player[16]; // offset 540
    struct SNDTrack track[32]; // offset 780
    struct SNDAlarm alarm[8]; // offset f80
};
typedef struct SNDWork SNDWork;
typedef struct SNDAlarm SNDAlarm;
struct SNDDriverInfo {
    struct SNDWork work; // offset 00
    u32 chCtrl[16]; // offset 1180
    struct SNDWork* workAddress; // offset 11c0
    u32 lockedChannels; // offset 11c4
    u32 padding[6]; // offset 11c8
};
typedef struct SNDDriverInfo SNDDriverInfo;
enum SNDEnvStatus {
    SND_ENV_ATTACK = 0,
    SND_ENV_DECAY = 1,
    SND_ENV_SUSTAIN = 2,
    SND_ENV_RELEASE = 3,
};
struct SNDChannelInfo {
    int activeFlag : 1; // offset 00
    int lockFlag : 1; // offset 00
    u16 volume; // offset 04
    u8 pan; // offset 06
    u8 pad_; // offset 07
    enum SNDEnvStatus envStatus; // offset 08
};
typedef struct SNDChannelInfo SNDChannelInfo;
struct SNDPlayerInfo {
    int activeFlag : 1; // offset 00
    int pauseFlag : 1; // offset 00
    u16 trackBitMask; // offset 04
    u16 tempo; // offset 06
    u8 volume; // offset 08
    u8 pad_; // offset 09
    u16 pad2_; // offset 0a
};
typedef struct SNDPlayerInfo SNDPlayerInfo;
struct SNDTrackInfo {
    u16 prgNo; // offset 00
    u8 volume; // offset 02
    u8 volume2; // offset 03
    s8 pitchBend; // offset 04
    u8 bendRange; // offset 05
    u8 pan; // offset 06
    s8 transpose; // offset 07
    u8 pad_; // offset 08
    u8 chCount; // offset 09
    u8 channel[16]; // offset 0a
};
typedef struct SNDTrackInfo SNDTrackInfo;
enum SNDCommandID {
    SND_COMMAND_START_SEQ = 0,
    SND_COMMAND_STOP_SEQ = 1,
    SND_COMMAND_PREPARE_SEQ = 2,
    SND_COMMAND_START_PREPARED_SEQ = 3,
    SND_COMMAND_PAUSE_SEQ = 4,
    SND_COMMAND_SKIP_SEQ = 5,
    SND_COMMAND_PLAYER_PARAM = 6,
    SND_COMMAND_TRACK_PARAM = 7,
    SND_COMMAND_MUTE_TRACK = 8,
    SND_COMMAND_ALLOCATABLE_CHANNEL = 9,
    SND_COMMAND_PLAYER_LOCAL_VAR = 10,
    SND_COMMAND_PLAYER_GLOBAL_VAR = 11,
    SND_COMMAND_START_TIMER = 12,
    SND_COMMAND_STOP_TIMER = 13,
    SND_COMMAND_SETUP_CHANNEL_PCM = 14,
    SND_COMMAND_SETUP_CHANNEL_PSG = 15,
    SND_COMMAND_SETUP_CHANNEL_NOISE = 16,
    SND_COMMAND_SETUP_CAPTURE = 17,
    SND_COMMAND_SETUP_ALARM = 18,
    SND_COMMAND_CHANNEL_TIMER = 19,
    SND_COMMAND_CHANNEL_VOLUME = 20,
    SND_COMMAND_CHANNEL_PAN = 21,
    SND_COMMAND_SURROUND_DECAY = 22,
    SND_COMMAND_MASTER_VOLUME = 23,
    SND_COMMAND_MASTER_PAN = 24,
    SND_COMMAND_OUTPUT_SELECTOR = 25,
    SND_COMMAND_LOCK_CHANNEL = 26,
    SND_COMMAND_UNLOCK_CHANNEL = 27,
    SND_COMMAND_STOP_UNLOCKED_CHANNEL = 28,
    SND_COMMAND_SHARED_WORK = 29,
    SND_COMMAND_INVALIDATE_SEQ = 30,
    SND_COMMAND_INVALIDATE_BANK = 31,
    SND_COMMAND_INVALIDATE_WAVE = 32,
    SND_COMMAND_READ_DRIVER_INFO = 33,
};
struct SNDCommand {
    struct SNDCommand* next; // offset 00
    enum SNDCommandID id; // offset 04
    u32 arg[4]; // offset 08
};
typedef struct SNDCommand SNDCommand;
enum SPIDeviceType {
    SPI_DEVICE_TYPE_TP = 0,
    SPI_DEVICE_TYPE_NVRAM = 1,
    SPI_DEVICE_TYPE_MIC = 2,
    SPI_DEVICE_TYPE_PM = 3,
    SPI_DEVICE_TYPE_ARM7 = 4,
    SPI_DEVICE_TYPE_MAX = 5,
};
struct SPIEntry {
    enum SPIDeviceType type; // offset 00
    u32 process; // offset 04
    u32 arg[4]; // offset 08
};
struct SPIWork {
    int exception; // offset 00
    enum SPIDeviceType type; // offset 04
    struct _OSThread thread; // offset 08
    u64 stack[64]; // offset ac
    struct OSMessageQueue message; // offset 2ac
    void* msg_buf[16]; // offset 2cc
    struct SPIEntry entry[16]; // offset 30c
    u32 entryIndex; // offset 48c
    struct _OSThreadQueue lock; // offset 490
    u32 lockId; // offset 498
};
typedef struct SPIWork SPIWork;
typedef struct SPIEntry SPIEntry;
enum PMStatus {
    PM_STATUS_READY = 0,
    PM_STATUS_START_SLEEP = 1,
    PM_STATUS_UTILITY = 2,
    PM_STATUS_READ_REGISTER = 3,
    PM_STATUS_WRITE_REGISTER = 4,
};
struct PMWork {
    u16 command[16]; // offset 00
    enum PMStatus status; // offset 20
    u32 param; // offset 24
    u32 regNumber; // offset 28
};
typedef struct PMWork PMWork;
enum _enum_63738 {
    PM_LED_NONE = 0,
    PM_LED_ON = 1,
    PM_LED_BLINK_LOW = 2,
    PM_LED_BLINK_HIGH = 3,
};
enum _enum_64984 {
    PM_LED_PATTERN_NONE = 0,
    PM_LED_PATTERN_ON = 1,
    PM_LED_PATTERN_BLINK_LOW = 2,
    PM_LED_PATTERN_BLINK_HIGH = 3,
    PM_LED_PATTERN_BLINK1 = 4,
    PM_LED_PATTERN_BLINK2 = 5,
    PM_LED_PATTERN_BLINK3 = 6,
    PM_LED_PATTERN_BLINK4 = 7,
    PM_LED_PATTERN_BLINK5 = 8,
    PM_LED_PATTERN_BLINK6 = 9,
    PM_LED_PATTERN_BLINK8 = 10,
    PM_LED_PATTERN_BLINK10 = 11,
    PM_LED_PATTERN_PATTERN1 = 12,
    PM_LED_PATTERN_PATTERN2 = 13,
    PM_LED_PATTERN_PATTERN3 = 14,
    PM_LED_PATTERN_WIRELESS = 15,
};
typedef struct {
    u64 pattern; // offset 00
    u16 patternSize; // offset 08
    u16 patternResolution; // offset 0a
} PMiBlinkPatternData;
struct NVRAMWork {
    u16 command[16]; // offset 00
};
typedef struct NVRAMWork NVRAMWork;
enum TPStatus {
    TP_STATUS_READY = 0,
    TP_STATUS_AUTO_START = 1,
    TP_STATUS_AUTO_SAMPLING = 2,
    TP_STATUS_AUTO_WAIT_END = 3,
};
struct TPWork {
    u16 command[16]; // offset 00
    enum TPStatus status; // offset 20
    long range; // offset 24
    long rangeMin; // offset 28
    struct OSiVAlarm vAlarm[4]; // offset 2c
    u16 vCount[4]; // offset cc
};
typedef struct TPWork TPWork;
union SPITpData {
    struct {
        u32 x : 12; // offset 00
        u32 y : 12; // offset 00
        u32 touch : 1; // offset 00
        u32 validity : 2; // offset 00
        u32 dummy : 5; // offset 00
    } e; // offset 00
    u32 raw; // offset 00
    u8 bytes[4]; // offset 00
    u16 halfs[2]; // offset 00
};
typedef union SPITpData SPITpData;
enum SPITpValidity {
    SPI_TP_VALIDITY_VALID = 0,
    SPI_TP_VALIDITY_INVALID_X = 1,
    SPI_TP_VALIDITY_INVALID_Y = 2,
    SPI_TP_VALIDITY_INVALID_XY = 3,
};
enum _enum_69614 {
    TP_DETECT_AXIS_X = 0,
    TP_DETECT_AXIS_Y = 1,
};
enum MICStatus {
    MIC_STATUS_READY = 0,
    MIC_STATUS_AUTO_START = 1,
    MIC_STATUS_AUTO_SAMPLING = 2,
    MIC_STATUS_AUTO_END = 3,
    MIC_STATUS_END_WAIT = 4,
};
struct MICWork {
    u16 command[16]; // offset 00
    enum MICStatus status; // offset 20
    u16 type; // offset 24
    u16 admode; // offset 26
    void* buf; // offset 28
    u32 index; // offset 2c
    u32 size; // offset 30
    u16 timerValue; // offset 34
    u16 timerPrescaler; // offset 36
    u16 temp16; // offset 38
    u16 temporary; // offset 3a
};
typedef struct MICWork MICWork;
struct MICIntrInfo {
    u32 count; // offset 00
    u32 sp; // offset 04
    u32 ie; // offset 08
    u32 handler; // offset 0c
};
typedef struct MICIntrInfo MICIntrInfo;
struct MICIntrPrio {
    u32 ieBit; // offset 00
    u32 tableIndex; // offset 04
};
typedef struct MICIntrPrio MICIntrPrio;
typedef void FSArgumentForSuspend;
typedef void FSArgumentForUnmount;
typedef void FSArgumentForActivate;
typedef void FSArgumentForIdle;
typedef void FSArgumentForMount;
typedef void FSArgumentForCloseFile;
typedef void FSArgumentForFlushFile;
typedef void FSArgumentForCloseDirectory;
typedef void FSArgumentForResume;
struct CARDRomRegion {
    u32 offset; // offset 00
    u32 length; // offset 04
};
struct FSOverlaySource {
    struct FSArchive* arc; // offset 00
    struct CARDRomRegion ovt9; // offset 04
    struct CARDRomRegion ovt7; // offset 0c
    void* digest_key_ptr; // offset 14
    u32 digest_key_len; // offset 18
};
typedef struct FSOverlaySource FSOverlaySource;
enum FSResult {
    FS_RESULT_SUCCESS = 0,
    FS_RESULT_FAILURE = 1,
    FS_RESULT_BUSY = 2,
    FS_RESULT_CANCELED = 3,
    FS_RESULT_CANCELLED = 3,
    FS_RESULT_UNSUPPORTED = 4,
    FS_RESULT_ERROR = 5,
    FS_RESULT_INVALID_PARAMETER = 6,
    FS_RESULT_NO_MORE_RESOURCE = 7,
    FS_RESULT_ALREADY_DONE = 8,
    FS_RESULT_PERMISSION_DENIED = 9,
    FS_RESULT_MEDIA_FATAL = 10,
    FS_RESULT_NO_ENTRY = 11,
    FS_RESULT_MEDIA_NOTHING = 12,
    FS_RESULT_MEDIA_UNKNOWN = 13,
    FS_RESULT_BAD_FORMAT = 14,
    FS_RESULT_MAX = 15,
    FS_RESULT_PROC_ASYNC = 256,
    FS_RESULT_PROC_DEFAULT = 257,
    FS_RESULT_PROC_UNKNOWN = 258,
};
struct FSArchive {
    union {
        char ptr[4]; // offset 00
        u32 pack; // offset 00
    } name; // offset 00
    struct FSArchive* next; // offset 04
    struct FSFile* list; // offset 08
    struct _OSThreadQueue queue; // offset 0c
    u32 flag; // offset 14
    u32 command; // offset 18
    enum FSResult result; // offset 1c
    void* userdata; // offset 20
    struct FSArchiveInterface* vtbl; // offset 24
    union {
        u8 reserved2[52]; // offset 00
        struct {
            u32 base; // offset 00
            u32 fat; // offset 04
            u32 fat_size; // offset 08
            u32 fnt; // offset 0c
            u32 fnt_size; // offset 10
            u32 fat_bak; // offset 14
            u32 fnt_bak; // offset 18
            void* load_mem; // offset 1c
            enum FSResult (*read_func)(struct FSArchive*, void*, u32, u32); // offset 20
            enum FSResult (*write_func)(struct FSArchive*, void*, u32, u32); // offset 24
            u8 reserved3[4]; // offset 28
            enum FSResult (*proc)(struct FSFile*, u32); // offset 2c
            u32 proc_flag; // offset 30
        }; // offset 00
    }; // offset 28
};
typedef struct FSArchive FSArchive;
struct FSFileID {
    struct FSArchive* arc; // offset 00
    u32 file_id; // offset 04
};
typedef struct {
    struct FSFileID id; // offset 00
} FSOpenFileFastInfo;
typedef struct {
    void* src; // offset 00
    u32 len_org; // offset 04
    u32 len; // offset 08
} FSWriteFileInfo;
struct FSDirPos {
    struct FSArchive* arc; // offset 00
    u16 own_id; // offset 04
    u16 index; // offset 06
    u32 pos; // offset 08
};
typedef struct {
    struct FSDirPos pos; // offset 00
} FSSeekDirInfo;
typedef struct {
    void* dst; // offset 00
    u32 len_org; // offset 04
    u32 len; // offset 08
} FSReadFileInfo;
typedef struct {
    struct FSDirPos pos; // offset 00
    char* path; // offset 0c
    int find_directory; // offset 10
    union {
        struct FSFileID* file; // offset 00
        struct FSDirPos* dir; // offset 00
    } result; // offset 14
} FSFindPathInfo;
typedef struct {
    u8* buf; // offset 00
    u32 buf_len; // offset 04
    u16 total_len; // offset 08
    u16 dir_id; // offset 0a
} FSGetPathInfo;
typedef struct {
    union {
        struct FSFileID file_id; // offset 00
        struct FSDirPos dir_id; // offset 00
    }; // offset 00
    u32 is_directory; // offset 0c
    u32 name_len; // offset 10
    char name[128]; // offset 14
} FSDirEntry;
typedef struct {
    FSDirEntry* p_entry; // offset 00
    int skip_string; // offset 04
} FSReadDirInfo;
typedef struct {
    u32 top; // offset 00
    u32 bottom; // offset 04
    u32 index; // offset 08
} FSOpenFileDirectInfo;
typedef struct {
    u32 reserved; // offset 00
} FSCloseFileInfo;
union FSROMFATCommandInfo {
    FSReadFileInfo readfile; // offset 00
    FSWriteFileInfo writefile; // offset 00
    FSSeekDirInfo seekdir; // offset 00
    FSReadDirInfo readdir; // offset 00
    FSFindPathInfo findpath; // offset 00
    FSGetPathInfo getpath; // offset 00
    FSOpenFileFastInfo openfilefast; // offset 00
    FSOpenFileDirectInfo openfiledirect; // offset 00
    FSCloseFileInfo closefile; // offset 00
};
struct FSROMFATFileProperty {
    u32 own_id; // offset 00
    u32 top; // offset 04
    u32 bottom; // offset 08
    u32 pos; // offset 0c
};
struct FSROMFATDirProperty {
    struct FSDirPos pos; // offset 00
    u32 parent; // offset 0c
};
union FSROMFATProperty {
    struct FSROMFATFileProperty file; // offset 00
    struct FSROMFATDirProperty dir; // offset 00
};
struct FSFile {
    struct FSFile* next; // offset 00
    void* userdata; // offset 04
    struct FSArchive* arc; // offset 08
    u32 stat; // offset 0c
    void* argument; // offset 10
    enum FSResult error; // offset 14
    struct _OSThreadQueue queue[1]; // offset 18
    union {
        u8 reserved1[16]; // offset 00
        union FSROMFATProperty prop; // offset 00
    }; // offset 20
    union {
        u8 reserved2[24]; // offset 00
        union FSROMFATCommandInfo arg; // offset 00
    }; // offset 30
};
typedef struct FSFile FSFile;
typedef union FSROMFATProperty FSROMFATProperty;
typedef struct FSROMFATFileProperty FSROMFATFileProperty;
typedef struct FSROMFATDirProperty FSROMFATDirProperty;
typedef struct FSDirPos FSDirPos;
typedef union FSROMFATCommandInfo FSROMFATCommandInfo;
typedef struct FSFileID FSFileID;
enum FSSeekFileMode {
    FS_SEEK_SET = 0,
    FS_SEEK_CUR = 1,
    FS_SEEK_END = 2,
};
typedef struct FSArchiveInterface FSArchiveInterface;
struct FSDateTime {
    u32 year; // offset 00
    u32 month; // offset 04
    u32 day; // offset 08
    u32 hour; // offset 0c
    u32 minute; // offset 10
    u32 second; // offset 14
};
struct FSDirectoryEntryInfo {
    char s16name[16]; // offset 00
    u32 s16name_length; // offset 10
    char longname[260]; // offset 14
    u64 name_length; // offset 118
    u32 attributes; // offset 11c
    struct FSDateTime atime; // offset 120
    struct FSDateTime mtime; // offset 138
    struct FSDateTime ctime; // offset 150
    u32 filesize; // offset 168
    u32 id; // offset 16c
};
typedef struct FSDirectoryEntryInfo FSDirectoryEntryInfo;
typedef struct FSDateTime FSDateTime;
struct FSPathInfo {
    u32 attributes; // offset 00
    struct FSDateTime ctime; // offset 04
    struct FSDateTime mtime; // offset 1c
    struct FSDateTime atime; // offset 34
    u32 filesize; // offset 4c
    u32 id; // offset 50
};
typedef struct FSPathInfo FSFileInfo;
typedef struct FSPathInfo FSPathInfo;
struct FSArchiveResource {
    u64 totalSize; // offset 00
    u64 availableSize; // offset 08
    u32 maxFileHandles; // offset 10
    u32 currentFileHandles; // offset 14
    u32 maxDirectoryHandles; // offset 18
    u32 currentDirectoryHandles; // offset 1c
    u32 bytesPerSector; // offset 20
    u32 sectorsPerCluster; // offset 24
    u32 totalClusters; // offset 28
    u32 availableClusters; // offset 2c
};
typedef struct FSArchiveResource FSArchiveResource;
typedef struct CARDRomRegion CARDRomRegion;
enum _enum_81074 {
    MI_PROCESSOR_ARM9 = 0,
    MI_PROCESSOR_ARM7 = 1,
};
struct FSOverlayInfoHeader {
    u32 id; // offset 00
    u8* ram_address; // offset 04
    u32 ram_size; // offset 08
    u32 bss_size; // offset 0c
    void (**sinit_init)(); // offset 10
    void (**sinit_init_end)(); // offset 14
    u32 file_id; // offset 18
    u32 compressed : 24; // offset 1c
    u32 flag : 8; // offset 1c
};
struct FSOverlayInfo {
    struct FSOverlayInfoHeader header; // offset 00
    enum _enum_81074 target; // offset 20
    struct CARDRomRegion file_pos; // offset 24
};
typedef struct FSOverlayInfo FSOverlayInfo;
typedef struct FSOverlayInfoHeader FSOverlayInfoHeader;

struct FSArchiveInterface {
    enum FSResult (*ReadFile)(struct FSArchive*, struct FSFile*, void*, u32*); // offset 00
    enum FSResult (*WriteFile)(struct FSArchive*, struct FSFile*, void*, u32*); // offset 04
    enum FSResult (*SeekDirectory)(struct FSArchive*, struct FSFile*, u32, u32); // offset 08
    enum FSResult (*ReadDirectory)(struct FSArchive*, struct FSFile*, struct FSDirectoryEntryInfo*); // offset 0c
    enum FSResult (*FindPath)(struct FSArchive*, u32, char*, u32*, int); // offset 10
    enum FSResult (*GetPath)(struct FSArchive*, struct FSFile*, int, char*, u32*); // offset 14
    enum FSResult (*OpenFileFast)(struct FSArchive*, struct FSFile*, u32, u32); // offset 18
    enum FSResult (*OpenFileDirect)(struct FSArchive*, struct FSFile*, u32, u32, u32*); // offset 1c
    enum FSResult (*CloseFile)(struct FSArchive*, struct FSFile*); // offset 20
    void (*Activate)(struct FSArchive*); // offset 24
    void (*Idle)(struct FSArchive*); // offset 28
    void (*Suspend)(struct FSArchive*); // offset 2c
    void (*Resume)(struct FSArchive*); // offset 30
    enum FSResult (*OpenFile)(struct FSArchive*, struct FSFile*, u32, char*, u32); // offset 34
    enum FSResult (*SeekFile)(struct FSArchive*, struct FSFile*, int*, enum FSSeekFileMode); // offset 38
    enum FSResult (*GetFileLength)(struct FSArchive*, struct FSFile*, u32*); // offset 3c
    enum FSResult (*GetFilePosition)(struct FSArchive*, struct FSFile*, u32*); // offset 40
    void (*Mount)(struct FSArchive*); // offset 44
    void (*Unmount)(struct FSArchive*); // offset 48
    enum FSResult (*GetArchiveCaps)(struct FSArchive*, u32*); // offset 4c
    enum FSResult (*CreateFile)(struct FSArchive*, u32, char*, u32); // offset 50
    enum FSResult (*DeleteFile)(struct FSArchive*, u32, char*); // offset 54
    enum FSResult (*RenameFile)(struct FSArchive*, u32, char*, u32, char*); // offset 58
    enum FSResult (*GetPathInfo)(struct FSArchive*, u32, char*, struct FSPathInfo*); // offset 5c
    enum FSResult (*SetPathInfo)(struct FSArchive*, u32, char*, struct FSPathInfo*); // offset 60
    enum FSResult (*CreateDirectory)(struct FSArchive*, u32, char*, u32); // offset 64
    enum FSResult (*DeleteDirectory)(struct FSArchive*, u32, char*); // offset 68
    enum FSResult (*RenameDirectory)(struct FSArchive*, u32, char*, u32, char*); // offset 6c
    enum FSResult (*GetArchiveResource)(struct FSArchive*, struct FSArchiveResource*); // offset 70
    void* unused_29; // offset 74
    enum FSResult (*FlushFile)(struct FSArchive*, struct FSFile*); // offset 78
    enum FSResult (*SetFileLength)(struct FSArchive*, struct FSFile*, u32); // offset 7c
    enum FSResult (*OpenDirectory)(struct FSArchive*, struct FSFile*, u32, char*, u32); // offset 80
    enum FSResult (*CloseDirectory)(struct FSArchive*, struct FSFile*); // offset 84
    enum FSResult (*SetSeekCache)(struct FSArchive*, struct FSFile*, void*, u32); // offset 88
    u8 reserved[116]; // offset 8c
};
struct CARDRomHeader {
    char game_name[12]; // offset 00
    u32 game_code; // offset 0c
    u16 maker_code; // offset 10
    u8 product_id; // offset 12
    u8 device_type; // offset 13
    u8 device_size; // offset 14
    u8 reserved_A[9]; // offset 15
    u8 game_version; // offset 1e
    u8 property; // offset 1f
    void* main_rom_offset; // offset 20
    void* main_entry_address; // offset 24
    void* main_ram_address; // offset 28
    u32 main_size; // offset 2c
    void* sub_rom_offset; // offset 30
    void* sub_entry_address; // offset 34
    void* sub_ram_address; // offset 38
    u32 sub_size; // offset 3c
    struct CARDRomRegion fnt; // offset 40
    struct CARDRomRegion fat; // offset 48
    struct CARDRomRegion main_ovt; // offset 50
    struct CARDRomRegion sub_ovt; // offset 58
    u8 rom_param_A[8]; // offset 60
    u32 banner_offset; // offset 68
    u16 secure_crc; // offset 6c
    u8 rom_param_B[2]; // offset 6e
    void* main_autoload_done; // offset 70
    void* sub_autoload_done; // offset 74
    u8 rom_param_C[8]; // offset 78
    u32 rom_size; // offset 80
    u32 header_size; // offset 84
    u32 main_module_param_offset; // offset 88
    u32 sub_module_param_offset; // offset 8c
    u16 normal_area_rom_offset; // offset 90
    u16 twl_ltd_area_rom_offset; // offset 92
    u8 reserved_B[44]; // offset 94
    u8 logo_data[156]; // offset c0
    u16 logo_crc; // offset 15c
    u16 header_crc; // offset 15e
};
typedef struct CARDRomHeader CARDRomHeaderNTR;
typedef struct CARDRomHeader CARDRomHeader;
struct FSArchiveFAT {
    u32 top; // offset 00
    u32 bottom; // offset 04
};
typedef struct FSArchiveFAT FSArchiveFAT;
struct MWiDestructorChain {
    struct MWiDestructorChain* next; // offset 00
    void (*dtor)(void*); // offset 04
    void* obj; // offset 08
};
typedef struct MWiDestructorChain MWiDestructorChain;
struct CARDEventListener {
    OSEvent event[1]; // offset 00
    struct OSiVAlarm valarm[1]; // offset 0c
    int (*Condition)(void*); // offset 34
    void* userdata; // offset 38
    u16 lockID; // offset 3c
    u8 padding[2]; // offset 3e
};
typedef struct CARDEventListener CARDEventListener;
typedef u32 CARDAccessLevel;
typedef long CARDiOwner;
struct CARDTaskQueue {
    struct CARDTask* list; // offset 00
    struct _OSThreadQueue workers[1]; // offset 04
    u32 quit : 1; // offset 0c
    u32 dummy : 31; // offset 0c
};
enum _enum_84897 {
    CARD_TARGET_NONE = 0,
    CARD_TARGET_ROM = 1,
    CARD_TARGET_BACKUP = 2,
    CARD_TARGET_RW = 3,
};
struct CARDTask {
    struct CARDTask* next; // offset 00
    u32 priority; // offset 04
    void* userdata; // offset 08
    void (*function)(struct CARDTask*); // offset 0c
    void (*callback)(struct CARDTask*); // offset 10
};
struct CARDiCommon {
    struct CARDiCommandArg* cmd; // offset 00
    u32 flag; // offset 04
    u32 priority; // offset 08
    long lock_owner; // offset 0c
    int lock_ref; // offset 10
    struct _OSThreadQueue lock_queue[1]; // offset 14
    enum _enum_84897 lock_target; // offset 1c
    struct {
        struct _OSThread context[1]; // offset 00
        u8 stack[1024]; // offset a4
    } thread; // offset 20
    struct CARDTask task[1]; // offset 4c4
    struct CARDTaskQueue task_q[1]; // offset 4d8
    int command; // offset 4e8
    u8 padding[20]; // offset 4ec
};
typedef struct CARDiCommon CARDiCommon;
enum CARDResult {
    CARD_RESULT_SUCCESS = 0,
    CARD_RESULT_FAILURE = 1,
    CARD_RESULT_INVALID_PARAM = 2,
    CARD_RESULT_UNSUPPORTED = 3,
    CARD_RESULT_TIMEOUT = 4,
    CARD_RESULT_ERROR = 5,
    CARD_RESULT_NO_RESPONSE = 6,
    CARD_RESULT_CANCELED = 7,
};
enum CARDBackupType {
    CARD_BACKUP_TYPE_EEPROM_4KBITS = 2305,
    CARD_BACKUP_TYPE_EEPROM_64KBITS = 3329,
    CARD_BACKUP_TYPE_EEPROM_512KBITS = 4097,
    CARD_BACKUP_TYPE_EEPROM_1MBITS = 4353,
    CARD_BACKUP_TYPE_FLASH_2MBITS = 4610,
    CARD_BACKUP_TYPE_FLASH_4MBITS = 4866,
    CARD_BACKUP_TYPE_FLASH_8MBITS = 5122,
    CARD_BACKUP_TYPE_FLASH_16MBITS = 5378,
    CARD_BACKUP_TYPE_FLASH_64MBITS = 5890,
    CARD_BACKUP_TYPE_FRAM_256KBITS = 3843,
    CARD_BACKUP_TYPE_NOT_USE = 0,
};
struct CARDiCommandArg {
    enum CARDResult result; // offset 00
    enum CARDBackupType type; // offset 04
    u32 id; // offset 08
    u32 src; // offset 0c
    u32 dst; // offset 10
    u32 len; // offset 14
    struct {
        u32 total_size; // offset 00
        u32 sect_size; // offset 04
        u32 subsect_size; // offset 08
        u32 page_size; // offset 0c
        u32 addr_width; // offset 10
        u32 program_page; // offset 14
        u32 write_page; // offset 18
        u32 write_page_total; // offset 1c
        u32 erase_chip; // offset 20
        u32 erase_chip_total; // offset 24
        u32 erase_sector; // offset 28
        u32 erase_sector_total; // offset 2c
        u32 erase_subsector; // offset 30
        u32 erase_subsector_total; // offset 34
        u32 erase_page; // offset 38
        u8 initial_status; // offset 3c
        u8 padding1[3]; // offset 3d
        u32 caps; // offset 40
        u8 padding2[4]; // offset 44
    } spec; // offset 18
};
typedef struct CARDiCommandArg CARDiCommandArg;
typedef struct CARDTask CARDTask;
typedef struct CARDTaskQueue CARDTaskQueue;
struct CARDHookContext {
    struct CARDHookContext* next; // offset 00
    void* userdata; // offset 04
    void (*callback)(void*, u32, void*); // offset 08
};
typedef struct CARDHookContext CARDHookContext;
struct CARDDmaInterface {
    void (*Recv)(u32, void*, void*, u32); // offset 00
    void (*Stop)(u32); // offset 04
};
typedef struct CARDDmaInterface CARDDmaInterface;
typedef struct {
    u32 rest_comm; // offset 00
    u32 src; // offset 04
    u32 dst; // offset 08
    int cmp; // offset 0c
} CARDiParam;
struct dst_string_tag {
    u32 len; // offset 00
    char* cur; // offset 04
    char* base; // offset 08
};
typedef struct dst_string_tag dst_string;
struct CTRDGWork {
    u16 subpInitialized; // offset 00
    u16 lockID; // offset 02
};
typedef struct CTRDGWork CTRDGWork;
struct CTRDGRomCycle {
    enum _enum_29898 c1; // offset 00
    enum _enum_30021 c2; // offset 04
};
typedef struct CTRDGRomCycle CTRDGRomCycle;
struct CTRDGLockByProc {
    int locked; // offset 00
    u32 irq; // offset 04
};
typedef struct CTRDGLockByProc CTRDGLockByProc;
enum _enum_95311 {
    VIB_STOP = 0,
    VIB_START = 2,
};
enum CTRDGStatus {
    CTRDG_STATUS_READY = 0,
    CTRDG_STATUS_INIT_MODULE_INFO = 1,
};
struct CTRDGWorkSp {
    union {
        u32 cmd32[4]; // offset 00
        u16 cmd16[8]; // offset 00
    }; // offset 00
    enum CTRDGStatus status; // offset 10
    u16 count; // offset 14
    u16 pad[1]; // offset 16
};
typedef struct CTRDGWorkSp CTRDGWorkSp;
typedef struct {
    u32 startAddress; // offset 00
    u8 nintendoLogo[156]; // offset 04
    char titleName[12]; // offset a0
    u32 gameCode; // offset ac
    u16 makerCode; // offset b0
    u8 isRomCode; // offset b2
    u8 machineCode; // offset b3
    u8 deviceType; // offset b4
    u8 exLsiID[3]; // offset b5
    u8 reserved_A[4]; // offset b8
    u8 softVersion; // offset bc
    u8 complement; // offset bd
    u16 moduleID; // offset be
} CTRDGHeader;
enum _enum_96095 {
    CTRDG_PHI_CLOCK_LOW = 0,
    CTRDG_PHI_CLOCK_4MHZ = 1,
    CTRDG_PHI_CLOCK_8MHZ = 2,
    CTRDG_PHI_CLOCK_16MHZ = 3,
};
struct RTCWork {
    struct OSMessageQueue msgQ; // offset 00
    void* msgArray[4]; // offset 20
    struct _OSThread thread; // offset 30
    u64 stack[32]; // offset d4
    int busy; // offset 1d4
    u16 command; // offset 1d8
    u16 padding; // offset 1da
    struct _OSThread polling; // offset 1dc
    struct _OSThreadQueue pollingQ; // offset 280
    u64 pollingStack[32]; // offset 288
    struct OSiAlarm pollingAlarm; // offset 388
};
typedef struct RTCWork RTCWork;
enum RTCPxiResult {
    RTC_PXI_RESULT_SUCCESS = 0,
    RTC_PXI_RESULT_INVALID_COMMAND = 1,
    RTC_PXI_RESULT_ILLEGAL_STATUS = 2,
    RTC_PXI_RESULT_BUSY = 3,
    RTC_PXI_RESULT_FATAL_ERROR = 4,
    RTC_PXI_RESULT_MAX = 5,
};
struct RTCRawStatus2 {
    u16 intr_mode : 4; // offset 00
    u16 dummy0 : 2; // offset 00
    u16 intr2_mode : 1; // offset 00
    u16 test : 1; // offset 00
    u16 dummy1 : 8; // offset 00
};
typedef struct RTCRawStatus2 RTCRawStatus2;
struct RTCRawStatus1 {
    u16 reset : 1; // offset 00
    u16 format : 1; // offset 00
    u16 dummy0 : 2; // offset 00
    u16 intr1 : 1; // offset 00
    u16 intr2 : 1; // offset 00
    u16 bld : 1; // offset 00
    u16 poc : 1; // offset 00
    u16 dummy1 : 8; // offset 00
};
typedef struct RTCRawStatus1 RTCRawStatus1;
struct RTCRawAlarm {
    u32 week : 3; // offset 00
    u32 dummy0 : 4; // offset 00
    u32 we : 1; // offset 00
    u32 hour : 6; // offset 00
    u32 afternoon : 1; // offset 00
    u32 he : 1; // offset 00
    u32 minute : 7; // offset 00
    u32 me : 1; // offset 00
    u32 dummy2 : 8; // offset 00
};
typedef struct RTCRawAlarm RTCRawAlarm;
struct RTCRawTime {
    u32 hour : 6; // offset 00
    u32 afternoon : 1; // offset 00
    u32 dummy0 : 1; // offset 00
    u32 minute : 7; // offset 00
    u32 dummy1 : 1; // offset 00
    u32 second : 7; // offset 00
    u32 dummy2 : 9; // offset 00
};
struct RTCRawAdjust {
    u32 adjust : 8; // offset 00
    u32 dummy : 24; // offset 00
};
struct RTCRawPulse {
    u32 pulse : 5; // offset 00
    u32 dummy : 27; // offset 00
};
struct RTCRawDate {
    u32 year : 8; // offset 00
    u32 month : 5; // offset 00
    u32 dummy0 : 3; // offset 00
    u32 day : 6; // offset 00
    u32 dummy1 : 2; // offset 00
    u32 week : 3; // offset 00
    u32 dummy2 : 5; // offset 00
};
struct RTCRawFree {
    u32 free : 8; // offset 00
    u32 dummy : 24; // offset 00
};
union RTCRawData {
    struct {
        struct RTCRawDate date; // offset 00
        struct RTCRawTime time; // offset 04
    } t; // offset 00
    struct {
        struct RTCRawStatus1 status1; // offset 00
        struct RTCRawStatus2 status2; // offset 02
        union {
            struct RTCRawAlarm alarm; // offset 00
        }; // offset 04
    } a; // offset 00
    u32 words[2]; // offset 00
    u16 halfs[4]; // offset 00
    u8 bytes[8]; // offset 00
};
typedef union RTCRawData RTCRawData;
typedef struct RTCRawDate RTCRawDate;
typedef struct RTCRawTime RTCRawTime;
typedef struct RTCRawPulse RTCRawPulse;
typedef struct RTCRawAdjust RTCRawAdjust;
typedef struct RTCRawFree RTCRawFree;
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
} WlStaElement;
struct WmInit {
    u32 dmaNo; // offset 00
    u32 indPrio_low; // offset 04
    u32 indPrio_high; // offset 08
    u32 reqPrio_low; // offset 0c
    u32 reqPrio_high; // offset 10
    u32 wlPrio_low; // offset 14
    u32 wlPrio_high; // offset 18
};
typedef struct WmInit WmInit;
typedef struct {
    u32 workingMemAdrs; // offset 00
    void* stack; // offset 04
    u32 stacksize; // offset 08
    u32 priority; // offset 0c
    struct OSMessageQueue* sendMsgQueuep; // offset 10
    struct OSMessageQueue* recvMsgQueuep; // offset 14
    u32 dmaChannel; // offset 18
    u32 dmaMaxSize; // offset 1c
    u32 heapType; // offset 20
    union {
        struct {
            enum _enum_16263 id; // offset 00
            int heapHandle; // offset 04
        } os; // offset 00
        struct {
            u32 (*alloc)(u32); // offset 00
            u32 (*free)(void*); // offset 04
        } ext; // offset 00
    } heapFunc; // offset 24
    void* camAdrs; // offset 2c
    u32 camSize; // offset 30
} WlInit;
struct WMCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 wlCmdID; // offset 04
    u16 wlResult; // offset 06
};
typedef struct WMCallback WMCallback;
struct WMSPWork {
    struct OSMessageQueue toWLmsgQ; // offset 00
    void* toWLmsg[2]; // offset 20
    struct OSMessageQueue fromWLmsgQ; // offset 28
    void* fromWLmsg[4]; // offset 48
    struct OSMessageQueue confirmQ; // offset 58
    void* confirm[4]; // offset 78
    struct OSMessageQueue requestQ; // offset 88
    void* request[32]; // offset a8
    u32 requestStack[1024]; // offset 128
    u32 indicateStack[256]; // offset 1128
    struct OSMutex fifoExclusive; // offset 1528
    u32 dmaNo; // offset 1540
    enum _enum_16263 arenaId; // offset 1544
    int heapHandle; // offset 1548
    struct WMArm7Buf* wm7buf; // offset 154c
    struct WMStatus* status; // offset 1550
    u8 rssiHistory[32]; // offset 1554
    u32 rssiIndex; // offset 1574
    u32 indPrio_high; // offset 1578
    u32 wlPrio_high; // offset 157c
    u32 reqPrio_high; // offset 1580
    u32 indPrio_low; // offset 1584
    u32 wlPrio_low; // offset 1588
    u32 reqPrio_low; // offset 158c
};
typedef struct WMSPWork WMSPWork;
struct WMGameInfo {
    u16 magicNumber; // offset 00
    u8 ver; // offset 02
    u8 platform; // offset 03
    u32 ggid; // offset 04
    u16 tgid; // offset 08
    u8 userGameInfoLength; // offset 0a
    union {
        u8 gameNameCount_attribute; // offset 00
        u8 attribute; // offset 00
    }; // offset 0b
    u16 parentMaxSize; // offset 0c
    u16 childMaxSize; // offset 0e
    union {
        u16 userGameInfo[56]; // offset 00
        struct {
            u16 userName[4]; // offset 00
            u16 gameName[8]; // offset 08
            u16 padd1[44]; // offset 18
        } old_type; // offset 00
    }; // offset 10
};
struct WMBssDesc {
    u16 length; // offset 00
    u16 rssi; // offset 02
    u8 bssid[6]; // offset 04
    u16 ssidLength; // offset 0a
    u8 ssid[32]; // offset 0c
    u16 capaInfo; // offset 2c
    struct {
        u16 basic; // offset 00
        u16 support; // offset 02
    } rateSet; // offset 2e
    u16 beaconPeriod; // offset 32
    u16 dtimPeriod; // offset 34
    u16 channel; // offset 36
    u16 cfpPeriod; // offset 38
    u16 cfpMaxDuration; // offset 3a
    u16 gameInfoLength; // offset 3c
    u16 otherElementCount; // offset 3e
    struct WMGameInfo gameInfo; // offset 40
};
struct WMArm7Buf {
    struct WMStatus* status; // offset 00
    u8 reserved_a[4]; // offset 04
    u32* fifo7to9; // offset 08
    u8 reserved_b[4]; // offset 0c
    struct WMBssDesc connectPInfo; // offset 10
    u32 requestBuf[128]; // offset d0
};
typedef struct WMArm7Buf WMarm7Buf;
typedef struct WMArm7Buf WMArm7Buf;
struct WMParentParam {
    u16* userGameInfo; // offset 00
    u16 userGameInfoLength; // offset 04
    u16 padding; // offset 06
    u32 ggid; // offset 08
    u16 tgid; // offset 0c
    u16 entryFlag; // offset 0e
    u16 maxEntry; // offset 10
    u16 multiBootFlag; // offset 12
    u16 KS_Flag; // offset 14
    u16 CS_Flag; // offset 16
    u16 beaconPeriod; // offset 18
    u16 rsv1[4]; // offset 1a
    u16 rsv2[8]; // offset 22
    u16 channel; // offset 32
    u16 parentMaxSize; // offset 34
    u16 childMaxSize; // offset 36
    u16 rsv[4]; // offset 38
};
typedef struct {
    u16 head; // offset 00
    u16 tail; // offset 02
} WMPortSendQueue;
typedef struct {
    u16 next; // offset 00
    u16 port; // offset 02
    u16 destBitmap; // offset 04
    u16 restBitmap; // offset 06
    u16 sentBitmap; // offset 08
    u16 sendingBitmap; // offset 0a
    u16 padding; // offset 0c
    u16 size; // offset 0e
    u16 seqNo; // offset 10
    u16 retryCount; // offset 12
    u16* data; // offset 14
    void (*callback)(void*); // offset 18
    void* arg; // offset 1c
} WMPortSendQueueData;
struct WMStatus {
    u16 state; // offset 00
    u16 BusyApiid; // offset 02
    int apiBusy; // offset 04
    int scan_continue; // offset 08
    int mp_flag; // offset 0c
    int dcf_flag; // offset 10
    int ks_flag; // offset 14
    int dcf_sendFlag; // offset 18
    int VSyncFlag; // offset 1c
    u8 wlVersion[8]; // offset 20
    u16 macVersion; // offset 28
    u16 rfVersion; // offset 2a
    u16 bbpVersion[2]; // offset 2c
    u16 mp_parentSize; // offset 30
    u16 mp_childSize; // offset 32
    u16 mp_parentMaxSize; // offset 34
    u16 mp_childMaxSize; // offset 36
    u16 mp_sendSize; // offset 38
    u16 mp_recvSize; // offset 3a
    u16 mp_maxSendSize; // offset 3c
    u16 mp_maxRecvSize; // offset 3e
    u16 mp_parentVCount; // offset 40
    u16 mp_childVCount; // offset 42
    u16 mp_parentInterval; // offset 44
    u16 mp_childInterval; // offset 46
    u64 mp_parentIntervalTick; // offset 48
    u64 mp_childIntervalTick; // offset 50
    u16 mp_minFreq; // offset 58
    u16 mp_freq; // offset 5a
    u16 mp_maxFreq; // offset 5c
    u16 mp_vsyncOrderedFlag; // offset 5e
    u16 mp_vsyncFlag; // offset 60
    s16 mp_count; // offset 62
    s16 mp_limitCount; // offset 64
    u16 mp_resumeFlag; // offset 66
    u16 mp_prevPollBitmap; // offset 68
    u16 mp_prevWmHeader; // offset 6a
    u16 mp_prevTxop; // offset 6c
    u16 mp_prevDataLength; // offset 6e
    u16 mp_recvBufSel; // offset 70
    u16 mp_recvBufSize; // offset 72
    struct WMMpRecvBuf* mp_recvBuf[2]; // offset 74
    u32* mp_sendBuf; // offset 7c
    u16 mp_sendBufSize; // offset 80
    u16 mp_ackTime; // offset 82
    u16 mp_waitAckFlag; // offset 84
    u16 mp_readyBitmap; // offset 86
    u16 mp_newFrameFlag; // offset 88
    u8 reserved_b[2]; // offset 8a
    u16 mp_sentDataFlag; // offset 8c
    u16 mp_bufferEmptyFlag; // offset 8e
    u16 mp_isPolledFlag; // offset 90
    u16 mp_minPollBmpMode; // offset 92
    u16 mp_singlePacketMode; // offset 94
    u8 reserved_c[2]; // offset 96
    u16 mp_defaultRetryCount; // offset 98
    u16 mp_ignoreFatalErrorMode; // offset 9a
    u16 mp_ignoreSizePrecheckMode; // offset 9c
    u16 mp_pingFlag; // offset 9e
    u16 mp_pingCounter; // offset a0
    u8 dcf_destAdr[6]; // offset a2
    u16* dcf_sendData; // offset a8
    u16 dcf_sendSize; // offset ac
    u16 dcf_recvBufSel; // offset ae
    struct WMDcfRecvBuf* dcf_recvBuf[2]; // offset b0
    u16 dcf_recvBufSize; // offset b8
    u16 curr_tgid; // offset ba
    u16 linkLevel; // offset bc
    u16 minRssi; // offset be
    u16 rssiCounter; // offset c0
    u16 beaconIndicateFlag; // offset c2
    u16 wepKeyId; // offset c4
    u16 pwrMgtMode; // offset c6
    u32 miscFlags; // offset c8
    u16 VSyncBitmap; // offset cc
    u16 valarm_queuedFlag; // offset ce
    u32 v_tsf; // offset d0
    u32 v_tsf_bak; // offset d4
    u32 v_remain; // offset d8
    u16 valarm_counter; // offset dc
    u8 reserved_e[2]; // offset de
    u8 MacAddress[6]; // offset e0
    u16 mode; // offset e6
    struct WMParentParam pparam; // offset e8
    u8 childMacAddress[15][6]; // offset 128
    u16 child_bitmap; // offset 182
    struct WMBssDesc* pInfoBuf; // offset 184
    u16 aid; // offset 188
    u8 parentMacAddress[6]; // offset 18a
    u16 scan_channel; // offset 190
    u8 reserved_f[4]; // offset 192
    u16 wepMode; // offset 196
    int wep_flag; // offset 198
    u16 wepKey[40]; // offset 19c
    u16 rate; // offset 1ec
    u16 preamble; // offset 1ee
    u16 tmptt; // offset 1f0
    u16 retryLimit; // offset 1f2
    u16 enableChannel; // offset 1f4
    u16 allowedChannel; // offset 1f6
    u16 portSeqNo[16][8]; // offset 1f8
    WMPortSendQueueData sendQueueData[32]; // offset 2f8
    WMPortSendQueue sendQueueFreeList; // offset 6f8
    WMPortSendQueue sendQueue[4]; // offset 6fc
    WMPortSendQueue readyQueue[4]; // offset 70c
    struct OSMutex sendQueueMutex; // offset 71c
    int sendQueueInUse; // offset 734
    u64 mp_lastRecvTick[16]; // offset 738
    u64 mp_lifeTimeTick; // offset 7b8
    u16 mp_current_minFreq; // offset 7c0
    u16 mp_current_freq; // offset 7c2
    u16 mp_current_maxFreq; // offset 7c4
    u16 mp_current_minPollBmpMode; // offset 7c6
    u16 mp_current_singlePacketMode; // offset 7c8
    u16 mp_current_defaultRetryCount; // offset 7ca
    u16 mp_current_ignoreFatalErrorMode; // offset 7cc
    u8 reserved_g[2]; // offset 7ce
};
typedef struct WMStatus WMStatus;
typedef struct WMStatus WMstatus;
struct WMMpRecvBuf {
    u16 rsv1[3]; // offset 00
    u16 length; // offset 06
    u16 rsv2[1]; // offset 08
    u16 ackTimeStamp; // offset 0a
    u16 timeStamp; // offset 0c
    u16 rate_rssi; // offset 0e
    u16 rsv3[2]; // offset 10
    u16 rsv4[2]; // offset 14
    u8 destAdrs[6]; // offset 18
    u8 srcAdrs[6]; // offset 1e
    u16 rsv5[3]; // offset 24
    u16 seqCtrl; // offset 2a
    u16 txop; // offset 2c
    u16 bitmap; // offset 2e
    u16 wmHeader; // offset 30
    u16 data[2]; // offset 32
};
typedef struct WMMpRecvBuf WMmpRecvBuf;
typedef struct WMMpRecvBuf WMMpRecvBuf;
struct WMDcfRecvBuf {
    u16 frameID; // offset 00
    u16 rsv1[2]; // offset 02
    u16 length; // offset 06
    u16 rsv2[3]; // offset 08
    u16 rate_rssi; // offset 0e
    u16 rsv3[4]; // offset 10
    u8 destAdrs[6]; // offset 18
    u8 srcAdrs[6]; // offset 1e
    u16 rsv4[4]; // offset 24
    u16 data[2]; // offset 2c
};
typedef struct WMDcfRecvBuf WMDcfRecvBuf;
typedef struct WMDcfRecvBuf WMdcfRecvBuf;
typedef struct WMParentParam WMpparam;
typedef struct WMParentParam WMParentParam;
typedef struct WMBssDesc WMbssDesc;
typedef struct WMBssDesc WMBssDesc;
typedef struct WMGameInfo WMGameInfo;
typedef struct WMGameInfo WMgameInfo;
struct WMIndCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 state; // offset 04
    u16 reason; // offset 06
};
typedef struct WMIndCallback WMindCallback;
typedef struct WMIndCallback WMIndCallback;
struct WMMpRecvData {
    u16 length; // offset 00
    u16 rate_rssi; // offset 02
    u16 aid; // offset 04
    u16 noResponse; // offset 06
    u16 wmHeader; // offset 08
    u16 cdata[1]; // offset 0a
};
typedef struct WMMpRecvData WMmpRecvData;
typedef struct WMMpRecvData WMMpRecvData;
struct WMMpRecvHeader {
    u16 bitmap; // offset 00
    u16 errBitmap; // offset 02
    u16 count; // offset 04
    u16 length; // offset 06
    u16 txCount; // offset 08
    struct WMMpRecvData data[1]; // offset 0a
};
typedef struct WMMpRecvHeader WMmpRecvHeader;
typedef struct WMMpRecvHeader WMMpRecvHeader;
typedef struct {
    u16 code; // offset 00
    u16 length; // offset 02
} WlCmdHeader;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 staMacAdrs[3]; // offset 10
    u16 retryLimit; // offset 16
    u16 enableChannel; // offset 18
    u16 rsv; // offset 1a
    u16 mode; // offset 1c
    u16 rate; // offset 1e
    u16 wepMode; // offset 20
    u16 wepKeyId; // offset 22
    u16 wepKey[4][10]; // offset 24
    u16 beaconType; // offset 74
    u16 probeRes; // offset 76
    u16 beaconLostTh; // offset 78
    u16 activeZoneTime; // offset 7a
    u8 ssidMask[32]; // offset 7c
    u16 preambleType; // offset 9c
    u16 authAlgo; // offset 9e
} WlParamSetAllReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlParamSetCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 buf[2]; // offset 10
} WlCmdReq;
typedef struct {
    u8 rsv1[6]; // offset 00
    u16 length; // offset 06
    u16 txKeySts; // offset 08
    u16 rsv3; // offset 0a
    u16 timeStamp; // offset 0c
    u8 rate; // offset 0e
    u8 rssi; // offset 0f
    u8 rsv4[4]; // offset 10
    u8 rsv5[4]; // offset 14
    u16 destAdrs[3]; // offset 18
    u16 srcAdrs[3]; // offset 1e
    u8 rsv6[6]; // offset 24
    u16 seqCtrl; // offset 2a
    u16 tmptt; // offset 2c
    u16 bitmap; // offset 2e
} WlRxMpAckFrame;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    WlRxMpAckFrame ack; // offset 10
} WlMaMpAckInd;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMlmeResetCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 mib; // offset 10
} WlMlmeResetReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMlmePowerManagementCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMlmePowerMgtCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 pwrMgtMode; // offset 10
    u16 wakeUp; // offset 12
    u16 recieveDtims; // offset 14
} WlMlmePowerManagementReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 pwrMgtMode; // offset 10
    u16 wakeUp; // offset 12
    u16 recieveDtims; // offset 14
} WlMlmePowerMgtReq;
union _element {
    u8 gameInfo[4]; // offset 00
    u8 otherElement[4]; // offset 00
};
typedef struct {
    u16 length; // offset 00
    u16 rssi; // offset 02
    u16 bssid[3]; // offset 04
    u16 ssidLength; // offset 0a
    u8 ssid[32]; // offset 0c
    u16 capaInfo; // offset 2c
    struct {
        u16 basic; // offset 00
        u16 support; // offset 02
    } rateSet; // offset 2e
    u16 beaconPeriod; // offset 32
    u16 dtimPeriod; // offset 34
    u16 channel; // offset 36
    u16 cfpPeriod; // offset 38
    u16 cfpMaxDuration; // offset 3a
    u16 gameInfoLength; // offset 3c
    u16 otherElementCount; // offset 3e
    union _element; // offset 40
} WlBssDesc;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 foundMap; // offset 06
    u16 bssDescCount; // offset 08
    WlBssDesc bssDescList[1]; // offset 0a
} WlMlmeScanCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 bssid[3]; // offset 10
    u16 ssidLength; // offset 16
    u8 ssid[32]; // offset 18
    u16 scanType; // offset 38
    u8 channelList[16]; // offset 3a
    u16 maxChannelTime; // offset 4a
    u16 bssidMaskCount; // offset 4c
    u16 bssidMask[3]; // offset 4e
} WlMlmeScanReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 statusCode; // offset 06
    u16 peerMacAdrs[3]; // offset 08
} WlMlmeJoinCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 timeOut; // offset 10
    u16 rsv; // offset 12
    WlBssDesc bssDesc; // offset 14
} WlMlmeJoinReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 statusCode; // offset 06
    u16 peerMacAdrs[3]; // offset 08
    u16 algorithm; // offset 0e
} WlMlmeAuthenticateCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 statusCode; // offset 06
    u16 peerMacAdrs[3]; // offset 08
    u16 algorithm; // offset 0e
} WlMlmeAuthCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 algorithm; // offset 16
    u16 timeOut; // offset 18
} WlMlmeAuthenticateReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 algorithm; // offset 16
    u16 timeOut; // offset 18
} WlMlmeAuthReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 peerMacAdrs[3]; // offset 06
} WlMlmeDeAuthCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 peerMacAdrs[3]; // offset 06
} WlMlmeDeAuthenticateCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 reasonCode; // offset 16
} WlMlmeDeAuthReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 reasonCode; // offset 16
} WlMlmeDeAuthenticateReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 statusCode; // offset 06
    u16 aid; // offset 08
} WlMlmeAssCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 statusCode; // offset 06
    u16 aid; // offset 08
} WlMlmeAssociateCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 listenInterval; // offset 16
    u16 timeOut; // offset 18
} WlMlmeAssReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 listenInterval; // offset 16
    u16 timeOut; // offset 18
} WlMlmeAssociateReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 statusCode; // offset 06
    u16 aid; // offset 08
} WlMlmeReAssociateCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 statusCode; // offset 06
    u16 aid; // offset 08
} WlMlmeReAssCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 newApMacAdrs[3]; // offset 10
    u16 listenInterval; // offset 16
    u16 timeOut; // offset 18
} WlMlmeReAssociateReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 newApMacAdrs[3]; // offset 10
    u16 listenInterval; // offset 16
    u16 timeOut; // offset 18
} WlMlmeReAssReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMlmeDisAssociateCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMlmeDisAssCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 reasonCode; // offset 16
} WlMlmeDisAssociateReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 reasonCode; // offset 16
} WlMlmeDisAssReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMlmeStartCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 ssidLength; // offset 10
    u8 ssid[32]; // offset 12
    u16 beaconPeriod; // offset 32
    u16 dtimPeriod; // offset 34
    u16 channel; // offset 36
    u16 basicRateSet; // offset 38
    u16 supportRateSet; // offset 3a
    u16 gameInfoLength; // offset 3c
    u8 gameInfo[2]; // offset 3e
} WlMlmeStartReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 reserved; // offset 06
    u16 ccaBusyInfo[16]; // offset 08
} WlMlmeMeasChanCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 reserved; // offset 06
    u16 ccaBusyInfo[16]; // offset 08
} WlMlmeMeasureChannelCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 rsv; // offset 10
    u16 ccaMode; // offset 12
    u16 edThreshold; // offset 14
    u16 measureTime; // offset 16
    u8 channelList[16]; // offset 18
} WlMlmeMeasChanReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 rsv; // offset 10
    u16 ccaMode; // offset 12
    u16 edThreshold; // offset 14
    u16 measureTime; // offset 16
    u8 channelList[16]; // offset 18
} WlMlmeMeasureChannelReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 txStatus; // offset 06
} WlMaDataCfm;
typedef struct {
    u16 frameId; // offset 00
    u8 rsv1[4]; // offset 02
    u16 length; // offset 06
    u16 status; // offset 08
    u16 rsvm1; // offset 0a
    u16 rsvm2; // offset 0c
    u8 rate; // offset 0e
    u8 rssi; // offset 0f
    u32 rsvm3; // offset 10
    u8 rsv4[4]; // offset 14
    u16 destAdrs[3]; // offset 18
    u16 srcAdrs[3]; // offset 1e
    u8 rsv5[8]; // offset 24
    u16* datap; // offset 2c
} WlTxFrame;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    WlTxFrame frame; // offset 10
} WlMaDataReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMaKeyDataCfm;
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
} WlMaMpCfm;
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
} WlMaClearDataCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlMaClrDataCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 flag; // offset 10
} WlMaClearDataReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 flag; // offset 10
} WlMaClrDataReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 staMacAdrs[3]; // offset 10
} WlParamSetMacAdrsReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 staMacAdrs[3]; // offset 10
} WlParamSetMacAddressReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 retryLimit; // offset 10
} WlParamSetRetryLimitReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 enableChannel; // offset 10
} WlParamSetEnableChannelReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 mode; // offset 10
} WlParamSetModeReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 rate; // offset 10
} WlParamSetRateReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 wepMode; // offset 10
} WlParamSetWepModeReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 wepKeyId; // offset 10
} WlParamSetWepKeyIdReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 wepKey[4][10]; // offset 10
} WlParamSetWepKeyReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 beaconType; // offset 10
} WlParamSetBeaconTypeReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 probeRes; // offset 10
} WlParamSetProbeResReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 probeRes; // offset 10
} WlParamSetProbeResponseReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 beaconLostTh; // offset 10
} WlParamSetBeaconLostThresholdReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 beaconLostTh; // offset 10
} WlParamSetBeaconLostThReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 activeZoneTime; // offset 10
} WlParamSetActiveZoneReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u8 mask[32]; // offset 10
} WlParamSetSsidMaskReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 type; // offset 10
} WlParamSetPreambleTypeReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 type; // offset 10
} WlParamSetAuthenticationAlgorithmReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 type; // offset 10
} WlParamSetAuthAlgoReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 ccaMode; // offset 10
    u16 edThreshold; // offset 12
    u16 agcLimit; // offset 14
} WlParamSetCCAModeEDThReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 ccaMode; // offset 10
    u16 edThreshold; // offset 12
    u16 agcLimit; // offset 14
} WlParamSetCCAModeEDThresholdReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 tableNumber; // offset 10
    u16 camLifeTime; // offset 12
    u16 frameLifeTime; // offset 14
} WlParamSetLifeTimeReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 count; // offset 10
} WlParamSetMaxConnReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 count; // offset 10
} WlParamSetMaxConnectableChildReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 mainAntenna; // offset 10
} WlParamSetMainAntennaReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 diversity; // offset 10
    u16 useAntenna; // offset 12
} WlParamSetDiversityReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 enableMessage; // offset 10
} WlParamSetBeaconSendRecvIndReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 mode; // offset 10
} WlParamSetNullKeyModeReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 bssid[3]; // offset 10
} WlParamSetBssidReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 ssidLength; // offset 10
    u8 ssid[32]; // offset 12
} WlParamSetSsidReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 beaconPeriod; // offset 10
} WlParamSetBeaconPeriodReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 dtimPeriod; // offset 10
} WlParamSetDtimPeriodReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 listenInterval; // offset 10
} WlParamSetIntervalReq;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 gameInfoLength; // offset 10
    u16 gameInfo[1]; // offset 12
} WlParamSetGameInfoReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 staMacAdrs[3]; // offset 06
    u16 retryLimit; // offset 0c
    u16 enableChannel; // offset 0e
    u16 channel; // offset 10
    u16 mode; // offset 12
    u16 rate; // offset 14
    u16 wepMode; // offset 16
    u16 wepKeyId; // offset 18
    u16 beaconType; // offset 1a
    u16 probeRes; // offset 1c
    u16 beaconLostTh; // offset 1e
    u16 activeZoneTime; // offset 20
    u8 ssidMask[32]; // offset 22
    u16 preambleType; // offset 42
    u16 authAlgo; // offset 44
} WlParamGetAllCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 staMacAdrs[3]; // offset 06
} WlParamGetMacAdrsCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 staMacAdrs[3]; // offset 06
} WlParamGetMacAddressCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 retryLimit; // offset 06
} WlParamGetRetryLimitCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 enableChannel; // offset 06
    u16 channel; // offset 08
} WlParamGetEnableChannelCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 mode; // offset 06
} WlParamGetModeCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 rate; // offset 06
} WlParamGetRateCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 wepMode; // offset 06
} WlParamGetWepModeCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 wepKeyId; // offset 06
} WlParamGetWepKeyIdCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 beaconType; // offset 06
} WlParamGetBeaconTypeCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 probe; // offset 06
} WlParamGetProbeResCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 probe; // offset 06
} WlParamGetProbeResponseCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 beaconLostTh; // offset 06
} WlParamGetBeaconLostThresholdCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 beaconLostTh; // offset 06
} WlParamGetBeaconLostThCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 activeZoneTime; // offset 06
} WlParamGetActiveZoneCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u8 mask[32]; // offset 06
} WlParamGetSsidMaskCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 type; // offset 06
} WlParamGetPreambleTypeCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 type; // offset 06
} WlParamGetAuthenticationAlgorithmCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 type; // offset 06
} WlParamGetAuthAlgoCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 ccaMode; // offset 06
    u16 edThreshold; // offset 08
    u16 agcLimit; // offset 0a
} WlParamGetCCAModeEDThCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 ccaMode; // offset 06
    u16 edThreshold; // offset 08
    u16 agcLimit; // offset 0a
} WlParamGetCCAModeEDThresholdCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 count; // offset 06
} WlParamGetMaxConnectableChildCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 count; // offset 06
} WlParamGetMaxConnCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 mainAntenna; // offset 06
} WlParamGetMainAntennaCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 diversity; // offset 06
    u16 useAntenna; // offset 08
} WlParamGetDiversityCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 enableMessage; // offset 06
} WlParamGetBeaconSendRecvIndCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 mode; // offset 06
} WlParamGetNullKeyModeCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 bssid[3]; // offset 06
} WlParamGetBssidCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 ssidLength; // offset 06
    u8 ssid[32]; // offset 08
} WlParamGetSsidCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 beaconPeriod; // offset 06
} WlParamGetBeaconPeriodCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 dtimPeriond; // offset 06
} WlParamGetDtimPeriodCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 listenInterval; // offset 06
} WlParamGetIntervalCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 gameInfoLength; // offset 06
    u16 gameInfo[1]; // offset 08
} WlParamGetGameInfoCfm;
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
} WlDevRestartCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlDevSetInitializeWirelessCounterCfm;
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
    u8 wlVersion[8]; // offset 06
    u16 macVersion; // offset 0e
    u16 bbpVersion[2]; // offset 10
    u16 rfVersion; // offset 14
} WlDevGetVersionCfm;
typedef struct {
    struct {
        u32 success; // offset 00
        u32 failed; // offset 04
        u32 retry; // offset 08
        u32 ackErr; // offset 0c
        u32 unicast; // offset 10
        u32 multicast; // offset 14
        u32 wep; // offset 18
        u32 beacon; // offset 1c
    } tx; // offset 00
    struct {
        u32 rts; // offset 00
        u32 fragment; // offset 04
        u32 unicast; // offset 08
        u32 multicast; // offset 0c
        u32 wep; // offset 10
        u32 beacon; // offset 14
        u32 fcsErr; // offset 18
        u32 duplicateErr; // offset 1c
        u32 mpDuplicateErr; // offset 20
        u32 icvErr; // offset 24
        u32 fcErr; // offset 28
        u32 lengthErr; // offset 2c
        u32 plcpErr; // offset 30
        u32 bufOvfErr; // offset 34
        u32 pathErr; // offset 38
        u32 rateErr; // offset 3c
        u32 fcsOk; // offset 40
    } rx; // offset 20
    struct {
        u32 txMp; // offset 00
        u32 txKey; // offset 04
        u32 txNull; // offset 08
        u32 rxMp; // offset 0c
        u32 rxMpAck; // offset 10
        u32 keyResponseErr[15]; // offset 14
    } multiPoll; // offset 64
} WlCounter;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 rsv1; // offset 06
    WlCounter counter; // offset 08
} WlDevGetWirelessCounterCfm;
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
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 state; // offset 06
} WlDevGetStationStateCfm;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
} WlDevTestSignalCfm;
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
} WlDevTestRxCfm;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 control; // offset 10
    u16 channel; // offset 12
} WlDevTestRxReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 buf[2]; // offset 06
} WlCmdCfm;
struct WMStartParentCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 wlCmdID; // offset 04
    u16 wlResult; // offset 06
    u16 state; // offset 08
    u8 macAddress[6]; // offset 0a
    u16 aid; // offset 10
    u16 reason; // offset 12
    u8 ssid[24]; // offset 14
    u16 parentSize; // offset 2c
    u16 childSize; // offset 2e
};
typedef struct WMStartParentCallback WMstartParentCallback;
typedef struct WMStartParentCallback WMStartParentCallback;
struct WMStartScanCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 wlCmdID; // offset 04
    u16 wlResult; // offset 06
    u16 state; // offset 08
    u8 macAddress[6]; // offset 0a
    u16 channel; // offset 10
    u16 linkLevel; // offset 12
    u16 ssidLength; // offset 14
    u16 ssid[16]; // offset 16
    u16 gameInfoLength; // offset 36
    struct WMGameInfo gameInfo; // offset 38
};
typedef struct WMStartScanCallback WMStartScanCallback;
typedef struct WMStartScanCallback WMstartScanCallback;
struct WMStartScanReq {
    u16 apiid; // offset 00
    u16 channel; // offset 02
    struct WMBssDesc* scanBuf; // offset 04
    u16 maxChannelTime; // offset 08
    u8 bssid[6]; // offset 0a
};
typedef struct WMStartScanReq WMStartScanReq;
typedef struct WMStartScanReq WMstartScanReq;
struct WMStartScanExCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 wlCmdID; // offset 04
    u16 wlResult; // offset 06
    u16 state; // offset 08
    u16 channelList; // offset 0a
    u8 reserved[2]; // offset 0c
    u16 bssDescCount; // offset 0e
    struct WMBssDesc* bssDesc[16]; // offset 10
    u16 linkLevel[16]; // offset 50
};
typedef struct WMStartScanExCallback WMStartScanExCallback;
typedef struct WMStartScanExCallback WMstartScanExCallback;
struct WMStartScanExReq {
    u16 apiid; // offset 00
    u16 channelList; // offset 02
    struct WMBssDesc* scanBuf; // offset 04
    u16 scanBufSize; // offset 08
    u16 maxChannelTime; // offset 0a
    u8 bssid[6]; // offset 0c
    u16 scanType; // offset 12
    u16 ssidLength; // offset 14
    u8 ssid[32]; // offset 16
    u16 ssidMatchLength; // offset 36
    u16 rsv[2]; // offset 38
};
typedef struct WMStartScanExReq WMStartScanExReq;
typedef struct WMStartScanExReq WMstartScanExReq;
struct WMStartConnectCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 wlCmdID; // offset 04
    u16 wlResult; // offset 06
    u16 state; // offset 08
    u16 aid; // offset 0a
    u16 reason; // offset 0c
    u16 wlStatus; // offset 0e
    u8 macAddress[6]; // offset 10
    u16 parentSize; // offset 16
    u16 childSize; // offset 18
};
typedef struct WMStartConnectCallback WMstartConnectCallback;
typedef struct WMStartConnectCallback WMStartConnectCallback;
struct WMStartConnectReq {
    u16 apiid; // offset 00
    u16 reserved; // offset 02
    struct WMBssDesc* pInfo; // offset 04
    u8 ssid[24]; // offset 08
    int powerSave; // offset 20
    u16 reserved2; // offset 24
    u16 authMode; // offset 26
};
typedef struct WMStartConnectReq WMStartConnectReq;
typedef struct WMStartConnectReq WMstartConnectReq;
struct WMDisconnectCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 wlCmdID; // offset 04
    u16 wlResult; // offset 06
    u16 tryBitmap; // offset 08
    u16 disconnectedBitmap; // offset 0a
};
typedef struct WMDisconnectCallback WMDisconnectCallback;
struct WMStartMPCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 state; // offset 04
    u8 reserved[2]; // offset 06
    struct WMMpRecvBuf* recvBuf; // offset 08
    u16 timeStamp; // offset 0c
    u16 rate_rssi; // offset 0e
    u8 destAdrs[6]; // offset 10
    u8 srcAdrs[6]; // offset 16
    u16 seqNum; // offset 1c
    u16 tmptt; // offset 1e
    u16 pollbmp; // offset 20
    u16 reserved2; // offset 22
};
typedef struct WMStartMPCallback WMStartMPCallback;
typedef struct WMStartMPCallback WMstartMPCallback;
struct WMMPTmpParam {
    u32 mask; // offset 00
    u16 minFrequency; // offset 04
    u16 frequency; // offset 06
    u16 maxFrequency; // offset 08
    u16 defaultRetryCount; // offset 0a
    u8 minPollBmpMode; // offset 0c
    u8 singlePacketMode; // offset 0d
    u8 ignoreFatalErrorMode; // offset 0e
    u8 reserved[1]; // offset 0f
};
typedef struct WMMPTmpParam WMMPTmpParam;
struct WMMPParam {
    u32 mask; // offset 00
    u16 minFrequency; // offset 04
    u16 frequency; // offset 06
    u16 maxFrequency; // offset 08
    u16 parentSize; // offset 0a
    u16 childSize; // offset 0c
    u16 parentInterval; // offset 0e
    u16 childInterval; // offset 10
    u16 parentVCount; // offset 12
    u16 childVCount; // offset 14
    u16 defaultRetryCount; // offset 16
    u8 minPollBmpMode; // offset 18
    u8 singlePacketMode; // offset 19
    u8 ignoreFatalErrorMode; // offset 1a
    u8 ignoreSizePrecheckMode; // offset 1b
};
typedef struct WMMPParam WMMPParam;
struct WMStartMPReq {
    u16 apiid; // offset 00
    u16 rsv1; // offset 02
    u32* recvBuf; // offset 04
    u32 recvBufSize; // offset 08
    u32* sendBuf; // offset 0c
    u32 sendBufSize; // offset 10
    struct WMMPParam param; // offset 14
    struct WMMPTmpParam tmpParam; // offset 30
};
typedef struct WMStartMPReq WMStartMPReq;
enum WMErrCode {
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
};
struct WMPortSendCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 wlCmdID; // offset 04
    u16 wlResult; // offset 06
    u16 state; // offset 08
    u16 port; // offset 0a
    u16 destBitmap; // offset 0c
    u16 restBitmap; // offset 0e
    u16 sentBitmap; // offset 10
    u16 rsv; // offset 12
    u16* data; // offset 14
    union {
        u16 size; // offset 00
        u16 length; // offset 00
    }; // offset 18
    u16 seqNo; // offset 1a
    void (*callback)(void*); // offset 1c
    void* arg; // offset 20
    u16 maxSendDataSize; // offset 24
    u16 maxRecvDataSize; // offset 26
};
typedef struct WMPortSendCallback WMPortSendCallback;
struct WMStartDCFCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 state; // offset 04
    u8 reserved[2]; // offset 06
    struct WMDcfRecvBuf* recvBuf; // offset 08
};
typedef struct WMStartDCFCallback WMStartDCFCallback;
typedef struct WMStartDCFCallback WMstartDCFCallback;
struct WMStartTestModeCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u32 RFadr5; // offset 04
    u32 RFadr6; // offset 08
    u16 PllLockCheck; // offset 0c
    u16 RFMDflag; // offset 0e
};
typedef struct WMStartTestModeCallback WMStartTestModeCallback;
struct WMMeasureChannelCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 wlCmdID; // offset 04
    u16 wlResult; // offset 06
    u16 channel; // offset 08
    u16 ccaBusyRatio; // offset 0a
};
typedef struct WMMeasureChannelCallback WMmeasureChannelCallback;
typedef struct WMMeasureChannelCallback WMMeasureChannelCallback;
struct WMMeasureChannelReq {
    u16 apiid; // offset 00
    u16 ccaMode; // offset 02
    u16 edThreshold; // offset 04
    u16 channel; // offset 06
    u16 measureTime; // offset 08
};
typedef struct WMMeasureChannelReq WMMeasureChannelReq;
typedef struct WMMeasureChannelReq WMmeasureChannelReq;
struct WMGetWirelessCounterCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 wlCmdID; // offset 04
    u16 wlResult; // offset 06
    u32 TX_Success; // offset 08
    u32 TX_Failed; // offset 0c
    u32 TX_Retry; // offset 10
    u32 TX_AckError; // offset 14
    u32 TX_Unicast; // offset 18
    u32 TX_Multicast; // offset 1c
    u32 TX_WEP; // offset 20
    u32 TX_Beacon; // offset 24
    u32 RX_RTS; // offset 28
    u32 RX_Fragment; // offset 2c
    u32 RX_Unicast; // offset 30
    u32 RX_Multicast; // offset 34
    u32 RX_WEP; // offset 38
    u32 RX_Beacon; // offset 3c
    u32 RX_FCSError; // offset 40
    u32 RX_DuplicateError; // offset 44
    u32 RX_MPDuplicateError; // offset 48
    u32 RX_ICVError; // offset 4c
    u32 RX_FrameCtrlError; // offset 50
    u32 RX_LengthError; // offset 54
    u32 RX_PLCPError; // offset 58
    u32 RX_BufferOverflowError; // offset 5c
    u32 RX_PathError; // offset 60
    u32 RX_RateError; // offset 64
    u32 RX_FCSOK; // offset 68
    u32 TX_MP; // offset 6c
    u32 TX_KeyData; // offset 70
    u32 TX_NullKey; // offset 74
    u32 RX_MP; // offset 78
    u32 RX_MPACK; // offset 7c
    u32 MPKeyResponseError[15]; // offset 80
};
typedef struct WMGetWirelessCounterCallback WMgetWirelessCounterCallback;
typedef struct WMGetWirelessCounterCallback WMGetWirelessCounterCallback;
struct WMPortRecvCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u16 state; // offset 04
    u16 port; // offset 06
    struct WMMpRecvBuf* recvBuf; // offset 08
    u16* data; // offset 0c
    u16 length; // offset 10
    u16 aid; // offset 12
    u8 macAddress[6]; // offset 14
    u16 seqNo; // offset 1a
    void* arg; // offset 1c
    u16 myAid; // offset 20
    u16 connectedAidBitmap; // offset 22
    u8 ssid[24]; // offset 24
    u16 reason; // offset 3c
    u16 rssi; // offset 3e
    u16 maxSendDataSize; // offset 40
    u16 maxRecvDataSize; // offset 42
};
typedef struct WMPortRecvCallback WMPortRecvCallback;
struct WMSetMPParameterCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u32 mask; // offset 04
    struct WMMPParam oldParam; // offset 08
};
typedef struct WMSetMPParameterCallback WMSetMPParameterCallback;
struct WMStopTestRxModeCallback {
    u16 apiid; // offset 00
    u16 errcode; // offset 02
    u32 fcsOk; // offset 04
    u32 fcsErr; // offset 08
};
typedef struct WMStopTestRxModeCallback WMStopTestRxModeCallback;
typedef struct {
    u16 NextId; // offset 00
    u16 Flag; // offset 02
    void (*pTaskFunc)(); // offset 04
} TASK_TBL;
typedef struct {
    u16 EnQ[4]; // offset 00
    u16 DeQ[4]; // offset 08
    u16 NextPri; // offset 10
    u16 TaskPri; // offset 12
    u16 CurrTaskID; // offset 14
    u16 pad; // offset 16
    struct _OSThread Thread; // offset 18
    TASK_TBL TaskTbl[24]; // offset bc
} TASK_MAN;
typedef struct {
    u32 Prev; // offset 00
    u32 Next; // offset 04
    u16 Flag; // offset 08
    u16 BufID; // offset 0a
} HEAPBUF_HEADER;
typedef struct {
    HEAPBUF_HEADER* Head; // offset 00
    HEAPBUF_HEADER* Tail; // offset 04
    u16 Count; // offset 08
    u16 Flag; // offset 0a
} HEAPBUF_MAN;
typedef struct {
    u32 heapType; // offset 00
    union {
        struct {
            enum _enum_16263 id; // offset 00
            int heapHandle; // offset 04
        } os; // offset 00
        struct {
            u32 (*alloc)(u32); // offset 00
            u32 (*free)(void*); // offset 04
        } ext; // offset 00
    } func; // offset 04
} HEAP_INFO;
typedef struct {
    HEAP_INFO HeapInfo; // offset 00
    HEAPBUF_MAN TmpBuf; // offset 0c
    HEAPBUF_MAN TxPri[3]; // offset 18
    HEAPBUF_MAN MaDataCfm; // offset 3c
    HEAPBUF_MAN RxData; // offset 48
    HEAPBUF_MAN RxBeacon; // offset 54
    HEAPBUF_MAN RxManCtrl; // offset 60
    HEAPBUF_MAN Defrag; // offset 6c
    HEAPBUF_MAN ToWM; // offset 78
    HEAPBUF_MAN RequestCmd; // offset 84
    HEAPBUF_MAN man_rsv[4]; // offset 90
} HEAP_MAN;
typedef struct {
    u16 Basic; // offset 00
    u16 Support; // offset 02
} RATE_SET;
typedef struct {
    u64 NextTbttTsf; // offset 00
    u16 STA; // offset 08
    u16 RSSI; // offset 0a
    u16 Mode; // offset 0c
    u16 PowerMgtMode; // offset 0e
    u16 PowerState; // offset 10
    u16 bSynchro; // offset 12
    u16 RxDtims; // offset 14
    u16 PN15Rate; // offset 16
    u16 SigTest2; // offset 18
    u16 bFirstTbtt; // offset 1a
    u16 ManCtrlRetry; // offset 1c
    u16 SSIDLength; // offset 1e
    u8 SSID[32]; // offset 20
    u8 SSIDMask[32]; // offset 40
    RATE_SET RateSet; // offset 60
    u16 BSSID[3]; // offset 64
    u16 AID; // offset 6a
    u16 SeqNum; // offset 6c
    u16 BeaconPeriod; // offset 6e
    u16 ListenInterval; // offset 70
    u16 CurrListenInterval; // offset 72
    u16 DTIMPeriod; // offset 74
    u16 DTIMCount; // offset 76
    u16 rsvcc; // offset 78
    u16 CurrChannel; // offset 7a
    u16 CapaInfo; // offset 7c
    u16 BeaconLostTh; // offset 7e
    u16 BeaconLostCnt; // offset 80
    u16 LinkAdrs[3]; // offset 82
    u16 APCamAdrs; // offset 88
    u16 FrameCtrl; // offset 8a
    u16 FrameLifeTime; // offset 8c
    u16 bExistTIM; // offset 8e
    u16 TxPower; // offset 90
    struct {
        struct {
            u16 SSID; // offset 00
            u16 TIM; // offset 02
            u16 GameInfo; // offset 04
        } Beacon; // offset 00
        struct {
            u16 SSID; // offset 00
        } ProbeReq; // offset 06
        struct {
            u16 Size; // offset 00
        } RxBuf; // offset 08
    } Ofst; // offset 92
    void* GameInfoAdrs; // offset 9c
    u16 GameInfoLength; // offset a0
    u16 GameInfoAlign; // offset a2
    u16 bUpdateGameInfo; // offset a4
    u16 TmpttPs; // offset a6
    u32 IntervalCount; // offset a8
    u16 Scrambler; // offset ac
    u16 DbgSeqNum; // offset ae
    u16 FatalErr; // offset b0
    u16 rsv; // offset b2
    u16 CurrErrCount; // offset b4
    u16 TxBufErrCount; // offset b6
    u16 TxBufResCount; // offset b8
    u16 WepErrCount; // offset ba
    u16 MpEndErrCount; // offset bc
    u16 NotPollTxErrCount; // offset be
} WORK_PARAM;
typedef struct {
    u16 adrs; // offset 00
    u16 value; // offset 02
} MAC_INIT_REGS;
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
    CAM_ELEMENT* pCAM; // offset 00
    u16 CamMaxStaNum; // offset 04
    u16 MaxStaNum; // offset 06
    u16 MacAdrs[3]; // offset 08
    u16 RetryLimit; // offset 0e
    u16 EnableChannel; // offset 10
    u16 Mode; // offset 12
    u16 Rate; // offset 14
    u16 AuthAlgo; // offset 16
    u16 WepMode; // offset 18
    u16 WepKeyId; // offset 1a
    u16 FrameLifeTimePerBeacon; // offset 1c
    u16 BeaconType : 1; // offset 1e
    u16 BcSsidResponse : 1; // offset 1e
    u16 PreambleType : 1; // offset 1e
    u16 MainAntenna : 1; // offset 1e
    u16 Diversity : 1; // offset 1e
    u16 UseAntenna : 1; // offset 1e
    u16 BcnTxRxIndMsg : 1; // offset 1e
    u16 NullKeyRes : 1; // offset 1e
    u16 MulticastPass : 1; // offset 1e
    u16 dmm : 7; // offset 1e
    u16 ActiveZone; // offset 20
    u16 DiagResult; // offset 22
    u32 ParamFlag; // offset 24
} CONFIG_PARAM;
typedef struct {
    u8 ID; // offset 00
    u8 Length; // offset 01
    u8 SupportedRate[2]; // offset 02
} SUP_RATE_ELEMENT;
typedef struct {
    u16 Id; // offset 00
    u16 Bits; // offset 02
    u16 InitNum; // offset 04
    u16 ChanNum; // offset 06
    u16 BbpCnt; // offset 08
    u16 pad; // offset 0a
    u32 BkReg; // offset 0c
} RF_CONFIG;
typedef struct {
    u16 FrameId; // offset 00
    u16 CamAdrs; // offset 02
    u16 FrameTime; // offset 04
    u16 Length; // offset 06
} FIRM_HEADER;
typedef union {
    struct {
        u16 Status; // offset 00
        u16 Status2; // offset 02
        u16 rsv_RetryCount; // offset 04
        u16 rsv_AppRate; // offset 06
        u16 Service_Rate; // offset 08
        u16 MPDU; // offset 0a
    } Tx; // offset 00
    struct {
        u16 Status; // offset 00
        u16 NextBnry; // offset 02
        u16 TimeStamp; // offset 04
        u16 Service_Rate; // offset 06
        u16 MPDU; // offset 08
        u16 rsv_RSSI; // offset 0a
    } Rx; // offset 00
} MAC_HEADER;
typedef union {
    u16 Data; // offset 00
    struct {
        u16 Version : 2; // offset 00
        u16 Type : 2; // offset 00
        u16 SubType : 4; // offset 00
        u16 ToDS : 1; // offset 00
        u16 FromDS : 1; // offset 00
        u16 MoreFrag : 1; // offset 00
        u16 Retry : 1; // offset 00
        u16 PowerMan : 1; // offset 00
        u16 MoreData : 1; // offset 00
        u16 WEP : 1; // offset 00
        u16 Order : 1; // offset 00
    } Bit; // offset 00
} FRAME_CTRL;
typedef union {
    u16 Data; // offset 00
    struct {
        u16 FragNum : 4; // offset 00
        u16 SeqNum : 12; // offset 00
    } Bit; // offset 00
} SEQ_CTRL;
typedef struct {
    FRAME_CTRL FrameCtrl; // offset 00
    u16 DurationID; // offset 02
    u16 Adrs1[3]; // offset 04
    u16 Adrs2[3]; // offset 0a
    u16 Adrs3[3]; // offset 10
    SEQ_CTRL SeqCtrl; // offset 16
} DATA_HEADER;
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    DATA_HEADER Dot11Header; // offset 14
    u8 Body[4]; // offset 2c
} RXFRM;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    RXFRM frame; // offset 10
} RXPACKET;
typedef struct {
    u16 DA[3]; // offset 00
    u16 SA[3]; // offset 06
    SEQ_CTRL SeqCtrl; // offset 0c
    u16 pad; // offset 0e
} DEFRAG_TBL;
typedef struct {
    u16 RestTime; // offset 00
    u16 UnitLength; // offset 02
    DEFRAG_TBL DefragTbl; // offset 04
    RXPACKET* pPacket; // offset 14
} DEFRAG_LIST;
typedef struct {
    u16 LastMpSeq; // offset 00
    u16 IcvOkCntFlag; // offset 02
    u16 wlCurr; // offset 04
    u16 TxKeyReg; // offset 06
    u16 TxKeyFrm; // offset 08
    u16 rsv; // offset 0a
    DEFRAG_LIST DefragList[3]; // offset 0c
} RX_CTRL;
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
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    DATA_HEADER Dot11Header; // offset 14
    union {
        u8 Body[4]; // offset 00
        u8* Pointer; // offset 00
    } Data; // offset 2c
} TXFRM;
typedef struct {
    MAC_HEADER MacHeader; // offset 00
    DATA_HEADER Dot11Header; // offset 0c
    u8 Body[4]; // offset 24
} TXFRM_MAC;
typedef struct {
    u16 Busy; // offset 00
    u16 InCount; // offset 02
    u16 OutCount; // offset 04
    u16 pad; // offset 06
    TXFRM_MAC* pMacFrm; // offset 08
    TXFRM* pFrm; // offset 0c
    void (*pEndFunc)(TXFRM*, u32); // offset 10
} TXQ;
typedef struct {
    u16 length; // offset 00
    u8 rate; // offset 02
    u8 rssi; // offset 03
    u16 aid; // offset 04
    u16 noResponse; // offset 06
    u8 cdata[4]; // offset 08
} WlMpKeyData;
typedef struct {
    u16 bitmap; // offset 00
    u16 errBitmap; // offset 02
    u16 count; // offset 04
    u16 length; // offset 06
    u16 txCount; // offset 08
    WlMpKeyData data[1]; // offset 0a
} WlMpKey;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    WlMpKey mpKey; // offset 10
} WlMaMpEndInd;
typedef struct {
    TXQ Txq[3]; // offset 00
    TXQ Mp; // offset 3c
    TXQ Key[2]; // offset 50
    TXQ Beacon; // offset 78
    u32 Flag; // offset 8c
    WlMaMpEndInd* pMpEndInd; // offset 90
    u16 TXOP; // offset 94
    u16 TMPTT; // offset 96
    u16 SetKeyMap; // offset 98
    u16 GetKeyMap; // offset 9a
    u16 DataLength; // offset 9c
    u16 RetryLimit; // offset 9e
    u16 RestBitmap; // offset a0
    u16 BkKeyIn; // offset a2
    u16 BkKeyOut; // offset a4
    u16 BkSeqNum; // offset a6
    u16 MpBlkCnt; // offset a8
    u16 MpRstCnt; // offset aa
    u16 MpLastOk; // offset ac
    u16 TimeOutFrm; // offset ae
} TX_CTRL;
typedef struct {
    u16 a; // offset 00
    u16 b; // offset 02
    u16 seed; // offset 04
    u16 rsv; // offset 06
} RAND_CTRL;
typedef struct {
    u8* pCmd; // offset 00
    u16 Busy; // offset 04
    u16 pad; // offset 06
} CMDIF_MAN;
typedef struct {
    u16 State; // offset 00
    u16 pad; // offset 02
    union {
        struct {
            u16 MaxConfirmLength; // offset 00
            u16 ChannelCount; // offset 02
            u16 bFound; // offset 04
            u16 ElapseTime; // offset 06
            u16 TxPeriod; // offset 08
        } Scan; // offset 00
        struct {
            u16 Result; // offset 00
            u16 Status; // offset 02
        } Join; // offset 00
        struct {
            TXFRM* pTxFrm; // offset 00
        } DeAuth; // offset 00
        struct {
            TXFRM* pTxFrm; // offset 00
        } DisAss; // offset 00
        struct {
            u32 Counter; // offset 00
            u32 CCA; // offset 04
            u16 bkPowerMode; // offset 08
            u16 bkCCAMode; // offset 0a
            u16 bkEdTh; // offset 0c
            u16 bkMode; // offset 0e
            u16 Channel; // offset 10
            u16 sts; // offset 12
        } Measure; // offset 00
    } Work; // offset 04
    union {
        WlMlmeResetReq* Reset; // offset 00
        WlMlmePowerMgtReq* PwrMgt; // offset 00
        WlMlmeScanReq* Scan; // offset 00
        WlMlmeJoinReq* Join; // offset 00
        WlMlmeAuthReq* Auth; // offset 00
        WlMlmeDeAuthReq* DeAuth; // offset 00
        WlMlmeAssReq* Ass; // offset 00
        WlMlmeReAssReq* ReAss; // offset 00
        WlMlmeDisAssReq* DisAss; // offset 00
        WlMlmeStartReq* Start; // offset 00
        WlMlmeMeasChanReq* MeasChannel; // offset 00
    } pReq; // offset 18
    union {
        WlCmdCfm* Cfm; // offset 00
        WlMlmeResetCfm* Reset; // offset 00
        WlMlmePowerMgtCfm* PwrMgt; // offset 00
        WlMlmeScanCfm* Scan; // offset 00
        WlMlmeJoinCfm* Join; // offset 00
        WlMlmeAuthCfm* Auth; // offset 00
        WlMlmeDeAuthCfm* DeAuth; // offset 00
        WlMlmeAssCfm* Ass; // offset 00
        WlMlmeReAssCfm* ReAss; // offset 00
        WlMlmeDisAssCfm* DisAss; // offset 00
        WlMlmeStartCfm* Start; // offset 00
        WlMlmeMeasChanCfm* MeasChannel; // offset 00
    } pCfm; // offset 1c
} MLME_MAN;
typedef struct {
    u16 rssi; // offset 00
    u16 channel; // offset 02
    u16 capaInfo; // offset 04
    u16 bssid[3]; // offset 06
    u16 ssidLength; // offset 0c
    u8 ssid[32]; // offset 0e
    u16 beaconInterval; // offset 2e
    u16 lifeTime; // offset 30
} WlApElement;
typedef struct {
    TASK_MAN TaskMan; // offset 00
    HEAP_MAN HeapMan; // offset 17c
    WlApElement ApList[4]; // offset 23c
    struct OSMessageQueue* pSendMsgQueue; // offset 304
    struct OSMessageQueue* pRecvMsgQueue; // offset 308
    u32 DmaChannel; // offset 30c
    u32 DmaMaxCount; // offset 310
    long lockID; // offset 314
    void* pFlashImg; // offset 318
    CONFIG_PARAM Config; // offset 31c
    WORK_PARAM Work; // offset 344
    MLME_MAN MLME; // offset 404
    CMDIF_MAN CmdIf; // offset 424
    TX_CTRL TxCtrl; // offset 42c
    RX_CTRL RxCtrl; // offset 4dc
    CAM_MAN CamMan; // offset 530
    WlCounter Counter; // offset 540
    RAND_CTRL Rand; // offset 5f4
    RF_CONFIG Rf; // offset 5fc
    struct OSiAlarm PeriodicAlarm; // offset 60c
    struct OSiAlarm Alarm; // offset 638
    struct OSiAlarm KeyAlarm; // offset 664
    u32 WlDbgLevel; // offset 690
    u16 WlOperation; // offset 694
    u16 EnableChannel; // offset 696
} WL_MAN;
typedef struct {
    u16 rssi; // offset 00
    u16 channel; // offset 02
    u16 capaInfo; // offset 04
    u16 bssid[3]; // offset 06
    u16 ssidLength; // offset 0c
    u8 ssid[32]; // offset 0e
    u16 beaconInterval; // offset 2e
    u16 lifeTime; // offset 30
} AP_LIST;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 errCode; // offset 10
} WlMaFatalErrInd;
typedef struct {
    u16 adrs; // offset 00
    u16 mask; // offset 02
} TEST_REGS;
typedef struct {
    MAC_HEADER MacHeader; // offset 00
    DATA_HEADER Dot11Header; // offset 0c
    u8 Body[4]; // offset 24
} RXFRM_MAC;
typedef struct {
    u8 ID; // offset 00
    u8 Length; // offset 01
    u8 DTIMCount; // offset 02
    u8 DTIMPeriod; // offset 03
    u8 BitmapCtrl; // offset 04
    u8 VitrualBitmap[5]; // offset 05
} TIM_ELEMENT;
typedef struct {
    FRAME_CTRL FrameCtrl; // offset 00
    u16 Duration; // offset 02
    u16 DA[3]; // offset 04
    u16 SA[3]; // offset 0a
    u16 BSSID[3]; // offset 10
    SEQ_CTRL SeqCtrl; // offset 16
} MAN_HEADER;
typedef struct {
    u16 ReasonCode; // offset 00
} DEAUTH_BODY;
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    MAN_HEADER Dot11Header; // offset 14
    DEAUTH_BODY Body; // offset 2c
} DEAUTH_FRAME;
typedef struct {
    u16 RequestMinLength; // offset 00
    u16 ConfirmMinLength; // offset 02
    u16 (*pCmdFunc)(WlCmdReq*, WlCmdCfm*); // offset 04
} WLLIB_CMD_TBL;
typedef struct {
    u8 ID; // offset 00
    u8 Length; // offset 01
    u8 Text[253]; // offset 02
} CHALLENGE_ELEMENT;
typedef struct {
    u16 AlgoType; // offset 00
    u16 SeqNum; // offset 02
    u16 StatusCode; // offset 04
    CHALLENGE_ELEMENT ChallengeText; // offset 06
    u8 pad[1]; // offset 105
} AUTH_BODY;
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    MAN_HEADER Dot11Header; // offset 14
    AUTH_BODY Body; // offset 2c
} AUTH_FRAME;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 algorithm; // offset 16
} WlMlmeAuthInd;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 reasonCode; // offset 16
} WlMlmeDeAuthInd;
typedef struct {
    u8 ID; // offset 00
    u8 Length; // offset 01
    u8 SSID[32]; // offset 02
} SSID_ELEMENT;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 aid; // offset 16
    u16 ssidLength; // offset 18
    u8 ssid[32]; // offset 1a
} WlMlmeAssInd;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 aid; // offset 16
    u16 ssidLength; // offset 18
    u8 ssid[32]; // offset 1a
} WlMlmeReAssInd;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 peerMacAdrs[3]; // offset 10
    u16 reasonCode; // offset 16
} WlMlmeDisAssInd;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 apMacAdrs[3]; // offset 10
} WlMlmeBeaconLostInd;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
} WlMlmeBeaconSendInd;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 reserved1[3]; // offset 10
    u16 gameInfoLength; // offset 16
    u16 reserved2[3]; // offset 18
    u8 rate; // offset 1e
    u8 rssi; // offset 1f
    u16 reserved3[4]; // offset 20
    u16 reserved4[3]; // offset 28
    u16 srcMacAdrs[3]; // offset 2e
    u16 reserved5[4]; // offset 34
    u16 gameInfo[1]; // offset 3c
} WlMlmeBeaconRecvInd;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    u16 mode; // offset 10
} WlParamSetMcPassModeReq;
typedef struct {
    WlCmdHeader header; // offset 00
    u16 resultCode; // offset 04
    u16 mode; // offset 06
} WlParamGetMcPassModeCfm;
typedef struct {
    MAC_HEADER MacHeader; // offset 00
    DATA_HEADER Dot11Header; // offset 0c
    u16 TXOP; // offset 24
    u16 Bitmap; // offset 26
    u8 Body[4]; // offset 28
} TXMPFRM_MAC;
typedef union {
    u16 Data; // offset 00
    struct {
        u16 ESS : 1; // offset 00
        u16 IBSS : 1; // offset 00
        u16 CFPollable : 1; // offset 00
        u16 CFPollRequest : 1; // offset 00
        u16 Privacy : 1; // offset 00
        u16 ShortPreamble : 1; // offset 00
        u16 rsv6 : 1; // offset 00
        u16 rsv7 : 1; // offset 00
        u16 rsv8 : 1; // offset 00
        u16 rsv9 : 1; // offset 00
        u16 rsv10 : 1; // offset 00
        u16 rsv11 : 1; // offset 00
        u16 rsv12 : 1; // offset 00
        u16 rsv13 : 1; // offset 00
        u16 rsv14 : 1; // offset 00
        u16 rsv15 : 1; // offset 00
    } Bit; // offset 00
} CAPA_INFO;
typedef struct {
    CAPA_INFO CapaInfo; // offset 00
    u16 StatusCode; // offset 02
    u16 AID; // offset 04
    u8 Buf[4]; // offset 06
} ASSRES_BODY;
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    MAN_HEADER Dot11Header; // offset 14
    ASSRES_BODY Body; // offset 2c
} ASSRES_FRAME;
typedef struct {
    u32 TimeStamp[2]; // offset 00
    u16 BeaconInterval; // offset 08
    CAPA_INFO CapaInfo; // offset 0a
    u8 Buf[54]; // offset 0c
    u8 pad[2]; // offset 42
} BEACON_BODY;
typedef struct {
    u8 ID; // offset 00
    u8 Length; // offset 01
    u8 OUI[3]; // offset 02
    u8 SubType; // offset 05
    u8 ActZone[2]; // offset 06
    u8 VTSF[2]; // offset 08
    u8 GameInfo[3]; // offset 0a
} GAME_INFO_ELEMENT;
typedef struct {
    u16 ReasonCode; // offset 00
} DISASS_BODY;
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    MAN_HEADER Dot11Header; // offset 14
    DISASS_BODY Body; // offset 2c
} DISASS_FRAME;
typedef struct {
    CAPA_INFO CapaInfo; // offset 00
    u16 ListenInterval; // offset 02
    u8 Buf[38]; // offset 04
} ASSREQ_BODY;
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    MAN_HEADER Dot11Header; // offset 14
    ASSREQ_BODY Body; // offset 2c
} ASSREQ_FRAME;
typedef struct {
    CAPA_INFO CapaInfo; // offset 00
    u16 ListenInterval; // offset 02
    u16 CurrAPMacAdrs[3]; // offset 04
    u8 Buf[38]; // offset 0a
} REASSREQ_BODY;
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    MAN_HEADER Dot11Header; // offset 14
    REASSREQ_BODY Body; // offset 2c
} REASSREQ_FRAME;
typedef struct {
    CAPA_INFO CapaInfo; // offset 00
    u16 StatusCode; // offset 02
    u16 AID; // offset 04
    u8 Buf[4]; // offset 06
} REASSRES_BODY;
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    MAN_HEADER Dot11Header; // offset 14
    REASSRES_BODY Body; // offset 2c
} REASSRES_FRAME;
typedef struct {
    u8 Buf[38]; // offset 00
} PRBREQ_BODY;
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    MAN_HEADER Dot11Header; // offset 14
    PRBREQ_BODY Body; // offset 2c
} PRBREQ_FRAME;
typedef struct {
    u32 TimeStamp[2]; // offset 00
    u16 BeaconInterval; // offset 08
    CAPA_INFO CapaInfo; // offset 0a
    u8 Buf[54]; // offset 0c
    u8 pad[2]; // offset 42
} PRBRES_BODY;
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    MAN_HEADER Dot11Header; // offset 14
    PRBRES_BODY Body; // offset 2c
} PRBRES_FRAME;
typedef struct {
    u16 frameId; // offset 00
    u8 rsv1[4]; // offset 02
    u16 length; // offset 06
    u16 status; // offset 08
    u16 rsvm2; // offset 0a
    u16 timeStamp; // offset 0c
    u8 rate; // offset 0e
    u8 rssi; // offset 0f
    u32 rsvm3; // offset 10
    u8 rsv4[4]; // offset 14
    u16 destAdrs[3]; // offset 18
    u16 srcAdrs[3]; // offset 1e
    u8 rsv5[8]; // offset 24
    u8 data[4]; // offset 2c
} WlRxFrame;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    WlRxFrame frame; // offset 10
} WlMaDataInd;
typedef struct {
    u8 rsv1[6]; // offset 00
    u16 length; // offset 06
    u16 txKeySts; // offset 08
    u16 ackTimeStamp; // offset 0a
    u16 timeStamp; // offset 0c
    u8 rate; // offset 0e
    u8 rssi; // offset 0f
    u8 rsv3[4]; // offset 10
    u8 rsv4[4]; // offset 14
    u16 destAdrs[3]; // offset 18
    u16 srcAdrs[3]; // offset 1e
    u8 rsv5[6]; // offset 24
    u16 seqCtrl; // offset 2a
    u16 txop; // offset 2c
    u16 bitmap; // offset 2e
    u16 data[1]; // offset 30
} WlRxMpFrame;
typedef struct {
    u16 wlRsv[6]; // offset 00
    WlCmdHeader header; // offset 0c
    WlRxMpFrame frame; // offset 10
} WlMaMpInd;
typedef struct {
    FIRM_HEADER FirmHeader; // offset 00
    MAC_HEADER MacHeader; // offset 08
    MAN_HEADER Dot11Header; // offset 14
    BEACON_BODY Body; // offset 2c
} BEACON_FRAME;
typedef struct {
    u8 ID; // offset 00
    u8 Length; // offset 01
    u8 CFPCount; // offset 02
    u8 CFPPeriod; // offset 03
    union {
        u16 u16; // offset 00
        u8 u8[2]; // offset 00
    } CFPMaxDuration; // offset 04
    union {
        u16 u16; // offset 00
        u8 u8[2]; // offset 00
    } CFPDurRemain; // offset 06
} CF_PARAM_ELEMENT;
typedef struct {
    u8* pElement; // offset 00
    u16 rxStatus; // offset 04
    u16 capability; // offset 06
    u16 bodyLength; // offset 08
    u16 matchFlag; // offset 0a
    u16 foundFlag; // offset 0c
    u16 activeZone; // offset 0e
    u16 vtsf; // offset 10
    u16 channel; // offset 12
    RATE_SET rateSet; // offset 14
    u16 otherElementCount; // offset 18
    u16 otherElementLength; // offset 1a
    SSID_ELEMENT* pSSID; // offset 1c
    CF_PARAM_ELEMENT* pCFP; // offset 20
    TIM_ELEMENT* pTIM; // offset 24
    GAME_INFO_ELEMENT* pGMIF; // offset 28
} ELEMENT_CHECKER;
