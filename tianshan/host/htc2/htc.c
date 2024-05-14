enum _enum_400542 HTCInit(struct _HTC_INIT_INFO* pInitInfo) { // htc.c:22
    struct htc_callbacks htcCallbacks; // None - :24
}
void HTCFreeControlBuffer(struct _HTC_TARGET* target, struct _HTC_PACKET* pPacket, struct _DL_LIST* pList) { // htc.c:48
}
struct _HTC_PACKET* HTCAllocControlBuffer(struct _HTC_TARGET* target, struct _DL_LIST* pList) { // htc.c:55
}
static enum _enum_400542 HTCTargetInsertedHandler(void* hif_handle) { // htc.c:86
    struct _HTC_TARGET* target; // r8 - :88
    enum _enum_400542 status; // r9 - :89
    int i; // r10 - :90
    struct _HTC_PACKET* pControlPacket; // r0 - :132
    struct _HTC_PACKET* pControlPacket; // r0 - :143
}
static enum _enum_400542 HTCTargetRemovedHandler(void* handle) { // htc.c:169
    struct _HTC_TARGET* target; // r4 - :171
}
void* HTCGetHifDevice(void* HTCHandle) { // htc.c:199
}
void HTCSetInstance(void* HTCHandle, void* Instance) { // htc.c:207
}
enum _enum_400542 HTCWaitTarget(void* HTCHandle) { // htc.c:216
    struct _HTC_TARGET* target; // r0 - :218
    enum _enum_400542 status; // r4 - :219
    struct _HTC_PACKET* pPacket; // None - :220
    HTC_READY_MSG* pRdyMsg; // r0 - :221
    struct _HTC_SERVICE_CONNECT_REQ connect; // None - :222
    struct _HTC_SERVICE_CONNECT_RESP resp; // None - :223
}
enum _enum_400542 HTCStart(void* HTCHandle) { // htc.c:307
    struct _HTC_TARGET* target; // r0 - :309
    struct _HTC_PACKET* pPacket; // r0 - :310
    enum _enum_400542 status; // r4 - :311
}
void HTCStop(void* HTCHandle) { // htc.c:367
    struct _HTC_TARGET* target; // r0 - :369
}
void HTCShutDown() { // htc.c:388
}
static void HTCReportFailure(void* Context) { // htc.c:410
    struct _HTC_TARGET* target; // r0 - :412
}
void DebugDumpBytes(unsigned char* buffer, unsigned short length) { // htc.c:422
    char stream[60]; // None - :424
    unsigned long i; // r6 - :425
    unsigned short count; // r8 - :426
    unsigned short offset; // r7 - :426
}