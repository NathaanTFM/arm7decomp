#include "Mongoose.h"

static void RxDisAssFrame(DISASS_FRAME* pFrm);
static void RxAssReqFrame(ASSREQ_FRAME* pFrm);
static void RxAssResFrame(ASSRES_FRAME* pFrm);
static void RxReAssReqFrame(REASSREQ_FRAME* pFrm);
static void RxReAssResFrame(REASSRES_FRAME* pFrm);
static void RxProbeReqFrame(PRBREQ_FRAME* pFrm);
static void RxProbeResFrame(PRBRES_FRAME* pFrm, ELEMENT_CHECKER* pChk);
static void RxAuthFrame(AUTH_FRAME* pFrm);
static void RxDeAuthFrame(DEAUTH_FRAME* pFrm);
static void RxPsPollFrame(PSPOLL_FRAME* pFrm);
static void RxCfEndFrame();
STATIC void ElementChecker(ELEMENT_CHECKER* p);
static void SetChallengeText(u32 camAdrs, AUTH_FRAME* pFrm);
static u32 CheckChallengeText(AUTH_FRAME* pFrm);
static void NewDefragment(RXFRM_MAC* pMFrm, DEFRAG_TBL* pDefragTbl);
static void MoreDefragment(RXFRM_MAC* pMFrm, DEFRAG_TBL* pDefragTbl);

extern u16 NULL_ADRS[3];

void RxDataFrameTask() { // RxCtrl.c:66
    DEAUTH_FRAME* pDeAuth; // r0 - :68
    WlMaDataInd* pInd; // r5 - :69
    RXFRM* pFrm; // r0 - :70
    CONFIG_PARAM* pConfig = &wlMan->Config; // r0 - :71
    WORK_PARAM* pWork = &wlMan->Work; // r6 - :72
    MLME_MAN* pMLME = &wlMan->MLME; // r1 - :73
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r7 - :74
    WlCounter* pCounter = &wlMan->Counter; // r0 - :75
    u32 camAdrs, err, multiCast; // r8, r9, r2 - :76

    pInd = (WlMaDataInd*)pHeapMan->RxData.Head;
    if (pInd == (WlMaDataInd*)-1)
        return;

    pFrm = (RXFRM*)&pInd->frame;

    multiCast = 0;
    if (pFrm->Dot11Header.FrameCtrl.Bit.ToDS) {
        if ((pFrm->Dot11Header.Adrs3[0] & 1) != 0)
            multiCast = 1;
    } else {
        if ((pFrm->Dot11Header.Adrs1[0] & 1) != 0)
            multiCast = 1;
    }

    if (pWork->STA != 0x40 && (pConfig->MulticastPass == 0 || multiCast == 0 || pWork->STA != 0x20 || pWork->bSynchro != 1 || (pMLME->State & 0xF0) == 0x30)) {
        ReleaseHeapBuf(&pHeapMan->RxData, pInd);
        
    } else {
        err = 1;
        if (multiCast) {
            pCounter->rx.multicast++;
        } else {
            pCounter->rx.unicast++;
        }
        pCounter->rx.fragment += (pFrm->MacHeader.Tx.Status & 0xF0) / 16 - 1;

        switch (pWork->Mode) {
            case 1:
                if ((pFrm->Dot11Header.FrameCtrl.Data & 1) == 0) {
                    camAdrs = CAM_Search(pFrm->Dot11Header.Adrs2);
                    if (camAdrs == 0xFF || CAM_GetStaState(camAdrs) != 0x40) {
                        if (CAM_GetStaState(camAdrs) == 0x30) {
                            if (!IsExistManFrame(pFrm->Dot11Header.Adrs2, 0xA0)) {
                                pDeAuth = (DEAUTH_FRAME*)MakeDisAssFrame(pFrm->Dot11Header.Adrs2, 7);
                                
                            } else {
                                break;
                            }
                            
                        } else {
                            if (!IsExistManFrame(pFrm->Dot11Header.Adrs2, 0xC0)) {
                                pDeAuth = MakeDeAuthFrame(pFrm->Dot11Header.Adrs2, 7, 1);
                                
                            } else {
                                break;
                            }
                        }
                        if (pDeAuth)
                            TxManCtrlFrame((TXFRM*)pDeAuth);
                        
                    } else {
                        CAM_SetPowerMgtMode(camAdrs, pFrm->Dot11Header.FrameCtrl.Bit.PowerMan);
                        
                        if (pFrm->Dot11Header.SeqCtrl.Data == CAM_GetLastSeqCtrl(camAdrs)) {
                            pCounter->rx.duplicateErr++;
                        } else {
                            WSetMacAdrs1(pFrm->Dot11Header.Adrs1, pFrm->Dot11Header.Adrs3);
                            err = 0;
                        }
                    }
                }
                break;
            
            case 2:
            case 3:
                if ((pFrm->Dot11Header.FrameCtrl.Data & 1) == 0) {
                    if (pWork->PowerMgtMode != 0 && (pFrm->Dot11Header.FrameCtrl.Data & 0x2000) == 0) {
                        if ((pFrm->Dot11Header.Adrs1[0] & 1) != 0) {
                            pWork->bExistTIM &= ~1;
                        } else if (pWork->Mode != 3) {
                            pWork->bExistTIM &= ~2;
                        }
                        
                        if (pWork->bExistTIM == 0 && pHeapMan->TxPri[0].Count == 0 && pHeapMan->TxPri[1].Count == 0) {
                            WSetPowerState(1);
                        }
                    }

                    camAdrs = pWork->APCamAdrs;
                    if (pFrm->Dot11Header.SeqCtrl.Data == CAM_GetLastSeqCtrl(camAdrs)) {
                        pCounter->rx.duplicateErr++;
                    } else {
                        WSetMacAdrs1(pFrm->Dot11Header.Adrs2, pFrm->Dot11Header.Adrs3);
                        err = 0;
                    }
                }
                break;
        }

        if (err == 0) {
            pFrm->FirmHeader.CamAdrs = camAdrs;
            CAM_SetRSSI(camAdrs, pFrm->MacHeader.Rx.rsv_RSSI & 0xFF);
            CAM_SetLastSeqCtrl(camAdrs, pFrm->Dot11Header.SeqCtrl.Data);
            CAM_UpdateLifeTime(camAdrs);
            pFrm->FirmHeader.Length = pFrm->MacHeader.Rx.MPDU - 24;
            pInd->header.code = 384;
            pInd->header.length = ((u32)pFrm->FirmHeader.Length + 45) / 2;
            SendMessageToWmDirect(&pHeapMan->RxData, pInd);
            
        } else {
            ReleaseHeapBuf(&pHeapMan->RxData, pInd);
        }

        if (pHeapMan->RxData.Count)
            AddTask(2, 6);
    }
}

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
    pInd = GET_HEADER(pFrm);
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
    
    pInd = GET_HEADER(pFrm);
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

