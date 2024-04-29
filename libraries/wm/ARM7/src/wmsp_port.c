#include "Mongoose.h"

STATIC u16 WmspGetTmptt(u32 dataLength, u32 txop, u32 pollBitmap, u32 targetVCount);

void WMSP_InitSendQueue() { // wmsp_port.c:38
    u16 i; // r4 - :43
    struct WMStatus* status = wmspW.status;
    
    OS_LockMutex(&status->sendQueueMutex);
    MIi_CpuClear16(0, status->sendQueueData, 0x400);
    
    for (i = 0; i < 31; i++) {
        status->sendQueueData[i].next = i + 1;
    }
    status->sendQueueData[i].next = -1;
    
    status->sendQueueFreeList.head = 0;
    status->sendQueueFreeList.tail = i;
    
    for (i = 0; i < 4; i++) {
        status->readyQueue[i].head = -1;
        status->readyQueue[i].tail = -1;
        status->sendQueue[i].head = -1;
        status->sendQueue[i].tail = -1;
    }
    
    OS_UnlockMutex(&status->sendQueueMutex);
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
    struct WMStatus* status = wmspW.status;
    
    // ???
    u32 e = OS_DisableInterrupts(); // r0 - :312
    OS_RestoreInterrupts(e);
    
    currTsf = W_US_COUNT0; // :318
    wmHeader = status->mp_prevWmHeader; // :320
    currPollBitmap = (pollBitmap & status->child_bitmap); // :321
    recvSize = status->mp_recvSize; // :324
    
    if (status->mp_recvBufSize < (((recvSize + 12) * MATH_CountPopulation(currPollBitmap) + 41) & ~0x1F)) {
        OS_Sleep(2);
        WMSP_RequestResumeMP();
        
    } else {
        if (status->mp_count == 1 || status->mp_limitCount == 1) {
            tmptt = WmspGetTmptt(status->mp_prevDataLength, status->mp_prevTxop, currPollBitmap, status->mp_parentVCount);
            wmHeader |= 0x8000;
            
        } else {
            tmptt = 0;
            wmHeader &= ~0x8000;
        }
        
        WMSP_WL_MaMp((u16*)wlBuf, 0x800C, 0, 0, currPollBitmap, tmptt, currTsf, 0, wmHeader, 0);
    }
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
    struct WMStatus* status = wmspW.status; // r5 - :1292
    int restSize = length; // r6 - :1293
    int firstPacketFlag; // None - :1294
    
    if (length == 0)
        return;

    if (length > 0x204) {
        OSi_TWarning("wmsp_port.c", 1313, "too huge packet from aid %d, len=%d, childMaxSize+header=%d+%d", aid, length, status->pparam.childMaxSize, 2);
        return;
    }
    
    if (aid == status->aid || aid >= 0x10) {
        OSi_TWarning("wmsp_port.c", 1320, "invalid aid %d ! @ WMSP_ParsePortPacket", aid);
        return;
    }
    
    if ((length & 1) != 0) {
        OSi_TWarning("wmsp_port.c", 1327, "odd length %d @ WMSP_ParsePortPacket", length);
        return;
    }
    
    firstPacketFlag = 1; // :1331
    
    while (restSize > 0) { // :1333
        int len, packetLength; // r7, r0 - :1335
        u16* header; // r1 - :1337
        u16* footer; // r1 - :1338
        u16* contents; // None - :1339
        u32 port; // r11 - :1340
        u32 seqNo = 0xFFFF; // r8 - :1341
        
        if (firstPacketFlag == 1) { // :1346
            firstPacketFlag = 0; // :1348
            header = &wmHeader; // :1349
            
        } else {
            header = data; // :1353
            data++; // :1354
            restSize -= 2; // :1355
        }
        
        len = (*header & 0xFF) << 1; // :1358
        if (len == 0)
            len = 512; // :1360
        
        int unk1 = (*header & 0x1000) ? 1 : 0; // :1361
        int unk2 = (*header & 0x800) ? 1 : 0; // :1362
        packetLength = len + (unk2 ? 2 : 0) + (unk1 ? 2 : 0); // :1363
        restSize -= packetLength; // :1364
        
        if (restSize < 0) { // :1366
            OSi_TWarning("wmsp_port.c", 1369, "corrupted packet. rest: %d, packet: %d + %id(destBmp) + %d(seqNo)", restSize, len, (unk1 ? 2 : 0), (unk2 ? 2 : 0)); // :1368
            return; // :1370?
        }
        
        port = (u16)((*header & 0xF00) >> 8); // :1373
        contents = data; // :1374
        footer = (u16*)((u32)data + len); // :1375
        data = (u16*)((u32)data + packetLength); // :1378
        
        if (unk2 == 1) { // :1386
            u16* pCurrSeqNo = &status->portSeqNo[aid][port & 0x7]; // r0 - :1388
            u16 currSeqNo = *pCurrSeqNo; // r0 - :1389            
            seqNo = *(footer); // :1391
            footer++;
            
            if ((currSeqNo & 1) != 0) { // :1398
                *pCurrSeqNo = 2 * seqNo; // :1401
                
            } else {
                seqNo = 2 * seqNo; // :1405
                if ((u16)(currSeqNo - seqNo) < 0x100) { // :1406
                    continue;
                }
                
                *pCurrSeqNo = seqNo; // :1421
            }
        }

        
        if (unk1 != 1 || (*footer & (1 << status->aid)) != 0) {
            if (len > 0) {
                struct WMPortRecvCallback* cb_Port; // r0 - :1451
                cb_Port = WMSP_GetBuffer4Callback2Wm9();
                cb_Port->apiid = 130;
                cb_Port->errcode = 0;
                cb_Port->state = 21; // :1456
                cb_Port->port = port; // :1457
                cb_Port->recvBuf = recvBuf; // :1458
                cb_Port->data = contents;
                cb_Port->length = len;
                cb_Port->aid = aid;
                cb_Port->rssi = rssi;
                cb_Port->myAid = status->aid;
                cb_Port->seqNo = seqNo; // :1464
    
                u16 parentSize = status->mp_parentSize; // r2 - :1467
                u16 childSize = status->mp_childSize; // r0 - :1468
                cb_Port->maxSendDataSize = (status->aid == 0 ? parentSize : childSize);
                cb_Port->maxRecvDataSize = (status->aid == 0 ? childSize : parentSize);
                
                WMSP_ReturnResult2Wm9(cb_Port); // :1473
                
            } else { // :1474?
                OSi_TWarning("wmsp_port.c", 1477, "RecvZeroLengthPacket."); // :1477
            }
        }
    }
}

#if 0
STATIC u16 WmspGetTmptt(u32 dataLength, u32 txop, u32 pollBitmap, u32 targetVCount) { // wmsp_port.c:1694
    long tmptt; // r3 - :1708
    u32 pollCnt; // r0 - :1710
    u32 mp_time; // r4 - :1711
}
#endif