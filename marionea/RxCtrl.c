#include "Mongoose.h"

static void RxDisAssFrame(DISASS_FRAME* pFrm);
static void RxAssReqFrame(ASSREQ_FRAME* pFrm);
static void RxAssResFrame(ASSRES_FRAME* pFrm);
static void RxReAssReqFrame(REASSREQ_FRAME* pFrm);
static void RxReAssResFrame(REASSRES_FRAME* pFrm);
static void RxProbeReqFrame(PRBREQ_FRAME* pFrm);
STATIC void RxProbeResFrame(PRBRES_FRAME* pFrm, ELEMENT_CHECKER* pChk);
STATIC void RxAuthFrame(AUTH_FRAME* pFrm);
static void RxDeAuthFrame(DEAUTH_FRAME* pFrm);
static void RxPsPollFrame(PSPOLL_FRAME* pFrm);
static void RxCfEndFrame();
STATIC void ElementChecker(ELEMENT_CHECKER* p);
static void SetChallengeText(u32 camAdrs, AUTH_FRAME* pFrm);
static u32 CheckChallengeText(AUTH_FRAME* pFrm);
static void NewDefragment(RXFRM_MAC* pMFrm, DEFRAG_TBL* pDefragTbl);
static void MoreDefragment(RXFRM_MAC* pMFrm, DEFRAG_TBL* pDefragTbl);



/*
Empty function IPA

void RxDataFrameTask() { // RxCtrl.c:66
    DEAUTH_FRAME* pDeAuth; // r0 - :68
    WlMaDataInd* pInd; // r5 - :69
    RXFRM* pFrm; // r0 - :70
    CONFIG_PARAM* pConfig; // r0 - :71
    WORK_PARAM* pWork; // r6 - :72
    MLME_MAN* pMLME; // r1 - :73
    HEAP_MAN* pHeapMan; // r7 - :74
    WlCounter* pCounter; // r0 - :75
    u32 multiCast; // r2 - :76
    u32 err; // r9 - :76
    u32 camAdrs; // r8 - :76
}
*/

u32 RxMpFrame(RXFRM* pFrm) { // RxCtrl.c:324
    WORK_PARAM* pWork = &wlMan->Work; // r5 - :326
    RX_CTRL* pRxCtrl = &wlMan->RxCtrl; // r6 - :327
    WlMaMpInd* pInd; // r5 - :328
    u32 cnt, x; // r7, r0 - :329
    u16 bitmap, out; // r2, r0 - :330
    
    if (pWork->STA != 0x40)
        return 1;
    
    if (!MatchMacAdrs(pFrm->Dot11Header.Adrs2, pWork->BSSID) || !MatchMacAdrs(pFrm->Dot11Header.Adrs3, pWork->LinkAdrs))
        return 1;
    
    if (( ((WlRxMpFrame*)pFrm)->bitmap & (u16)(1 << pWork->AID)) == 0) // weird cast
        pRxCtrl->TxKeyReg = 0;
    else
        pRxCtrl->TxKeyReg = 0x2000;
    
    out = W_TXBUF_REPLY2;
    if ((out & 0x8000) != 0 && (&W_MACMEM((u16)(2 * out)))[2] != 0) // TODO: ugliest thing in the code
        pRxCtrl->TxKeyReg |= 0x4000;
    
    CAM_UpdateLifeTime(pWork->APCamAdrs);
    pInd = SubtractAddr(pFrm, 0x10);
    pFrm->FirmHeader.Length = pFrm->MacHeader.Rx.MPDU - 28;
    
    pInd->header.code = 0x182; // ?
    pInd->header.length = ((u32)pFrm->FirmHeader.Length + 49) / 2;
    
    bitmap = pInd->frame.bitmap;
    cnt = 0;
    while (bitmap) {
        if (bitmap & 1)
            ++cnt;
        
        bitmap /= 2;
    }
    
    x = OS_DisableIrqMask(0x1000000);
    pInd->frame.txKeySts = pRxCtrl->TxKeyFrm | pRxCtrl->TxKeyReg | ((W_TXBUF_REPLY2 & 0x8000) >> 4) | ((W_TXBUF_REPLY1 & 0x8000) >> 3);
    pRxCtrl->TxKeyFrm = 0;
    OS_EnableIrqMask(x);
    
    pInd->frame.ackTimeStamp = pInd->frame.timeStamp + ((cnt * (pInd->frame.txop + 10) + 252) >> 4);
    SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
    return 0;
}