static void RxProbeResFrame(PRBRES_FRAME* pFrm, ELEMENT_CHECKER* pChk) { // RxCtrl.c:1758
    MLME_MAN* pMLME = &wlMan->MLME; // r6 - :1760
    WlMlmeScanCfm* pCfm; // r7 - :1761
    PRBRES_BODY* pPrbRes; // r0 - :1762
    WlBssDesc* pDesc; // r8 - :1763
    u32 bodyLen, i, j, id, len; // r9, r8, r5, r0, r0 - :1764
    ELEMENT_CHECKER elementCheck; // None - :1765
    u16* pBssidMask; // r9 - :1766
    u8 *pSrc, *pDst; // r9, r11 - :1767

    if (pMLME->State != 0x13)
        return;
    
    pCfm = pMLME->pCfm.Scan;
    pBssidMask = pMLME->pReq.Scan->bssidMask;
    for (i = 0; i < pMLME->pReq.Scan->bssidMaskCount; i++) {
        if (MatchMacAdrs(pBssidMask, pFrm->Dot11Header.BSSID)) {
            pCfm->foundMap |= (1 << i);
            return;
        }
        pBssidMask += 3;
    }

    pDesc = pCfm->bssDescList;
    for (j = 0; j < pCfm->bssDescCount; j++) {
        if (MatchMacAdrs(pFrm->Dot11Header.BSSID, pDesc->bssid))
            return;

        pDesc = (WlBssDesc*)((u32)pDesc + 2 * pDesc->length);
    }

    MIi_CpuClear16(0, pDesc, 0x40);
    bodyLen = pFrm->FirmHeader.Length;

    if (bodyLen <= 12)
        return;

    pPrbRes = &pFrm->Body;
    if (!pChk) {
        pChk = &elementCheck;
        MIi_CpuClear32(0, &elementCheck, sizeof(elementCheck));
        elementCheck.pElement = pPrbRes->Buf;
        elementCheck.bodyLength = bodyLen - 12;
        elementCheck.matchFlag = 0x3;
        elementCheck.foundFlag = 0x38;
        elementCheck.rxStatus = pFrm->MacHeader.Tx.Status;
        elementCheck.capability = pPrbRes->CapaInfo.Data;
        ElementChecker(&elementCheck);
    }

    if (pChk->pGMIF) {
        pDesc->gameInfoLength = WL_ReadByte(&pChk->pGMIF->Length) - 8;
        pDesc->length = ((u32)pDesc->gameInfoLength + 65) / 2;
    } else {
        pDesc->length = ((u32)pChk->otherElementLength + 65) / 2;
    }

    if ((pChk->matchFlag & 1) == 1 && wlMan->MLME.Work.Scan.MaxConfirmLength >= pDesc->length) {
        pDesc->capaInfo = pPrbRes->CapaInfo.Data;
        WSetMacAdrs1(pDesc->bssid, pFrm->Dot11Header.BSSID);
        pDesc->beaconPeriod = pPrbRes->BeaconInterval;
        pDesc->rssi = (u8)pFrm->MacHeader.Tx.MPDU;

        if (pChk->pGMIF) {
            for (i = 0; i < pDesc->gameInfoLength; i++) {
                WL_WriteByte(&pDesc->gameInfo[i], WL_ReadByte(&pChk->pGMIF->GameInfo[i]));
            }
            
        } else {
            pDesc->otherElementCount = pChk->otherElementCount;
            if (pChk->otherElementCount != 0) {
                pSrc = (u8*)pPrbRes->Buf;
                pDst = (u8*)pDesc->gameInfo;

                for (i = 0; i < pChk->otherElementCount; ) {
                    id = WL_ReadByte(pSrc);
                    len = WL_ReadByte(pSrc + 1);
                    if (id > 6 && pSrc != (u8*)pChk->pGMIF) {
                        // i don't get any of this
                        for (j = 0; j < len + 2; j++) {
                            WL_WriteByte(pDst, WL_ReadByte(pSrc));
                            pDst++; pSrc++;
                        }
                        i++;
                        
                    } else {
                        pSrc += len + 2;
                    }
                }
            }
        }

        if (pChk->pSSID) {
            pDesc->ssidLength = WL_ReadByte(&pChk->pSSID->Length);
            for (i = 0; i < pDesc->ssidLength; i++) {
                WL_WriteByte(&pDesc->ssid[i], WL_ReadByte(&pChk->pSSID->SSID[i]));
            }
            
        } else {
            pDesc->ssidLength = 0;
            for (i = 0; i < 0x20; i++) {
                WL_WriteByte(&pDesc->ssid[i], 0);
            }
        }

        pDesc->rateSet.basic = pChk->rateSet.Basic;
        pDesc->rateSet.support = pChk->rateSet.Support;
        pDesc->channel = pChk->channel;

        if (pChk->pCFP)
            pDesc->cfpPeriod = WL_ReadByte(&pChk->pCFP->CFPPeriod);

        if (pChk->pTIM)
            pDesc->dtimPeriod = WL_ReadByte(&pChk->pTIM->DTIMPeriod);

        pCfm->header.length += pDesc->length;
        pCfm->bssDescCount++;
        pMLME->Work.Scan.MaxConfirmLength -= pDesc->length;

        if (pMLME->Work.Scan.MaxConfirmLength < 0x20) {
            ClearTimeOut();
            pMLME->State = 0x15;
            AddTask(2, 0);
        }
        
    } else {
        WSetMacAdrs1(pDesc->bssid, NULL_ADRS);
    }
}

