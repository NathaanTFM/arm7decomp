#include "Marionea.h"

#ifndef WL_INTR_H
#define WL_INTR_H

void WlIntr();
void MacBugTxMp();
void* AdjustRingPointer(void* p);
void InitializeIntr();
void ReleaseIntr();

#endif