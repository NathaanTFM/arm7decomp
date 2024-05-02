#include "Mongoose.h"

STATIC u16 WmspGetTmptt(u32 dataLength, u32 txop, u32 pollBitmap, u32 targetVCount);

void WMSP_InitSendQueue()
{          // wmsp_port.c:38
    u16 i; // r4 - :43
    WMStatus *status = wmspW.status;

    OS_LockMutex(&status->sendQueueMutex);
    MIi_CpuClear16(0, status->sendQueueData, 0x400);

    for (i = 0; i < 31; i++)
    {
        status->sendQueueData[i].next = i + 1;
    }
    status->sendQueueData[i].next = -1;

    status->sendQueueFreeList.head = 0;
    status->sendQueueFreeList.tail = i;

    for (i = 0; i < 4; i++)
    {
        status->readyQueue[i].head = -1;
        status->readyQueue[i].tail = -1;
        status->sendQueue[i].head = -1;
        status->sendQueue[i].tail = -1;
    }

    OS_UnlockMutex(&status->sendQueueMutex);
}

void WMSP_SendMaKeyData()
{                   // wmsp_port.c:79
    u32 wlBuf[128]; // None - :84
    u32 dataLength; // r0 - :88
    u16 wmHeader;   // None - :89
}

void WMSP_SendMaMP(u16 pollBitmap)
{                       // wmsp_port.c:144
    u32 wlBuf[128];     // None - :149
    u32 txop;           // r7 - :154
    u16 tmptt;          // r5 - :155
    u32 destBitmap;     // r0 - :157
    u32 dataLength;     // r4 - :158
    u16 wmHeader;       // None - :159
    u32 currPollBitmap; // r0 - :160
    u32 childBitmap;    // None - :161
    u32 mpReadyBitmap;  // r5 - :162
    u32 e;              // r0 - :165
    u16 recvSize;       // r0 - :182
    if (0)
    {
        u32 e; // r0 - :260
    }
}

void WMSP_ResumeMaMP(u16 pollBitmap)
{                       // wmsp_port.c:294
    u32 wlBuf[128];     // None - :299
    u16 tmptt;          // r0 - :302
    u32 currPollBitmap; // r4 - :303
    u16 currTsf;        // r5 - :304
    u32 wmHeader;       // r6 - :305
    u16 recvSize;       // r8 - :309
    WMStatus *status = wmspW.status;

    // ???
    u32 e = OS_DisableInterrupts(); // r0 - :312
    OS_RestoreInterrupts(e);

    currTsf = W_US_COUNT0;                                // :318
    wmHeader = status->mp_prevWmHeader;                   // :320
    currPollBitmap = (pollBitmap & status->child_bitmap); // :321
    recvSize = status->mp_recvSize;                       // :324

    if (status->mp_recvBufSize < (((recvSize + 12) * MATH_CountPopulation(currPollBitmap) + 41) & ~0x1F))
    {
        OS_Sleep(2);
        WMSP_RequestResumeMP();
    }
    else
    {
        if (status->mp_count == 1 || status->mp_limitCount == 1)
        {
            tmptt = WmspGetTmptt(status->mp_prevDataLength, status->mp_prevTxop, currPollBitmap, status->mp_parentVCount);
            wmHeader |= 0x8000;
        }
        else
        {
            tmptt = 0;
            wmHeader &= ~0x8000;
        }

        WMSP_WL_MaMp((u16 *)wlBuf, 0x800C, 0, 0, currPollBitmap, tmptt, currTsf, 0, wmHeader, 0);
    }
}