void RxKeyDataFrame(RXFRM* pFrm) { // RxCtrl.c:450
    TX_CTRL* pTxCtrl = &wlMan->TxCtrl; // r5 - :452
    WlMpKey* pMpKey = &pTxCtrl->pMpEndInd->mpKey; // r0 - :453
    WlMpKeyData* pKeyData; // r6 - :454
    u32 camAdrs, aid; // r6, r0 - :455
    u16 bitmap; // r1 - :456
    
    if (pTxCtrl->Mp.Busy
            && MatchMacAdrs(pFrm->Dot11Header.Adrs1, wlMan->Work.BSSID)
            && pFrm->MacHeader.Rx.MPDU - 24 <= pMpKey->length - 8) {
        
        camAdrs = CAM_Search(pFrm->Dot11Header.Adrs2);
        
        if (camAdrs == 0xFF || (camAdrs != 0 && CAM_GetStaState(camAdrs) != 0x40)) {
            DEAUTH_FRAME* pTxDeAuthFrm; // r0 - :519
            
            if (!IsExistManFrame(pFrm->Dot11Header.Adrs2, 0xC0)) {
                pTxDeAuthFrm = MakeDeAuthFrame(pFrm->Dot11Header.Adrs2, 7, 0); 
                
                if (pTxDeAuthFrm) {
                    pTxDeAuthFrm->FirmHeader.FrameId = 2;
                    TxManCtrlFrame((TXFRM*)pTxDeAuthFrm);
                }
            }
            
        } else if (camAdrs != 0) {
            CAM_SetPowerMgtMode(camAdrs, pFrm->Dot11Header.FrameCtrl.Bit.PowerMan);
            CAM_UpdateLifeTime(camAdrs);
            
            aid = CAM_GetAID(camAdrs);
            bitmap = (1 << aid);
            
            if ((bitmap & pTxCtrl->GetKeyMap) == 0 && (bitmap & pTxCtrl->SetKeyMap) != 0) {
                pTxCtrl->GetKeyMap |= bitmap;
                pMpKey->bitmap &= ~bitmap;
                
                pKeyData = pMpKey->data;
                bitmap >>= 1;
                
                while (bitmap != 1) {
                    if (bitmap & pTxCtrl->SetKeyMap)
                        pKeyData = (WlMpKeyData*)((char*)pKeyData + pMpKey->length);
                    
                    bitmap >>= 1;
                }
                
                pKeyData->length = pFrm->MacHeader.Rx.MPDU - 24; // sizeof?
                WL_WriteByte(&pKeyData->rssi, pFrm->MacHeader.Rx.rsv_RSSI);
                WL_WriteByte(&pKeyData->rate, pFrm->MacHeader.Rx.Service_Rate);
                
                if (pKeyData->length)
                    MIi_CpuCopy16(pFrm->Body, pKeyData->cdata, pKeyData->length + 1);
            }
        }
    }
    
    
}

u32 RxMpAckFrame(RXFRM* pFrm) { // RxCtrl.c:645
    WORK_PARAM* pWork = &wlMan->Work; // r6 - :647
    WlMaMpAckInd* pInd; // r0 - :648
    
    if (wlMan->Work.STA != 0x40)
        return 1;
    
    if (!MatchMacAdrs(pFrm->Dot11Header.Adrs2, pWork->BSSID) || !MatchMacAdrs(pFrm->Dot11Header.Adrs3, pWork->LinkAdrs))
        return 1;
    
    pInd = SubtractAddr(pFrm, 0x10);
    pFrm->FirmHeader.Length = pFrm->MacHeader.Rx.MPDU - 28;
    
    pInd->header.code = 0x185; // ?
    pInd->header.length = 24;
    pInd->ack.txKeySts |= wlMan->RxCtrl.TxKeyReg | ((W_TXBUF_REPLY2 & 0x8000) >> 4) | ((W_TXBUF_REPLY1 & 0x8000) >> 3);
    
    SendMessageToWmDirect(&wlMan->HeapMan.TmpBuf, pInd);
    return 0;
}

