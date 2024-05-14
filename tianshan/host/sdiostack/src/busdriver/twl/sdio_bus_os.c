int SDIO_RegisterHostController(struct _SDHCD* pHcd) { // sdio_bus_os.c:46
}
int SDIO_UnregisterHostController(struct _SDHCD* pHcd) { // sdio_bus_os.c:59
}
int SDIO_RegisterFunction(struct _SDFUNCTION* pFunction) { // sdio_bus_os.c:67
    int status; // r0 - :69
}
int SDIO_UnregisterFunction(struct _SDFUNCTION* pFunction) { // sdio_bus_os.c:97
    int status; // r0 - :98
}
int SDIO_HandleHcdEvent(struct _SDHCD* pHcd, unsigned char Event) { // sdio_bus_os.c:107
}
int _SDIO_BusGetDefaultSettings(struct _BDCONTEXT* pBdc) { // sdio_bus_os.c:115
}
int InitializeTimers() { // sdio_bus_os.c:136
}
int CleanupTimers() { // sdio_bus_os.c:144
}
int SDIO_CheckResponse(struct _SDHCD* pHcd, struct _SDREQUEST* pReq, unsigned char CheckMode) { // sdio_bus_os.c:162
}
int OS_InitializeDevice() { // sdio_bus_os.c:171
}
int OS_AddDevice() { // sdio_bus_os.c:179
}
void OS_RemoveDevice() { // sdio_bus_os.c:190
}
int Do_OS_IncHcdReference() { // sdio_bus_os.c:199
}
int Do_OS_DecHcdReference() { // sdio_bus_os.c:208
}
void* KernelAlloc(unsigned int size) { // sdio_bus_os.c:216
    void* startAddr; // r0 - :218
}
void KernelFree(void* ptr) { // sdio_bus_os.c:229
}
void SDIO_DeleteCardDetectTsk() { // sdio_bus_os.c:239
}
void SDIO_BusDriverCleanup() { // sdio_bus_os.c:245
}
int SDIO_BusDriverInit(SDIO_INIT_SETTINGS setting) { // sdio_bus_os.c:254
}
unsigned long _DoAtomicTest_Set(unsigned long* pValue, int BitMask) { // sdio_bus_os.c:304
    unsigned long oldValue; // r0 - :305
}
unsigned long _DoAtomicTest_Clear(unsigned long* pValue, int BitMask) { // sdio_bus_os.c:313
    unsigned long oldValue; // r0 - :314
}