#include "Marionea.h"

#ifndef WL_INTR_TASK_H
#define WL_INTR_TASK_H

void WlIntrTxBeaconTask();
void WlIntrTxEndTask();
void WlIntrRxEndTask();
void WlIntrMpEndTask();
void SetParentTbttTxqTask();

#endif