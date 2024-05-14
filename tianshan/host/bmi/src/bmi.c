void BMIInit() { // bmi.c:56
}
enum _enum_397339 BMIDone(struct hif_device* device) { // bmi.c:62
    enum _enum_397339 status; // r0 - :65
    unsigned long cid; // None - :66
}
enum _enum_397339 BMIGetTargetInfo(struct hif_device* device, struct bmi_target_info* targ_info, int* pSwflag) { // bmi.c:87
    enum _enum_397339 status; // r0 - :90
    unsigned long cid; // None - :91
}
enum _enum_397339 BMIWriteMemory(struct hif_device* device, unsigned long address, unsigned char* buffer, unsigned long length) { // bmi.c:258
}
enum _enum_397339 BMIExecute(struct hif_device* device, unsigned long address, unsigned long* param) { // bmi.c:267
    unsigned long cid; // None - :272
    enum _enum_397339 status; // r0 - :273
}
enum _enum_397339 BMIReadSOCRegister(struct hif_device* device, unsigned long address, unsigned long* param) { // bmi.c:350
    unsigned long cid; // None - :355
    enum _enum_397339 status; // r0 - :356
}
enum _enum_397339 BMIWriteSOCRegister(struct hif_device* device, unsigned long address, unsigned long param) { // bmi.c:394
    unsigned long cid; // None - :399
    enum _enum_397339 status; // r0 - :400
}
enum _enum_397339 BMIrompatchInstall(struct hif_device* device, unsigned long ROM_addr, unsigned long RAM_addr, unsigned long nbytes, unsigned long do_activate, unsigned long* rompatch_id) { // bmi.c:434
    unsigned long cid; // None - :442
    enum _enum_397339 status; // r0 - :443
}
static enum _enum_397339 _BMIrompatchChangeActivation(struct hif_device* device, unsigned long rompatch_count, unsigned long* rompatch_list, unsigned long do_activate) { // bmi.c:525
    unsigned long cid; // None - :531
    enum _enum_397339 status; // r0 - :532
    unsigned long offset; // r10 - :533
    unsigned long length; // r2 - :535
}
enum _enum_397339 BMIrompatchActivate(struct hif_device* device, unsigned long rompatch_count, unsigned long* rompatch_list) { // bmi.c:569
}
enum _enum_397339 BMIrompatchDeactivate(struct hif_device* device, unsigned long rompatch_count, unsigned long* rompatch_list) { // bmi.c:577
}
enum _enum_397339 BMILZData(struct hif_device* device, unsigned char* buffer, unsigned long length) { // bmi.c:586
    unsigned long cid; // None - :591
    enum _enum_397339 status; // r0 - :592
    unsigned long offset; // r0 - :593
    unsigned long txlen; // None - :594
    unsigned long remaining; // r5 - :594
}
enum _enum_397339 BMILZStreamStart(struct hif_device* device, unsigned long address) { // bmi.c:636
    unsigned long cid; // None - :640
    enum _enum_397339 status; // r0 - :641
}
enum _enum_397339 bmiBufferSend(struct hif_device* device, unsigned char* buffer, unsigned long length) { // bmi.c:672
    enum _enum_397339 status; // r0 - :677
    unsigned long timeout; // r0 - :678
    unsigned long mboxAddress[4]; // None - :681
}
enum _enum_397339 bmiBufferReceive(struct hif_device* device, unsigned char* buffer, unsigned long length) { // bmi.c:721
    enum _enum_397339 status; // r0 - :726
    unsigned long mboxAddress[4]; // None - :728
}