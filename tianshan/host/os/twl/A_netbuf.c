void a_netbuf_init() { // A_netbuf.c:197
    unsigned char i; // r10 - :200
}
void a_netbuf_cleanup() { // A_netbuf.c:298
}
void* a_netbuf_alloc(unsigned long size) { // A_netbuf.c:349
    struct ath_buf_hdr* hdr; // r0 - :352
    struct ath_queue_element* element; // r2 - :353
}
void a_netbuf_free(void* bufPtr) { // A_netbuf.c:396
    struct ath_buf_hdr* hdr; // r0 - :399
    struct ath_queue* q; // r5 - :400
    unsigned long size; // r2 - :401
}
void* a_netbuf_to_data(void* bufPtr) { // A_netbuf.c:469
}
unsigned long a_netbuf_to_len(void* bufPtr) { // A_netbuf.c:476
}
enum _enum_342646 a_netbuf_push(void* bufPtr, unsigned long len) { // A_netbuf.c:483
    struct ath_buf_hdr* hdr; // r0 - :486
}
enum _enum_342646 a_netbuf_put(void* bufPtr, unsigned long len) { // A_netbuf.c:495
}
long a_netbuf_headroom(void* bufPtr) { // A_netbuf.c:514
    struct ath_buf_hdr* hdr; // r0 - :517
}
enum _enum_342646 a_netbuf_pull(void* bufPtr, unsigned long len) { // A_netbuf.c:524
    struct ath_buf_hdr* hdr; // r0 - :527
}
void* a_netbuf_head(void* bufPtr) { // A_netbuf.c:535
}
unsigned long a_netbuf_size(void* bufPtr) { // A_netbuf.c:542
}
void* a_netbuf_alloc_rx(int size) { // A_netbuf.c:604
    struct ath_buf_hdr* hdr; // r0 - :607
    struct ath_queue_element* element; // r2 - :608
}
void a_netbuf_register_rx_callback(unsigned long length, void (*callback)(void*), void* arg) { // A_netbuf.c:650
}