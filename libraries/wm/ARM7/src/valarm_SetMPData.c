#include "Mongoose.h"

void WMSP_VAlarmSetMPData() { // valarm_SetMPData.c:29
    u32 enabled; // r0 - :35
    int kick; // r5 - :129
}

void WMSP_KickNextMP_Parent(void* msg) { // valarm_SetMPData.c:173
    u32* reqBuf = (u32*)msg; // r0 - :177
    
    if (wmspW.status->state == 9) {
        WMSP_SendMaMP(reqBuf[1]);
    }
}

void WMSP_KickNextMP_Child() { // valarm_SetMPData.c:201
    if (wmspW.status->state == 10) {
        WMSP_SendMaKeyData();
    }
}

void WMSP_KickNextMP_Resume(void* msg) { // valarm_SetMPData.c:230
    u32* reqBuf = (u32*)msg; // r0 - :236
    
    if (wmspW.status->state == 9) {
        WMSP_ResumeMaMP(reqBuf[1]);
    }
}