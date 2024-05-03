#include "Mongoose.h"

static int IsIncludeValidSSID(WMBssDesc *bssDesc);
static u32 WmspGetScanExBufSize4WL(u16 arm9_BufSize);
static void WmspError(u16 wlCommand, u16 wlResult, int exFlag);

void WMSP_StartScan(void *msg)
{                                    // req_StartScan.c:44
    u32 wlBuf[128];                  // None - :47
    u16 *buf = (u16 *)wlBuf;         // :48
    WMStatus *status = wmspW.status; // :50
    u16 scanChannel;                 // r8 - :54
    u16 scanBssid[3];                // None - :55
    u16 scanMaxChannelTime;          // r9 - :56

    WMStartScanReq *args = (WMStartScanReq *)msg; // r0 - :59

    if (status->state != WM_STATE_IDLE && status->state != WM_STATE_CLASS1 && status->state != WM_STATE_SCAN)
    {
        WMStartScanCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :65
        callback->apiid = WM_APIID_START_SCAN;
        callback->errcode = WM_ERRCODE_ILLEGAL_STATE;
        callback->state = WM_STATECODE_PARENT_NOT_FOUND;
        WMSP_ReturnResult2Wm9(callback);
        return;
    }

    wmspW.status->pInfoBuf = args->scanBuf;
    status->scan_channel = scanChannel = args->channel;
    scanMaxChannelTime = args->maxChannelTime;
    MI_CpuCopy8(args->bssid, scanBssid, sizeof(scanBssid));

    if (scanBssid[0] != 0xFFFF && (scanBssid[0] & 1) != 0)
    {
        scanBssid[0] &= ~1;
    }

    if (scanChannel == 0)
    {
        WMStartScanCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :96
        callback->apiid = WM_APIID_START_SCAN;
        callback->errcode = WM_ERRCODE_INVALID_PARAM;
        callback->state = WM_STATECODE_PARENT_NOT_FOUND;
        WMSP_ReturnResult2Wm9(callback);
        return;
    }

    if ((status->enableChannel & (1 << scanChannel)) == 0)
    {
        WMStartScanCallback *cb = WMSP_GetBuffer4Callback2Wm9(); // r0 - :107
        cb->apiid = WM_APIID_START_SCAN;
        cb->errcode = WM_ERRCODE_INVALID_PARAM;
        cb->state = WM_STATECODE_PARENT_NOT_FOUND;
        WMSP_ReturnResult2Wm9(cb);
        return;
    }

    status->mode = MODE_CHILD;

    {
        WlDevGetStationStateCfm *p_confirm; // r0 - :120
        p_confirm = WMSP_WL_DevGetStationState(buf);

        if (p_confirm->resultCode != 0)
        {
            WmspError(DEV_GET_STATE_REQ_CMD, p_confirm->resultCode, 0);
            return;
        }

        if (p_confirm->state == STA_IDLE)
        {
            if (!WMSP_SetAllParams(WM_APIID_START_SCAN, buf))
                return;

            {
                WlDevClass1Cfm *p_confirm; // r0 - :139
                p_confirm = WMSP_WL_DevClass1(buf);

                if (p_confirm->resultCode != 0)
                {
                    WmspError(DEV_CLASS1_REQ_CMD, p_confirm->resultCode, 0);
                    return;
                }

                status->state = WM_STATE_CLASS1;
            }

            {
                WlMlmePowerMgtCfm *p_confirm; // r0 - :151
                p_confirm = WMSP_WL_MlmePowerManagement(buf, 1, 0, 1);

                if (p_confirm->resultCode != 0)
                {
                    WmspError(MLME_PWR_MGT_REQ_CMD, p_confirm->resultCode, 0);
                    return;
                }

                status->pwrMgtMode = 1;
            }
        }
    }

    status->state = WM_STATE_SCAN;

    {
        WlMlmeScanCfm *p_confirm; // r0 - :178
        u16 ssid[16];             // None - :183
        u8 channelList[16];       // None - :185

        MIi_CpuClear16(0xFFFF, ssid, sizeof(ssid));

        channelList[0] = scanChannel;
        MI_CpuFill8(&channelList[1], 0, sizeof(channelList) - 1);

        p_confirm = WMSP_WL_MlmeScan(
            buf,
            0x11E, /* ?? */
            scanBssid,
            0,
            (u8 *)ssid,
            1,
            (u8 *)channelList,
            scanMaxChannelTime);

        if (p_confirm->resultCode != 0)
        {
            WmspError(MLME_SCAN_REQ_CMD, p_confirm->resultCode, 0);
            return;
        }
        else
        {
            WMStartScanCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r9 - :237

            if (p_confirm->bssDescCount == 0)
            {
                callback->apiid = WM_APIID_START_SCAN;
                callback->errcode = WM_ERRCODE_SUCCESS;
                callback->state = WM_STATECODE_PARENT_NOT_FOUND;
                callback->channel = scanChannel;
                callback->linkLevel = 0;
            }
            else
            {
                MIi_CpuClear16(0, &status->pInfoBuf->gameInfo, 0x80);
                MI_CpuCopy8(&p_confirm->bssDescList[0], status->pInfoBuf, 2 * p_confirm->bssDescList[0].length);

                callback->apiid = WM_APIID_START_SCAN;
                callback->errcode = WM_ERRCODE_SUCCESS;
                callback->state = WM_STATECODE_PARENT_FOUND;
                callback->channel = p_confirm->bssDescList[0].channel;

                {
                    u8 tempRssi; // r0 - :261
                    tempRssi = WMSP_GetRssi8(p_confirm->bssDescList[0].rssi);
                    callback->linkLevel = WMSP_GetLinkLevel(tempRssi);
                    WMSP_AddRssiToRandomPool(tempRssi);
                }

                callback->ssidLength = p_confirm->bssDescList[0].ssidLength;
                MI_CpuCopy8(p_confirm->bssDescList[0].bssid, callback->macAddress, 6);
                MIi_CpuCopy16(p_confirm->bssDescList[0].ssid, callback->ssid, 0x20);
                callback->gameInfoLength = p_confirm->bssDescList[0].gameInfoLength;

                if (callback->gameInfoLength > 0x80)
                {
                    callback->apiid = WM_APIID_START_SCAN;
                    callback->errcode = WM_ERRCODE_SUCCESS;
                    callback->state = WM_STATECODE_PARENT_NOT_FOUND;
                    callback->channel = scanChannel;
                    callback->linkLevel = 0;
                }
                else
                {
                    MIi_CpuClear16(0, &callback->gameInfo, 0x80);
                    MIi_CpuCopy16(&p_confirm->bssDescList[0].gameInfo, &callback->gameInfo, (callback->gameInfoLength + 1) & ~1);
                }
            }

            WMSP_ReturnResult2Wm9(callback);
        }
    }
}

