#include "Mongoose.h"

void WMSP_SetMPData(void* msg) { // req_SetMPData.c:28
    u32* buf; // r0 - :30
    u16* sendData; // r11 - :33
    u16 sendSize; // r4 - :34
    u32 destBitmap; // r5 - :35
    u16 port; // None - :36
    u16 priority; // r0 - :37
    void (*callback)(void*); // r6 - :38
    void* arg; // r7 - :39
    int result; // r8 - :40
    u32 childBitmap; // r9 - :42
    struct WMPortSendCallback* cb_PortSend; // r0 - :83
    u16 parentSize; // r2 - :107
    u16 childSize; // r0 - :108
}