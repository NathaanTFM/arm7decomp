#include "Marionea.h"

#ifndef AP_LIST_H
#define AP_LIST_H

#ifdef MARIONEA_INTERNAL
void UpdateApList(u16 channel, BEACON_FRAME* pFrm, SSID_ELEMENT* pSSID);
void InitApList();
void UpdateApListTask();
#endif

#endif