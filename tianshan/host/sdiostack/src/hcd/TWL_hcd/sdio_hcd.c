static void HcdUnmaskCardIrq(int FromISR) { // sdio_hcd.c:86
}
static void HcdMaskCardIrq(int FromISR) { // sdio_hcd.c:109
}
int GetResponseData(struct _SDREQUEST* pReq) { // sdio_hcd.c:302
    unsigned short regValue; // r0 - :304
}
void DumpCurrentRequestInfo() { // sdio_hcd.c:356
}
int HcdRequest(struct _SDHCD* pHcd) { // sdio_hcd.c:622
    int status; // r0 - :624
    struct _SDHCD_DRIVER_CONTEXT* pHct; // r11 - :625
    struct _SDREQUEST* pReq; // r7 - :626
    unsigned short cmdReg; // r8 - :627
    int extendedCmd; // r10 - :628
    unsigned short Status1; // r0 - :774
    unsigned long timeout; // r0 - :775
}
int HcdConfig(struct _SDHCD* pHcd, struct _SDCONFIG* pConfig) { // sdio_hcd.c:858
    int status; // r6 - :860
    struct _SDHCD_DRIVER_CONTEXT* pHct; // r7 - :861
    unsigned char irqModeFlags; // r0 - :885
}
static int DpcTask(struct _OSKERNEL_HELPER* pHelper) { // sdio_hcd.c:956
}
int HcdInitialize() { // sdio_hcd.c:1009
    int status; // r0 - :1011
}
void HcdDeinitialize() { // sdio_hcd.c:1056
}
int SDIO_SampleHCDInit(unsigned long DMAFifoMode) { // sdio_hcd.c:1072
    int err; // r0 - :1077
    int status; // r0 - :1078
}
int SDIO_SampleHCDCleanup() { // sdio_hcd.c:1158
}
static int TransferWriteData(struct _SDREQUEST* pReq) { // sdio_hcd.c:1178
    int done; // r1 - :1180
    int copyBytes; // r0 - :1181
    unsigned short data; // r6 - :1203
    unsigned char* pData; // r7 - :1204
}
static int TransferReadData(struct _SDREQUEST* pReq) { // sdio_hcd.c:1231
    int done; // r3 - :1233
    unsigned long copyBytes; // r0 - :1234
    unsigned char* pData; // r12 - :1235
    unsigned short data; // r0 - :1260
}
void AckInterrupt() { // sdio_hcd.c:1288
    unsigned short info1MaskSave; // r0 - :1290
    unsigned short info2MaskSave; // r0 - :1292
    unsigned short sdioinfo1MaskSave; // r0 - :1294
}
static void HcdInterruptHandler() { // sdio_hcd.c:1341
    int status; // r7 - :1343
    unsigned short info1; // r0 - :1344
    unsigned short info2; // r0 - :1345
    unsigned short info1mask; // r0 - :1346
    unsigned short info2mask; // r0 - :1347
    struct _SDREQUEST* pReq; // r8 - :1350
    int fifoInts; // r1 - :1436
}