#include "Marionea.h"

#ifndef AP_LIST_H
#define AP_LIST_H

void UpdateApList(u16 channel, BEACON_FRAME* pFrm, SSID_ELEMENT* pSSID);
void InitApList();
void UpdateApListTask();

#endif