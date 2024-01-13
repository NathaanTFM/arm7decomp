#define MARIONEA_INTERNAL 1
#include "WlSys.h"

void WlessLibReboot() { // WlSys.c:70
    ClearTimeOut();
    WShutdown();
    InitMac();
    ReleaseAllWlHeapBuf();
    InitializeTask();
    InitializeParam(wlMan->Config.pCAM, wlMan->Config.CamMaxStaNum);
    InitializeCmdIf();
    InitializeMLME();
    InitializeCAM();
    WSetDefaultParameters();
}

u32 WL_InitDriver(WlInit* pInit) { // WlSys.c:120
    u32 adrs; // r0 - :122
    u32 crc; // None - :212
    
    wlMan = (WL_MAN *)pInit->workingMemAdrs;
    MIi_CpuClearFast(0, wlMan, sizeof(*wlMan));
    
    wlMan->lockID = OS_GetLockID();
    wlMan->DmaChannel = pInit->dmaChannel;
    wlMan->DmaMaxCount = pInit->dmaMaxSize / 2;
    
    if (!wlMan->DmaMaxCount)
        wlMan->DmaMaxCount = -1;
    
    InitializeHeapBuf((HEAP_INFO *)&pInit->heapType);
    FLASH_MakeImage();
    
    REG_POWERCNT |= 2;
    REG_WIFIWAITCNT = 0x30;
    
    wlMan->WlDbgLevel = 3;
    
    FLASH_Read(0x3C, 2, (u8*)&wlMan->EnableChannel);
    FLASH_Read(0x3E, 2, (u8*)&wlMan->WlOperation);
    
    wlMan->pRecvMsgQueue = pInit->sendMsgQueuep;
    wlMan->pSendMsgQueue = pInit->recvMsgQueuep;
    
    adrs = pInit->camSize / 0x1C;
    InitializeParam((CAM_ELEMENT *)pInit->camAdrs, adrs);
    InitializeTask();
    InitializeCmdIf();
    InitializeMLME();
    InitializeCAM();
    InitializeAlarm();
    
    if (FLASH_VerifyCheckSum(&crc)) {
        wlMan->Config.DiagResult |= 0x80;
        
    } else {
        WConfigDevice();
        DiagMacRegister();
        WWakeUp();
        InitMac();
        InitRF();
        DiagMacMemory();
        DiagBaseBand();
        InitBaseBand();
        WSetDefaultParameters();
        WShutdown();
    }
    
    OS_CreateThread(
        &wlMan->TaskMan.Thread, MainTaskRoutine, 0, 
        pInit->stack, pInit->stacksize, pInit->priority
    );
    OS_WakeupThreadDirect(&wlMan->TaskMan.Thread);
    
    InitializeIntr();
    return wlMan->Config.DiagResult;
}

struct _OSThread* WL_GetThreadStruct() { // WlSys.c:310
    return &wlMan->TaskMan.Thread;
}