#define MARIONEA_INTERNAL 1
#include "Flash.h"

u32 FLASH_VerifyCheckSum(u32* pCrc) { // Flash.c:45
    u8* p; // r0 - :47
    u32 crc; // r5 - :48
    u32 rcrc; // r1 - :48
    u32 tbsize; // r6 - :48
    
    tbsize = *((u16*)wlMan->pFlashImg + 1); // :50
    
    if (tbsize < 0xA4 || tbsize > 0x1D6)
        return 2;
    
    p = (u8*)wlMan->pFlashImg + 2; // :58
    crc = 0; // :59
    
    for (; tbsize > 0; tbsize--) { // :61
        // TODO seems like there could be an extra variable that's missing in DWARF?
        // could it be from an inline method?
        crc = calc_NextCRC(WL_ReadByte(p++) & 0xFF, crc); // :64
    }
    
    rcrc = *(u16*)wlMan->pFlashImg;
    if (pCrc)
        *pCrc = rcrc | (crc << 16);
    
    return crc != rcrc;
}

void FLASH_Wait() { // Flash.c:96
    volatile u32 statusReg; // None - :98
    do {
        while (1) {
            NVRAM_ReadStatusRegister((u8*)&statusReg);
            if ((statusReg & 0x20) == 0)
                break;
            
            NVRAM_SoftwareReset();
        }
        
    } while ((statusReg & 0x01) != 0);
}

void FLASH_Read(u32 adrs, u32 size, u8* pBuf) { // Flash.c:128
    u8* p; // r0 - :139
    if (wlMan->pFlashImg) {
        p = (u8*)wlMan->pFlashImg + adrs - 0x2A;
        while (size) {
            WL_WriteByte(pBuf++, WL_ReadByte(p++));
            size--;
        }
    }
}

void FLASH_DirectRead(u32 adrs, u32 size, u8* pBuf) { // Flash.c:183
    SPI_Lock(wlMan->lockID);
    FLASH_Wait();
    NVRAM_ReadDataBytes(adrs, size, pBuf);
    SPI_Unlock(wlMan->lockID);
}

u32 FLASH_MakeImage() { // Flash.c:206
    u32 length; // None - :208
    
    SPI_Lock(wlMan->lockID);
    FLASH_Wait();
    
    length = 0;
    NVRAM_ReadDataBytes(0x2C, 2, (u8*)&length);
    SPI_Unlock(wlMan->lockID);
    
    if (length < 0xA4 || length > 0x1D6)
        return 0;
    
    length += 2;
    
    wlMan->pFlashImg = AllocateHeapBuf(&wlMan->HeapMan.TmpBuf, length);
    if (!wlMan->pFlashImg)
        return 0;
    
    wlMan->pFlashImg = (void*)((u8*)wlMan->pFlashImg + 12);
    
    SPI_Lock(wlMan->lockID);
    FLASH_Wait();
    NVRAM_ReadDataBytes(0x2A, length, (u8*)wlMan->pFlashImg);
    SPI_Unlock(wlMan->lockID);
    return 1;
}