int WMSP_PutSendQueue(u32 childBitmap, u16 priority, u16 port, u32 destBitmap, u16 *sendData, u16 sendDataSize, void (*callback)(void *), void *arg)
{ // wmsp_port.c:748
    WMStatus *status = wmspW.status;
    WMPortSendQueueData *queueData = status->sendQueueData; // r9 - :755
    WMPortSendQueue *queue = &status->readyQueue[priority]; // r10 - :756
    WMPortSendQueueData *newData;                           // r0 - :757
    u16 newIndex;                                           // r1 - :758

    if (sendDataSize == 0)
    {
        OSi_TWarning("wmsp_port.c", 762, "put null data");
        return 6;
    }

    if (sendDataSize + ((port & 8) ? 2 : 0) > 516)
    {
        OSi_TWarning("wmsp_port.c", 772, "size=%d+%d < maxSize=%d", sendDataSize, (port & 8) ? 2 : 0, wmspW.status->mp_maxSendSize);
        return 6;
    }

    OS_LockMutex(&wmspW.status->sendQueueMutex);
    newIndex = status->sendQueueFreeList.head;

    if (status->mp_flag == 0)
    {
        OS_UnlockMutex(&status->sendQueueMutex);
        return 15;
    }

    if (newIndex == 0xFFFF)
    {
        OS_UnlockMutex(&status->sendQueueMutex);
        return 10;
    }

    status->sendQueueFreeList.head = queueData[newIndex].next;
    newData = &queueData[newIndex];

    if (status->sendQueueFreeList.tail == newIndex)
    {
        status->sendQueueFreeList.tail = 0xFFFF;
    }

    newData->port = port;
    newData->destBitmap = destBitmap;
    newData->restBitmap = destBitmap & childBitmap;
    newData->sentBitmap = 0;
    newData->sendingBitmap = 0;
    newData->data = sendData;
    newData->size = sendDataSize;
    newData->callback = callback;
    newData->arg = arg;
    newData->next = 0xFFFF;
    newData->seqNo = 0xFFFF;
    newData->retryCount = status->mp_current_defaultRetryCount;

    if (queue->tail == 0xFFFF)
    {
        queue->head = newIndex;
    }
    else
    {
        queueData[queue->tail].next = newIndex;
    }

    queue->tail = newIndex;
    OS_UnlockMutex(&status->sendQueueMutex);
    return 2;
}

