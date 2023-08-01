#include "Mongoose.h"

void InitializeTask() { // TaskMan.c:91
    static void (*pTaskFunc[24])() = {
        MLME_ScanTask, MLME_JoinTask, MLME_AuthTask, MLME_AssTask,
        MLME_ReAssTask, MLME_MeasChannelTask, RxDataFrameTask,
        RxManCtrlTask, WlIntrTxBeaconTask, DefragTask, CAM_TimerTask,
        RequestCmdTask, LowestIdleTask, MLME_BeaconLostTask,
        WlIntrTxEndTask, WlIntrRxEndTask, WlIntrMpEndTask,
        DefragTimerTask, UpdateApListTask, SendMessageToWmTask,
        SetParentTbttTxqTask, SendFatalErrMsgTask, TerminateWlTask,
        ReleaseWlTask
    };

    TASK_MAN* pTaskMan = &wlMan->TaskMan; // r14 - :93
    u32 i; // r12 - :94
    
    pTaskMan->NextPri = 0;
    pTaskMan->TaskPri = 0;
    for (i = 0; i < 4; i++) {
        pTaskMan->EnQ[i] = pTaskMan->DeQ[i] = 0xFFFF;
    }
    
    for (i = 0; i < 24; i++) {
        pTaskMan->TaskTbl[i].NextId = 0xFFFF;
        pTaskMan->TaskTbl[i].Flag = 0;
        pTaskMan->TaskTbl[i].pTaskFunc = pTaskFunc[i];
    }
    
    AddTask(3, 12);
}

void MainTaskRoutine() { // TaskMan.c:136
    TASK_MAN* pTaskMan = &wlMan->TaskMan; // r8 - :140
    u32 x; // r0 - :141
    
    pTaskMan->NextPri = 0;
    pTaskMan->CurrTaskID = 0;
    
    while (1) {
        void* msg; // None - :150
        if (OS_ReceiveMessage(wlMan->pRecvMsgQueue, &msg, 0))
            ExecuteMessage(&msg);
        
        x = OS_DisableIrqMask(0x1000010);
        pTaskMan->TaskPri = pTaskMan->NextPri;
        
        if (pTaskMan->EnQ[pTaskMan->TaskPri] == 0xFFFF) {
            pTaskMan->NextPri++;
            OS_EnableIrqMask(x);
            
        } else {
            OS_EnableIrqMask(x);
            pTaskMan->CurrTaskID = DeleteTask(pTaskMan->TaskPri);
            pTaskMan->TaskTbl[pTaskMan->CurrTaskID].pTaskFunc();
            pTaskMan->CurrTaskID = 0xFFFF;
        }
    }
    
}

void AddTask(long nPriority, u32 nTaskID) { // TaskMan.c:291
    TASK_MAN* pTaskMan = &wlMan->TaskMan; // r4 - :293
    TASK_TBL* pTaskTbl = pTaskMan->TaskTbl; // r5 - :294
    u32 x; // r0 - :295
    
    x = OS_DisableIrqMask(0x1000010);
    if (!pTaskTbl[nTaskID].Flag) {
        pTaskTbl[nTaskID].Flag = 1;
        pTaskTbl[nTaskID].NextId = 0xFFFF;
        
        if (pTaskMan->DeQ[nPriority] == 0xFFFF) {
            pTaskMan->EnQ[nPriority] = nTaskID;
            
        } else {
            pTaskMan->TaskTbl[pTaskMan->DeQ[nPriority]].NextId = nTaskID;
        }
        
        pTaskMan->DeQ[nPriority] = nTaskID;
        if (nPriority < pTaskMan->NextPri) {
            pTaskMan->NextPri = nPriority;
        }
    }
    
    OS_EnableIrqMask(x);
    
    if (nPriority != 3 && pTaskMan->TaskPri == 3)
        OS_SendMessage(wlMan->pRecvMsgQueue, 0, 0);
}

// TODO: register swap
// (pTaskTbl is probably just pTaskMan->TaskTbl)
u32 DeleteTask(u32 nPriority) { // TaskMan.c:362
    TASK_MAN* pTaskMan = &wlMan->TaskMan; // r4 - :364
    TASK_TBL* pTaskTbl; // r0 - :365
    u32 x, nTaskID; // r0, r5 - :366
    
    x = OS_DisableIrqMask(0x1000010);
    nTaskID = pTaskMan->EnQ[nPriority];
    
    if (nTaskID != 0xFFFF) {
        pTaskTbl = &pTaskMan->TaskTbl[nTaskID];
        pTaskTbl->Flag = 0;
        
        if (pTaskTbl->NextId == 0xFFFF) {
            pTaskMan->EnQ[nPriority] = 0xFFFF;
            pTaskMan->DeQ[nPriority] = 0xFFFF;
            
        } else {
            pTaskMan->EnQ[nPriority] = pTaskTbl->NextId;
            pTaskTbl->NextId = 0xFFFF;
        }
    }
    
    OS_EnableIrqMask(x);
    return nTaskID;
}

void LowestIdleTask() { // TaskMan.c:416
    void* msg; // None - :418
    
    OS_ReceiveMessage(wlMan->pRecvMsgQueue, &msg, 1);
    ExecuteMessage(&msg);
    AddTask(3, 12);
}

void ExecuteMessage(void** pMsg) { // TaskMan.c:501
    if (*pMsg) {
        NewHeapBuf(&wlMan->HeapMan.RequestCmd, *pMsg);
        AddTask(2, 11);
    }
}
