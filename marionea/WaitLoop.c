#define MARIONEA_INTERNAL 1
#include "WaitLoop.h"

WRAM_FUNC void WaitLoop_Rxpe()
{
    u32 i;

    for (i = 4000; i; --i)
    {
        if ((W_RF_PINS & 0x80) != 0)
            break;
    }
}

WRAM_FUNC void WaitLoop_Waitus(u32 us, void (*TimeoutFunc)(void *))
{
    volatile u32 flag = 1;

    OS_SetAlarm(&wlMan->Alarm, ((33514ULL * us) >> 6ULL) / 1000ULL, TimeoutFunc, (void *)&flag);
    while (flag)
        ;
}

WRAM_FUNC void WaitLoop_ClrAid()
{
    u32 x, state, b = 1;

    while (b)
    {                        // :102
        x = OS_DisableIrq(); // :105

        if ((W_RF_PINS & 0x03) != 0x03)
        {
            state = W_RF_STATUS;

            if (state != 0x05 && state != 0x07 && state != 0x08)
            {
                W_AID_LOW = 0;
                b = 0;
            }
        }

        OS_RestoreIrq(x);
    }
}

WRAM_FUNC u32 WaitLoop_BbpAccess()
{
    u32 i;
    for (i = 0; i < 0x2800; ++i)
    {
        if ((W_BB_BUSY & 0x01) == 0)
            return 0;
    }
    return 1;
}

WRAM_FUNC u32 WaitLoop_RfAccess()
{
    u32 i;
    for (i = 0; i < 0x2800; ++i)
    {
        if ((W_RF_BUSY & 0x01) == 0)
            return 0;
    }
    return 1;
}