void RxBeaconFrame(BEACON_FRAME* pFrm) { // RxCtrl.c:740
    BEACON_BODY* pBeacon = &pFrm->Body; // r0 - :742 - unused
    WORK_PARAM* pWork = &wlMan->Work; // r7 - :743
    MLME_MAN* pMLME = &wlMan->MLME; // r4 - :744
    CONFIG_PARAM* pConfig = &wlMan->Config; // r8 - :745
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r9 - :746
    ELEMENT_CHECKER elementCheck; // None - :747
    u32 n, n1, n2, ofst; // r0, r4, r0, r4 - :748
    u32 BodyLen, cam_adrs; // r5, r11 - :749
    u64 lltsf; // None - :750
    u16 t1[4], t2[4]; // None, None - :751
    u16* ptsf; // r0 - :752
    
    wlMan->Counter.rx.beacon++; // :758
    cam_adrs = CAM_SearchAdd(pFrm->Dot11Header.SA); // :761
    pFrm->FirmHeader.CamAdrs = cam_adrs; // :762
    
    if (cam_adrs != 0xFF) { // :763
        CAM_SetRSSI(cam_adrs, pFrm->MacHeader.Tx.MPDU); // :766
        BodyLen = pFrm->FirmHeader.Length; // :769
        if (BodyLen > 12) { // :772
            MIi_CpuClear32(0, &elementCheck, sizeof(elementCheck)); // :785
            elementCheck.pElement = pFrm->Body.Buf; // :786
            elementCheck.bodyLength = BodyLen - 12; // :787
            elementCheck.matchFlag = 2; // :788
            if (pWork->SSIDLength == 0) { // :789
                elementCheck.matchFlag |= 1; // :791
            }
            elementCheck.foundFlag = 56; // :793
            elementCheck.rxStatus = pFrm->MacHeader.Rx.Status; // :794
            elementCheck.capability = pFrm->Body.CapaInfo.Data; // :795
            ElementChecker(&elementCheck); // :796
            
            if (elementCheck.pCFP && (pFrm->Dot11Header.Duration & 0x8000) != 0) { // :802
                W_CONTENTFREE = (
                    WL_ReadByte(&elementCheck.pCFP->CFPDurRemain.u8[0])
                    + (WL_ReadByte(&elementCheck.pCFP->CFPDurRemain.u8[1]) << 8)
                ); // :804
            }
            
            if (pMLME->State == 0x13 && pMLME->pReq.Scan->scanType == 1) { // :808
                if ((elementCheck.matchFlag & 9) == 9) { // :812
                    RxProbeResFrame((PRBRES_FRAME*)pFrm, &elementCheck); // :816
                }
                
                // :818 missing?
                // (goes to if pSSID UpdateApList)
                
            } else {
                if ((elementCheck.matchFlag & 8) != 0) { // :821
                    if (pMLME->State == 0x21) { // :830
                        ClearTimeOut(); // :833
                        if ((elementCheck.matchFlag & 0x30) != 0x30) { // :839
                            pMLME->Work.Scan.MaxConfirmLength = 12; // :841
                            pMLME->Work.Scan.ChannelCount = 10; // :842
                            
                        } else if (pFrm->Body.BeaconInterval > 1000) { // :855
                            pMLME->Work.Scan.MaxConfirmLength = 12; // :857
                            pMLME->Work.Scan.ChannelCount = 1; // :858
                            
                        } else {
                            pMLME->Work.Scan.MaxConfirmLength = 0; // :865
                            if ((elementCheck.foundFlag & 2) != 0 && (elementCheck.matchFlag & 2) == 0) { // :869
                                WSetChannel(elementCheck.channel, 0); // :874
                            }
                            CAM_SetSupRate(cam_adrs, elementCheck.rateSet.Support); // :878
                            
                            if (pWork->Mode == 2) { // :881
                                if (elementCheck.pGMIF) { // :883
                                    u32 actZone = (WL_ReadByte(&elementCheck.pGMIF->ActZone[0])) + (WL_ReadByte(&elementCheck.pGMIF->ActZone[1]) << 8); // r0 - :886
                                    WSetActiveZoneTime(actZone, 1); // :887
                                    
                                    global_vtsf_var = (WL_ReadByte(&elementCheck.pGMIF->VTSF[0])) + (WL_ReadByte(&elementCheck.pGMIF->VTSF[1]) << 8); // :890
                                    
                                    // :891 missing?
                                    
                                } else {
                                    WSetActiveZoneTime(0xFFFF, 1); // :894
                                    global_vtsf_var = 0; // :895
                                }
                            }
                            
                            WSetDTIMPeriod(WL_ReadByte(&elementCheck.pTIM->DTIMPeriod)); // :900
                            pWork->DTIMCount = WL_ReadByte(&elementCheck.pTIM->DTIMCount); // :901
                            WSetBeaconPeriod(pFrm->Body.BeaconInterval); // :904
                            pWork->bSynchro = 1; // :907
                            pWork->bFirstTbtt = 1; // :908
                            
                            if (pWork->Mode == 2) // :911
                                W_POWER_unk = 3; // :913
                            
                            W_POWER_TX = W_POWER_TX | 1; // :915
                        }
                        
                        WSetMacAdrs1(pMLME->pCfm.Auth->peerMacAdrs, pFrm->Dot11Header.SA); // :919
                        pMLME->State = 0x25; // :922
                        AddTask(2, 1); // :925
                    }
                    
                    switch (pWork->Mode) { // :929, incl cases.
                        case 2:
                            if (elementCheck.pGMIF) { // :936
                                WSetActiveZoneTime((WL_ReadByte(&elementCheck.pGMIF->ActZone[0])) + (WL_ReadByte(&elementCheck.pGMIF->ActZone[1]) << 8), 0); // :939
                                global_vtsf_var = (WL_ReadByte(&elementCheck.pGMIF->VTSF[0])) + (WL_ReadByte(&elementCheck.pGMIF->VTSF[1]) << 8); // :942
                                pWork->GameInfoLength = WL_ReadByte(&elementCheck.pGMIF->Length) - 8; // :945
                                
                                if (pWork->GameInfoLength > 0x80) // :949
                                    pWork->GameInfoLength = 0; // :952
                                
                                if (pWork->GameInfoLength != 0) { // :955
                                    if ((int)elementCheck.pGMIF & 1) { // :957
                                        // unaligned
                                        MIi_CpuCopy16(&elementCheck.pGMIF->GameInfo[-1], pWork->GameInfoAdrs, pWork->GameInfoLength + 2); // :959
                                        pWork->GameInfoAlign = 1; // :960
                                        
                                        // :961 missing
                                        
                                    } else {
                                        // aligned
                                        MIi_CpuCopy16(&elementCheck.pGMIF->GameInfo[0], pWork->GameInfoAdrs, pWork->GameInfoLength + 1); // :964
                                        pWork->GameInfoAlign = 0; // :965
                                    }
                                }
                            }
                        
                        case 3:
                            pWork->BeaconLostCnt = 0; // :974
                            CAM_UpdateLifeTime(cam_adrs); // :977

                            lltsf = *(u64*)pFrm->Body.TimeStamp;
                            n = (pWork->BeaconPeriod << 10);
                            
                            // this avoids a reg swap
                            // lltsf = ((lltsf / n) + 1) * n
                            lltsf /= n;
                            lltsf++;
                            lltsf *= n;
                            
                            ptsf = (u16*)&lltsf;
                            W_US_COMPARE3 = ptsf[3]; // :987
                            W_US_COMPARE2 = ptsf[2]; // :988
                            W_US_COMPARE1 = ptsf[1]; // :989
                            W_US_COMPARE0 = ptsf[0] | 1; // :990
                            
                            if (pWork->Mode == 2 && pWork->bFirstTbtt) { // :993
                                lltsf -= n;
                                
                                // check dest variable?
                                n = OS_DisableInterrupts(); // :999
                                
                                t1[0] = W_US_COUNT0; // :1000
                                t1[1] = W_US_COUNT1; // :1001
                                t1[2] = W_US_COUNT2; // :1002
                                t1[3] = W_US_COUNT3; // :1003
                                
                                t2[0] = W_US_COUNT0; // :1004
                                t2[1] = W_US_COUNT1; // :1005
                                t2[2] = W_US_COUNT2; // :1006
                                t2[3] = W_US_COUNT3; // :1007
                                
                                OS_RestoreInterrupts(n); // :1008

                                if (t1[0] < t2[0])
                                    n = ((*(u64*)t1) - lltsf) >> 10;
                                else
                                    n = ((*(u64*)t2) - lltsf) >> 10; // :1016
                                    
                                if (n < pConfig->ActiveZone) { // :1018
                                    W_POST_BEACON = pConfig->ActiveZone - n; // :1020
                                } else {
                                    W_POST_BEACON = 0; // :1024
                                }
                            }
                            
                            if (pWork->STA == 0x40 && elementCheck.pTIM && pWork->PowerMgtMode == 1) { // :1035
                                u16 dtimCnt = WL_ReadByte(&elementCheck.pTIM->DTIMCount); // r0 - :1039
                                if (pWork->DTIMCount != dtimCnt) { // :1041
                                    pWork->DTIMCount = dtimCnt; // :1045
                                }
                                pWork->bExistTIM = 0; // :1049
                                if (dtimCnt == 0 && (WL_ReadByte(&elementCheck.pTIM->BitmapCtrl) & 1) != 0) // :1054
                                    pWork->bExistTIM |= 1; // :1057
                                
                                ofst = WL_ReadByte(&elementCheck.pTIM->BitmapCtrl) & 0xFE; // :1063
                                n1 = 8 * ofst; // :1064
                                n2 = 8 * (ofst + WL_ReadByte(&elementCheck.pTIM->Length) - 3); // :1065
                                if (n1 <= pWork->AID && pWork->AID <= n2) { // :1066
                                    n = pWork->AID - n1; // :1069
                                    if ((WL_ReadByte(&elementCheck.pTIM->VitrualBitmap[n >> 3]) & (1 << (n & 7))) != 0) { // :1073
                                        pWork->bExistTIM |= 2; // :1076
                                        TxPsPollFrame(); // :1079
                                    }
                                }
                                
                                if (pHeapMan->TxPri[0].Count == 0 && pHeapMan->TxPri[1].Count == 0 && pWork->bExistTIM == 0) // :1089
                                    WSetPowerState(1); // :1098
                            }
                    }
                    if (pConfig->BcnTxRxIndMsg) { // :1105
                        MLME_IssueBeaconRecvIndication((RXFRM*)pFrm); // :1107
                    }
                }
            }
            if (elementCheck.pSSID) { // :1115
                UpdateApList(elementCheck.channel, pFrm, elementCheck.pSSID); // :1117
            }
        }
    }
}

