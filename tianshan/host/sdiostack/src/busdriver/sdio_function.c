int _SDIO_RegisterFunction(struct _SDFUNCTION* pFunction) { // sdio_function.c:74
    int status; // r0 - :76
}
int _SDIO_UnregisterFunction(struct _SDFUNCTION* pFunction) { // sdio_function.c:147
    int status; // r0 - :149
    struct _SDDEVICE* pDevice; // r0 - :150
    struct _SDLIST* pTemp; // r5 - :161
}
int ProbeForFunction(struct _SDDEVICE* pDevice) { // sdio_function.c:276
    int status; // r4 - :277
    struct _SDLIST* pList; // r4 - :278
    struct _SDFUNCTION* pFunction; // r5 - :279
}
int IsPotentialIdMatch(struct _SD_PNP_INFO* pIdsDev, struct _SD_PNP_INFO* pIdsFuncList) { // sdio_function.c:461
    struct _SD_PNP_INFO* pTFn; // r0 - :462
    int match; // r2 - :463
}
int NotifyDeviceRemove(struct _SDDEVICE* pDevice) { // sdio_function.c:522
    int status; // r0 - :523
    struct _SDREQUESTQUEUE cancelQueue; // None - :524
    struct _SDREQUEST* pReq; // r8 - :525
    struct _SDLIST* pTemp; // r9 - :542
}
int RemoveHcdFunctions(struct _SDHCD* pHcd) { // sdio_function.c:613
    int status; // r0 - :614
    struct _SDLIST* pList; // r4 - :615
    struct _SDFUNCTION* pFunction; // r0 - :616
    struct _SDDEVICE* pDevice; // r0 - :617
    struct _SDLIST* pTemp; // r8 - :633
}
int RemoveAllFunctions() { // sdio_function.c:655
    int status; // r0 - :657
    struct _SDLIST* pList; // r5 - :658
}