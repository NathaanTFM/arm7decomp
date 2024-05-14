int DeviceDetach(struct _SDHCD* pHcd) { // sdio_bus_events.c:27
    struct _SDCONFIG_SDIO_INT_CTRL_DATA irqData; // None - :29
}
int DeviceAttach(struct _SDHCD* pHcd) { // sdio_bus_events.c:55
    int status; // r4 - :57
    struct _SDDEVICE* pDevice; // r0 - :58
    unsigned int ii; // r7 - :59
}
static void CompleteRequestCheckCancel(struct _SDREQUEST* pReqToComplete) { // sdio_bus_events.c:159
    int cancel; // r4 - :161
    struct _SDFUNCTION* pFunc; // r5 - :162
}
int _SDIO_HandleHcdEvent(struct _SDHCD* pHcd, unsigned char Event) { // sdio_bus_events.c:204
    struct _SDREQUEST* pReq; // r7 - :206
    struct _SDREQUEST* pReqToComplete; // r8 - :207
    struct _SDREQUEST* pNextReq; // r9 - :208
    int status; // r0 - :209
}
int CardDetectHelperFunction(struct _OSKERNEL_HELPER* pHelper) { // sdio_bus_events.c:399
    int status; // r0 - :401
    struct _HCD_EVENT_MESSAGE message; // None - :402
    int length; // None - :403
}
int DeviceInterrupt(struct _SDHCD* pHcd) { // sdio_bus_events.c:680
    int status; // r4 - :682
    struct _SDREQUEST* pReq; // r4 - :684
}
int SDIOIrqHelperFunction(struct _OSKERNEL_HELPER* pHelper) { // sdio_bus_events.c:775
    struct _SDHCD* pHcd; // r4 - :777
    int status; // r0 - :778
    struct _SDLIST* pListItem; // r2 - :779
    struct _SDDEVICE* pDevice; // r0 - :780
    struct _SDDEVICE* pDeviceIRQ[7]; // None - :782
    unsigned int deviceIrqCount; // None - :783
    unsigned int ii; // r5 - :784
}
static void GetPendingIrqComplete(struct _SDREQUEST* pReq) { // sdio_bus_events.c:866
    struct _SDHCD* pHcd; // r0 - :869
}
static void ProcessPendingIrqs(struct _SDHCD* pHcd, unsigned char IntPendingMsk) { // sdio_bus_events.c:906
    struct _SDLIST* pListItem; // r5 - :908
    struct _SDDEVICE* pDevice; // r0 - :909
    unsigned char funcMask; // r0 - :910
    int status; // r0 - :911
}
int TryNoIrqPendingCheck(struct _SDDEVICE* pDevice) { // sdio_bus_events.c:995
}