static void RxDisAssFrame(DISASS_FRAME* pFrm) { // RxCtrl.c:1146
    WORK_PARAM* pWork = &wlMan->Work; // r1 - :1148
    DEAUTH_FRAME* pDeAuth; // r0 - :1149
    u32 st, cam_adrs = pFrm->FirmHeader.CamAdrs; // r0, r5 - :1150
    
    switch (pWork->Mode) {
        case 1:
            st = CAM_GetStaState(cam_adrs);
            
            if (st == 0x40) {
                CAM_SetStaState(cam_adrs, 0x30);
                MLME_IssueDisAssIndication(pFrm->Dot11Header.SA, pFrm->Body.ReasonCode);
                DeleteTxFrames(cam_adrs);
                
            } else {
                if (st == 0x30) {
                    // ugly cast is ugly. should just have been a TXFRM*
                    pDeAuth = (DEAUTH_FRAME*)MakeDisAssFrame(pFrm->Dot11Header.SA, 7);
                    
                } else {
                    pDeAuth = MakeDeAuthFrame(pFrm->Dot11Header.SA, 7, 1);
                }
                
                if (pDeAuth)
                    TxManCtrlFrame((TXFRM*)pDeAuth);
            }
            
            break;
            
        case 2:
        case 3:
            if (pWork->STA == 0x40 && MatchMacAdrs(pFrm->Dot11Header.SA, pWork->LinkAdrs)) {
                WSetStaState(0x30);
                WClearAids();
                MLME_IssueDisAssIndication(pFrm->Dot11Header.SA, pFrm->Body.ReasonCode);
            }
            break;
    }
}