void WMSP_StartScanEx(void *msg)
{                                    // req_StartScan.c:307
    u32 wlBuf[278];                  // None - :310
    u16 *buf = (u16 *)wlBuf;         // :312
    WMStatus *status = wmspW.status; // :314
    u16 scanChannelList;             // r0 - :318
    u16 scanBssid[3];                // None - :319
    u16 scanMaxChannelTime;          // None - :320
    u16 scanType;                    // r5 - :321
    u16 ssidMatchLength;             // r11 - :323
    int ssidMaskCustomize;           // r6 - :324
    u16 scanSsidLength;              // r4 - :326
    u8 scanSsid[32];                 // None - :327
    u16 scanBufSize;                 // r8 - :328

    WMStartScanExReq *args = (WMStartScanExReq *)msg; // r0 - :331

    if (status->state != WM_STATE_IDLE && status->state != WM_STATE_CLASS1 && status->state != WM_STATE_SCAN)
    {
        WMStartScanExCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :337
        callback->apiid = WM_APIID_START_SCAN_EX;
        callback->errcode = WM_ERRCODE_ILLEGAL_STATE;
        callback->state = WM_STATECODE_PARENT_NOT_FOUND; // ???
        WMSP_ReturnResult2Wm9(callback);
        return;
    }

    status->pInfoBuf = args->scanBuf;
    status->scan_channel = scanChannelList = args->channelList;
    scanMaxChannelTime = args->maxChannelTime;
    MI_CpuCopy8(args->bssid, scanBssid, sizeof(scanBssid));
    scanType = args->scanType;
    ssidMatchLength = args->ssidMatchLength;

    switch (scanType)
    {
    case 2:
        ssidMaskCustomize = 1;
        scanType = 0;
        break;
    case 3:
        ssidMaskCustomize = 1;
        scanType = 1;
        break;
    default:
        ssidMaskCustomize = 0;
    }

    scanSsidLength = args->ssidLength;
    MI_CpuCopy8(args->ssid, scanSsid, sizeof(scanSsid));

    scanBufSize = args->scanBufSize;

    if (scanBssid[0] != 0xFFFF && (scanBssid[0] & 1) != 0)
    {
        scanBssid[0] &= ~1;
    }

    scanChannelList = (scanChannelList << 1);
    scanChannelList &= status->enableChannel;

    if (scanChannelList == 0 ||
        (((status->miscFlags & 1) != 0) && scanType != 1) ||
        args->scanBuf == 0 ||
        ((u32)args->scanBuf & 3) != 0 ||
        args->scanBufSize < 0x40)
    {
        WMStartScanExCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r0 - :397
        callback->apiid = WM_APIID_START_SCAN_EX;
        callback->errcode = WM_ERRCODE_INVALID_PARAM;
        callback->state = WM_STATECODE_PARENT_NOT_FOUND; // ???
        WMSP_ReturnResult2Wm9(callback);
        return;
    }

    status->mode = MODE_CHILD;

    {
        WlDevGetStationStateCfm *p_confirm = WMSP_WL_DevGetStationState(buf); // r0 - :410

        if (p_confirm->resultCode != 0)
        {
            WmspError(DEV_GET_STATE_REQ_CMD, p_confirm->resultCode, 1);
            return;
        }

        if (p_confirm->state == STA_IDLE)
        {
            if (!WMSP_SetAllParams(WM_APIID_START_SCAN_EX, buf))
                return;

            {
                WlDevClass1Cfm *p_confirm; // r0 - :429
                p_confirm = WMSP_WL_DevClass1(buf);

                if (p_confirm->resultCode != 0)
                {
                    WmspError(DEV_CLASS1_REQ_CMD, p_confirm->resultCode, 1);
                    return;
                }

                status->state = WM_STATE_CLASS1;
            }

            {
                WlMlmePowerMgtCfm *p_confirm; // r0 - :441
                p_confirm = WMSP_WL_MlmePowerManagement(buf, 1, 0, 1);

                if (p_confirm->resultCode != 0)
                {
                    WmspError(MLME_PWR_MGT_REQ_CMD, p_confirm->resultCode, 1);
                    return;
                }

                status->pwrMgtMode = 1;
            }
        }
    }

    if (scanType == 0)
    {
        if (status->preamble == 1)
        {
            WlParamSetCfm *p_confirm; // r0 - :469
            p_confirm = WMSP_WL_ParamSetPreambleType(buf, 0);

            if (p_confirm->resultCode != 0)
            {
                WmspError(PARAMSET_PREAMBLE_TYPE_REQ_CMD, p_confirm->resultCode, 1);
                return;
            }

            status->preamble = 0;
        }
    }
    else
    {
        if (status->preamble == 0)
        {
            WlParamSetCfm *p_confirm; // r0 - :485
            p_confirm = WMSP_WL_ParamSetPreambleType(buf, 1);

            if (p_confirm->resultCode != 0)
            {
                WmspError(PARAMSET_PREAMBLE_TYPE_REQ_CMD, p_confirm->resultCode, 1);
                return;
            }

            status->preamble = 1;
        }
    }

    if (ssidMaskCustomize == 1)
    {
        u8 mask[32];              // None - :501
        WlParamSetCfm *p_confirm; // r0 - :502

        MI_CpuFill8(mask, 0xFF, sizeof(mask));

        if (ssidMatchLength <= sizeof(mask))
        {
            MI_CpuFill8(mask, 0, ssidMatchLength);
        }

        p_confirm = WMSP_WL_ParamSetSsidMask(buf, mask);

        if (p_confirm->resultCode != 0)
        {
            WmspError(PARAMSET_SSID_MASK_REQ_CMD, p_confirm->resultCode, 1);
            return;
        }
    }

    status->state = WM_STATE_SCAN; // :519?

    {
        WlMlmeScanCfm *p_confirm; // r5 - :523
        u32 bufSize;              // in older version
        u8 channelList[16];       // None - :527
        u16 i, j;                 // r1, r9 - :528

        j = 0;
        MI_CpuFill8(channelList, 0, sizeof(channelList));

        for (i = 1; i < 15; i++)
        {
            if ((scanChannelList & (1 << i)) != 0)
            {
                channelList[j] = i;
                j++;
            }
        }

        bufSize = WmspGetScanExBufSize4WL(scanBufSize);

        p_confirm = WMSP_WL_MlmeScan(
            buf,
            bufSize,
            scanBssid,
            scanSsidLength,
            scanSsid,
            scanType,
            (u8 *)channelList,
            scanMaxChannelTime);

        if (p_confirm->resultCode != 0)
        {
            WmspError(MLME_SCAN_REQ_CMD, p_confirm->resultCode, 1);
            return;
        }
        else
        {
            WMStartScanExCallback *callback = WMSP_GetBuffer4Callback2Wm9(); // r6 - :561

            if (p_confirm->bssDescCount == 0)
            {
                callback->apiid = WM_APIID_START_SCAN_EX;
                callback->errcode = WM_ERRCODE_SUCCESS;
                callback->state = WM_STATECODE_PARENT_NOT_FOUND;
                callback->bssDescCount = 0;
                callback->channelList = (scanChannelList >> 1);
            }
            else
            {
                int i;                 // r7 - :575
                WMBssDesc *src, *dest; // r8, r9 - :576

                src = (WMBssDesc *)p_confirm->bssDescList;
                dest = status->pInfoBuf;

                MIi_CpuClear16(0, dest, scanBufSize);

                for (i = 0; i < p_confirm->bssDescCount; i++)
                {
                    u16 length_byte; // r11 - :586

                    length_byte = (2 * src->length);
                    MI_CpuCopy8(src, dest, length_byte);

                    if (scanSsidLength != 0 && !IsIncludeValidSSID(dest))
                    {
                        dest->ssidLength = scanSsidLength;
                        MI_CpuCopy8(scanSsid, dest->ssid, sizeof(scanSsid));
                    }

                    callback->bssDesc[i] = dest;

                    {
                        u8 tempRssi; // r0 - :608
                        tempRssi = WMSP_GetRssi8(src->rssi);
                        callback->linkLevel[i] = WMSP_GetLinkLevel(tempRssi);
                        WMSP_AddRssiToRandomPool(tempRssi);
                    }

                    src = (WMBssDesc *)((u32)src + length_byte);
                    dest = (WMBssDesc *)((u32)dest + length_byte);

                    if (((u32)dest & 2) != 0)
                        dest = (WMBssDesc *)(((u32)dest + 2) & ~3);
                }

                callback->apiid = WM_APIID_START_SCAN_EX;
                callback->errcode = WM_ERRCODE_SUCCESS;
                callback->state = WM_STATECODE_PARENT_FOUND;
                callback->bssDescCount = p_confirm->bssDescCount;
                callback->channelList = (scanChannelList >> 1);
            }

            WMSP_ReturnResult2Wm9(callback);
        }
    }
}

static int IsIncludeValidSSID(WMBssDesc *bssDesc)
{
    long i;

    if (bssDesc->ssidLength == 0)
        return 0;

    if (bssDesc->ssidLength > 0x20)
        return 0;

    for (i = 0; i < bssDesc->ssidLength; i++)
    {
        if (bssDesc->ssid[i] != 0)
            return 1;
    }

    return 0;
}

static u32 WmspGetScanExBufSize4WL(u16 arm9_BufSize)
{
    return arm9_BufSize - 2 * ((arm9_BufSize - 64) / 66) + 94;
}

static void WmspError(u16 wlCommand, u16 wlResult, int exFlag)
{                                  // req_StartScan.c:713
    WMStartScanCallback *callback; // r0 - :715
    callback = WMSP_GetBuffer4Callback2Wm9();
    if (exFlag)
    {
        callback->apiid = WM_APIID_START_SCAN_EX;
    }
    else
    {
        callback->apiid = WM_APIID_START_SCAN;
    }
    callback->errcode = WM_ERRCODE_FAILED;
    callback->state = WM_STATECODE_PARENT_NOT_FOUND;
    callback->wlCmdID = wlCommand;
    callback->wlResult = wlResult;
    WMSP_ReturnResult2Wm9(callback);
}