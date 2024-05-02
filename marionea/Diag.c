#define MARIONEA_INTERNAL 1
#include "Diag.h"

static const u16 test_pattern[3] = {0xFFFF, 0x5A5A, 0xA5A5}; // :63

static const TEST_REGS test_reg[27] = {
    {0x006, 0x003F},
    {0x018, 0xFFFF},
    {0x01A, 0xFFFF},
    {0x01C, 0xFFFF},
    {0x020, 0xFFFF},
    {0x022, 0xFFFF},
    {0x024, 0xFFFF},
    {0x02A, 0x07FF},
    {0x050, 0xFFFF},
    {0x052, 0xFFFF},
    {0x056, 0x0FFE},
    {0x058, 0x1FFE},
    {0x05A, 0x0FFE},
    {0x05C, 0x0FFF},
    {0x062, 0x1FFE},
    {0x064, 0x0FFF},
    {0x068, 0x1FFE},
    {0x06C, 0x0FFF},
    {0x074, 0x1FFE},
    {0x122, 0xFFFF},
    {0x124, 0xFFFF},
    {0x128, 0xFFFF},
    {0x130, 0x0FFF},
    {0x132, 0x8FFF},
    {0x134, 0xFFFF},
    {0x140, 0xFFFF},
    {0x142, 0xFFFF}}; // :65

void DiagMacRegister()
{                       // Diag.c:101
    volatile u16 *pReg; // r0 - :103
    u16 wd, rd;         // r6, r0 - :104
    u32 err;            // None - :105
    u32 j;              // r2 - :105
    u32 i;              // r3 - :107

    err = 0;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 0x1B; j++)
        {
            pReg = (u16 *)(0x4808000 + test_reg[j].adrs);
            *pReg = (test_pattern[i] & test_reg[j].mask);
            if (*pReg != (test_pattern[i] & test_reg[j].mask))
            {
                if (err++ > 0x20)
                    goto exit;
            }
        }
    }

    wd = 0x1234;
    for (i = 0; i < 0x1B; i++)
    {
        pReg = (u16 *)(0x4808000 + test_reg[i].adrs);
        *pReg = (wd & test_reg[i].mask);
        wd += 0x1234;
    }

    rd = 0x1234;
    for (i = 0; i < 0x1B; i++)
    {
        pReg = (u16 *)(0x4808000 + test_reg[i].adrs);
        if (*pReg != (rd & test_reg[i].mask))
        {
            if (err++ > 0x20)
                goto exit;
        }

        rd += 0x1234;
    }

    for (i = 0; i < 0x1B; i++)
    {
        pReg = (u16 *)(0x4808000 + test_reg[i].adrs);
        *pReg = 0;
        if (*pReg != 0)
        {
            if (err++ > 0x20)
                goto exit;
        }
    }

exit:
    if (err)
        wlMan->Config.DiagResult |= 1;
}

void DiagMacMemory()
{                      // Diag.c:229
    u32 err;           // None - :231
    u32 i;             // r2 - :231
    volatile u16 *p16; // r0 - :232
    u16 w16;           // r3 - :233

    err = 0;

    // test 1
    p16 = &W_MACMEM(0);
    w16 = 0xFFFF;

    for (i = 0; i < 0x2000; i += 2)
    {
        *(p16++) = w16--;
    }

    p16 = &W_MACMEM(0);
    w16 = 0xFFFF;

    for (i = 0; i < 0x2000; i += 2, p16++, w16--)
    {
        if (*p16 != w16)
        {
            if (err++ > 0x20)
                goto exit;
        }
    }

    // test 2
    p16 = &W_MACMEM(0);
    w16 = 0x5A5A;

    for (i = 0; i < 0x2000; i += 2)
    {
        *(p16++) = w16;
        w16 = ~w16;
    }

    p16 = &W_MACMEM(0);
    w16 = 0x5A5A;

    for (i = 0; i < 0x2000; i += 2, p16++, w16 = ~w16)
    {
        if (*p16 != w16)
        {
            if (err++ > 0x20)
                goto exit;
        }
    }

    // test 3
    p16 = &W_MACMEM(0);
    w16 = 0xA5A5;

    for (i = 0; i < 0x2000; i += 2)
    {
        *(p16++) = w16;
        w16 = ~w16;
    }

    p16 = &W_MACMEM(0);
    w16 = 0xA5A5;

    for (i = 0; i < 0x2000; i += 2, p16++, w16 = ~w16)
    {
        if (*p16 != w16)
        {
            if (err++ > 0x20)
                goto exit;
        }
    }

exit:
    if (err)
    {
        wlMan->Config.DiagResult |= 2;
    }
}

static const u16 BBPDiagSkipAdrsES1[25] = {
    0x02, 0x04, 0x05, 0x06, 0x07,
    0x1C, 0x1D, 0x1E, 0x1F, 0x20,
    0x21, 0x26, 0x29, 0x2C, 0x2D,
    0x2E, 0x2F, 0x30, 0x33, 0x34,
    0x35, 0x36, 0x37, 0x65, 0x00}; // :832

static const u16 BBPDiagSkipAdrsRelease[28] = {
    0x00, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    0x0F, 0x10, 0x11, 0x12, 0x14, 0x15, 0x16,
    0x17, 0x18, 0x19, 0x1A, 0x27, 0x4D, 0x5D,
    0x5E, 0x5F, 0x60, 0x61, 0x64, 0x66, 0x00}; // :838

void DiagBaseBand()
{                         // Diag.c:843
    u16 *BBPDiagSkipAdrs; // r0 - :845
    u32 err;              // None - :846
    u32 i;                // r4 - :846
    u32 adrs;             // r6 - :846
    u16 r8;               // r0 - :847
    u16 w8;               // r4 - :847
    u32 j;                // r5 - :849
    u16 bit;              // r6 - :850
}
