#include "Mongoose.h"

void WMSP_VAlarmSetMPData() { // valarm_SetMPData.c:29
    WMStatus* status = wmspW.status;
    u32 enabled; // r0 - :35
    
    wmspW.status->valarm_queuedFlag = 0;
    status->mp_newFrameFlag = 1;
    
    status->rssiCounter--;
    if (status->rssiCounter == 0) {
        if (status->minRssi == 0xFFFF) {
            status->minRssi = 4;
        }
        
        WMSP_AddRssiToList(status->minRssi);
        status->linkLevel = WMSP_GetAverageLinkLevel();
        status->minRssi = -1;
        status->rssiCounter = 1;
    }
    
    if (status->state == WM_STATE_MP_PARENT) {
        enabled = OS_DisableInterrupts();
        
        if (status->child_bitmap == 0) {
            status->mp_count = 0;
            OS_RestoreInterrupts(enabled);
            
        } else {
            int flag = (status->mp_count <= 0 || status->mp_limitCount <= 0);
            
            if (status->mp_count < 0) {
                status->mp_count = 0;
            }
            
            status->mp_count += status->mp_current_freq;
            
            if (status->mp_count > 256) {
                status->mp_count = 256;
            }
            
            status->mp_limitCount = status->mp_current_maxFreq;
            
            flag = flag && (status->mp_count > 0 && status->mp_limitCount > 0);
            OS_RestoreInterrupts(enabled);
            
            if (flag) {
                WMSP_SendMaMP(0xFFFF);
            }
            
            if (status->mp_current_minPollBmpMode == 1) {
                status->mp_pingCounter--;
                
                if (status->mp_pingCounter == 0) {
                    status->mp_pingFlag = 1;
                    status->mp_pingCounter = 60;
                }
            }
        }
        
    } else if (status->state == WM_STATE_MP_CHILD) {
        int kick = 0; // r5 - :129
        enabled = OS_DisableInterrupts();
        
        if (status->sendQueueInUse != 1) {
            kick = 1;
            status->mp_vsyncFlag = 0;
        }
        
        OS_RestoreInterrupts(enabled);
        if (kick == 1) {
            WMSP_SendMaKeyData();
        }
    }
    
}

void WMSP_KickNextMP_Parent(void* msg) { // valarm_SetMPData.c:173
    u32* reqBuf = (u32*)msg; // r0 - :177
    
    if (wmspW.status->state == WM_STATE_MP_PARENT) {
        WMSP_SendMaMP(reqBuf[1]);
    }
}

void WMSP_KickNextMP_Child() { // valarm_SetMPData.c:201
    if (wmspW.status->state == WM_STATE_MP_CHILD) {
        WMSP_SendMaKeyData();
    }
}

void WMSP_KickNextMP_Resume(void* msg) { // valarm_SetMPData.c:230
    u32* reqBuf = (u32*)msg; // r0 - :236
    
    if (wmspW.status->state == WM_STATE_MP_PARENT) {
        WMSP_ResumeMaMP(reqBuf[1]);
    }
}