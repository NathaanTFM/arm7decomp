static enum _enum_403588 HTCProcessRecvHeader(struct _HTC_TARGET* target, struct _HTC_PACKET* pPacket, unsigned long* pNextLookAhead) { // htc_recv.c:145
    unsigned char temp; // r0 - :147
    unsigned char* pBuf; // r4 - :148
    enum _enum_403588 status; // r0 - :149
    unsigned short payloadLen; // r0 - :150
    unsigned long lookAhead; // None - :151
}
void HTCRecvCompleteHandler(void* Context, struct _HTC_PACKET* pPacket) { // htc_recv.c:258
    struct _HTC_TARGET* target; // r0 - :260
    struct _HTC_ENDPOINT* pEndpoint; // r5 - :261
    unsigned long nextLookAhead; // None - :262
    enum _enum_403588 status; // r6 - :263
    enum _enum_403588 nextStatus; // r0 - :289
}
enum _enum_403588 HTCWaitforControlMessage(struct _HTC_TARGET* target, struct _HTC_PACKET** ppControlPacket) { // htc_recv.c:332
    enum _enum_403588 status; // r6 - :334
    unsigned long lookAhead; // None - :335
    struct _HTC_PACKET* pPacket; // r7 - :336
    struct _HTC_FRAME_HDR* pHdr; // r0 - :337
}
enum _enum_403588 HTCRecvMessagePendingHandler(void* Context, unsigned long LookAhead, int* pAsyncProc) { // htc_recv.c:431
    struct _HTC_TARGET* target; // r0 - :433
    enum _enum_403588 status; // r5 - :434
    struct _HTC_PACKET* pPacket; // r6 - :435
    struct _HTC_FRAME_HDR* pHdr; // r7 - :436
    struct _HTC_ENDPOINT* pEndpoint; // r8 - :437
    int asyncProc; // r9 - :438
    unsigned long rxPendingLength; // None - :439
}
enum _enum_403588 HTCAddReceivePkt(void* HTCHandle, struct _HTC_PACKET* pPacket) { // htc_recv.c:618
    struct _HTC_TARGET* target; // r0 - :620
    struct _HTC_ENDPOINT* pEndpoint; // r4 - :621
    int unblockRecv; // r5 - :622
    enum _enum_403588 status; // r6 - :623
}
void HTCUnblockRecv(void* HTCHandle) { // htc_recv.c:668
    struct _HTC_TARGET* target; // r0 - :670
    int unblockRecv; // r5 - :671
}
void HTCFlushRecvBuffers(struct _HTC_TARGET* target) { // htc_recv.c:719
    struct _HTC_ENDPOINT* pEndpoint; // r7 - :721
    int i; // r8 - :722
}