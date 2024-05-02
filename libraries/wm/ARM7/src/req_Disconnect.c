#include "Mongoose.h"

static void WmspIndError(u16 wlCommand, u16 wlResult, u16 tryBmp, u16 resBmp);
static void WmspError(u16 wlCommand, u16 wlResult, u16 tryBmp, u16 resBmp);

void WMSP_Disconnect(void *msg)
{                                   // req_Disconnect.c:189
    int result;                     // r0 - :191
    u16 tryBmp;                     // r5 - :192
    u16 resBmp;                     // None - :193
    WMDisconnectCallback *callback; // r0 - :200

    tryBmp = ((u32 *)msg)[1];
    if (WMSP_DisconnectCore((u32 *)msg, 0, &resBmp) == 1)
    {
        callback = (WMDisconnectCallback *)WMSP_GetBuffer4Callback2Wm9();
        callback->apiid = WM_APIID_DISCONNECT;
        callback->errcode = WM_ERRCODE_SUCCESS;
        callback->tryBitmap = tryBmp;
        callback->disconnectedBitmap = resBmp;
        WMSP_ReturnResult2Wm9(callback);
    }
}

int WMSP_DisconnectCore(u32 *args, int indicateFlag, u16 *disconnected)
{                            // req_Disconnect.c:225
    u32 wlBuf[128];          // None - :228
    u16 *buf = (u16 *)wlBuf; // :229
    WlCmdCfm *pConfirm;      // r0 - :230
    WMStatus *status = wmspW.status;

    u16 aidBitmap = args[1];                 // r0 - :234
    u16 reason = indicateFlag ? args[2] : 0; // None - :235
    u16 resBitmap = 0;                       // r5 - :236
    int fCleanQueue = 0;                     // r6 - :238

    if (status->state == WM_STATE_MP_PARENT || status->state == WM_STATE_PARENT)
    {
        if (wmspW.status->mp_flag == 1)
        {
            fCleanQueue = 1;
        }
    }
    else if (status->state == WM_STATE_MP_CHILD || status->state == WM_STATE_CHILD)
    {
        u32 e = OS_DisableInterrupts(); // r7 - :252

        if (status->child_bitmap == 0)
        {
            OS_RestoreInterrupts(e);

            if (!indicateFlag)
            {
                WMDisconnectCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :260
                callback->apiid = WM_APIID_DISCONNECT;
                callback->errcode = WM_ERRCODE_ILLEGAL_STATE;
                callback->wlCmdID = 0;
                callback->wlResult = 0;
                callback->tryBitmap = aidBitmap;
                callback->disconnectedBitmap = 0;
                WMSP_ReturnResult2Wm9(callback);
            }

            return 0;
        }

        if (status->mp_flag == 1)
        {
            WMSP_FlushSendQueue(0, 0);
            status->mp_flag = 0;
            fCleanQueue = 1;
            WMSP_CancelVAlarm();
            WMSP_SetThreadPriorityLow();

            if (status->state == WM_STATE_MP_CHILD)
            {
                status->state = WM_STATE_CHILD;
            }
        }

        status->child_bitmap = 0;
        status->mp_readyBitmap = 0;
        status->ks_flag = 0;
        status->dcf_flag = 0;
        status->VSyncFlag = 0;
        OS_RestoreInterrupts(e);
    }
    else
    {
        if (!indicateFlag)
        {
            WMDisconnectCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :301
            callback->apiid = WM_APIID_DISCONNECT;
            callback->errcode = WM_ERRCODE_ILLEGAL_STATE;
            callback->wlCmdID = 0;
            callback->wlResult = 0;
            callback->tryBitmap = aidBitmap;
            callback->disconnectedBitmap = 0;
            WMSP_ReturnResult2Wm9(callback);
        }

        return 0;
    }

    if (status->state == WM_STATE_MP_CHILD || status->state == WM_STATE_CHILD)
    {
        u16 wMac[3];     // None - :319
        long auth_retry; // r5 - :332

        MI_CpuCopy8(status->parentMacAddress, wMac, 6);

        for (auth_retry = 0; auth_retry < 2;)
        {
            pConfirm = (WlCmdCfm *)WMSP_WL_MlmeDeAuthenticate(buf, wMac, 3);

            switch (pConfirm->resultCode)
            {
            case 7:
            case 12:
                auth_retry++;
                continue;

            case 0:
            case 1:
                break;

            default:
                if (indicateFlag)
                {
                    WmspIndError(MLME_DE_AUTH_REQ_CMD, pConfirm->resultCode, aidBitmap, 0);
                }
                else
                {
                    WmspError(MLME_DE_AUTH_REQ_CMD, pConfirm->resultCode, aidBitmap, 0);
                }

                if (fCleanQueue)
                {
                    WMSP_CleanSendQueue(1);
                }

                return 0;
            }

            break;
        }

        resBitmap = 1;
        status->beaconIndicateFlag = 0;
        status->state = WM_STATE_CLASS1;

        pConfirm = (WlCmdCfm *)WMSP_WL_MlmeReset(buf, 1);
        if (pConfirm->resultCode != 0)
        {
            if (indicateFlag)
            {
                WmspIndError(MLME_RESET_REQ_CMD, pConfirm->resultCode, aidBitmap, 1);
            }
            else
            {
                WmspError(MLME_RESET_REQ_CMD, pConfirm->resultCode, aidBitmap, 1);
            }

            if (fCleanQueue)
            {
                WMSP_CleanSendQueue(1);
            }

            return 0;
        }

        pConfirm = (WlCmdCfm *)WMSP_WL_DevIdle(buf);
        if (pConfirm->resultCode != 0)
        {
            if (indicateFlag)
            {
                WmspIndError(DEV_IDLE_REQ_CMD, pConfirm->resultCode, aidBitmap, 1);
            }
            else
            {
                WmspError(DEV_IDLE_REQ_CMD, pConfirm->resultCode, aidBitmap, 1);
            }

            if (fCleanQueue)
            {
                WMSP_CleanSendQueue(1);
            }

            return 0;
        }

        status->state = WM_STATE_IDLE;
        status->wep_flag = 0;
        status->wepMode = 0;
        MI_CpuFill8(status->wepKey, 0, 0x50);
        WMSP_ResetSizeVars();

        if (indicateFlag == 1)
        {
            WMStartConnectCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :437
            cb->apiid = WM_APIID_START_CONNECT;
            cb->errcode = WM_ERRCODE_SUCCESS;
            cb->state = WM_STATECODE_DISCONNECTED;
            cb->reason = reason;
            cb->aid = status->aid;
            MI_CpuCopy8(wMac, cb->macAddress, 6);
            cb->parentSize = status->mp_parentSize;
            cb->childSize = status->mp_childSize;
            WMSP_ReturnResult2Wm9(cb);
        }
        else
        {
            WMSP_IndicateDisconnectionFromMyself(0, 0, wMac);
        }

        if (fCleanQueue)
        {
            WMSP_CleanSendQueue(1);
        }
    }
    else
    {
        u16 wMac[3]; // None - :466
        long i;      // r7 - :467
        u16 aid;     // None - :468

        for (i = 1; i < 16; i++)
        {
            if (((1 << i) & (status->child_bitmap & aidBitmap)) == 0)
            {
                continue;
            }

            aid = i;
            MI_CpuCopy8(status->childMacAddress[i - 1], &wMac, sizeof(wMac));

            long auth_retry; // r4 - :487
            for (auth_retry = 0; auth_retry < 2;)
            {
                pConfirm = (WlCmdCfm *)WMSP_WL_MlmeDeAuthenticate(buf, wMac, 3);

                switch (pConfirm->resultCode)
                {
                case 0:
                    break;

                case 7:
                case 12:
                    auth_retry++;
                    continue;

                default:
                    if (indicateFlag)
                    {
                        WmspIndError(MLME_DE_AUTH_REQ_CMD, pConfirm->resultCode, aidBitmap, resBitmap);
                    }
                    else
                    {
                        WmspError(MLME_DE_AUTH_REQ_CMD, pConfirm->resultCode, aidBitmap, resBitmap);
                    }

                    if (fCleanQueue)
                    {
                        WMSP_CleanSendQueue(1);
                    }

                    return 0;
                }

                break;
            }

            u32 e = OS_DisableInterrupts(); // r0 - :529

            if ((status->child_bitmap & (1 << i)) != 0)
            {
                resBitmap |= (1 << aid);

                status->child_bitmap &= ~(1 << i);
                status->mp_readyBitmap &= ~(1 << i);
                status->mp_lastRecvTick[aid] = 0;

                MI_CpuFill8(&status->childMacAddress[i - 1], 0, sizeof(status->childMacAddress[i - 1]));
                OS_RestoreInterrupts(e);

                if (indicateFlag == 1)
                {
                    WMStartParentCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :552
                    cb->apiid = WM_APIID_START_PARENT;
                    cb->errcode = WM_ERRCODE_SUCCESS;
                    cb->state = WM_STATECODE_DISCONNECTED;
                    cb->reason = reason;
                    cb->aid = aid;
                    MI_CpuCopy8(wMac, cb->macAddress, sizeof(wMac));
                    cb->parentSize = status->mp_parentSize;
                    cb->childSize = status->mp_childSize;
                    WMSP_ReturnResult2Wm9(cb);
                }
                else
                {
                    WMSP_IndicateDisconnectionFromMyself(1, i, &wMac);
                }

                if (fCleanQueue)
                {
                    WMSP_CleanSendQueue(1 << i);
                }
            }
            else
            {
                OS_RestoreInterrupts(e);
            }
        }
    }

    if (disconnected)
    {
        *disconnected = resBitmap;
    }

    return 1;
}

