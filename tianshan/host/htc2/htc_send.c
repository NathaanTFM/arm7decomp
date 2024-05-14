static void HTCSendPktCompletionHandler(void* Context, struct _HTC_PACKET* pPacket) { // htc_send.c:40
    struct _HTC_ENDPOINT* pEndpoint; // r0 - :43
}
enum _enum_406198 HTCIssueSend(struct _HTC_TARGET* target, struct _HTC_PACKET* pPacket, unsigned char SendFlags) { // htc_send.c:57
    unsigned char* pHdrBuf; // r0 - :60
    int sync; // r4 - :61
}
static enum _HTC_SEND_QUEUE_RESULT HTCTrySend(struct _HTC_TARGET* target, struct _HTC_ENDPOINT* pEndpoint, struct _HTC_PACKET* pPacketToSend) { // htc_send.c:101
    struct _HTC_PACKET* pPacket; // r6 - :105
    int creditsRequired; // r7 - :106
    unsigned char sendFlags; // r8 - :108
}
enum _enum_406198 HTCSendPkt(void* HTCHandle, struct _HTC_PACKET* pPacket) { // htc_send.c:235
    struct _HTC_TARGET* target; // r0 - :237
    struct _HTC_ENDPOINT* pEndpoint; // r5 - :238
    enum _enum_406746 ep; // r0 - :239
    enum _enum_406198 status; // r6 - :240
    enum _HTC_SEND_QUEUE_RESULT result; // r0 - :241
}
void HTCProcessCreditRpt(struct _HTC_TARGET* target, HTC_CREDIT_REPORT* pRpt, int NumEntries) { // htc_send.c:315
    int i; // r1 - :317
    struct _HTC_ENDPOINT* pEndpoint; // r0 - :318
    int totalCredits; // r4 - :319
    int doDist; // r5 - :320
}
static void HTCFlushEndpointTX(struct _HTC_TARGET* target, struct _HTC_ENDPOINT* pEndpoint, unsigned short Tag) { // htc_send.c:396
    struct _HTC_PACKET* pPacket; // r5 - :398
    struct _DL_LIST discardQueue; // None - :399
    struct _DL_LIST* pTemp; // r6 - :407
}
void DumpCreditDistStates(struct _HTC_TARGET* target) { // htc_send.c:457
    struct _HTC_ENDPOINT_CREDIT_DIST* pEPList; // r1 - :459
}
void HTCFlushSendPkts(struct _HTC_TARGET* target) { // htc_send.c:475
    struct _HTC_ENDPOINT* pEndpoint; // r0 - :477
    int i; // r6 - :478
}
void HTCFlushEndpoint(void* HTCHandle, enum _enum_406746 Endpoint, unsigned short Tag) { // htc_send.c:495
    struct _HTC_TARGET* target; // r0 - :497
    struct _HTC_ENDPOINT* pEndpoint; // r0 - :498
}
void HTCIndicateActivityChange(void* HTCHandle, enum _enum_406746 Endpoint, int Active) { // htc_send.c:510
    struct _HTC_TARGET* target; // r0 - :514
    struct _HTC_ENDPOINT* pEndpoint; // r5 - :515
    int doDist; // r6 - :516
}