static void RxAssReqFrame(ASSREQ_FRAME* pFrm) { // RxCtrl.c:1228
    CONFIG_PARAM* pConfig = &wlMan->Config; // r4 - :1230
    ASSREQ_BODY* pAssReq = &pFrm->Body; // r0 - :1231
    DEAUTH_FRAME* pTxDeAuthFrm; // r0 - :1232
    ASSRES_FRAME* pTxFrm; // r0 - :1233
    u32 bodyLen; // r5 - :1234
    u16 stsCode, cam_adrs; // r0, r0 - :1235
    ELEMENT_CHECKER elementCheck; // None - :1236
    
    bodyLen = pFrm->FirmHeader.Length; // :1239
    
    if (bodyLen <= 4) // :1244
        return;
    
    if (wlMan->Work.Mode != 1) // :1258
        return;
    
    if (IsExistManFrame(pFrm->Dot11Header.SA, 0x10)) // :1265
        return;
    
    cam_adrs = pFrm->FirmHeader.CamAdrs; // :1268
    
    if (cam_adrs == 0 || CAM_GetStaState(cam_adrs) < 0x30) { // :1272
        if (IsExistManFrame(pFrm->Dot11Header.SA, 0xC0)) // :1276
            return;
            
        pTxDeAuthFrm = MakeDeAuthFrame(pFrm->Dot11Header.SA, 6, 1); // :1279
        if (pTxDeAuthFrm) // :1280
            TxManCtrlFrame((TXFRM*)pTxDeAuthFrm); // :1282
        
        return;
    }
    
    if (CAM_GetStaState(cam_adrs) == 0x40) {
        CAM_SetStaState(cam_adrs, 0x30);
        MLME_IssueDisAssIndication(pFrm->Dot11Header.SA, 1);    
    }
    
    else if (CAM_GetAID(cam_adrs) != 0) {
        return;
    }
    
    MIi_CpuClear32(0, &elementCheck, sizeof(ELEMENT_CHECKER));
    elementCheck.pElement = pAssReq->Buf;
    elementCheck.bodyLength = bodyLen - 4;
    ElementChecker(&elementCheck);
    
    // holy
    if (
        (pAssReq->CapaInfo.Data & 0xFFC2) != 0
        || (pAssReq->CapaInfo.Bit.ESS == 0)
        || (!pConfig->WepMode && pAssReq->CapaInfo.Bit.Privacy == 1)
        || (pConfig->WepMode && pAssReq->CapaInfo.Bit.Privacy == 0)
        || (pConfig->PreambleType == 1 && pAssReq->CapaInfo.Bit.ShortPreamble == 0)
    ) {
        stsCode = 10;
        
    } else {
        CAM_SetCapaInfo(cam_adrs, pAssReq->CapaInfo.Data); // :1325
        
        if ((elementCheck.matchFlag & 1) == 0) { // :1328
            stsCode = 1; // :1331
            
        } else if ((elementCheck.matchFlag & 4) == 0) { // :1336
            stsCode = 18; // :1341
            
        } else {
            CAM_SetSupRate(cam_adrs, elementCheck.rateSet.Support); // :1346
            stsCode = 0; // :1349
        }
    }
    
    pTxFrm = MakeAssResFrame(cam_adrs, stsCode, elementCheck.pSSID); // :1354
    if (pTxFrm)
        TxManCtrlFrame((TXFRM*)pTxFrm);
}

// THIS ONE MUST BE INLINED
static void RxAssResFrame(ASSRES_FRAME* pFrm) {
    WORK_PARAM* pWork = &wlMan->Work;
    MLME_MAN* pMLME = &wlMan->MLME;
    ASSRES_BODY* pAssRes = &pFrm->Body;
    
    if (pWork->Mode != 2 && pWork->Mode != 3)
        return;
    
    if (pMLME->State != 81)
        return;
    
    if (!MatchMacAdrs(pMLME->pReq.Ass->peerMacAdrs, pFrm->Dot11Header.SA))
        return;
    
    ClearTimeOut();
    if (pAssRes->StatusCode == 0) {
        WSetAids(pAssRes->AID & 0xFFF);
        MakePsPollFrame(pWork->AID);
        WSetMacAdrs1(pWork->LinkAdrs, pFrm->Dot11Header.SA);
        pWork->APCamAdrs = CAM_Search(pFrm->Dot11Header.SA);
        CAM_SetStaState(pWork->APCamAdrs, 0x40);
    }
    
    if (pAssRes->StatusCode == 0) {
        pMLME->pCfm.Ass->resultCode = 0;
        pMLME->pCfm.Ass->statusCode = 0;
        WSetStaState(0x40);
        
    } else {
        pMLME->pCfm.Ass->resultCode = 12;
        pMLME->pCfm.Ass->statusCode = pAssRes->StatusCode;
    }
    
    pMLME->pCfm.Ass->aid = pWork->AID;
    pMLME->State = 83;
    AddTask(2, 3);
}

// THIS ONE MUST BE INLINED
static void RxReAssReqFrame(REASSREQ_FRAME* pFrm) {
    WL_MAN* pWlMan = wlMan;
    REASSREQ_BODY* pReAssReq;
    DEAUTH_FRAME* pTxDeAuthFrm;
    REASSRES_FRAME* pTxFrm; 
    u32 bodyLen;
    u16 stsCode;
    u16 cam_adrs;
    ELEMENT_CHECKER elementCheck;

    bodyLen = pFrm->FirmHeader.Length;
    pReAssReq = &pFrm->Body;
    
    if (bodyLen <= 10)
        return;
    
    if (pWlMan->Work.Mode != 1)
        return;
    
    if (IsExistManFrame(pFrm->Dot11Header.SA, 0x30))
        return;
    
    cam_adrs = pFrm->FirmHeader.CamAdrs;
    
    if (CAM_GetStaState(cam_adrs) < 0x30) {
        if (IsExistManFrame(pFrm->Dot11Header.SA, 0xC0)) // :1276
            return;
            
        pTxDeAuthFrm = MakeDeAuthFrame(pFrm->Dot11Header.SA, 6, 1); 
        if (pTxDeAuthFrm) 
            TxManCtrlFrame((TXFRM*)pTxDeAuthFrm); 
        
        return;
    }
    
    if (CAM_GetStaState(cam_adrs) == 0x40) {
        CAM_SetStaState(cam_adrs, 0x30);
        MLME_IssueDisAssIndication(pFrm->Dot11Header.SA, 1);
        
    } else if (CAM_GetAID(cam_adrs) != 0) {
        return;
    }
    
    MIi_CpuClear32(0, &elementCheck, sizeof(ELEMENT_CHECKER));
    elementCheck.pElement = pReAssReq->Buf;
    elementCheck.bodyLength = bodyLen - 10;
    elementCheck.foundFlag = 0x800;
    ElementChecker(&elementCheck);
    
    // holy
    if (
        (pReAssReq->CapaInfo.Data & 0xFFC2) != 0
        || (!pWlMan->Config.WepMode && pReAssReq->CapaInfo.Bit.Privacy == 1)
        || (pWlMan->Config.WepMode && pReAssReq->CapaInfo.Bit.Privacy == 0)
    ) {
        stsCode = 10;
        
    } else {
        CAM_SetCapaInfo(cam_adrs, pReAssReq->CapaInfo.Data); // :1325
        
        if ((elementCheck.matchFlag & 1) == 0) { // :1328
            stsCode = 1; // :1331
            
        } else if ((elementCheck.matchFlag & 4) == 0) { // :1336
            stsCode = 18; // :1341
            
        } else {
            CAM_SetSupRate(cam_adrs, elementCheck.rateSet.Support); // :1346
            stsCode = 0; // :1349
        }
    }
    
    pTxFrm = MakeReAssResFrame(cam_adrs, stsCode, elementCheck.pSSID); // :1354
    if (pTxFrm)
        TxManCtrlFrame((TXFRM*)pTxFrm);
}

