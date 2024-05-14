void AR6KFreeIOPacket(struct _AR6K_DEVICE* pDev, struct _HTC_PACKET* pPacket) { // ar6k.c:40
}
struct _HTC_PACKET* AR6KAllocIOPacket(struct _AR6K_DEVICE* pDev) { // ar6k.c:47
}
enum _enum_412412 DevSetup(struct _AR6K_DEVICE* pDev) { // ar6k.c:58
    unsigned long mailboxaddrs[4]; // None - :60
    unsigned long blocksizes[4]; // None - :61
    enum _enum_412412 status; // r5 - :62
    int i; // r6 - :63
    struct _HTC_PACKET* pIOPacket; // r0 - :87
}
static enum _enum_412412 DevDisableInterrupts(struct _AR6K_DEVICE* pDev) { // ar6k.c:235
    struct _AR6K_IRQ_ENABLE_REGISTERS regs; // None - :237
}
enum _enum_412412 DevUnmaskInterrupts(struct _AR6K_DEVICE* pDev) { // ar6k.c:370
}
enum _enum_412412 DevMaskInterrupts(struct _AR6K_DEVICE* pDev) { // ar6k.c:390
    enum _enum_412412 status; // r0 - :392
}
static void DevDoEnableDisableRecvAsyncHandler(void* Context, struct _HTC_PACKET* pPacket) { // ar6k.c:405
}
static enum _enum_412412 DevDoEnableDisableRecvOverride(struct _AR6K_DEVICE* pDev, int EnableRecv, int AsyncMode) { // ar6k.c:423
    enum _enum_412412 status; // r5 - :425
    struct _HTC_PACKET* pIOPacket; // r6 - :426
}
static enum _enum_412412 DevDoEnableDisableRecvNormal(struct _AR6K_DEVICE* pDev, int EnableRecv, int AsyncMode) { // ar6k.c:471
    enum _enum_412412 status; // r6 - :473
    struct _HTC_PACKET* pIOPacket; // r7 - :474
    struct _AR6K_IRQ_ENABLE_REGISTERS regs; // None - :475
}
enum _enum_412412 DevStopRecv(struct _AR6K_DEVICE* pDev, int AsyncMode) { // ar6k.c:537
}
enum _enum_412412 DevEnableRecv(struct _AR6K_DEVICE* pDev, int AsyncMode) { // ar6k.c:546
}