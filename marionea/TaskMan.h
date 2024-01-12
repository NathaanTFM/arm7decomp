#include "Marionea.h"

#ifndef TASK_MAN_H
#define TASK_MAN_H

// Task priorities
#define PRIORITY_LOWEST 3
#define PRIORITY_LOW 2
#define PRIORITY_HIGH 1
#define PRIORITY_HIGHEST 0

// Task IDs
#define TASK_SCAN   0
#define TASK_JOIN   1
#define TASK_AUTH   2
#define TASK_ASS    3
#define TASK_RE_ASS  4
#define TASK_MEAS_CHANNEL 5
#define TASK_RX_DATA_FRAME    6
#define TASK_RX_MAN_CTRL      7
#define TASK_WL_INTR_TX_BEACON  8
#define TASK_DEFRAG 9
#define TASK_TIMER  10
#define TASK_REQUEST_CMD    11
#define TASK_LOWEST_IDLE 12
#define TASK_BEACON_LOST 13
#define TASK_WL_INTR_TX_END 14
#define TASK_WL_INTR_RX_END 15
#define TASK_WL_INTR_MP_END 16
#define TASK_DEFRAG_TIMER   17
#define TASK_UPDATE_AP_LIST 18
#define TASK_SEND_MESSAGE_TO_WM 19
#define TASK_SET_PARENT_TBTT_TXQ    20
#define TASK_SEND_FATAL_ERR_MSG 21
#define TASK_TERMINATE_WL   22
#define TASK_RELEASE    23

typedef struct {
    u16 NextId; // offset 00
    u16 Flag; // offset 02
    void (*pTaskFunc)(); // offset 04
} TASK_TBL;

typedef struct {
    u16 EnQ[4]; // offset 00
    u16 DeQ[4]; // offset 08
    u16 NextPri; // offset 10
    u16 TaskPri; // offset 12
    u16 CurrTaskID; // offset 14
    u16 pad; // offset 16
    struct _OSThread Thread; // offset 18
    TASK_TBL TaskTbl[24]; // offset bc
} TASK_MAN;

void InitializeTask();
void MainTaskRoutine();
void AddTask(long nPriority, u32 nTaskID);
u32 DeleteTask(u32 nPriority);
void LowestIdleTask();
void ExecuteMessage(void** pMsg);

#endif