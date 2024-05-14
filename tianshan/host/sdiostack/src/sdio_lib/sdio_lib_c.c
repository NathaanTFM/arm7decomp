static void _iSDLIB_SetupCMD52Request(unsigned char FuncNo, unsigned long Address, int Write, unsigned char WriteData, struct _SDREQUEST* pRequest) { // sdio_lib_c.c:28
}
int _SDLIB_IssueCMD52(struct _SDDEVICE* pDevice, unsigned char FuncNo, unsigned long Address, unsigned char* pData, int ByteCount, int Write) { // sdio_lib_c.c:120
    int status; // r0 - :127
    struct _SDREQUEST* pReq; // r5 - :129
}
int _SDLIB_FindTuple(struct _SDDEVICE* pDevice, unsigned char Tuple, unsigned long* pTupleScanAddress, unsigned char* pBuffer, unsigned char* pLength) { // sdio_lib_c.c:191
    int status; // r0 - :197
    unsigned long scanStart; // r5 - :198
    unsigned char tupleCode; // None - :199
    unsigned char tupleLink; // None - :200
}
int _SDLIB_IssueConfig(struct _SDDEVICE* pDevice, unsigned short Command, void* pData, int Length) { // sdio_lib_c.c:290
    struct _SDCONFIG configHdr; // None - :295
}
int _SDLIB_SetFunctionBlockSize(struct _SDDEVICE* pDevice, unsigned short BlockSize) { // sdio_lib_c.c:319
    unsigned char data[2]; // None - :322
}
int _SDLIB_GetDefaultOpCurrent(struct _SDDEVICE* pDevice, unsigned short* pOpCurrent) { // sdio_lib_c.c:438
    unsigned long nextTpl; // None - :440
    unsigned char tplLength; // None - :441
    struct SDIO_FUNC_EXT_FUNCTION_TPL_1_1 funcTuple; // None - :442
    int status; // r0 - :443
}
static struct _SDMESSAGE_BLOCK* GetFreeMessageBlock(struct _SDMESSAGE_QUEUE* pQueue) { // sdio_lib_c.c:483
    struct _SDLIST* pItem; // r0 - :484
}
static struct _SDMESSAGE_BLOCK* GetQueuedMessage(struct _SDMESSAGE_QUEUE* pQueue) { // sdio_lib_c.c:490
    struct _SDLIST* pItem; // r0 - :491
}
struct _SDMESSAGE_QUEUE* _CreateMessageQueue(int MaxMessages, int MaxMessageLength) { // sdio_lib_c.c:532
    struct _SDMESSAGE_QUEUE* pQueue; // r5 - :534
    int status; // r4 - :535
    int ii; // r6 - :536
    struct _SDMESSAGE_BLOCK* pMsg; // r0 - :537
}
void _DeleteMessageQueue(struct _SDMESSAGE_QUEUE* pQueue) { // sdio_lib_c.c:600
    struct _SDMESSAGE_BLOCK* pMsg; // r0 - :602
}
int _PostMessage(struct _SDMESSAGE_QUEUE* pQueue, void* pMessage, int MessageLength) { // sdio_lib_c.c:670
    int status; // r6 - :673
    struct _SDMESSAGE_BLOCK* pMsg; // r0 - :674
}
int _GetMessage(struct _SDMESSAGE_QUEUE* pQueue, void* pData, int* pBufferLength) { // sdio_lib_c.c:754
    int status; // r5 - :757
    struct _SDMESSAGE_BLOCK* pMsg; // r0 - :758
}