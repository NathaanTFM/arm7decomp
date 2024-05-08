#define MARIONEA_INTERNAL 1
#include "DevCmd.h"

u16 DEV_ShutdownReqCmd(WlCmdReq *pReqt, WlCmdCfm *pCfmt)
{ // DevCmd.c:49
    WlDevShutdownCfm *pCfm = (WlDevShutdownCfm *)pCfmt;
    pCfm->header.length = 1;

    if (!(wlMan->Work.STA == STA_SHUTDOWN || wlMan->Work.STA == STA_IDLE))
        return 1;

    WSetStaState(STA_SHUTDOWN);
    return 0;
}

u16 DEV_IdleReqCmd(WlCmdReq *pReqt, WlCmdCfm *pCfmt)
{ // DevCmd.c:84
    WlDevIdleCfm *pCfm = (WlDevIdleCfm *)pCfmt;
    pCfm->header.length = 1;

    if (wlMan->Work.STA > STA_CLASS1)
        return 1; // :94

    if (wlMan->Work.bSynchro) // :97
        return 1;

    if (FLASH_VerifyCheckSum(0) != 0)
        return 14; // :104

    WSetStaState(STA_IDLE); // :108
    return 0;               // :110
}

u16 DEV_Class1ReqCmd(WlCmdReq *pReqt, WlCmdCfm *pCfmt)
{ // DevCmd.c:127
    WlDevClass1Cfm *pCfm = (WlDevClass1Cfm *)pCfmt;
    pCfm->header.length = 1; // :137

    if (wlMan->Work.STA == STA_IDLE || (wlMan->Work.STA == STA_CLASS1 && !wlMan->Work.bSynchro))
    {
        WSetStaState(STA_CLASS1); // :144
        return 0;                 // :146
    }
    else
    {
        return 1; // :149
    }
}

u16 DEV_RebootReqCmd(WlCmdReq *pReqt, WlCmdCfm *pCfmt)
{ // DevCmd.c:166
    WlDevRebootCfm *pCfm = (WlDevRebootCfm *)pCfmt;
    pCfm->header.length = 1;

    if (wlMan->Work.STA >= STA_CLASS1)
        WStop();

    WlessLibReboot();
    return 0;
}

u16 DEV_ClearWlInfoReqCmd(WlCmdReq *pReqt, WlCmdCfm *pCfmt)
{ // DevCmd.c:201
    WlDevClrInfoCfm *pCfm = (WlDevClrInfoCfm *)pCfmt;
    if (wlMan->Work.STA == STA_SHUTDOWN)
        return 1;

    pCfm->header.length = 1;
    WInitCounter();
    return 0;
}

u16 DEV_GetVerInfoReqCmd(WlCmdReq *pReqt, WlCmdCfm *pCfmt)
{                                                           // DevCmd.c:239
    static u8 wlVersion[] = "2.88.00";                      // :242
    WlDevGetVerInfoCfm *pCfm = (WlDevGetVerInfoCfm *)pCfmt; // r0 - :245

    pCfm->header.length = 9;

    MIi_CpuCopy16(wlVersion, pCfm->wlVersion, sizeof(wlVersion));
    pCfm->macVersion = W_ID;

    if ((wlMan->WlOperation & 0x8000) != 0)
    {
        pCfm->bbpVersion[0] = BBP_Read(0);  // :259
        pCfm->bbpVersion[1] = CalcBbpCRC(); // :260
    }
    else
    {
        pCfm->bbpVersion[0] = 0x6D;   // :264
        pCfm->bbpVersion[1] = 0x933D; // :265
    }

    if ((wlMan->WlOperation & 0x4000) != 0)
    {
        pCfm->rfVersion = wlMan->Rf.Id; // :269
    }
    else
    {
        pCfm->rfVersion = 2; // :273
    }

    return 0;
}

u16 DEV_GetWlInfoReqCmd(WlCmdReq *pReqt, WlCmdCfm *pCfmt)
{                                                     // DevCmd.c:293
    WlDevGetInfoCfm *pCfm = (WlDevGetInfoCfm *)pCfmt; // r0 - :297

    if (wlMan->Work.STA == STA_SHUTDOWN)
        return 1;

    pCfm->header.length = 92; // sizeof...?

    WUpdateCounter();
    MIi_CpuCopy32(&wlMan->Counter, &pCfm->counter, sizeof(pCfm->counter));
    return 0;
}

u16 DEV_GetStateReqCmd(WlCmdReq *pReqt, WlCmdCfm *pCfmt)
{                                                       // DevCmd.c:330
    WlDevGetStateCfm *pCfm = (WlDevGetStateCfm *)pCfmt; // r0 - :334

    pCfm->header.length = 2;
    pCfm->state = wlMan->Work.STA;
    return 0;
}

u16 DEV_TestSignalReqCmd(WlCmdReq *pReqt, WlCmdCfm *pCfmt)
{                                                           // DevCmd.c:364
    WlDevTestSignalReq *pReq = (WlDevTestSignalReq *)pReqt; // r0 - :366
    WlDevTestSignalCfm *pCfm = (WlDevTestSignalCfm *)pCfmt;
    WORK_PARAM *pWork; // r9 - :368
    u32 tmp;           // None - :369

    // this one is way too long
}

void IntrCarrierSuppresionSignal()
{ // DevCmd.c:644
    if (wlMan->Work.SigTest2)
    {
        // honestly, those two registers are kind of weird
        W_MACMEM(0) = 0;
        W_MACMEM(4) = 0;

        W_TXBUF_LOC1 |= 0x8000;
    }
    else
    {
        W_TXREQ_RESET = 1;
        W_MODE_RST = 0;
        W_IE = 2;
        W_IF = 0xFFFF;
        W_TX_HDR_CNT = 0;
        W_POWERFORCE = 0;
    }
}

u16 DEV_TestRxReqCmd(WlCmdReq *pReqt, WlCmdCfm *pCfmt)
{                                                   // DevCmd.c:690
    WlDevTestRxReq *pReq = (WlDevTestRxReq *)pReqt; // r0 - :692
    WlDevTestRxCfm *pCfm = (WlDevTestRxCfm *)pCfmt;
    WORK_PARAM *pWork = &wlMan->Work; // r4 - :694

    pCfm->header.length = 1;
    if ((pWork->STA & 0xF0) != STA_IDLE)
        return 1;

    switch (pReq->control)
    {
    case 1:
        if (pWork->STA != STA_IDLE)
        {
            return 1;
        }
        else
        {
            WSetChannel(pReq->channel, 1);
            pWork->Mode = MODE_TEST;
            WStart();
            WSetForcePowerState(0x8000);
        }

        pWork->STA = STA_IDLE_TEST1;
        break;

    case 0:
        if (pWork->STA == STA_IDLE_TEST1)
        {
            WSetForcePowerState(0);
            WStop();
        }
        else
        {
            return 1;
        }

        pWork->STA = STA_IDLE;
        break;
    }

    return 0;
}
