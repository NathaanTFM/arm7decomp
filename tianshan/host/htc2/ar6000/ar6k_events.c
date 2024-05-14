enum _enum_414453 DevRWCompletionHandler(void* context, enum _enum_414453 status) { // ar6k_events.c:29
    struct _HTC_PACKET* pPacket; // r0 - :31
}
enum _enum_414453 DevPollMboxMsgRecv(struct _AR6K_DEVICE* pDev, unsigned long* pLookAhead, int TimeoutMS) { // ar6k_events.c:39
    enum _enum_414453 status; // r7 - :43
    int timeout; // r8 - :44
    struct _HIF_PENDING_EVENTS_INFO events; // None - :55
}
static enum _enum_414453 DevServiceDebugInterrupt(struct _AR6K_DEVICE* pDev) { // ar6k_events.c:226
    unsigned long dummy; // None - :228
    enum _enum_414453 status; // r0 - :229
}
static void DevGetEventAsyncHandler(void* Context, struct _HTC_PACKET* pPacket) { // ar6k_events.c:275
    struct _AR6K_DEVICE* pDev; // r0 - :277
    unsigned long lookAhead; // r0 - :278
    int otherInts; // r12 - :279
    struct _HIF_PENDING_EVENTS_INFO* pEvents; // r0 - :294
    struct _AR6K_IRQ_PROC_REGISTERS* pReg; // r0 - :306
    unsigned char host_int_status; // r2 - :307
}
enum _enum_414453 DevCheckPendingRecvMsgsAsync(void* context) { // ar6k_events.c:354
    struct _AR6K_DEVICE* pDev; // r0 - :356
    enum _enum_414453 status; // r2 - :357
    struct _HTC_PACKET* pIOPacket; // r0 - :358
}
enum _enum_414453 DevDsrHandler(void* context) { // ar6k_events.c:581
    struct _AR6K_DEVICE* pDev; // r0 - :583
    enum _enum_414453 status; // r0 - :584
    int done; // r7 - :585
    int asyncProc; // None - :586
}