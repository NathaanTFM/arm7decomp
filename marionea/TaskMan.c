#define MARIONEA_INTERNAL 1
#include "TaskMan.h"

void InitializeTask()
{ // TaskMan.c:91
    static void (*pTaskFunc[24])() = {
        MLME_ScanTask, MLME_JoinTask, MLME_AuthTask, MLME_AssTask,
        MLME_ReAssTask, MLME_MeasChannelTask, RxDataFrameTask, RxManCtrlTask,
        WlIntrTxBeaconTask, DefragTask, CAM_TimerTask, RequestCmdTask,
        LowestIdleTask, MLME_BeaconLostTask, WlIntrTxEndTask, WlIntrRxEndTask,
        WlIntrMpEndTask, DefragTimerTask, UpdateApListTask, SendMessageToWmTask,
        SetParentTbttTxqTask, SendFatalErrMsgTask, TerminateWlTask, ReleaseWlTask};

    TASK_MAN *pTaskMan = &wlMan->TaskMan; // r14 - :93
    u32 i;                                // r12 - :94

    pTaskMan->NextPri = 0;
    pTaskMan->TaskPri = 0;
    for (i = 0; i < 4; i++)
    {
        pTaskMan->EnQ[i] = pTaskMan->DeQ[i] = 0xFFFF;
    }

    for (i = 0; i < 24; i++)
    {
        pTaskMan->TaskTbl[i].NextId = 0xFFFF;
        pTaskMan->TaskTbl[i].Flag = 0;
        pTaskMan->TaskTbl[i].pTaskFunc = pTaskFunc[i];
    }

    AddTask(PRIORITY_LOWEST, TASK_LOWEST_IDLE);
}

WRAM_FUNC void MainTaskRoutine()
{                                         // TaskMan.c:136
    TASK_MAN *pTaskMan = &wlMan->TaskMan; // r8 - :140
    u32 x;                                // r0 - :141

    pTaskMan->NextPri = 0;
    pTaskMan->CurrTaskID = 0;

    while (1)
    {
        void *msg; // None - :150
        if (OS_ReceiveMessage(wlMan->pRecvMsgQueue, &msg, 0))
            ExecuteMessage(&msg);

        x = OS_DisableIrqMask(0x1000010);
        pTaskMan->TaskPri = pTaskMan->NextPri;

        if (pTaskMan->EnQ[pTaskMan->TaskPri] == 0xFFFF)
        {
            pTaskMan->NextPri++;
            OS_EnableIrqMask(x);
        }
        else
        {
            OS_EnableIrqMask(x);
            pTaskMan->CurrTaskID = DeleteTask(pTaskMan->TaskPri);
            pTaskMan->TaskTbl[pTaskMan->CurrTaskID].pTaskFunc();
            pTaskMan->CurrTaskID = 0xFFFF;
        }
    }
}

WRAM_FUNC void AddTask(long nPriority, u32 nTaskID)
{                                           // TaskMan.c:291
    TASK_MAN *pTaskMan = &wlMan->TaskMan;   // r4 - :293
    TASK_TBL *pTaskTbl = pTaskMan->TaskTbl; // r5 - :294
    u32 x;                                  // r0 - :295

    x = OS_DisableIrqMask(0x1000010);
    if (!pTaskTbl[nTaskID].Flag)
    {
        pTaskTbl[nTaskID].Flag = 1;
        pTaskTbl[nTaskID].NextId = 0xFFFF;

        if (pTaskMan->DeQ[nPriority] == 0xFFFF)
        {
            pTaskMan->EnQ[nPriority] = nTaskID;
        }
        else
        {
            pTaskMan->TaskTbl[pTaskMan->DeQ[nPriority]].NextId = nTaskID;
        }

        pTaskMan->DeQ[nPriority] = nTaskID;
        if (nPriority < pTaskMan->NextPri)
        {
            pTaskMan->NextPri = nPriority;
        }
    }

    OS_EnableIrqMask(x);

    if (nPriority != 3 && pTaskMan->TaskPri == 3)
        OS_SendMessage(wlMan->pRecvMsgQueue, 0, 0);
}

WRAM_FUNC u32 DeleteTask(u32 nPriority)
{                                         // TaskMan.c:362
    TASK_MAN *pTaskMan = &wlMan->TaskMan; // r4 - :364
    TASK_TBL *pTaskTbl;                   // r0 - :365
    u32 x, nTaskID;                       // r0, r5 - :366

    x = OS_DisableIrqMask(0x1000010);   // :369
    nTaskID = pTaskMan->EnQ[nPriority]; // :372

    if (nTaskID != 0xFFFF)
    {
        pTaskTbl = pTaskMan->TaskTbl; // :375
        pTaskTbl[nTaskID].Flag = 0;   // :379

        if (pTaskTbl[nTaskID].NextId == 0xFFFF)
        {                                      // :382
            pTaskMan->EnQ[nPriority] = 0xFFFF; // :384
            pTaskMan->DeQ[nPriority] = 0xFFFF; // :385
        }
        else
        {
            pTaskMan->EnQ[nPriority] = pTaskTbl[nTaskID].NextId; // :389
            pTaskTbl[nTaskID].NextId = 0xFFFF;                   // :390
        }
    }

    OS_EnableIrqMask(x); // :395
    return nTaskID;      // :398
}

WRAM_FUNC void LowestIdleTask()
{              // TaskMan.c:416
    void *msg; // None - :418

    OS_ReceiveMessage(wlMan->pRecvMsgQueue, &msg, 1);
    ExecuteMessage(&msg);
    AddTask(PRIORITY_LOWEST, TASK_LOWEST_IDLE);
}

WRAM_FUNC void ExecuteMessage(void **pMsg)
{ // TaskMan.c:501
    if (*pMsg)
    {
        NewHeapBuf(&wlMan->HeapMan.RequestCmd, *pMsg);
        AddTask(PRIORITY_LOW, TASK_REQUEST_CMD);
    }
}
