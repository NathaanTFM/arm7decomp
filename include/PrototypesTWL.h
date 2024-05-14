int WMSP_CheckInitialized();                // wmsp_common.c:30
int NWMSP_CheckInitialized();               // wmsp_common.c:44
void WMSPi_SwitchModule(WmspModule module); // wmsp_common.h:61

int WMSPi_CheckInitialized(); // wmsp_system.c:1167

void NWMSP_Init(NwmspInit *nwmInit);      // nwmsp_system.c:37
void *NWMSP_GetBuffer4Callback2Nwm9();    // nwmsp_system.c:221
void NWMSP_ReturnResult2Nwm9(void *ptr);  // nwmsp_system.c:253
void NWMSP_SetAPIActiveFlag(u16 apiid);   // nwmsp_system.c:268
void NWMSP_ClearAPIActiveFlag(u16 apiid); // nwmsp_system.c:281
int NWMSP_IsAPIActive(u16 apiid);         // nwmsp_system.c:295
int NWMSPi_CheckInitialized();            // nwmsp_system.c:311