static void RxAuthFrame(AUTH_FRAME* pFrm) { // RxCtrl.c:1993
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :1995
    MLME_MAN* pMLME = &wlMan->MLME; // r5 - :1996
    AUTH_BODY* pAuth = &pFrm->Body; // r0 - :1997
    AUTH_FRAME* pTxFrm; // r4 - :1998
    u32 cam_adrs; // r6 - :1999
    u32 bTxAuth; // r7 - :2000
    u16 seqNum, stsCode; // r8, r9 - :2001
    TXQ* pTxq; // r1 - :2013
    u16 map; // r0 - :2014

    if (wlMan->WlOperation & 8) {
        pTxq = wlMan->TxCtrl.Txq;
        map = W_TXREQ_READ;
        if (
            ((map & 1) == 0 || !pTxq[0].Busy)
            && ((map & 4) == 0 || !pTxq[1].Busy)
            && ((map & 8) == 0 || !pTxq[2].Busy)
            && (W_RF_PINS & 1) == 0
        ) {
            W_WEP_CNT = 0;
            W_WEP_CNT = 0x8000;
            wlMan->RxCtrl.IcvOkCntFlag = 0;
        }
    }

    if (!IsExistManFrame(pFrm->Dot11Header.SA, 0xB0)) {
        cam_adrs = pFrm->FirmHeader.CamAdrs;
        bTxAuth = 0;
        seqNum = pFrm->Body.SeqNum + 1;

        if (cam_adrs == 0) {
            stsCode = 19;
            bTxAuth = 1;
            goto exit; // TODO: remove this goto?
        }

        if (pWork->Mode == 1) {
            if (CAM_GetStaState(cam_adrs) > 0x20) {
                CAM_SetStaState(cam_adrs, 0x20);
                MLME_IssueDeAuthIndication(pFrm->Dot11Header.SA, 1);
            }
            if ((pFrm->MacHeader.Tx.Status & 0x400) != 0 && CAM_GetAuthSeed(cam_adrs) != 0) {
                bTxAuth = 1;
                pFrm->Body.AlgoType = 1;
                stsCode = 15;
                seqNum = 4;
                CAM_SetAuthSeed(cam_adrs, 0);
                goto exit; // TODO: remove this goto?
            }
        }

        switch (pFrm->Body.AlgoType) {
            case 0:
                if (pWork->Mode == 1 && wlMan->Config.AuthAlgo == 1) {
                    stsCode = 13;
                    bTxAuth = 1;
                }
                
                else if (pWork->Mode == 1) { // :2103
                    bTxAuth = 1;
                    if (pFrm->Body.SeqNum == 1) { // :2105
                        stsCode = 0;
                        
                    } else {
                        stsCode = 14;
                        seqNum = 2;
                    }
                }

                else if (pWork->Mode != 1) { // "else" does not match
                    if (pFrm->Body.SeqNum == 2) {
                        if (pMLME->pReq.Auth->algorithm == 0 && MatchMacAdrs(pMLME->pReq.Auth->peerMacAdrs, pFrm->Dot11Header.SA) && pMLME->State == 0x31) {
                            ClearTimeOut();
                            if (pFrm->Body.StatusCode == 0) {
                                WSetStaState(0x30);
                                pMLME->pCfm.Auth->resultCode = 0;
                                pMLME->pCfm.Auth->statusCode = 0;
                                
                            } else {
                                pMLME->pCfm.Auth->resultCode = 12;
                                pMLME->pCfm.Auth->statusCode = pFrm->Body.StatusCode;
                            }
                            pMLME->State = 0x35;
                            AddTask(2, 2);
                        }
                    }
                }
                
                break;

            case 1:
                if (pWork->Mode == 1) {
                    CAM_SetStaState(cam_adrs, 0x20);
                    if (pFrm->Body.SeqNum == 1) {
                        pTxFrm = MakeAuthFrame(pFrm->Dot11Header.SA, 0x80, 1);
                        if (pTxFrm) {
                            pTxFrm->Body.AlgoType = pFrm->Body.AlgoType;
                            pTxFrm->Body.SeqNum = seqNum;
                            pTxFrm->Body.StatusCode = 0;
                            SetChallengeText(cam_adrs, pTxFrm);
                            TxManCtrlFrame((TXFRM*)pTxFrm);
                        }
                        
                    } else if (pFrm->Body.SeqNum == 3) {
                        if (CAM_GetStaState(cam_adrs) != 32 || CAM_GetAuthSeed(cam_adrs) == 0) {
                            stsCode = 1;
                            bTxAuth = 1;
                            
                        } else if (!CheckChallengeText(pFrm)) {
                            stsCode = 15;
                            bTxAuth = 1;
                            CAM_SetAuthSeed(cam_adrs, 0);
                            
                        } else {
                            CAM_SetAuthSeed(cam_adrs, 0);
                            stsCode = 0;
                            bTxAuth = 1;
                        }
                        
                    } else {
                        CAM_SetAuthSeed(cam_adrs, 0);
                        stsCode = 14;
                        seqNum = 2;
                        bTxAuth = 1;
                    }
                    
                } else {
                    if (pMLME->pReq.Auth->algorithm == 1 && MatchMacAdrs(pMLME->pReq.Auth->peerMacAdrs, pFrm->Dot11Header.SA)) {
                        if (pFrm->Body.SeqNum == 2) {
                            if (pMLME->State == 0x31) {
                                if (pFrm->Body.StatusCode != 0) {
                                    ClearTimeOut();
                                    pMLME->State = 0x35;
                                    pMLME->pCfm.Auth->resultCode = 12;
                                    pMLME->pCfm.Auth->statusCode = pFrm->Body.StatusCode;
                                    AddTask(2, 2);
                                    WSetStaState(0x20);
                                    
                                } else {
                                    pMLME->State = 0x33;
                                    if (WL_ReadByte(&pFrm->Body.ChallengeText.ID) == 16) {
                                        pTxFrm = MakeAuthFrame(pFrm->Dot11Header.SA, WL_ReadByte(&pFrm->Body.ChallengeText.Length), 1);
                                        if (pTxFrm) {
                                            pTxFrm->Dot11Header.FrameCtrl.Data |= 0x4000;
                                            MIi_CpuCopy16(&pFrm->Body, &pTxFrm->Body, WL_ReadByte(&pFrm->Body.ChallengeText.Length) + 8);
                                            pTxFrm->Body.AlgoType = pFrm->Body.AlgoType;
                                            pTxFrm->Body.SeqNum = 3;
                                            pTxFrm->Body.StatusCode = 0;
                                            TxManCtrlFrame((TXFRM*)pTxFrm);
                                        }
                                    }
                                }
                            }
                            
                        } else if (pFrm->Body.SeqNum == 4 && pMLME->State == 0x33) {
                            ClearTimeOut();
                            if (pFrm->Body.StatusCode != 0) {
                                pMLME->pCfm.Auth->resultCode = 12;
                                pMLME->pCfm.Auth->statusCode = pFrm->Body.StatusCode;
                            } else {
                                WSetStaState(0x30);
                                pMLME->pCfm.Auth->resultCode = 0;
                                pMLME->pCfm.Auth->statusCode = 0;
                            }
                            pMLME->State = 0x35;
                            AddTask(2, 2);
                        }
                    }
                }

                break;

            default:
                if (pWork->Mode == 1) {
                    stsCode = 13;
                    bTxAuth = 1;
                }
        }

exit:
        if (bTxAuth) {
            pTxFrm = MakeAuthFrame(pFrm->Dot11Header.SA, 0, stsCode != 0);
            if (pTxFrm) {
                pTxFrm->Body.AlgoType = pFrm->Body.AlgoType;
                pTxFrm->Body.SeqNum = seqNum;
                pTxFrm->Body.StatusCode = stsCode;
                TxManCtrlFrame((TXFRM*)pTxFrm);
            }
        }
    }
}

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

