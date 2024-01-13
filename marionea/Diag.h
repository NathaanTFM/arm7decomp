#include "Marionea.h"

#ifndef DIAG_H
#define DIAG_H

#ifdef MARIONEA_INTERNAL
typedef struct {
    u16 adrs; // offset 00
    u16 mask; // offset 02
} TEST_REGS;

void DiagMacRegister();
void DiagMacMemory();
void DiagBaseBand();
#endif

#endif