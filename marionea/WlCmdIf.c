#include "Mongoose.h"

static WLLIB_CMD_TBL WlibCmdTbl_MLME[11] = { // :39
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

static WLLIB_CMD_TBL WlibCmdTbl_MA[5] = { // :70
    {0x18, 0x02, MA_DataReqCmd},
    {0x04, 0x01, MA_KeyDataReqCmd},
    {0x0A, 0x01, MA_MpReqCmd},
    {0x0C, 0x01, MA_TestDataReqCmd},
    {0x01, 0x01, MA_ClrDataReqCmd}
};

static WLLIB_CMD_TBL WlibCmdTbl_PARAMSET[24] = { // :81
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

static WLLIB_CMD_TBL WlibCmdTbl_PARAMSET2[6] = { // :138
    {0x03, 0x01, PARAMSET_BSSIDReqCmd},
    {0x11, 0x01, PARAMSET_SSIDReqCmd},
    {0x01, 0x01, PARAMSET_BeaconPeriodReqCmd},
    {0x01, 0x01, PARAMSET_DTIMPeriodReqCmd},
    {0x01, 0x01, PARAMSET_ListenIntervalReqCmd},
    {0x00, 0x01, PARAMSET_GameInfoReqCmd}
};

static WLLIB_CMD_TBL WlibCmdTbl_PARAMGET[24] = { // :159 
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

static WLLIB_CMD_TBL WlibCmdTbl_PARAMGET2[6] = { // :216
    {0x00, 0x04, PARAMGET_BSSIDReqCmd},
    {0x00, 0x12, PARAMGET_SSIDReqCmd},
    {0x00, 0x02, PARAMGET_BeaconPeriodReqCmd},
    {0x00, 0x02, PARAMGET_DTIMPeriodReqCmd},
    {0x00, 0x02, PARAMGET_ListenIntervalReqCmd},
    {0x00, 0x01, PARAMGET_GameInfoReqCmd}
};

static WLLIB_CMD_TBL WlibCmdTbl_DEV[11] = { // :237
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
    WLLIB_CMD_TBL* pCmdTbl; // r3 - :273
    u16 vCode, vCodeMax, err = 0; // r1, r9, r12 - :274
    u16 currBusy = 0; // r4 - :275
    
    if (pCmdIf->Busy) // :278
        return;
    
    pReq = (WlCmdReq*)wlMan->HeapMan.RequestCmd.Head;
    pCmdIf->pCmd = (u8*)pReq;
        
    if (pReq == (WlCmdReq*)-1)
        return;
    
    
}

u16 CMD_ReservedReqCmd() { // WlCmdIf.c:548
    return 3;
}

void SendMessageToWmDirect(HEAPBUF_MAN* pBufMan, void* pMsg) { // WlCmdIf.c:568
    if (wlMan->HeapMan.ToWM.Count) {
        MoveHeapBuf(pBufMan, &wlMan->HeapMan.ToWM, pMsg);
        AddTask(2, 19);
        
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
