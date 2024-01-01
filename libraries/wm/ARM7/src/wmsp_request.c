#include "Mongoose.h"

static void WmspRequestFuncDummy();

static void (*WmspRequestFuncTable[46])(void*) = {
    WMSP_Initialize, WMSP_Reset, WMSP_End, WMSP_Enable,
    WMSP_Disable, WMSP_PowerOn, WMSP_PowerOff, WMSP_SetParentParam,
    WMSP_StartParent, WMSP_EndParent, WMSP_StartScan, WMSP_EndScan,
    WMSP_StartConnectEx, WMSP_Disconnect, WMSP_StartMP,
    WMSP_SetMPData, WMSP_EndMP, WMSP_StartDCF, WMSP_SetDCFData,
    WMSP_EndDCF, WMSP_SetWEPKey, WmspRequestFuncDummy,
    WmspRequestFuncDummy, WmspRequestFuncDummy, WMSP_SetGameInfo,
    WMSP_SetBeaconTxRxInd, WMSP_StartTestMode, WMSP_StopTestMode,
    WMSP_VAlarmSetMPData, WMSP_SetLifeTime, WMSP_MeasureChannel,
    WMSP_InitWirelessCounter, WMSP_GetWirelessCounter,
    WMSP_SetEntry, WMSP_AutoDeAuth, WMSP_SetMPParameter,
    WMSP_SetBeaconPeriod, WMSP_AutoDisconnect, WMSP_StartScanEx,
    WMSP_SetWEPKeyEx, WMSP_SetPowerSaveMode, WMSP_StartTestRxMode,
    WMSP_StopTestRxMode, WMSP_KickNextMP_Parent, WMSP_KickNextMP_Child,
    WMSP_KickNextMP_Resume
}; // :30

void WMSP_RequestThread() { // wmsp_request.c:89
    struct WMSPWork* p; // r0 - :93
    struct WMStatus* status; // r9 - :94
    void* msg; // None - :95
    u16 apiid; // r10 - :96
}

static void WmspRequestFuncDummy() { // wmsp_request.c:145
}