// THIS ONE MUST BE INLINED
static void RxReAssResFrame(REASSRES_FRAME* pFrm) {
    WORK_PARAM* pWork = &wlMan->Work;
    MLME_MAN* pMLME = &wlMan->MLME;
    REASSRES_BODY* pReAssRes = &pFrm->Body;
    
    if (pWork->Mode != 2 && pWork->Mode != 3)
        return;
    
    if (pMLME->State != 97)
        return;
    
    if (!MatchMacAdrs(pMLME->pReq.ReAss->newApMacAdrs, pFrm->Dot11Header.SA))
        return;
    
    ClearTimeOut();
    if (pReAssRes->StatusCode == 0) {
        WSetAids(pReAssRes->AID & 0xFFF);
        MakePsPollFrame(pWork->AID);
        WSetMacAdrs1(pWork->LinkAdrs, pFrm->Dot11Header.SA);
        pWork->APCamAdrs = CAM_Search(pFrm->Dot11Header.SA);
        CAM_SetStaState(pWork->APCamAdrs, 0x40);
        WSetStaState(0x40);
    }
    
    if (pReAssRes->StatusCode == 0) {
        pMLME->pCfm.ReAss->resultCode = 0;
        pMLME->pCfm.ReAss->statusCode = 0;
        WSetStaState(0x40);
        
    } else {
        pMLME->pCfm.ReAss->resultCode = 12;
        pMLME->pCfm.ReAss->statusCode = pReAssRes->StatusCode;
    }
    
    pMLME->pCfm.ReAss->aid = pWork->AID;
    pMLME->State = 99;
    AddTask(2, 4);
}

static void RxProbeReqFrame(PRBREQ_FRAME* pFrm) { // RxCtrl.c:1703
    PRBRES_FRAME* pTxFrm; // r0 - :1705
    ELEMENT_CHECKER elementCheck; // None - :1706
    
    if (IsExistManFrame(pFrm->Dot11Header.SA, 0x50))
        return;
    
    if ((pFrm->Dot11Header.BSSID[0] & 1) || (pFrm->MacHeader.Rx.Status & 0x8000)) {
        MIi_CpuClear32(0, &elementCheck, sizeof(ELEMENT_CHECKER));
        elementCheck.pElement = pFrm->Body.Buf;
        elementCheck.bodyLength = pFrm->FirmHeader.Length;
        if (!wlMan->Config.BcSsidResponse)
            elementCheck.foundFlag = 0x800;
        
        ElementChecker(&elementCheck);
        
        if ((elementCheck.matchFlag & 1) == 1) {
            pTxFrm = MakeProbeResFrame(pFrm->Dot11Header.SA);
            if (pTxFrm)
                TxManCtrlFrame((TXFRM*)pTxFrm);
        }
    }
    
}

/*
(IPA)

STATIC void RxProbeResFrame(PRBRES_FRAME* pFrm, ELEMENT_CHECKER* pChk) { // RxCtrl.c:1758
    MLME_MAN* pMLME; // r6 - :1760
    WlMlmeScanCfm* pCfm; // r7 - :1761
    PRBRES_BODY* pPrbRes; // r0 - :1762
    WlBssDesc* pDesc; // r8 - :1763
    u32 len; // r0 - :1764
    u32 id; // r0 - :1764
    u32 j; // r5 - :1764
    u32 i; // r8 - :1764
    u32 bodyLen; // r9 - :1764
    ELEMENT_CHECKER elementCheck; // None - :1765
    u16* pBssidMask; // r9 - :1766
    u8* pDst; // r11 - :1767
    u8* pSrc; // r9 - :1767
}
*/

/*
(IPA)

STATIC void RxAuthFrame(AUTH_FRAME* pFrm) { // RxCtrl.c:1993
    WORK_PARAM* pWork; // r4 - :1995
    MLME_MAN* pMLME; // r5 - :1996
    AUTH_BODY* pAuth; // r0 - :1997
    AUTH_FRAME* pTxFrm; // r4 - :1998
    u32 cam_adrs; // r6 - :1999
    u32 bTxAuth; // r7 - :2000
    u16 stsCode; // r9 - :2001
    u16 seqNum; // r8 - :2001
    TXQ* pTxq; // r1 - :2013
    u16 map; // r0 - :2014
}
*/

