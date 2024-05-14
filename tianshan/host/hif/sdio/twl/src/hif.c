int HIFInit(struct htc_callbacks* callbacks) { // hif.c:58
    unsigned long i; // r4 - :61
}
enum _enum_420561 HIFReadWrite(struct hif_device* device, unsigned long address, unsigned char* buffer, unsigned long length, unsigned long request, void* context) { // hif.c:123
    unsigned char rw; // r0 - :131
    unsigned char mode; // r2 - :132
    unsigned char opcode; // r3 - :134
    unsigned short count; // r11 - :135
    struct _SDREQUEST* sdrequest; // r0 - :136
    int sdiostatus; // r0 - :137
    enum _enum_420561 status; // r5 - :139
}
enum _enum_420561 HIFConfigureDevice(enum _enum_421430 opcode, void* config) { // hif.c:287
    unsigned long count; // r1 - :291
}
void HIFShutDownDevice(struct hif_device* device) { // hif.c:319
    unsigned long count; // r8 - :323
    struct _SDCONFIG_BUS_MODE_DATA busSettings; // None - :325
}
void hifRWCompletionHandler(struct _SDREQUEST* request) { // hif.c:391
    enum _enum_420561 status; // r0 - :394
    void* context; // r6 - :395
    struct bus_request* busrequest; // r7 - :396
}
void hifIRQHandler(void* context) { // hif.c:415
}
int hifDeviceInserted(struct _SDDEVICE* handle) { // hif.c:428
    int enabled; // r7 - :431
    unsigned char data; // None - :432
    unsigned long count; // r0 - :433
    int status; // r0 - :435
    struct _SDCONFIG_BUS_MODE_DATA busSettings; // None - :438
    struct _SDCONFIG_FUNC_ENABLE_DISABLE_DATA fData; // None - :439
    struct _SDCONFIG_FUNC_SLOT_CURRENT_DATA slotCurrent; // None - :441
    unsigned long currentBusClock; // r0 - :442
}
void HIFAckInterrupt(struct hif_device* device) { // hif.c:657
}
void HIFUnMaskInterrupt(struct hif_device* device) { // hif.c:670
}
void HIFMaskInterrupt(struct hif_device* device) { // hif.c:687
}
static void hifFreeBusRequest(struct bus_request* busrequest) { // hif.c:721
}
void hifDeviceRemoved() { // hif.c:737
}
void HIFSetHandle(void* hif_handle, void* handle) { // hif.c:810
}
void HIFDelCardDetectTask() { // hif.c:819
}