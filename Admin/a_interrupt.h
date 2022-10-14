#ifndef _A_INTERRUPT_H_
#define _A_INTERRUPT_H_

#include "a_include.h"
#include "a_key.h"
#include "a_rtc.h"
#include "a_adc.h"
#include "a_flash.h"




#define getStr(key)     cJSON_GetStringValue(cJSON_GetObjectItem(root, key))
#define getNum(key)     cJSON_GetNumberValue(cJSON_GetObjectItem(root, key))
#define is(cmd, str)    strcmp(cmd, str) == 0


// 串口中断
extern uint8_t Uart_Rx_byte;
extern uint8_t Uart_Rx_bytes[128];
extern uint16_t Uart_Rx_size;
extern cJSON        *root;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart);

extern uint8_t ADC_Flag;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);





#endif