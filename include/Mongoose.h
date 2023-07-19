#include "Registers.h"

#define NELEMS(a) (sizeof(a) / sizeof(*(a)))

#include "Structures.h"
#include "Prototypes.h"

// tick

#define HW_SYSTEM_CLOCK             33514000
#define OS_SYSTEM_CLOCK                HW_SYSTEM_CLOCK

#define OSi_BOUND_SEC1	 			   128154       // 2^8 / (OS_SYSTEM_CLOCK/1000) = 128154.4
#define OSi_BOUND_SEC2 				   128  // 2^8 / OS_SYSTEM_CLOCK = 128.2
#define OSi_BOUND_TICK1				   67108        // 2^8 / (64*1000) = 67108.9
#define OSi_BOUND_TICK2 			   67108863     // 2^8 / 64 = 67108864

//---- sec to tick
#define  OS_MicroSecondsToTicks(   usec ) ((OSTick)( ((OS_SYSTEM_CLOCK/1000) * (u64)(usec)) / 64 / 1000 ))
#define  OS_MicroSecondsToTicks32( usec ) ((OSTick)( ((OS_SYSTEM_CLOCK/1000) * (u32)(usec)) / 64 / 1000 ))

#define  OS_MilliSecondsToTicks(   msec ) ((OSTick)( ((OS_SYSTEM_CLOCK/1000) * (u64)(msec)) / 64 ))
#define  OS_MilliSecondsToTicks32( msec ) ((OSTick)( ((OS_SYSTEM_CLOCK/1000) * (u32)(msec)) / 64 ))

#define  OS_SecondsToTicks(         sec ) ((OSTick)( (OS_SYSTEM_CLOCK * (u64)(sec)) / 64 ))
#define  OS_SecondsToTicks32(       sec ) ((OSTick)( (OS_SYSTEM_CLOCK * (u32)(sec)) / 64 ))

//---- tick to sec
#define  OS_TicksToMicroSeconds(   tick ) ( ((u64)(tick) * 64 * 1000) / (OS_SYSTEM_CLOCK/1000) )
#define  OS_TicksToMicroSeconds32( tick ) ( ((u32)(tick) * 64 * 1000) / (OS_SYSTEM_CLOCK/1000) )

#define  OS_TicksToMilliSeconds(   tick ) ( ((u64)(tick) * 64) / (OS_SYSTEM_CLOCK/1000) )
#define  OS_TicksToMilliSeconds32( tick ) ( ((u32)(tick) * 64) / (OS_SYSTEM_CLOCK/1000) )

#define  OS_TicksToSeconds(        tick ) ( ((u64)(tick) * 64) / OS_SYSTEM_CLOCK )
#define  OS_TicksToSeconds32(      tick ) ( ((u32)(tick) * 64) / OS_SYSTEM_CLOCK )

// 0x10 is sizeof(WlCmdHeader) + sizeof(HEAPBUF_HEADER)
#define GetCfm(req) ((void*)((u32)(req) + ((req)->header.length * 2) + 0x10))
#define SubtractAddr(ptr, value) (void*)((u32)(ptr) - (value))

// this one has no name
#define global_vtsf_var (*(u16*)0x0380FFF0)
#define wlMan (*(WL_MAN**)0x0380FFF4)

#define REG_IME (*(volatile u16*)0x4000208)
#define REG_POWERCNT (*(volatile u16*)0x4000304)

#define RING_END_PTR ((void*)0x04805F60)

#pragma optimize_for_size off

static inline BOOL OS_RestoreIrq(BOOL enable) {
    u16 prep = REG_IME;
    REG_IME = (u16)enable;
    return (BOOL)prep;
}

static inline BOOL OS_DisableIrq() {
    u16 prep = REG_IME;
    REG_IME = 0;
    return (BOOL)prep;
}

#pragma optimize_for_size on