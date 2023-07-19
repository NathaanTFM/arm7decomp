#include "Mongoose.h"

static void (*pTaskFunc[24])(); // :47

void InitializeTask() { // TaskMan.c:91
    TASK_MAN* pTaskMan; // r14 - :93
    u32 i; // r12 - :94
}

void MainTaskRoutine() { // TaskMan.c:136
    TASK_MAN* pTaskMan; // r8 - :140
    u32 x; // r0 - :141
    void* msg; // None - :150
}

void AddTask(long nPriority, u32 nTaskID) { // TaskMan.c:291
    TASK_MAN* pTaskMan; // r4 - :293
    TASK_TBL* pTaskTbl; // r5 - :294
    u32 x; // r0 - :295
}

u32 DeleteTask(u32 nPriority) { // TaskMan.c:362
    TASK_MAN* pTaskMan; // r4 - :364
    TASK_TBL* pTaskTbl; // r0 - :365
    u32 nTaskID; // r5 - :366
    u32 x; // r0 - :366
}

void LowestIdleTask() { // TaskMan.c:416
    void* msg; // None - :418
}

void ExecuteMessage(void** pMsg) { // TaskMan.c:501
}
