#include "Mongoose.h"

static u16 test_pattern[3]; // :63
static TEST_REGS test_reg[27]; // :65

void DiagMacRegister() { // Diag.c:101
    u16* pReg; // r0 - :103
    u16 rd; // r0 - :104
    u16 wd; // r6 - :104
    u32 err; // None - :105
    u32 j; // r2 - :105
    u32 i; // r3 - :107
}

void DiagMacMemory() { // Diag.c:229
    u32 err; // None - :231
    u32 i; // r2 - :231
    u16* p16; // r0 - :232
    u16 w16; // r3 - :233
}

static u16 BBPDiagSkipAdrsES1[25]; // :832
static u16 BBPDiagSkipAdrsRelease[28]; // :838

void DiagBaseBand() { // Diag.c:843
    u16* BBPDiagSkipAdrs; // r0 - :845
    u32 err; // None - :846
    u32 i; // r4 - :846
    u32 adrs; // r6 - :846
    u16 r8; // r0 - :847
    u16 w8; // r4 - :847
    u32 j; // r5 - :849
    u16 bit; // r6 - :850
}
