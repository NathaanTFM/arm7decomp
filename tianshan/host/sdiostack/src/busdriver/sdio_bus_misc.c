int _IssueBusRequestBd(struct _SDHCD* pHcd, unsigned char Cmd, unsigned long Argument, unsigned long Flags, struct _SDREQUEST* pReqToUse, void* pData, int Length) { // sdio_bus_misc.c:32
    int status; // r4 - :40
    struct _SDREQUEST* pReq; // r5 - :41
}
static unsigned long ConvertVoltageCapsToOCRMask(unsigned char VoltageCaps) { // sdio_bus_misc.c:79
    unsigned long ocrMask; // r1 - :81
}
static unsigned long GetUsableOCRValue(unsigned long CardOCR, unsigned long SlotOCRMask) { // sdio_bus_misc.c:107
    int i; // r3 - :109
    unsigned long mask; // r12 - :110
}
static int ReadOCR(struct _SDHCD* pHcd, unsigned short ReadType, struct _SDREQUEST* pReq, unsigned long OCRValue, unsigned long* pOCRValueRd) { // sdio_bus_misc.c:229
    int status; // r12 - :235
}
static int AdjustSlotPower(struct _SDHCD* pHcd, unsigned long* pOCRvalue) { // sdio_bus_misc.c:434
    struct _SDCONFIG_POWER_CTRL_DATA pwrSetting; // None - :436
    int status; // r0 - :437
}
static int SelectDeselectCard(struct _SDHCD* pHcd, int Select) { // sdio_bus_misc.c:544
    int status; // r0 - :546
}
int SetOperationalBusMode(struct _SDDEVICE* pDevice, struct _SDCONFIG_BUS_MODE_DATA* pBusMode) { // sdio_bus_misc.c:1068
    int status; // r9 - :1071
    unsigned char regData; // None - :1072
    int doSet; // r0 - :1233
    unsigned char buswidth; // r0 - :1373
}
int CardInitSetup(struct _SDHCD* pHcd) { // sdio_bus_misc.c:1431
    struct _SDCONFIG_INIT_CLOCKS_DATA initClocks; // None - :1433
    struct _SDCONFIG_BUS_MODE_DATA busMode; // None - :1434
    unsigned long OCRvalue; // None - :1435
    int status; // r0 - :1436
}
int SDInitializeCard(struct _SDHCD* pHcd) { // sdio_bus_misc.c:1508
    struct _SDCONFIG_BUS_MODE_DATA busMode; // None - :1510
    int status; // r0 - :1511
    struct _SDREQUEST* pReq; // r5 - :1512
    unsigned long OCRvalue; // None - :1513
    unsigned long tplAddr; // None - :1514
    unsigned char temp; // None - :1515
    struct SDIO_MANFID_TPL manfid; // None - :1516
    unsigned char cisBuffer[3]; // None - :1520
}
int SDQuerySDIOInfo(struct _SDDEVICE* pDevice) { // sdio_bus_misc.c:2013
    int status; // r0 - :2015
    unsigned char fInfo; // None - :2017
    unsigned long nextTpl; // None - :2018
    unsigned char tplLength; // None - :2019
    unsigned char cisPtrBuffer[3]; // None - :2020
    struct SDIO_FUNC_EXT_FUNCTION_TPL_1_1 funcTuple; // None - :2021
    struct SDIO_MANFID_TPL manfid; // None - :2119
}
int SDEnableFunction(struct _SDDEVICE* pDevice, struct _SDCONFIG_FUNC_ENABLE_DISABLE_DATA* pEnData) { // sdio_bus_misc.c:2155
    int status; // r0 - :2157
    unsigned char registerValue; // None - :2158
    unsigned char mask; // r0 - :2159
    unsigned long retry; // r6 - :2160
}
int SDAllocFreeSlotCurrent(struct _SDDEVICE* pDevice, int Allocate, struct _SDCONFIG_FUNC_SLOT_CURRENT_DATA* pData) { // sdio_bus_misc.c:2261
    int status; // r0 - :2263
}
static void RawHcdIrqControl(struct _SDHCD* pHcd, int Enable) { // sdio_bus_misc.c:2334
    struct _SDCONFIG_SDIO_INT_CTRL_DATA irqData; // None - :2337
}
static void RawHcdEnableIrqPseudoComplete(struct _SDREQUEST* pReq) { // sdio_bus_misc.c:2371
}
static void RawHcdDisableIrqPseudoComplete(struct _SDREQUEST* pReq) { // sdio_bus_misc.c:2379
}
static void HcdIrqControl(struct _SDHCD* pHcd, int Enable) { // sdio_bus_misc.c:2385
    struct _SDCONFIG_SDIO_INT_CTRL_DATA irqData; // None - :2388
}
static int CheckWriteIntEnableSuccess(struct _SDREQUEST* pReq) { // sdio_bus_misc.c:2449
}
static void HcdIrqEnableComplete(struct _SDREQUEST* pReq) { // sdio_bus_misc.c:2466
}
static void HcdIrqDisableComplete(struct _SDREQUEST* pReq) { // sdio_bus_misc.c:2475
}
static void WriteIntEnableComplete(struct _SDREQUEST* pReq) { // sdio_bus_misc.c:2482
}
static void HcdAckComplete(struct _SDREQUEST* pReq) { // sdio_bus_misc.c:2491
}
int SDFunctionAckInterrupt(struct _SDDEVICE* pDevice) { // sdio_bus_misc.c:2512
    int status; // r4 - :2514
    struct _SDREQUEST* pReq; // r5 - :2516
    int setHcd; // r6 - :2517
}
int SDMaskUnmaskFunctionIRQ(struct _SDDEVICE* pDevice, int MaskInt) { // sdio_bus_misc.c:2584
    int status; // r4 - :2586
    unsigned char mask; // r3 - :2587
    unsigned char controlVal; // r2 - :2588
    int setHcd; // r5 - :2589
    struct _SDREQUEST* pReq; // r6 - :2590
}
int _SDIO_CheckResponse(struct _SDREQUEST* pReq, unsigned char CheckMode) { // sdio_bus_misc.c:2788
    int status; // r0 - :2790
    unsigned char* pResponse; // r0 - :2794
    unsigned char spiToken; // r2 - :2840
}