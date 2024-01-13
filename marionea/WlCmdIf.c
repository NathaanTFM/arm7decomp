#define MARIONEA_INTERNAL 1
#include "WlCmdIf.h"

static const WLLIB_CMD_TBL WlibCmdTbl_MLME[11] = { // :39
    {0x01, 0x01, MLME_ResetReqCmd},
    {0x03, 0x01, MLME_PwrMgtReqCmd},
    {0x1F, 0x23, MLME_ScanReqCmd},
    {0x22, 0x05, MLME_JoinReqCmd},
    {0x05, 0x06, MLME_AuthReqCmd},
    {0x04, 0x04, MLME_DeAuthReqCmd},
    {0x05, 0x03, MLME_AssReqCmd},
    {0x05, 0x03, MLME_ReAssReqCmd},
    {0x04, 0x01, MLME_DisAssReqCmd},
    {0x17, 0x01, MLME_StartReqCmd},
    {0x0C, 0x12, MLME_MeasChanReqCmd}
};

static const WLLIB_CMD_TBL WlibCmdTbl_MA[5] = { // :70
    {0x18, 0x02, MA_DataReqCmd},
    {0x04, 0x01, MA_KeyDataReqCmd},
    {0x0A, 0x01, MA_MpReqCmd},
    {0x0C, 0x01, MA_TestDataReqCmd},
    {0x01, 0x01, MA_ClrDataReqCmd}
};

static const WLLIB_CMD_TBL WlibCmdTbl_PARAMSET[24] = { // :81
    {0x48, 0x01, PARAMSET_AllReqCmd},
    {0x03, 0x01, PARAMSET_MacAdrsReqCmd},
    {0x01, 0x01, PARAMSET_RetryReqCmd},
    {0x01, 0x01, PARAMSET_EnableChannelReqCmd},
    {0x01, 0x01, PARAMSET_ModeReqCmd},
    {0x01, 0x01, PARAMSET_RateReqCmd},
    {0x01, 0x01, PARAMSET_WepModeReqCmd},
    {0x01, 0x01, PARAMSET_WepKeyIdReqCmd},
    {0x28, 0x01, PARAMSET_WepKeyReqCmd},
    {0x01, 0x01, PARAMSET_BeaconTypeReqCmd},
    {0x01, 0x01, PARAMSET_ResBcSsidReqCmd},
    {0x01, 0x01, PARAMSET_BeaconLostThReqCmd},
    {0x01, 0x01, PARAMSET_ActiveZoneReqCmd},
    {0x10, 0x01, PARAMSET_SSIDMaskReqCmd},
    {0x01, 0x01, PARAMSET_PreambleTypeReqCmd},
    {0x01, 0x01, PARAMSET_AuthAlgoReqCmd},
    {0x03, 0x01, PARAMSET_CCAModeEDThReqCmd},
    {0x03, 0x01, PARAMSET_LifeTimeReqCmd},
    {0x01, 0x01, PARAMSET_MaxConnReqCmd},
    {0x01, 0x01, PARAMSET_MainAntennaReqCmd},
    {0x02, 0x01, PARAMSET_DiversityReqCmd},
    {0x01, 0x01, PARAMSET_BcnSendRecvIndReqCmd},
    {0x01, 0x01, PARAMSET_NullKeyModeReqCmd},
    {0x01, 0x01, PARAMSET_McPassModeReqCmd}
};

static const WLLIB_CMD_TBL WlibCmdTbl_PARAMSET2[6] = { // :138
    {0x03, 0x01, PARAMSET_BSSIDReqCmd},
    {0x11, 0x01, PARAMSET_SSIDReqCmd},
    {0x01, 0x01, PARAMSET_BeaconPeriodReqCmd},
    {0x01, 0x01, PARAMSET_DTIMPeriodReqCmd},
    {0x01, 0x01, PARAMSET_ListenIntervalReqCmd},
    {0x00, 0x01, PARAMSET_GameInfoReqCmd}
};