void WMSP_IndicateDisconnectionFromMyself(int parent, u16 aid, void *mac)
{                   // req_Disconnect.c:605
    WMCallback *cb; // r4 - :608
    WMStatus *status = wmspW.status;

    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->errcode = WM_ERRCODE_SUCCESS;

    if (parent)
    {
        WMStartParentCallback *cb_Parent = (WMStartParentCallback *)cb; // r0 - :614
        cb_Parent->apiid = WM_APIID_START_PARENT;
        cb_Parent->state = WM_STATECODE_DISCONNECTED_FROM_MYSELF;
        cb_Parent->reason = 0xF001;
        cb_Parent->aid = aid;
        MI_CpuCopy8(mac, cb_Parent->macAddress, 6);
        cb_Parent->parentSize = status->mp_parentSize;
        cb_Parent->childSize = status->mp_childSize;
    }
    else
    {
        WMStartConnectCallback *cb_Child = (WMStartConnectCallback *)cb; // r0 - :625
        cb_Child->apiid = WM_APIID_START_CONNECT;
        cb_Child->state = WM_STATECODE_DISCONNECTED_FROM_MYSELF;
        cb_Child->reason = 0xF001;
        cb_Child->aid = status->aid;
        MI_CpuCopy8(mac, cb_Child->macAddress, 6);
        cb_Child->parentSize = status->mp_parentSize;
        cb_Child->childSize = status->mp_childSize;
    }

    WMSP_ReturnResult2Wm9(cb);
}

static void WmspError(u16 wlCommand, u16 wlResult, u16 tryBmp, u16 resBmp)
{                                   // req_Disconnect.c:650
    WMDisconnectCallback *callback; // r0 - :652
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_DISCONNECT;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    callback->tryBitmap = tryBmp;
    callback->disconnectedBitmap = resBmp;
    WMSP_ReturnResult2Wm9(callback);
}

static void WmspIndError(u16 wlCommand, u16 wlResult, u16 tryBmp, u16 resBmp)
{                                   // req_Disconnect.c:674
    WMDisconnectCallback *callback; // r0 - :676
    callback = WMSP_GetBuffer4Callback2Wm9();
    callback->apiid = WM_APIID_AUTO_DISCONNECT;
    callback->errcode = WM_ERRCODE_FAILED;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    callback->tryBitmap = tryBmp;
    callback->disconnectedBitmap = resBmp;
    WMSP_ReturnResult2Wm9(callback);
}