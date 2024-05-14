int _SDIO_BusDriverInitialize() { // sdio_bus.c:38
    int status; // r7 - :40
}
void _SDIO_BusDriverCleanup() { // sdio_bus.c:159
}
void _SDIO_DeleteCardDetectTsk() { // sdio_bus.c:190
}
static void CleanupHcd(struct _SDHCD* pHcd) { // sdio_bus.c:201
}
int _SDIO_RegisterHostController(struct _SDHCD* pHcd) { // sdio_bus.c:327
    int status; // r4 - :328
    int slotNumber; // r0 - :347
}
int _SDIO_UnregisterHostController(struct _SDHCD* pHcd) { // sdio_bus.c:408
    int status; // r4 - :409
}
struct _SDDEVICE* AllocateDevice(struct _SDHCD* pHcd) { // sdio_bus.c:613
    struct _SDDEVICE* pDevice; // r5 - :615
}
void FreeDevice(struct _SDDEVICE* pDevice) { // sdio_bus.c:628
}
int AddDeviceToList(struct _SDDEVICE* pDevice) { // sdio_bus.c:636
    int success; // r5 - :638
}
int DeleteDevices(struct _SDHCD* pHcd) { // sdio_bus.c:664
    int status; // r0 - :666
    struct _SDDEVICE* pDevice; // r6 - :667
    struct _SDLIST* pTemp; // r7 - :673
}
void FreeRequest(struct _SDREQUEST* pReq) { // sdio_bus.c:780
}
struct _SDREQUEST* AllocateRequest() { // sdio_bus.c:817
    struct _SDLIST* pItem; // r0 - :819
    struct _SDREQUEST* pReq; // r5 - :821
    unsigned long internalflags; // r0 - :822
}
void FreeSignal(struct _SIGNAL_ITEM* pSignal) { // sdio_bus.c:899
}
struct _SIGNAL_ITEM* AllocateSignal() { // sdio_bus.c:930
    struct _SDLIST* pItem; // r0 - :932
    struct _SIGNAL_ITEM* pSignal; // r0 - :933
}
struct _SDREQUEST* IssueAllocRequest() { // sdio_bus.c:976
}
void IssueFreeRequest(struct _SDREQUEST* pReq) { // sdio_bus.c:984
}
int IssueBusRequest(struct _SDDEVICE* pDev, struct _SDREQUEST* pReq) { // sdio_bus.c:992
}
static void HcdConfigComplete(struct _SDREQUEST* pReq) { // sdio_bus.c:1000
}
int IssueBusConfig(struct _SDDEVICE* pDev, struct _SDCONFIG* pConfig) { // sdio_bus.c:1069
    int status; // r0 - :1071
    int cmdLength; // r2 - :1072
}
static void SynchCompletion(struct _SDREQUEST* pRequest) { // sdio_bus.c:1489
}
int IssueRequestToHCD(struct _SDHCD* pHcd, struct _SDREQUEST* pReq) { // sdio_bus.c:1524
    int status; // r0 - :1526
    struct _SIGNAL_ITEM* pSignal; // r5 - :1527
    int handleFailedReqSubmit; // r6 - :1528
}