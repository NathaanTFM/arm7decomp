#include "Marionea.h"

#ifndef WL_INTR_TASK_H
#define WL_INTR_TASK_H

#ifdef MARIONEA_INTERNAL
void WlIntrTxBeaconTask();
void WlIntrTxEndTask();
void WlIntrRxEndTask();
void WlIntrMpEndTask();
void SetParentTbttTxqTask();
#endif

#endif