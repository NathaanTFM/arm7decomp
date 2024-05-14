static void ReduceCredits(struct _COMMON_CREDIT_STATE_INFO* pCredInfo, struct _HTC_ENDPOINT_CREDIT_DIST* pEpDist, int Limit) { // credit_dist.c:37
    int credits; // r0 - :41
}
static void ar6000_credit_init(void* Context, struct _HTC_ENDPOINT_CREDIT_DIST* pEPList, int TotalCredits) { // credit_dist.c:69
    struct _HTC_ENDPOINT_CREDIT_DIST* pCurEpDist; // r0 - :73
    int count; // r0 - :74
    struct _COMMON_CREDIT_STATE_INFO* pCredInfo; // r0 - :75
}
static void ar6000_credit_distribute(void* Context, struct _HTC_ENDPOINT_CREDIT_DIST* pEPDistList, enum _HTC_CREDIT_DIST_REASON Reason) { // credit_dist.c:148
    struct _HTC_ENDPOINT_CREDIT_DIST* pCurEpDist; // r0 - :152
    struct _COMMON_CREDIT_STATE_INFO* pCredInfo; // r0 - :153
}
enum _enum_423491 ar6000_setup_credit_dist(void* HTCHandle, struct _COMMON_CREDIT_STATE_INFO* pCredInfo) { // credit_dist.c:314
    unsigned short servicepriority[5]; // None - :316
}