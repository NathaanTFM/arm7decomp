void HTCControlTxComplete() { // htc_services.c:17
}
void HTCControlRecv(void* Context, struct _HTC_PACKET* pPacket) { // htc_services.c:25
}
enum _enum_409277 HTCSendSetupComplete(struct _HTC_TARGET* target) { // htc_services.c:43
    struct _HTC_PACKET* pSendPacket; // r7 - :45
    enum _enum_409277 status; // r5 - :46
    HTC_SETUP_COMPLETE_MSG* pSetupComplete; // r8 - :47
}
enum _enum_409277 HTCConnectService(void* HTCHandle, struct _HTC_SERVICE_CONNECT_REQ* pConnectReq, struct _HTC_SERVICE_CONNECT_RESP* pConnectResp) { // htc_services.c:85
    struct _HTC_TARGET* target; // r0 - :89
    enum _enum_409277 status; // r11 - :90
    struct _HTC_PACKET* pRecvPacket; // None - :91
    struct _HTC_PACKET* pSendPacket; // r4 - :92
    HTC_CONNECT_SERVICE_RESPONSE_MSG* pResponseMsg; // r0 - :93
    HTC_CONNECT_SERVICE_MSG* pConnectMsg; // r6 - :94
    enum _enum_409825 assignedEndpoint; // r5 - :95
    struct _HTC_ENDPOINT* pEndpoint; // r6 - :96
    int maxMsgSize; // r7 - :97
}
static void AddToEndpointDistList(struct _HTC_TARGET* target, struct _HTC_ENDPOINT_CREDIT_DIST* pEpDist) { // htc_services.c:258
    struct _HTC_ENDPOINT_CREDIT_DIST* pLastEntry; // r3 - :260
    struct _HTC_ENDPOINT_CREDIT_DIST* pCurEntry; // r2 - :260
}
static void HTCDefaultCreditInit(struct _HTC_ENDPOINT_CREDIT_DIST* pEPList, int TotalCredits) { // htc_services.c:286
    struct _HTC_ENDPOINT_CREDIT_DIST* pCurEpDist; // r0 - :290
    int totalEps; // r0 - :291
    int creditsPerEndpoint; // r0 - :292
}
void HTCDefaultCreditDist(struct _HTC_ENDPOINT_CREDIT_DIST* pEPDistList, enum _HTC_CREDIT_DIST_REASON Reason) { // htc_services.c:329
    struct _HTC_ENDPOINT_CREDIT_DIST* pCurEpDist; // r0 - :333
}
void HTCSetCreditDistribution(void* HTCHandle, void* pCreditDistContext, void (*CreditDistFunc)(void*, struct _HTC_ENDPOINT_CREDIT_DIST*, enum _HTC_CREDIT_DIST_REASON), void (*CreditInitFunc)(void*, struct _HTC_ENDPOINT_CREDIT_DIST*, int), unsigned short* ServicePriorityOrder, int ListLength) { // htc_services.c:352
    struct _HTC_TARGET* target; // r0 - :359
    int i; // r5 - :360
    int ep; // r12 - :361
}