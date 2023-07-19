#include "Mongoose.h"

static void WvrBegin(int handle);

static void* wvrVramImageBuf; // :72
static u32 wvrVramImageBufSize; // :73
static int wvrHeapHandle; // :74
static u8 wvrStatus; // :75
static struct _OSThread wvrThread; // :77
static u32 wvrWlWork[448]; // :80
static u32 wvrWlStack[384]; // :81
static WlStaElement wvrWlStaElement[16]; // :82

void WVR_Begin(int handle) { // wvr_sp.c:113
    wvrHeapHandle = handle;
    wvrVramImageBuf = 0;
    
    MI_CpuFill8(&wvrThread, 0, 0xA4);
    WvrBegin(handle);
    wvrStatus = 3;
}

void WVR_Shutdown() { // wvr_sp.c:229
    REG_POWERCNT &= ~2;
    PM_SetLEDPattern(PM_LED_PATTERN_ON);
    PMi_SetLed(PM_LED_ON);
}

#pragma dont_inline on

static void WvrBegin(int handle) { // wvr_sp.c:248
    WlInit wlInit; // None - :250
    struct WmInit wmInit; // None - :251
    
    wlInit.heapFunc.os.heapHandle = handle;
    wlInit.workingMemAdrs = (u32)wvrWlWork;
    wlInit.stack = wvrWlStack + NELEMS(wvrWlStack);
    wlInit.stacksize = sizeof(wvrWlStack);
    wlInit.priority = 4;
    wlInit.heapType = 0;
    wlInit.heapFunc.os.id = OS_ARENA_WRAM_SUBPRIV;
    wlInit.camAdrs = wvrWlStaElement;
    wlInit.camSize = 448;
    
    wmInit.dmaNo = 3;
    wlInit.dmaMaxSize = 64;
    
    wmInit.indPrio_high = 3;
    wmInit.wlPrio_high = 4;
    wmInit.reqPrio_high = 5;
    wmInit.indPrio_low = 7;
    wmInit.wlPrio_low = 8;
    wmInit.reqPrio_low = 9;
    
    WM_sp_init(&wlInit, &wmInit);
}

#pragma dont_inline off