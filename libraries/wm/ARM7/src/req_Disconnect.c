#include "Mongoose.h"

void WMSP_Disconnect(void* msg) { // req_Disconnect.c:189
    int result; // r0 - :191
    u16 tryBmp; // r5 - :192
    u16 resBmp; // None - :193
    struct WMDisconnectCallback* callback; // r0 - :200
    
    tryBmp = ((u32*)msg)[1];
    if (WMSP_DisconnectCore((u32*)msg, 0, &resBmp) == 1) {
        callback = (struct WMDisconnectCallback*)WMSP_GetBuffer4Callback2Wm9();
        callback->apiid = 13;
        callback->errcode = 0;
        callback->tryBitmap = tryBmp;
        callback->disconnectedBitmap = resBmp;
        WMSP_ReturnResult2Wm9(callback);
    }
}

#if 0 // Disabling for IPA
int WMSP_DisconnectCore(u32* args, int indicateFlag, u16* disconnected) { // req_Disconnect.c:225
    u32 wlBuf[128]; // None - :228
    WlCmdCfm* pConfirm; // r0 - :230
    u16 aidBitmap; // r0 - :234
    u16 reason; // None - :235
    u16 resBitmap; // r5 - :236
    int fCleanQueue; // r6 - :238
    u32 e; // r7 - :252
    struct WMDisconnectCallback* callback; // r0 - :260
    
    if (0) {
        struct WMDisconnectCallback* callback; // r0 - :301
        u16 wMac[3]; // None - :319
        long auth_retry; // r5 - :332
        struct WMStartConnectCallback* cb; // r0 - :437
    }
    if (0) {
        u16 wMac[3]; // None - :466
        long i; // r7 - :467
        u16 aid; // None - :468
        long auth_retry; // r4 - :487
        u32 e; // r0 - :529
        struct WMStartParentCallback* cb; // r0 - :552
    }
}
#endif

void WMSP_IndicateDisconnectionFromMyself(int parent, u16 aid, void* mac) { // req_Disconnect.c:605
    struct WMCallback* cb; // r4 - :608
    struct WMStartParentCallback* cb_Parent; // r0 - :614
    struct WMStartConnectCallback* cb_Child; // r0 - :625
}

static void WmspError(u16 wlCommand, u16 wlResult, u16 tryBmp, u16 resBmp) { // req_Disconnect.c:650
    struct WMDisconnectCallback* callback; // r0 - :652
}

static void WmspIndError(u16 wlCommand, u16 wlResult, u16 tryBmp, u16 resBmp) { // req_Disconnect.c:674
    struct WMDisconnectCallback* callback; // r0 - :676
}