int WMSP_FlushSendQueue(int timeout, u16 pollBitmap)
{ // wmsp_port.c:869
    WMStatus *status = wmspW.status;
    WMPortSendQueueData *queueData = wmspW.status->sendQueueData; // r4 - :874
    int iPrio;                                                    // r0 - :875
    int isParent;                                                 // r6 - :876
    u32 childBitmap;                                              // None - :877
    int retryFlag = 0;                                            // None - :879

    if (status->state == WM_STATE_MP_PARENT)
    {
        isParent = 1;
    }
    else if (status->state == WM_STATE_MP_CHILD)
    {
        isParent = 0;
    }
    else
    {
        OSi_TWarning("wmsp_port.c", 897, "invalid state: %d", status->state);
        return retryFlag;
    }

    OS_LockMutex(&status->sendQueueMutex);

    if (!status->sendQueueInUse)
    {
        OS_UnlockMutex(&status->sendQueueMutex);
        return retryFlag;
    }

    if (isParent)
    {
        u32 e = OS_DisableInterrupts(); // r0 - :911
        childBitmap = status->child_bitmap;
        OS_RestoreInterrupts(e);
    }
    else
    {
        childBitmap = 1;
    }

    for (iPrio = 0; iPrio < 4; iPrio++)
    {
        WMPortSendQueue *queue = &status->sendQueue[iPrio]; // r0 - :924
        u32 index = queue->head;                            // r5 - :926
        u16 *prevPointer = &queue->head;                    // r6 - :927
        u16 prevIndex = 0xFFFF;
        u16 newIndex = 0xFFFF;

        while (index != 0xFFFF)
        {
            WMPortSendQueueData *data = &queueData[index]; // r7 - :938
            if (timeout == 0)
            {
                data->sentBitmap |= data->sendingBitmap & ~pollBitmap;
                data->restBitmap &= ~(data->sentBitmap);
            }
            data->restBitmap &= childBitmap;
            data->sendingBitmap = 0;

            if (data->restBitmap != 0 && ((data->port & 8) != 0 || data->retryCount != 0))
            {
                retryFlag = 1;
                if (data->retryCount != 0)
                {
                    data->retryCount--;
                }

                if (data->next == 0xFFFF)
                {
                    queue->tail = 0xFFFF;
                }

                queue->head = data->next;
                data->next = 0xFFFF;

                if (prevIndex == 0xFFFF)
                {
                    newIndex = index;
                }
                else
                {
                    queueData[prevIndex].next = index;
                }

                prevIndex = index;
            }
            else
            {
                WMPortSendCallback *cb_PortSend = WMSP_GetBuffer4Callback2Wm9(); // r0 - :997
                cb_PortSend->apiid = WM_APIID_PORT_SEND;
                cb_PortSend->errcode = data->restBitmap == 0 ? WM_ERRCODE_SUCCESS : WM_ERRCODE_SEND_FAILED;
                cb_PortSend->state = WM_STATECODE_PORT_SEND;
                cb_PortSend->port = data->port;
                cb_PortSend->destBitmap = data->destBitmap;
                cb_PortSend->restBitmap = data->restBitmap;
                cb_PortSend->sentBitmap = data->sentBitmap;
                cb_PortSend->size = data->size;
                cb_PortSend->data = data->data;
                cb_PortSend->callback = data->callback;
                cb_PortSend->arg = data->arg;
                cb_PortSend->seqNo = data->seqNo;

                u16 parentSize = status->mp_parentSize; // r1 - :1022
                u16 childSize = status->mp_childSize;   // r0 - :1023
                cb_PortSend->maxSendDataSize = status->aid == 0 ? parentSize : childSize;
                cb_PortSend->maxRecvDataSize = status->aid == 0 ? childSize : parentSize;

                WMSP_ReturnResult2Wm9(cb_PortSend);

                if (data->next == 0xFFFF)
                {
                    queue->tail = 0xFFFF;
                }

                *prevPointer = data->next;
                data->next = 0xFFFF;

                if (status->sendQueueFreeList.tail == 0xFFFF)
                {
                    status->sendQueueFreeList.head = index;
                }
                else
                {
                    queueData[status->sendQueueFreeList.tail].next = index;
                };

                status->sendQueueFreeList.tail = index;
            }

            index = queue->head;
        }

        if (prevIndex != 0xFFFF)
        {
            queueData[prevIndex].next = status->readyQueue[iPrio].head;
            if (status->readyQueue[iPrio].tail == 0xFFFF)
            {
                status->readyQueue[iPrio].tail = prevIndex;
            }
            status->readyQueue[iPrio].head = newIndex;
        }
    }

    status->sendQueueInUse = 0;
    OS_UnlockMutex(&status->sendQueueMutex);
    return retryFlag;
}

