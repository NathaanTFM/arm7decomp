#include "Mongoose.h"

struct WMSPWork wmspW; // :27
static struct _OSThread wmspRequestThread; // :32
static struct _OSThread wmspIndicateThread; // :33

void WM_sp_init(WlInit* wlInit, struct WmInit* wmInit) { // wmsp_system.c:55
    long i; // r5 - :135
}

void WMSP_ReturnResult2Wm9(void* ptr) { // wmsp_system.c:192
}

void* WMSP_GetBuffer4Callback2Wm9() { // wmsp_system.c:237
}

u16* WMSP_WlRequest(u16* request) { // wmsp_system.c:264
    void* msg; // None - :266
    struct WMIndCallback* callback; // r0 - :278
}

static void WmspPxiCallback(u32 data, int err) { // wmsp_system.c:306
    int result; // r0 - :310
    struct WMCallback* cb; // r0 - :328
}

int WMSP_CheckMacAddress(u8* macAdr) { // wmsp_system.c:348
    u8* myMacAddress; // r0 - :351
}

void WMSP_CopyParentParam(struct WMGameInfo* gameInfop, struct WMParentParam* pparamp) { // wmsp_system.c:422
}

int WMSP_SetAllParams(u16 wmApiID, u16* buf) { // wmsp_system.c:461
    WlParamSetCfm* pConfirm; // r0 - :468
    WlParamSetAllReq* pReq; // r0 - :472
}

u16 WMSP_GetAllowedChannel(u16 bitField) { // wmsp_system.c:576
    u16 temp; // r0 - :578
    long max; // r12 - :579
    long min; // r0 - :580
    long center; // r14 - :581
    long i; // r4 - :615
}

void WMSP_AddRssiToList(u8 rssi8) { // wmsp_system.c:650
}

void WMSP_FillRssiIntoList(u8 rssi8) { // wmsp_system.c:666
    int i; // r3 - :668
}

u16 WMSP_GetAverageLinkLevel() { // wmsp_system.c:685
    long i; // r2 - :687
    u32 temp; // r3 - :688
}

u16 WMSP_GetLinkLevel(u32 rssi) { // wmsp_system.c:715
}

void WMSP_SetThreadPriorityLow() { // wmsp_system.c:756
    u32 e; // r0 - :758
}

void WMSP_SetThreadPriorityHigh() { // wmsp_system.c:792
    u32 e; // r0 - :794
}

u32* WMSP_GetInternalRequestBuf() { // wmsp_system.c:914
    long i; // r3 - :916
    u32* ret; // r4 - :917
    u32 e; // r0 - :918
}

void WMSP_ResetSizeVars() { // wmsp_system.c:943
}

void WMSP_SetParentMaxSize(u16 parentMaxSize) { // wmsp_system.c:962
    u16 maxSendSize; // r0 - :976
    u16 maxRecvSize; // r0 - :983
}

void WMSP_SetChildMaxSize(u16 childMaxSize) { // wmsp_system.c:994
    u16 maxRecvSize; // r0 - :1008
    u16 maxSendSize; // r0 - :1015
}

void WMSP_SetParentSize(u16 parentSize) { // wmsp_system.c:1026
}

void WMSP_SetChildSize(u16 childSize) { // wmsp_system.c:1051
    // FIXME: this variable isn't a thing(?) but it made compilation accurate
    // for that specific function because the compiler won't optimize
    struct WMStatus* status = wmspW.status;
    
    status->mp_childSize = childSize;
    
    if (status->aid == 0) {
        status->mp_recvSize = childSize + 2;
        
    } else {
        status->mp_sendSize = childSize + 2;
    }
}