static void ElementChecker(ELEMENT_CHECKER* p) { // RxCtrl.c:2564
    WORK_PARAM* pWork = &wlMan->Work; // r4 - :2566
    u8* pBuf; // r5 - :2567
    long i; // r6 - :2568
    u32 len; // r7 - :2569
    
    p->channel = pWork->CurrChannel;
    pBuf = p->pElement;
    
    if (p->foundFlag & 0x800)
        p->matchFlag |= 1;
    
    for (i = p->bodyLength; i > 0; i -= len + 2) {
        u32 unk = WL_ReadByte(pBuf++);
        len = WL_ReadByte(pBuf++);
        
        switch (unk) {
            case 0:
                if (len <= 0x20) {
                    p->foundFlag |= 1;
                    p->pSSID = (SSID_ELEMENT*)(pBuf-2);
                    if (len == 0 && (p->foundFlag & 0x800) != 0) {
                        p->matchFlag |= 1;
                    } else {
                        p->matchFlag &= ~1;
                        if (WCheckSSID(len, pBuf)) {
                            p->matchFlag |= 1;
                        }
                    }
                }
                break;
            case 1:
                if (len >= 1) {
                    p->foundFlag |= 4;
                    WElement2RateSet((SUP_RATE_ELEMENT *)(pBuf - 2), &p->rateSet);
                    if ((p->rateSet.Basic & ~(pWork->RateSet.Basic | pWork->RateSet.Support)) == 0 && pWork->RateSet.Basic == (pWork->RateSet.Basic & (p->rateSet.Basic | p->rateSet.Support))) {
                        p->matchFlag |= 4;
                    } else {
                        p->matchFlag &= ~4;
                    }
                }
                break;
            
            case 2:
            case 6:
                break;
            
            case 3:
                if (len >= 1) {
                    p->foundFlag |= 2;
                    p->channel = WL_ReadByte(pBuf);
                    if (p->channel == wlMan->MLME.pReq.Join->bssDesc.channel) {
                        p->matchFlag |= 2;
                    } else {
                        p->matchFlag &= ~2;
                    }
                }
                break;
            
            case 5:
                if (len >= 3) {
                    p->foundFlag |= 0x100;
                    p->pTIM = (TIM_ELEMENT*)(pBuf - 2);
                }
                break;
            
            case 4:
                if (len >= 6) {
                    p->foundFlag |= 0x200;
                    p->pCFP = (CF_PARAM_ELEMENT*)(pBuf - 2);
                }
                break;
            
            case 221: // 0xDD
                // 00:09:BF:00
                if (len < 8 || WL_ReadByte(pBuf) != 0 || WL_ReadByte(pBuf+1) != 0x09 || WL_ReadByte(pBuf+2) != 0xBF || WL_ReadByte(pBuf+3) != 0) {
                    p->otherElementCount++;
                    p->otherElementLength += len + 2;
                    
                } else {
                    p->foundFlag |= 0x400;
                    p->pGMIF = (GAME_INFO_ELEMENT*)(pBuf - 2);
                }
                break;
            
            default:
                p->otherElementCount++;
                p->otherElementLength += len + 2;
                break;

        }
        pBuf += len;
    }
    if ((p->foundFlag & 8) != 0 && ((pWork->BSSID[0] & 1) != 0 || (p->rxStatus & 0x8000) != 0)) {
        p->matchFlag |= 8;
    }
    if ((p->foundFlag & 0x30) != 0) {
        if ((p->capability & 3) == (pWork->CapaInfo & 3))
            p->matchFlag |= 0x10;
        if ((p->capability & 0x10) == (pWork->CapaInfo & 0x10))
            p->matchFlag |= 0x20;
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

static void SetChallengeText(unsigned long camAdrs, AUTH_FRAME* pFrm) {
    u32 i;
    u32 txtLen;
    u16* pText;
    u16 rnd;

    rnd = W_RANDOM + (W_RANDOM << 8);
    if (rnd == 0) rnd = 1;

    RND_seed(rnd);
    CAM_SetAuthSeed(camAdrs, rnd);

    pText = (u16*)pFrm->Body.ChallengeText.Text;
    txtLen = WL_ReadByte(&pFrm->Body.ChallengeText.Length);

    for (i = 0; i < txtLen; i += 2) {
        *(pText++) = RND_rand();
    }
}

static u32 CheckChallengeText(AUTH_FRAME* pFrm) {
    u32 i;
    u32 txtLen;
    u16* pText;
    
    if (WL_ReadByte(&pFrm->Body.ChallengeText) != 16)
        return 0;
    
    RND_seed(CAM_GetAuthSeed(pFrm->FirmHeader.CamAdrs));
    
    pText = (u16*)pFrm->Body.ChallengeText.Text;
    txtLen = WL_ReadByte(&pFrm->Body.ChallengeText.Length);

    for (i = 0; i < txtLen/2; i++) {
        if (*(pText++) != RND_rand())
            return 0;
    }

    if (((txtLen & 1) != 0) && ((*pText) & 0xFF) != (RND_rand() & 0xFF))
        return 0;
    
    return 1;
}

#define DEFRAG_MATCHES(tbl1, tbl2) \
    (MatchMacAdrs((tbl1)->DA, (tbl2)->DA) \
    && MatchMacAdrs((tbl1)->SA, (tbl2)->SA) \
    && (tbl1)->SeqCtrl.Bit.SeqNum == (tbl2)->SeqCtrl.Bit.SeqNum)

static void NewDefragment(RXFRM_MAC* pMFrm, DEFRAG_TBL* pDefragTbl) { 
    // TODO: check the variables (names might be incorrect)
    
    DEFRAG_LIST* pList = wlMan->RxCtrl.DefragList; // r8 - :3187
    RXPACKET* pPacket; // r0 - :3187
    RXFRM* pFrm; // r7 - :3187
    u32 i; // r7 - :3187
    u32 pos; // r6 - :3187
    u32 fragCnt; // r0 - :3187
    u32 SrcOfst; // r0 - :3187
    u32 WSize; // r5 - :3187
    u32 OvrCnt; // r0 - :3187

    pos = 0xFFFFFFFF;

    for (i = 0; i < 3; i++) {
        if (pList[i].RestTime != 0) {
            if (DEFRAG_MATCHES(&pList[i].DefragTbl, pDefragTbl)) {
                fragCnt = (pMFrm->MacHeader.Rx.Status & 0xF0) / 16;
                OvrCnt = fragCnt - (pList[i].DefragTbl.SeqCtrl.Bit.FragNum); 
                if (OvrCnt != 0 && (OvrCnt & 0x80000000) == 0) {
                    pPacket = pList[i].pPacket;
                    pFrm = (RXFRM*)&pPacket->frame;
                    WSize = pMFrm->MacHeader.Rx.MPDU - pFrm->MacHeader.Rx.MPDU - 0x18;
                    if (WSize != 0 && (WSize & 0x80000000) == 0) {
                        MIi_CpuCopy16(
                            pMFrm->Body + pFrm->MacHeader.Rx.MPDU,
                            pFrm->Body + pFrm->MacHeader.Rx.MPDU,
                            WSize
                        );
                        pFrm->MacHeader.Rx.MPDU += WSize;
                        pList[i].DefragTbl.SeqCtrl.Bit.FragNum = fragCnt;
                        wlMan->Counter.rx.fragment += fragCnt;
                    }
                }
                return;
            }
            
        } else {
            pos = i;
        }
    }

    if (pos != 0xFFFFFFFF) {
        pPacket = (RXPACKET*)AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, 0x622);
        
        if (pPacket) {
            MIi_CpuCopy16(pDefragTbl, &pList[pos].DefragTbl, sizeof(DEFRAG_TBL));
            pList[pos].RestTime = 5;
            pList[pos].pPacket = pPacket;

            pFrm = (RXFRM*)&pPacket->frame;
            MIi_CpuCopy16(pMFrm, &pFrm->MacHeader, pMFrm->MacHeader.Rx.MPDU + 0xC);
            MI_WaitDma(wlMan->DmaChannel);

            fragCnt = (pFrm->MacHeader.Rx.Status & 0xF0) / 16;
            pList[pos].DefragTbl.SeqCtrl.Bit.FragNum = fragCnt;
            wlMan->Counter.rx.fragment += fragCnt;
            
            pFrm->MacHeader.Rx.MPDU = pMFrm->MacHeader.Rx.MPDU - 0x18;
            pList[pos].UnitLength = pFrm->MacHeader.Rx.MPDU / fragCnt;
            
        } else {
            SetFatalErr(4);
        }
    }
}

static void MoreDefragment(RXFRM_MAC* pMFrm, DEFRAG_TBL* pDefragTbl) {
    // TODO: check the variables (names might be incorrect)
    
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // r8 - :3313
    DEFRAG_LIST* pList = wlMan->RxCtrl.DefragList; // r7 - :3313
    RXFRM* pFrm; // r6 - :3313
    u32 i; // r6 - :3313
    u32 fragCnt; // r0 - :3313
    u32 length; // r9 - :3313
    u32 OvrCnt; // r11 - :3313
    u32 SrcOfst; // None - :3313
    u32 WSize; // r5 - :3313

    pMFrm->MacHeader.Rx.MPDU -= 0x18;

    for (i = 0; i < 3; i++) {
        if (pList[i].RestTime != 0 && DEFRAG_MATCHES(&pList[i].DefragTbl, pDefragTbl)) {
            OvrCnt = pList[i].DefragTbl.SeqCtrl.Data - pDefragTbl->SeqCtrl.Data;
            if ((OvrCnt & 0x80000000) == 0) {
                SrcOfst = OvrCnt * pList[i].UnitLength;
                WSize = pMFrm->MacHeader.Rx.MPDU - SrcOfst;
            
                if (WSize != 0 && (WSize & 0x80000000) == 0) {
                    break;
                }
            }

            return;
        }
    }

    if (i != 3) {
        pFrm = &pList[i].pPacket->frame;
        length = pFrm->MacHeader.Rx.MPDU + WSize;
        if (length > 0x5E4) {
            ReleaseHeapBuf(&pHeapMan->TmpBuf, GET_HEADER(pFrm));
            pList[i].RestTime = 0;
            
        } else {
            MIi_CpuCopy16(
                pMFrm->Body + SrcOfst,
                pFrm->Body + pFrm->MacHeader.Rx.MPDU,
                WSize + 1
            );
            pFrm->MacHeader.Rx.MPDU = length;

            fragCnt = (pMFrm->MacHeader.Rx.Status & 0xF0) / 16;
            pList[i].DefragTbl.SeqCtrl.Bit.FragNum += (fragCnt - OvrCnt);
            wlMan->Counter.rx.fragment += fragCnt;
            
            if ((pMFrm->MacHeader.Rx.Status & 0x100) == 0) {
                pList[i].RestTime = 0;
                
                pFrm->MacHeader.Rx.Status = (pFrm->MacHeader.Rx.Status & ~0xF0) + 0x10;
                pFrm->MacHeader.Rx.MPDU += 24;
                
                switch ((pFrm->MacHeader.Rx.Status & 0xF)) {
                    case 8:
                        MoveHeapBuf(&pHeapMan->TmpBuf, &pHeapMan->RxData, GET_HEADER(pFrm));
                        AddTask(2, 6);
                        break;
                    
                    case 0:
                        MoveHeapBuf(&pHeapMan->TmpBuf, &pHeapMan->RxManCtrl, GET_HEADER(pFrm));
                        AddTask(1, 7);
                        break;
                    
                    default:
                        ReleaseHeapBuf(&pHeapMan->TmpBuf, GET_HEADER(pFrm));
                }
            }
        }
    }
}

void DefragTask() { // RxCtrl.c:3011
    HEAP_MAN* pHeapMan = &wlMan->HeapMan; // None - :3013
    RXPACKET* pPacket; // r8 - :3014
    RXFRM* pFrm; // r0 - :3015
    DEFRAG_TBL defragTbl; // None - :3016
    u32 fc; // r4 - :3017

    pPacket = (RXPACKET *)pHeapMan->Defrag.Head;
    
    if (pPacket == (RXPACKET*)-1) {
        return;
    }
    
    pFrm = (RXFRM*)&pPacket->frame;
    
    if (wlMan->Work.STA == 0x40 && pFrm->MacHeader.Rx.MPDU <= 1532) {
        fc = pFrm->Dot11Header.FrameCtrl.Data;
        
        if ((fc & 0x100) != 0) {
            WSetMacAdrs1(defragTbl.DA, pFrm->Dot11Header.Adrs3);
            
            if ((fc & 0x200) != 0) {
                goto exit;
            } else {
                WSetMacAdrs1(defragTbl.SA, pFrm->Dot11Header.Adrs2);
            }
            
        } else {
            WSetMacAdrs1(defragTbl.DA, pFrm->Dot11Header.Adrs1);
        
            if ((fc & 0x200) != 0) {
                WSetMacAdrs1(defragTbl.SA, pFrm->Dot11Header.Adrs3);
                // then we defrag
            } else {
                WSetMacAdrs1(defragTbl.SA, pFrm->Dot11Header.Adrs2);
                // then we defrag
            }
        }

        defragTbl.SeqCtrl.Data = pFrm->Dot11Header.SeqCtrl.Data;
        if ((fc & 0x400) != 0 && pFrm->Dot11Header.SeqCtrl.Bit.FragNum == 0) {
            NewDefragment((RXFRM_MAC*)&pFrm->MacHeader, &defragTbl);
        } else {
            MoreDefragment((RXFRM_MAC*)&pFrm->MacHeader, &defragTbl);
        }
    }

exit:
    ReleaseHeapBuf(&pHeapMan->Defrag, pPacket);
    if (pHeapMan->Defrag.Count)
        AddTask(2, 9);
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
