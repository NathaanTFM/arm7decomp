void add_patch_id(unsigned int patch_id) { // fwpatch.c:64
}
void activate_patches() { // fwpatch.c:78
    void* dev; // r0 - :81
}
void commit_patch_buffer_to_RAM(unsigned char* buf, unsigned long ramaddr, unsigned long length) { // fwpatch.c:113
    unsigned int remaining; // r0 - :118
    unsigned int position; // r6 - :119
    unsigned int chunk_size; // r7 - :120
}
void install_patch(unsigned long romaddr, unsigned long ramaddr, unsigned long hwsz) { // fwpatch.c:147
    unsigned long rompatch_installed_id; // None - :150
}
int handle_patch_specification(unsigned char* rpdf_fd) { // fwpatch.c:193
    unsigned long rpdf_cmd; // None - :196
    unsigned long hw_remapsz; // None - :197
    unsigned long ROM_addr; // None - :198
    unsigned long RAM_addr; // None - :199
    unsigned long index; // r4 - :200
    unsigned long patch_length; // None - :200
}
int handle_patch_distribution(unsigned char* rpdf_fd) { // fwpatch.c:259
    int length; // r0 - :262
    int i; // r5 - :262
    unsigned long patch_spec_count; // None - :263
    unsigned long index; // r6 - :264
}
int start_rom_patch(void* start_addr) { // fwpatch.c:282
    unsigned char* rpdf_fd; // r0 - :285
}