#include "Mongoose.h"

void WMSP_SetMPData(void *msg)
{                          // req_SetMPData.c:28
    u32 *buf = (u32 *)msg; // r0 - :30
    WMStatus *status = wmspW.status;
    u16 *sendData = (u16 *)buf[1];                // r11 - :33
    u16 sendSize = buf[2];                        // r4 - :34
    u32 destBitmap = buf[3];                      // r5 - :35
    u16 port = buf[4];                            // None - :36
    u16 priority = buf[5];                        // r0 - :37
    void (*callback)(void *) = (void *)buf[6];    // r6 - :38
    void *arg = (void *)buf[7];                   // r7 - :39
    int result;                                   // r8 - :40
    u32 childBitmap = wmspW.status->child_bitmap; // r9 - :42

    if (status->aid)
        destBitmap = 1;

    if (!status->mp_flag)
    {
        result = 3;
    }
    else
    {
        if ((destBitmap & childBitmap) == 0)
        {
            result = 0;
        }
        else
        {
            result = WMSP_PutSendQueue(childBitmap, priority, port, destBitmap, sendData, sendSize, callback, arg);
        }
    }

    if (result != 2)
    {
        WMPortSendCallback *cb_PortSend = WMSP_GetBuffer4Callback2Wm9(); // r0 - :83
        cb_PortSend->apiid = WM_APIID_PORT_SEND;
        cb_PortSend->errcode = result;
        cb_PortSend->state = WM_STATECODE_PORT_SEND;
        cb_PortSend->port = port;
        cb_PortSend->destBitmap = destBitmap;
        cb_PortSend->restBitmap = result == 10 ? (destBitmap & childBitmap) : 0;
        cb_PortSend->sentBitmap = 0;
        cb_PortSend->size = sendSize;
        cb_PortSend->data = sendData;
        cb_PortSend->callback = callback;
        cb_PortSend->arg = arg;
        cb_PortSend->seqNo = -1;

        u16 parentSize = status->mp_parentSize; // r2 - :107
        u16 childSize = status->mp_childSize;   // r0 - :108

        cb_PortSend->maxSendDataSize = status->aid == 0 ? parentSize : childSize;
        cb_PortSend->maxRecvDataSize = status->aid == 0 ? childSize : parentSize;
        WMSP_ReturnResult2Wm9(cb_PortSend);
    }
}