static const WLLIB_CMD_TBL WlibCmdTbl_PARAMGET[24] = { // :159 
    {0x00, 0x21, PARAMGET_AllReqCmd},
    {0x00, 0x04, PARAMGET_MacAdrsReqCmd},
    {0x00, 0x02, PARAMGET_RetryReqCmd},
    {0x00, 0x03, PARAMGET_EnableChannelReqCmd},
    {0x00, 0x02, PARAMGET_ModeReqCmd},
    {0x00, 0x02, PARAMGET_RateReqCmd},
    {0x00, 0x02, PARAMGET_WepModeReqCmd},
    {0x00, 0x02, PARAMGET_WepKeyIdReqCmd},
    {0x00, 0x01, CMD_ReservedReqCmd},
    {0x00, 0x02, PARAMGET_BeaconTypeReqCmd},
    {0x00, 0x02, PARAMGET_ResBcSsidReqCmd},
    {0x00, 0x02, PARAMGET_BeaconLostThReqCmd},
    {0x00, 0x02, PARAMGET_ActiveZoneReqCmd},
    {0x00, 0x11, PARAMGET_SSIDMaskReqCmd},
    {0x00, 0x02, PARAMGET_PreambleTypeReqCmd},
    {0x00, 0x02, PARAMGET_AuthAlgoReqCmd},
    {0x00, 0x04, PARAMGET_CCAModeEDThReqCmd},
    {0x00, 0x01, CMD_ReservedReqCmd},
    {0x00, 0x02, PARAMGET_MaxConnReqCmd},
    {0x00, 0x02, PARAMGET_MainAntennaReqCmd},
    {0x00, 0x03, PARAMGET_DiversityReqCmd},
    {0x00, 0x02, PARAMGET_BcnSendRecvIndReqCmd},
    {0x00, 0x02, PARAMGET_NullKeyModeReqCmd},
    {0x00, 0x02, PARAMGET_McPassModeReqCmd}
};

static const WLLIB_CMD_TBL WlibCmdTbl_PARAMGET2[6] = { // :216
    {0x00, 0x04, PARAMGET_BSSIDReqCmd},
    {0x00, 0x12, PARAMGET_SSIDReqCmd},
    {0x00, 0x02, PARAMGET_BeaconPeriodReqCmd},
    {0x00, 0x02, PARAMGET_DTIMPeriodReqCmd},
    {0x00, 0x02, PARAMGET_ListenIntervalReqCmd},
    {0x00, 0x01, PARAMGET_GameInfoReqCmd}
};

static const WLLIB_CMD_TBL WlibCmdTbl_DEV[11] = { // :237
    {0x00, 0x01, CMD_ReservedReqCmd},
    {0x00, 0x01, DEV_ShutdownReqCmd},
    {0x00, 0x01, DEV_IdleReqCmd},
    {0x00, 0x01, DEV_Class1ReqCmd},
    {0x00, 0x01, DEV_RebootReqCmd},
    {0x00, 0x01, DEV_ClearWlInfoReqCmd},
    {0x00, 0x09, DEV_GetVerInfoReqCmd},
    {0x00, 0x5C, DEV_GetWlInfoReqCmd},
    {0x00, 0x02, DEV_GetStateReqCmd},
    {0x04, 0x01, DEV_TestSignalReqCmd},
    {0x02, 0x01, DEV_TestRxReqCmd}
};

