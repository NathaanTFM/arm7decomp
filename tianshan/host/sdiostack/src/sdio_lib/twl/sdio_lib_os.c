int SDLIB_OSCreateHelper(struct _OSKERNEL_HELPER* pHelper, int (*pFunction)(struct _OSKERNEL_HELPER*), void* pContext) { // sdio_lib_os.c:30
    void* tskStack; // r5 - :35
    unsigned long stackSize; // r6 - :36
    unsigned long pri; // r7 - :37
}
void HelperLaunch(void* pExtendedInformation) { // sdio_lib_os.c:73
    struct _OSKERNEL_HELPER* pHelperContext; // r0 - :76
}
void SDLIB_OSDeleteHelper(struct _OSKERNEL_HELPER* pHelper) { // sdio_lib_os.c:88
    int tskState; // r0 - :90
}