void ATH_InitFlag(struct _ATH_FLAG* mq, long value) { // ath_flag.c:87
}
int ATH_SetFlag(struct _ATH_FLAG* mq, long flags) { // ath_flag.c:106
    unsigned long enabled; // r0 - :108
}
void ATH_WaitFlag(struct _ATH_FLAG* mq, long flags) { // ath_flag.c:123
    unsigned long enabled; // r4 - :125
}
int ATH_WaitFlagTimeout(struct _ATH_FLAG* mq, long flags, long tmout) { // ath_flag.c:139
    unsigned long enabled; // r4 - :141
}