void WMSP_CleanSendQueue(u16 aidBitmap)
{ // wmsp_port.c:1134
    WMStatus *status = wmspW.status;
    WMPortSendQueueData *queueData = wmspW.status->sendQueueData; // r5 - :1139
    int iQueue;                                                   // None - :1140
    int iPrio;                                                    // r6 - :1141
    u32 aidBitmapMask = ~aidBitmap & wmspW.status->child_bitmap;  // None - :1142

    OS_LockMutex(&wmspW.status->sendQueueMutex);

    for (iQueue = 0; iQueue < 2; iQueue++)
    {                                                    // Is this even useful? Maybe an older version had [2][4] queues?
        WMPortSendQueue *baseQueue = status->readyQueue; // r11 - :1158

        for (iPrio = 0; iPrio < 4; iPrio++)
        {
            WMPortSendQueue *queue = &baseQueue[iPrio]; // r7 - :1165
            u32 index = queue->head;                    // r0 - :1166
            u16 *prevPointer = &queue->head;            // None - :1167
            u32 prevIndex = 0xFFFF;                     // None - :1168

            while (index != 0xFFFF)
            {
                WMPortSendQueueData *data = &queueData[index]; // r8 - :1176
                data->restBitmap &= aidBitmapMask;
                data->sendingBitmap &= aidBitmapMask;

                if (data->restBitmap == 0)
                {
                    WMPortSendCallback *cb_PortSend = WMSP_GetBuffer4Callback2Wm9(); // r0 - :1193
                    cb_PortSend->apiid = WM_APIID_PORT_SEND;
                    cb_PortSend->errcode = WM_ERRCODE_SUCCESS;
                    cb_PortSend->state = WM_STATECODE_PORT_SEND;
                    cb_PortSend->port = data->port;
                    cb_PortSend->destBitmap = data->destBitmap;
                    cb_PortSend->restBitmap = data->restBitmap;
                    cb_PortSend->sentBitmap = data->sentBitmap;
                    cb_PortSend->size = data->size;
                    cb_PortSend->data = data->data;
                    cb_PortSend->callback = data->callback;
                    cb_PortSend->arg = data->arg;
                    cb_PortSend->seqNo = data->seqNo;

                    u16 parentSize = status->mp_parentSize; // r1 - :1211
                    u16 childSize = status->mp_childSize;   // r0 - :1212
                    cb_PortSend->maxSendDataSize = status->aid == 0 ? parentSize : childSize;
                    cb_PortSend->maxRecvDataSize = status->aid == 0 ? childSize : parentSize;

                    WMSP_ReturnResult2Wm9(cb_PortSend);

                    if (data->next == 0xFFFF)
                    {
                        queue->tail = prevIndex;
                    }

                    *prevPointer = data->next;
                    data->next = 0xFFFF;

                    if (status->sendQueueFreeList.tail == 0xFFFF)
                    {
                        status->sendQueueFreeList.head = index;
                    }
                    else
                    {
                        queueData[status->sendQueueFreeList.tail].next = index;
                    }

                    status->sendQueueFreeList.tail = index;
                    index = prevIndex;
                }

                prevIndex = index;
                if (index != 0xFFFF)
                {
                    prevPointer = &queueData[index].next;
                }
                else
                {
                    prevPointer = &queue->head;
                }

                index = (index != 0xFFFF) ? queueData[index].next : queue->head;
            }
        }
    }

    OS_UnlockMutex(&status->sendQueueMutex);
}

