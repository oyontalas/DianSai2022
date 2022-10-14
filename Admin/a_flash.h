#ifndef _A_FLASH_H_
#define _A_FLASH_H_

#include "a_include.h"
#include "Flash.h"

#define PARA_ADDR 0x08008000

// void FLASH_save_10_data(float value0,RTC_TimeTypeDef RTC_Time);
extern uint64_t FLASH_read_data[4];
float FLASH_read(uint8_t record_num);

extern uint8_t FLASH_uart_tx_En[10];
void FLASH_uart_tx();

#endif