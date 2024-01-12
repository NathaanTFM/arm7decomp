#include "Marionea.h"

#ifndef WAIT_LOOP_H
#define WAIT_LOOP_H

void WaitLoop_Rxpe();
void WaitLoop_Waitus(u32 us, void (*TimeoutFunc)(void *));
void WaitLoop_ClrAid();
u32 WaitLoop_BbpAccess();
u32 WaitLoop_RfAccess();

#endif