static void RxDeAuthFrame(DEAUTH_FRAME* pFrm) { // RxCtrl.c:2418
    WORK_PARAM* pWork = &wlMan->Work; // r1 - :2420
    u32 cam_adrs = pFrm->FirmHeader.CamAdrs; // r5 - :2422
    
    switch (pWork->Mode) {
        case 1:
            if (CAM_GetStaState(cam_adrs) > 0x20) {
                CAM_SetStaState(cam_adrs, 0x20);
                MLME_IssueDeAuthIndication(pFrm->Dot11Header.SA, pFrm->Body.ReasonCode);
                DeleteTxFrames(cam_adrs);
            }
            break;
            
        case 2:
        case 3:
            if (pWork->STA > 0x20 && MatchMacAdrs(pFrm->Dot11Header.SA, pWork->LinkAdrs)) {
                WSetStaState(0x20);
                WClearAids();
                MLME_IssueDeAuthIndication(pFrm->Dot11Header.SA, pFrm->Body.ReasonCode);
            }
            break;
    }
}

// THIS ONE MUST BE INLINED
static void RxPsPollFrame(PSPOLL_FRAME* pFrm) {
    HEAP_MAN* pHeapMan = &wlMan->HeapMan;
    u32 cam_adrs = pFrm->FirmHeader.CamAdrs;
    
    if (CAM_GetStaState(cam_adrs) != 64)
        return;
    
    CAM_SetAwake(cam_adrs);
    if (pHeapMan->TxPri[1].Count)
        TxqPri(1);
    
    if (pHeapMan->TxPri[0].Count)
        TxqPri(0);
}

// THIS ONE MUST BE INLINED
static void RxCfEndFrame() { 
    // it's empty lol
}

STATIC void ElementChecker(ELEMENT_CHECKER* p) { // RxCtrl.c:2564
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :2566
    u8* pBuf; // r5 - :2567
    long i; // r6 - :2568
    u32 len; // r7 - :2569
    
    p->channel = pWork->CurrChannel;
    
    pBuf = p->pElement;
    
    if (p->foundFlag & 0x800)
        p->matchFlag |= 1;
    
    for (i = p->bodyLength; i > 0; i -= len + 2) {
        len = WL_ReadByte(pBuf + 1);
        
        WL_ReadByte(pBuf++);
    }
}

void RxManCtrlTask() { // RxCtrl.c:2717
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r10 - :2719
    WlCounter* pCounter = &wlMan->Counter; // r0 - :2720
    u32 mode; // r5 - :2721
    RXFRM* pFrm; // r0 - :2722
    RXPACKET* pPacket; // r6 - :2723
    MAN_HEADER* pDot11Header; // r0 - :2724
    u32 cam_adrs, type, subtype; // r7, r8, r9 - :2725
    
    pPacket = (RXPACKET*)pHeapMan->RxManCtrl.Head;
    mode = wlMan->Work.Mode;
    
    if (pPacket == (RXPACKET*)-1) return;
    if (pPacket->frame.Dot11Header.Adrs1[0] & 1)
        pCounter->rx.multicast++;
    else
        pCounter->rx.unicast++;
    
    pCounter->rx.fragment += (pPacket->frame.MacHeader.Rx.Status & 0xF0) / 16 - 1;
    
    type = pPacket->frame.Dot11Header.FrameCtrl.Bit.Type;
    subtype = pPacket->frame.Dot11Header.FrameCtrl.Bit.SubType;
    
    cam_adrs = CAM_SearchAdd(pPacket->frame.Dot11Header.Adrs2);
    pPacket->frame.FirmHeader.CamAdrs = cam_adrs;
    
    if (cam_adrs == 0xFF) {
        pPacket->frame.FirmHeader.CamAdrs = 0;
        
        if (mode == 1 && type == 0) {
            switch (subtype) {
                case 11:
                    RxAuthFrame((AUTH_FRAME*)&pPacket->frame); // :2772
                    break;
                    
                case 4:
                    RxProbeReqFrame((PRBREQ_FRAME*)&pPacket->frame); // :2773
                    break;
                    
                case 0:
                    RxAssReqFrame((ASSREQ_FRAME*)&pPacket->frame); // :2774
                    break;
            }
        }
        
        goto fast_exit;
    }
    
    CAM_UpdateLifeTime(cam_adrs); // :2783
    CAM_SetRSSI(cam_adrs, (u8)pPacket->frame.MacHeader.Rx.rsv_RSSI);
    
    if (type == 0) {
        u32 seqctrl = pPacket->frame.Dot11Header.SeqCtrl.Data; // None - :2789
        if (seqctrl == CAM_GetLastSeqCtrl(cam_adrs)) {
            pCounter->rx.duplicateErr++;
            goto fast_exit;
        }
        
        CAM_SetLastSeqCtrl(cam_adrs, seqctrl);
    }
    
    switch (mode) {
        case 1:
            CAM_SetPowerMgtMode(cam_adrs, pPacket->frame.Dot11Header.FrameCtrl.Bit.PowerMan); // :2810
            
            if (type == 0) {
                switch (subtype) {
                    case 8:
                        RxBeaconFrame((BEACON_FRAME*)&pPacket->frame);
                        break;
                        
                    case 0:
                        RxAssReqFrame((ASSREQ_FRAME*)&pPacket->frame);
                        break;
                        
                    case 2:
                        RxReAssReqFrame((REASSREQ_FRAME*)&pPacket->frame);
                        break;
                        
                    case 4:
                        RxProbeReqFrame((PRBREQ_FRAME*)&pPacket->frame);
                        break;
                        
                    case 5:
                        RxProbeResFrame((PRBRES_FRAME*)&pPacket->frame, 0);
                        break;
                        
                    case 10:
                        RxDisAssFrame((DISASS_FRAME*)&pPacket->frame);
                        break;
                        
                    case 11:
                        RxAuthFrame((AUTH_FRAME*)&pPacket->frame);
                        break;
                        
                    case 12:
                        RxDeAuthFrame((DEAUTH_FRAME*)&pPacket->frame);
                        break;
                        
                    default:
                        break;
                }
                
            } else if (type == 1) {
                if (subtype == 10)
                    RxPsPollFrame((PSPOLL_FRAME*)&pPacket->frame);
            }
            break;
        
        case 2:
        case 3:
            if (type == 0) {
                switch (subtype) {
                    case 8:
                        RxBeaconFrame((BEACON_FRAME*)&pPacket->frame);
                        break;
                        
                    case 1:
                        RxAssResFrame((ASSRES_FRAME*)&pPacket->frame);
                        break;
                        
                    case 3:
                        RxReAssResFrame((REASSRES_FRAME*)&pPacket->frame);
                        break;
                        
                    case 5:
                        RxProbeResFrame((PRBRES_FRAME*)&pPacket->frame, 0);
                        break;
                        
                    case 10:
                        RxDisAssFrame((DISASS_FRAME*)&pPacket->frame);
                        break;
                        
                    case 11:
                        RxAuthFrame((AUTH_FRAME*)&pPacket->frame);
                        break;
                        
                    case 12:
                        RxDeAuthFrame((DEAUTH_FRAME*)&pPacket->frame);
                        break;
                        
                    default:
                        break;
                }
                
            } else if (type == 1) {
                if (subtype == 14 || subtype == 15) {
                    RxCfEndFrame();
                }
            }
            
            break;
    }
    
    
fast_exit:
    ReleaseHeapBuf(&pHeapMan->RxManCtrl, pPacket); // :2884
    if (pHeapMan->RxManCtrl.Count != 0)
        AddTask(1, 7); // :2889
}