void WMSP_ParsePortPacket(u16 aid, u16 wmHeader, u16 *data, u8 rssi, u16 length, WMMpRecvBuf *recvBuf)
{                                    // wmsp_port.c:1290
    WMStatus *status = wmspW.status; // r5 - :1292
    int restSize = length;           // r6 - :1293
    int firstPacketFlag;             // None - :1294

    if (length == 0)
        return;

    if (length > 0x204)
    {
        OSi_TWarning("wmsp_port.c", 1313, "too huge packet from aid %d, len=%d, childMaxSize+header=%d+%d", aid, length, status->pparam.childMaxSize, 2);
        return;
    }

    if (aid == status->aid || aid >= 0x10)
    {
        OSi_TWarning("wmsp_port.c", 1320, "invalid aid %d ! @ WMSP_ParsePortPacket", aid);
        return;
    }

    if ((length & 1) != 0)
    {
        OSi_TWarning("wmsp_port.c", 1327, "odd length %d @ WMSP_ParsePortPacket", length);
        return;
    }

    firstPacketFlag = 1; // :1331

    while (restSize > 0)
    {                          // :1333
        int len, packetLength; // r7, r0 - :1335
        u16 *header;           // r1 - :1337
        u16 *footer;           // r1 - :1338
        u16 *contents;         // None - :1339
        u32 port;              // r11 - :1340
        u32 seqNo = 0xFFFF;    // r8 - :1341

        if (firstPacketFlag == 1)
        {                        // :1346
            firstPacketFlag = 0; // :1348
            header = &wmHeader;  // :1349
        }
        else
        {
            header = data; // :1353
            data++;        // :1354
            restSize -= 2; // :1355
        }

        len = (*header & 0xFF) << 1; // :1358
        if (len == 0)
            len = 512; // :1360

        int unk1 = (*header & 0x1000) ? 1 : 0;                // :1361
        int unk2 = (*header & 0x800) ? 1 : 0;                 // :1362
        packetLength = len + (unk2 ? 2 : 0) + (unk1 ? 2 : 0); // :1363
        restSize -= packetLength;                             // :1364

        if (restSize < 0)
        {                                                                                                                                                          // :1366
            OSi_TWarning("wmsp_port.c", 1369, "corrupted packet. rest: %d, packet: %d + %id(destBmp) + %d(seqNo)", restSize, len, (unk1 ? 2 : 0), (unk2 ? 2 : 0)); // :1368
            return;                                                                                                                                                // :1370?
        }

        port = (u16)((*header & 0xF00) >> 8);     // :1373
        contents = data;                          // :1374
        footer = (u16 *)((u32)data + len);        // :1375
        data = (u16 *)((u32)data + packetLength); // :1378

        if (unk2 == 1)
        {                                                          // :1386
            u16 *pCurrSeqNo = &status->portSeqNo[aid][port & 0x7]; // r0 - :1388
            u16 currSeqNo = *pCurrSeqNo;                           // r0 - :1389
            seqNo = *(footer);                                     // :1391
            footer++;

            if ((currSeqNo & 1) != 0)
            {                            // :1398
                *pCurrSeqNo = 2 * seqNo; // :1401
            }
            else
            {
                seqNo = 2 * seqNo; // :1405
                if ((u16)(currSeqNo - seqNo) < 0x100)
                { // :1406
                    continue;
                }

                *pCurrSeqNo = seqNo; // :1421
            }
        }

        if (unk1 != 1 || (*footer & (1 << status->aid)) != 0)
        {
            if (len > 0)
            {
                WMPortRecvCallback *cb_Port; // r0 - :1451
                cb_Port = WMSP_GetBuffer4Callback2Wm9();
                cb_Port->apiid = WM_APIID_PORT_RECV;
                cb_Port->errcode = WM_ERRCODE_SUCCESS;
                cb_Port->state = WM_STATECODE_PORT_RECV; // :1456
                cb_Port->port = port;                    // :1457
                cb_Port->recvBuf = recvBuf;              // :1458
                cb_Port->data = contents;
                cb_Port->length = len;
                cb_Port->aid = aid;
                cb_Port->rssi = rssi;
                cb_Port->myAid = status->aid;
                cb_Port->seqNo = seqNo; // :1464

                u16 parentSize = status->mp_parentSize; // r2 - :1467
                u16 childSize = status->mp_childSize;   // r0 - :1468
                cb_Port->maxSendDataSize = (status->aid == 0 ? parentSize : childSize);
                cb_Port->maxRecvDataSize = (status->aid == 0 ? childSize : parentSize);

                WMSP_ReturnResult2Wm9(cb_Port); // :1473
            }
            else
            {                                                               // :1474?
                OSi_TWarning("wmsp_port.c", 1477, "RecvZeroLengthPacket."); // :1477
            }
        }
    }
}

// I can't get it to match, so enjoy this partial version
STATIC u16 WmspGetTmptt(u32 dataLength, u32 txop, u32 pollBitmap, u32 targetVCount)
{                // wmsp_port.c:1694
    long tmptt;  // r3 - :1708
    u32 pollCnt; // r0 - :1710
    u32 mp_time; // r4 - :1711

    if ((txop & 0x8000) != 0)
    {                            // :1714
        mp_time = txop & 0x7FFF; // :1716
    }
    else
    {
        mp_time = 4 * (txop + 0x1C) + 0x66; // :1721
    }

    pollCnt = MATH_CountPopulation(pollBitmap);         // :1723
    dataLength = (dataLength + 0x22) * 4 + 0x60;        // :1724
    mp_time = dataLength + (mp_time * pollCnt) + 0x388; // :1725

    u16 tmp = REG_VCOUNT; // :1728
    tmptt = targetVCount - 2 - tmp;
    if (tmptt < 0)
    {
        do
        {
            tmptt = tmptt + 0x100 + 7;
        } while (tmptt < 0);
    }

    tmptt = (127 * tmptt) / 20;
    if (10 * tmptt < mp_time)
        tmptt &= ~0xFFFF;

    return tmptt;
}