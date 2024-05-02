#define MARIONEA_INTERNAL 1
#include "ApList.h"

void UpdateApList(u16 channel, BEACON_FRAME *pFrm, SSID_ELEMENT *pSSID)
{                                         // ApList.c:53
    WlApElement *pApList = wlMan->ApList; // r4 - :55
    u32 i, j;                             // r5, r6 - :56
    u32 free0, free1, life1;              // r6, r7, r8 - :57

    if (WL_ReadByte(&pSSID->Length) > 0x20)
        return;

    free0 = 4;
    free1 = 4;
    life1 = 1024;

    for (i = 0; i < 4; i++, pApList++)
    {
        if (pApList->rssi)
        {
            if (MatchMacAdrs(pApList->bssid, pFrm->Dot11Header.BSSID))
                goto lbl; // exact match when using a goto

            if (pApList->lifeTime < life1)
            {
                life1 = pApList->lifeTime;
                free1 = i;
            }
        }
        else
        {
            free0 = i;
        }
    }

    // TODO: is it a goto
    if (free0 != 4)
        i = free0;
    else if (free1 != 4)
        i = free1;
    else
        return;

lbl:
    // now we do the freeing or something, i don't know
    pApList = &wlMan->ApList[i]; // lmao it would have been faster if it was before the goto
    MIi_CpuClear16(0, pApList, sizeof(*pApList));

    pApList->lifeTime = 1024;
    pApList->rssi = pFrm->MacHeader.Rx.rsv_RSSI & 0xFF;
    pApList->channel = channel;
    WSetMacAdrs1(pApList->bssid, pFrm->Dot11Header.BSSID);

    // copy bssid
    pApList->ssidLength = WL_ReadByte(&pSSID->Length);
    for (j = 0; j < pApList->ssidLength; ++j)
    {
        WL_WriteByte(&pApList->ssid[j], WL_ReadByte(&pSSID->SSID[j]));
    }

    pApList->beaconInterval = pFrm->Body.BeaconInterval;
    pApList->capaInfo = pFrm->Body.CapaInfo.Data;
}

void InitApList()
{ // ApList.c:134
    MIi_CpuClear16(0, wlMan->ApList, sizeof(wlMan->ApList));
}

// TODO: I think there's a bug in this function
// pApList never gets incremented and we're not using pApList[i]
// However, it's a 100% match
void UpdateApListTask()
{                                         // ApList.c:153
    WlApElement *pApList = wlMan->ApList; // r2 - :155
    u32 i;                                // r3 - :156

    for (i = 0; i < 4; i++)
    {
        if (pApList->lifeTime > 0)
        {
            pApList->lifeTime--;

            if (pApList->lifeTime == 0)
            {
                pApList->rssi = 0;
            }
        }
    }
}