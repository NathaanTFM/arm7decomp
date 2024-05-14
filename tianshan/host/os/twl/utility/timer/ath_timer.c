void a_timer_init(unsigned long timerTskPri) { // ath_timer.c:65
}
void a_timer_cleanup() { // ath_timer.c:86
}
static void timer_wakeup_function() { // ath_timer.c:109
}
static void ath_timer_task() { // ath_timer.c:116
    struct _ath_timer_desc* current; // r0 - :119
}
void _ath_init_timer(struct a_timer* pTimer, void* pFunction, void* pArg) { // ath_timer.c:134
    struct _ath_timer_desc* timer_desc; // r0 - :136
}
void _ath_timeout_ms(struct a_timer* pTimer, unsigned long periodMSec) { // ath_timer.c:156
    struct _ath_timer_desc* timer_desc; // r5 - :158
    struct _ath_timer_desc* init_timer_desc; // r0 - :159
    struct ath_queue_element* pre; // r6 - :160
    struct ath_queue_element* next; // r0 - :161
}
void _ath_untimeout(struct a_timer* pTimer) { // ath_timer.c:187
    struct ath_queue_element* iterator; // r4 - :190
    struct OSMutex* mutex; // r5 - :191
    struct ath_queue_element* pre; // r6 - :192
    struct _ath_timer_desc* current; // r0 - :193
}
void _ath_delete_timer(struct a_timer* pTimer) { // ath_timer.c:220
    struct ath_queue_element* iterator; // r4 - :222
    struct OSMutex* mutex; // r5 - :223
    struct _ath_timer_desc* current; // r0 - :224
    struct ath_queue_element* pre; // r6 - :225
}