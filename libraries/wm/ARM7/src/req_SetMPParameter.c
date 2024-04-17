#include "Mongoose.h"

void WMSP_SetMPParameter(void* msg) { // req_SetMPParameter.c:28
    struct WMMPParam* param; // r0 - :33
    struct WMMPParam old_param; // None - :34
    enum WMErrCode result; // r0 - :36
    struct WMSetMPParameterCallback* cb; // r0 - :45
    
    param = (WMMPParam*)((u32)msg + 4);
    result = WMSP_SetMPParameterCore(param, &old_param);
    
    cb = WMSP_GetBuffer4Callback2Wm9();
    cb->apiid = 35;
    cb->errcode = result;
    cb->mask = param->mask;
    MI_CpuCopy8(&old_param, &cb->oldParam, sizeof(struct WMMPParam));
    WMSP_ReturnResult2Wm9(cb);
}

enum WMErrCode WMSP_SetMPParameterCore(struct WMMPParam* param, struct WMMPParam* old_param) { // req_SetMPParameter.c:65
    struct WMStatus* status = wmspW.status;
    u32 enabled; // r4 - :68
    enum WMErrCode result = WM_ERRCODE_SUCCESS; // r5 - :70
    u32 mask = param->mask; // r6 - :71
    
    if ((u16)(status->state + 0xFFF7) <= 1 && (mask & 0x2C00) != 0) {
        mask &= ~0x2C00;
        result = WM_ERRCODE_ILLEGAL_STATE;
    }
    
    enabled = OS_DisableInterrupts();
    if (old_param) {
        old_param->mask = 0x3FFF;
        old_param->minFrequency = status->mp_minFreq;
        old_param->frequency = status->mp_freq;
        old_param->maxFrequency = status->mp_maxFreq;
        old_param->parentSize = status->mp_parentSize;
        old_param->childSize = status->mp_childSize;
        old_param->parentInterval = status->mp_parentInterval;
        old_param->childInterval = status->mp_childInterval;
        old_param->parentVCount = status->mp_parentVCount;
        old_param->childVCount = status->mp_childVCount;
        old_param->defaultRetryCount = status->mp_defaultRetryCount;
        old_param->minPollBmpMode = status->mp_minPollBmpMode;
        old_param->singlePacketMode = status->mp_singlePacketMode;
        old_param->ignoreFatalErrorMode = status->mp_ignoreFatalErrorMode;
        old_param->ignoreSizePrecheckMode = status->mp_ignoreSizePrecheckMode;
    }
    
    if ((mask & 1) != 0) {
        u16 freq = param->minFrequency; // r1 - :117
        if (freq == 0)
            freq = 16;
        
        status->mp_minFreq = freq;
        status->mp_current_minFreq = freq;
    }
    
    if ((mask & 2) != 0) {
        u16 freq = param->frequency; // r1 - :131
        if (freq == 0)
            freq = 16;
        
        status->mp_freq = freq;
        status->mp_current_freq = freq;
        if (status->mp_count > freq)
            status->mp_count = freq;
    }
    
    if ((mask & 4) != 0) {
        u16 freq = param->maxFrequency; // r1 - :149
        if (freq == 0)
            freq = 16;
        
        status->mp_minFreq = freq;
        status->mp_current_minFreq = freq;
        if (status->mp_count > freq)
            status->mp_count = freq;
    }
    
    if ((mask & 8) != 0) {
        u16 parentSize = param->parentSize;
        if ((u16)((parentSize + 1) & ~1) > status->mp_parentMaxSize) {
            result = WM_ERRCODE_INVALID_PARAM;
            
        } else {
            WMSP_SetParentSize(parentSize);
        }
    }
    
    if ((mask & 0x10) != 0) {
        u16 childSize = param->childSize;
        if ((u16)((childSize + 1) & ~1) > 0x200) {
            result = WM_ERRCODE_INVALID_PARAM;
            
        } else {
            WMSP_SetChildSize(childSize);
        }
    }
    
    if ((mask & 0x20) != 0) {
        u16 parentInterval = param->parentInterval;
        if (parentInterval > 10000) {
            result = WM_ERRCODE_INVALID_PARAM;
            
        } else {
            status->mp_parentInterval = parentInterval;
            status->mp_parentIntervalTick = (33514 * (u64)parentInterval) / 64 / 1024;
        }
    }
    
    if ((mask & 0x40) != 0) {
        u16 childInterval = param->childInterval;
        if (childInterval > 10000) {
            result = WM_ERRCODE_INVALID_PARAM;
            
        } else {
            status->mp_childInterval = childInterval;
            status->mp_childIntervalTick = (33514 * (u64)childInterval) / 64 / 1024;
        }
    }
    
    if ((mask & 0x80) != 0) {
        u16 parentVCount = param->parentVCount;
        if (parentVCount <= 190 || (parentVCount >= 220 && parentVCount <= 262)) {
            status->mp_parentVCount = parentVCount;
            
        } else {
            result = WM_ERRCODE_INVALID_PARAM;
        }
    }
    
    if ((mask & 0x100) != 0) {
        u16 childVCount = param->childVCount;
        if (childVCount <= 190 || (childVCount >= 220 && childVCount <= 262)) {
            status->mp_childVCount = childVCount;
            
        } else {
            result = WM_ERRCODE_INVALID_PARAM;
        }
    }
    
    if ((mask & 0x200) != 0) {
        u16 defaultRetryCount = param->defaultRetryCount;
        status->mp_defaultRetryCount = defaultRetryCount;
        status->mp_current_defaultRetryCount = defaultRetryCount;
    }
    
    if ((mask & 0x400) != 0) {
        status->mp_minPollBmpMode = param->minPollBmpMode;
    }
    
    if ((mask & 0x800) != 0) {
        status->mp_singlePacketMode = param->singlePacketMode;
    }
    
    if ((mask & 0x1000) != 0) {
        status->mp_ignoreFatalErrorMode = param->ignoreFatalErrorMode;
        status->mp_current_ignoreFatalErrorMode = status->mp_ignoreFatalErrorMode;
    }
    
    if ((mask & 0x2000) != 0) {
        status->mp_ignoreSizePrecheckMode = param->ignoreSizePrecheckMode;
    }
    
    OS_RestoreInterrupts(enabled);
    return result;
}