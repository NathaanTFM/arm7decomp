#include "Marionea.h"

#ifndef FLASH_H
#define FLASH_H

#ifdef MARIONEA_INTERNAL
u32 FLASH_VerifyCheckSum(u32 *pCrc);
void FLASH_Wait();
void FLASH_Read(u32 adrs, u32 size, u8 *pBuf);
void FLASH_DirectRead(u32 adrs, u32 size, u8 *pBuf);
u32 FLASH_MakeImage();
#endif

#endif