void RequestCmdTask() { // WlCmdIf.c:267
    CMDIF_MAN* pCmdIf = &wlMan->CmdIf; // r7 - :269
    WlCmdReq* pReq; // r0 - :271
    WlCmdCfm* pCfm; // r0 - :272
    const WLLIB_CMD_TBL* pCmdTbl; // r3 - :273
    u16 vCode, vCodeMax, err = 0; // r1, r9, r12 - :274
    u16 currBusy = 0; // r4 - :275
    
    if (pCmdIf->Busy) // :278
        return;
    
    pReq = (WlCmdReq*)wlMan->HeapMan.RequestCmd.Head;
    pCmdIf->pCmd = (u8*)pReq;
        
    if (pReq == (WlCmdReq*)-1)
        return;

    pCfm = (WlCmdCfm*)GET_CFM(pReq);
    
    if (wlMan->Config.DiagResult) {
        pCfm->header.length = 1;
        pCfm->resultCode = 6;
        
    } else if (pReq->header.code != pCfm->header.code) {
        pCfm->resultCode = 13;
        
    } else {
        switch (pReq->header.code & 0xFF00) {
            case 0:
                currBusy = 0x1;
                pCmdTbl = WlibCmdTbl_MLME;
                vCode = pReq->header.code & 0xFF;
                vCodeMax = 11;

                if ((pCmdIf->Busy & 1) != 0) {
                    err = 2;
                } else if (wlMan->Work.STA < 0x20) {
                    err = 1;
                }
                break;

            case 0x100:
                currBusy = 0x2;
                pCmdTbl = WlibCmdTbl_MA;
                vCode = pReq->header.code & 0xFF;
                vCodeMax = 5;
                
                if (wlMan->Work.STA != 0x40) {
                    err = 1;
                }
                break;
            
            case 0x200:
                vCode = pReq->header.code & 0xFF;
                
                if (vCode < 0x40) {
                    if (wlMan->Work.STA < 0x10) {
                        err = 1;
                    }
                    currBusy = 0x4;
                    pCmdTbl = WlibCmdTbl_PARAMSET;
                    vCodeMax = 24;
                    
                } else if (vCode < 0x80) {
                    if (wlMan->Work.STA != 0x40) {
                        err = 1;
                    }
                    currBusy = 0x8;
                    pCmdTbl = WlibCmdTbl_PARAMSET2;
                    vCode -= 0x40;
                    vCodeMax = 6;
                    
                } else if (vCode < 0xC0) {
                    if (wlMan->Work.STA < 0x10) {
                        err = 1;
                    }
                    currBusy = 0x10;
                    pCmdTbl = WlibCmdTbl_PARAMGET;
                    vCode -= 0x80;
                    vCodeMax = 24;
                    
                } else {
                    if (wlMan->Work.STA < 0x10) {
                        err = 1;
                    }
                    currBusy = 0x20;
                    pCmdTbl = WlibCmdTbl_PARAMGET2;
                    vCode -= 0xC0;
                    vCodeMax = 6;
                }
                break;

            case 0x300:
                currBusy = 0x40;
                pCmdTbl = WlibCmdTbl_DEV;
                vCode = pReq->header.code & 0xFF;
                vCodeMax = 11;
                break;

            default:
                vCode = 1;
                vCodeMax = 0;
                break;
        }

        if (vCode > vCodeMax) {
            err = 3;
            
        } else if ((pReq->header.length < pCmdTbl[vCode].RequestMinLength) || (pCfm->header.length < pCmdTbl[vCode].ConfirmMinLength)) {
            err = 4;
        }

        if (err != 0) {
            pCfm->header.length = 1;
            pCfm->resultCode = err;
            
        } else {
            pCmdIf->Busy |= currBusy;
            pCfm->resultCode = pCmdTbl[vCode].pCmdFunc(pReq, pCfm);

            switch (pCfm->resultCode) {
                case 128:
                    return;
                
                case 129:
                    pCmdIf->Busy &= ~currBusy;
                    goto skip;
            }
        }
    }
    
    pCmdIf->Busy &= ~currBusy;
    SendMessageToWmDirect(&wlMan->HeapMan.RequestCmd, pCmdIf->pCmd);

skip:
    if (wlMan->HeapMan.RequestCmd.Count)
        AddTask(PRIORITY_LOW, TASK_REQUEST_CMD);

}

u16 CMD_ReservedReqCmd() { // WlCmdIf.c:548
    return 3;
}

void SendMessageToWmDirect(HEAPBUF_MAN* pBufMan, void* pMsg) { // WlCmdIf.c:568
    if (wlMan->HeapMan.ToWM.Count) {
        MoveHeapBuf(pBufMan, &wlMan->HeapMan.ToWM, pMsg);
        AddTask(PRIORITY_LOW, TASK_SEND_MESSAGE_TO_WM);
        
    } else if (OS_SendMessage(wlMan->pSendMsgQueue, pMsg, 0)) {
        DeleteHeapBuf(pBufMan, pMsg);
        
    } else {
        MoveHeapBuf(pBufMan, &wlMan->HeapMan.ToWM, pMsg);
    }
}

void SendMessageToWmTask() { // WlCmdIf.c:620
    u16* pMsg = (u16*)wlMan->HeapMan.ToWM.Head; // r7 - :622
    
    while (pMsg != (u16*)-1) {
        if (!OS_SendMessage(wlMan->pSendMsgQueue, pMsg, 0))
            break;
        
        DeleteHeapBuf(&wlMan->HeapMan.ToWM, pMsg);
        pMsg = (u16*)wlMan->HeapMan.ToWM.Head;
    }
}

void InitializeCmdIf() { // WlCmdIf.c:657
    wlMan->CmdIf.Busy = 0;
}
