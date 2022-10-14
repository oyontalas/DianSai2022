#ifndef __FLASH_H__
#define __FLASH_H__	 
#include "main.h"
HAL_StatusTypeDef flash_write(uint64_t address,uint8_t bank, uint64_t data);
void flash_page_erase(uint32_t page,uint8_t BANK);
void flash_read(uint64_t address,uint8_t BANK, uint64_t *data);	
void Flash_ReadData(uint32_t ReadAddr,uint64_t *pBuffer,uint16_t NumToRead);
void Flash_WriteData(uint32_t addr,uint64_t *data,uint16_t Size);
#endif