// THIS ONE MUST BE INLINED
static void SetChallengeText(u32 camAdrs, AUTH_FRAME* pFrm) {
    u32 i; // r6 - :2924
    u32 txtLen; // r4 - :2924
    u16* pText; // r5 - :2924
    u16 rnd; // r6 - :2924
}

// THIS ONE MUST BE INLINED
static u32 CheckChallengeText(AUTH_FRAME* pFrm) {
    u32 i; // r7 - :2963
    u32 txtLen; // r6 - :2963
    u16* pText; // r5 - :2963
}

/* Removed to prevent IPA (inter-procedure   analysis?)
void DefragTask() { // RxCtrl.c:3011
    HEAP_MAN* pHeapMan; // None - :3013
    RXPACKET* pPacket; // r8 - :3014
    RXFRM* pFrm; // r0 - :3015
    DEFRAG_TBL defragTbl; // None - :3016
    u32 fc; // r4 - :3017
}
*/

// THIS ONE MUST BE INLINED
static void NewDefragment(RXFRM_MAC* pMFrm, DEFRAG_TBL* pDefragTbl) {
    DEFRAG_LIST* pList;
    RXPACKET* pPacket;
    RXFRM* pFrm; 
    u32 i; 
    u32 pos; 
    u32 fragCnt; 
    u32 SrcOfst;
    u32 WSize; 
    u32 OvrCnt;
}

// THIS ONE MUST BE INLINED
static void MoreDefragment(RXFRM_MAC* pMFrm, DEFRAG_TBL* pDefragTbl) {
    HEAP_MAN* pHeapMan;
    DEFRAG_LIST* pList;
    RXFRM* pFrm;
    u32 i; 
    u32 fragCnt;
    u32 length;
    u32 OvrCnt; 
    u32 SrcOfst; 
    u32 WSize;
}

void DefragTimerTask() { // RxCtrl.c:3348
    DEFRAG_LIST* pList = wlMan->RxCtrl.DefragList; // r5 - :3350
    u32 i; // r6 - :3351
    
    for (i = 0; i < 3; i++) {
        if (pList[i].RestTime) {
            if (--pList[i].RestTime == 0) {
                ReleaseHeapBuf(&wlMan->HeapMan.TmpBuf, pList[i].pPacket);
            }
        }
    }
}

void InitRxCtrl() { // RxCtrl.c:3395
    WORK_PARAM* pWork = &wlMan->Work; // r6 - :3397
    RX_CTRL* pRxCtrl = &wlMan->RxCtrl; // r7 - :3398
    u32 str_madrs; // r8 - :3399
    
    MIi_CpuClear32(0, &wlMan->RxCtrl, sizeof(wlMan->RxCtrl));
    W_RXCNT = 0x8000;
    
    // TODO: are those sizeofs?
    switch (wlMan->Work.Mode) {
        case 0:
            str_madrs = 0x794;
            break;
            
        case 1:
            str_madrs = 0x10C4;
            break;
            
        case 2:
            str_madrs = 0xBFC;
            break;
            
        case 3:
            str_madrs = 0x794;
            break;
    }
    
    W_RXBUF_BEGIN = (u32)&W_MACMEM(str_madrs);
    W_RXBUF_WR_ADDR = str_madrs >> 1;
    W_RXBUF_END = 0x5F60;
    W_RXBUF_READCSR = str_madrs >> 1;
    
    pRxCtrl->wlCurr = str_madrs >> 1;
    pRxCtrl->LastMpSeq = -1;
    pWork->Ofst.RxBuf.Size = 0x1F60 - str_madrs;
    
    W_RXBUF_GAP = 0x5F5E;
    W_RXCNT = 0x8001;
    W_INTERNAL_11 = 0xFFFF;
    W_INTERNAL_12 = 0xFFFF;
    W_RX_MAC_0 = 0xFFFF;
    W_RX_MAC_1 = 0xFFFF;
    W_RX_UNK = 0xFFFF;
    W_RX_SEQ = 0xFFFF;
}
