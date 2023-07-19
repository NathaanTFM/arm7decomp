#include "Mongoose.h"

void WMSP_InitSendQueue() { // wmsp_port.c:38
    u16 i; // r4 - :43
}

void WMSP_SendMaKeyData() { // wmsp_port.c:79
    u32 wlBuf[128]; // None - :84
    u32 dataLength; // r0 - :88
    u16 wmHeader; // None - :89
}

void WMSP_SendMaMP(u16 pollBitmap) { // wmsp_port.c:144
    u32 wlBuf[128]; // None - :149
    u32 txop; // r7 - :154
    u16 tmptt; // r5 - :155
    u32 destBitmap; // r0 - :157
    u32 dataLength; // r4 - :158
    u16 wmHeader; // None - :159
    u32 currPollBitmap; // r0 - :160
    u32 childBitmap; // None - :161
    u32 mpReadyBitmap; // r5 - :162
    u32 e; // r0 - :165
    u16 recvSize; // r0 - :182
    if (0) {
        u32 e; // r0 - :260
    }
}

void WMSP_ResumeMaMP(u16 pollBitmap) { // wmsp_port.c:294
    u32 wlBuf[128]; // None - :299
    u16 tmptt; // r0 - :302
    u32 currPollBitmap; // r4 - :303
    u16 currTsf; // r5 - :304
    u32 wmHeader; // r6 - :305
    u16 recvSize; // r8 - :309
    u32 e; // r0 - :312
}

int WMSP_PutSendQueue(u32 childBitmap, u16 priority, u16 port, u32 destBitmap, u16* sendData, u16 sendDataSize, void (*callback)(void*), void* arg) { // wmsp_port.c:748
    WMPortSendQueueData* queueData; // r9 - :755
    WMPortSendQueue* queue; // r10 - :756
    WMPortSendQueueData* newData; // r0 - :757
    u16 newIndex; // r1 - :758
}

int WMSP_FlushSendQueue(int timeout, u16 pollBitmap) { // wmsp_port.c:869
    WMPortSendQueueData* queueData; // r4 - :874
    int iPrio; // r0 - :875
    int isParent; // r6 - :876
    u32 childBitmap; // None - :877
    int retryFlag; // None - :879
    u32 e; // r0 - :911
    WMPortSendQueue* queue; // r0 - :924
    u32 index; // r5 - :926
    u16* prevPointer; // r6 - :927
    WMPortSendQueueData* data; // r7 - :938
    struct WMPortSendCallback* cb_PortSend; // r0 - :997
    u16 parentSize; // r1 - :1022
    u16 childSize; // r0 - :1023
}

void WMSP_CleanSendQueue(u16 aidBitmap) { // wmsp_port.c:1134
    WMPortSendQueueData* queueData; // r5 - :1139
    int iQueue; // None - :1140
    int iPrio; // r6 - :1141
    u32 aidBitmapMask; // None - :1142
    WMPortSendQueue* baseQueue; // r11 - :1158
    WMPortSendQueue* queue; // r7 - :1165
    u32 index; // r0 - :1166
    u16* prevPointer; // None - :1167
    u32 prevIndex; // None - :1168
    WMPortSendQueueData* data; // r8 - :1176
    struct WMPortSendCallback* cb_PortSend; // r0 - :1193
    u16 parentSize; // r1 - :1211
    u16 childSize; // r0 - :1212
}

void WMSP_ParsePortPacket(u16 aid, u16 wmHeader, u16* data, u8 rssi, u16 length, struct WMMpRecvBuf* recvBuf) { // wmsp_port.c:1290
    struct WMStatus* status; // r5 - :1292
    int restSize; // r6 - :1293
    int firstPacketFlag; // None - :1294
    int packetLength; // r0 - :1335
    int len; // r7 - :1335
    u16* header; // r1 - :1337
    u16* footer; // r1 - :1338
    u16* contents; // None - :1339
    u32 port; // r11 - :1340
    u32 seqNo; // r8 - :1341
    u16* pCurrSeqNo; // r0 - :1388
    u32 currSeqNo; // r0 - :1389
    struct WMPortRecvCallback* cb_Port; // r0 - :1451
    u16 parentSize; // r2 - :1467
    u16 childSize; // r0 - :1468
}

static u16 WmspGetTmptt(u32 dataLength, u32 txop, u32 pollBitmap, u32 targetVCount) { // wmsp_port.c:1694
    long tmptt; // r3 - :1708
    u32 pollCnt; // r0 - :1710
    u32 mp_time; // r4 - :1711
}