#include "Mongoose.h"

NWMSPWork nwsWork; // :23

void NWMSP_Init(NwmspInit *nwmInit)
{ // nwmsp_system.c:37
}

static void NWMSPi_ReceiveFifo7(u32 data, int err)
{                    // nwmsp_system.c:136
    NWMSPWork *sys;  // r4 - :139
    int result;      // r0 - :140
    u32 *reqbuf;     // r0 - :141
    u16 recvBufSize; // r0 - :158
    NWMCallback *cb; // r0 - :187
}

void *NWMSP_GetBuffer4Callback2Nwm9()
{                              // nwmsp_system.c:221
    NWMSPWork *sys = &nwsWork; // r4 - :223

    if (sys->nwm7buf == 0)
        return 0;

    while ((sys->nwm7buf->callbackSyncFlag & 1) != 0)
        SVC_WaitByLoop(256);

    sys->nwm7buf->callbackSyncFlag |= 1;
    return sys->nwm7buf->fifo7to9;
}

void NWMSP_ReturnResult2Nwm9(void *ptr)
{ // nwmsp_system.c:253
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_WMW, (u32)ptr, 0) < 0)
        SVC_WaitByLoop(256);
}

void NWMSP_SetAPIActiveFlag(u16 apiid)
{                              // nwmsp_system.c:268
    NWMSPWork *sys = &nwsWork; // r0 - :270

    if (sys->status != 0)
    {
        OS_LockMutex(&sys->apiFlagMtx);
        sys->status->apiActiveFlag[apiid] = 1;
        OS_UnlockMutex(&sys->apiFlagMtx);
    }
}

void NWMSP_ClearAPIActiveFlag(u16 apiid)
{                              // nwmsp_system.c:281
    NWMSPWork *sys = &nwsWork; // r0 - :283

    if (sys->status != 0)
    {
        OS_LockMutex(&sys->apiFlagMtx);
        sys->status->apiActiveFlag[apiid] = 0;
        OS_UnlockMutex(&sys->apiFlagMtx);
    }
}

int NWMSP_IsAPIActive(u16 apiid)
{ // nwmsp_system.c:295
    return nwsWork.status->apiActiveFlag[apiid] == 1;
}

int NWMSPi_CheckInitialized()
{ // nwmsp_system.c:311
    return nwsWork.nwmInitialized != 0;
}