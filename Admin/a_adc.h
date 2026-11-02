#ifndef _A_ADC_H_
#define _A_ADC_H_

#include "a_include.h"

// 同时作为 DMA 采样点数和 FFT 长度；修改时需满足 radix-2 FFT 支持的长度，
// 并检查主循环局部数组、FFT 临时数组带来的栈和堆占用。
#define ADC_BUFFER_SIZE ((uint32_t)512)

// DMA 循环覆盖该数组，保存 12 位 ADC 原始码值，尚未换算为电压。
extern uint16_t ADC_Buffer[ADC_BUFFER_SIZE];
extern uint16_t ADC_Value;
extern uint8_t ADC_GetTemp_En;
extern uint8_t ADC_GetTemp_Show_En;

extern float ADC_Temp;

void ADC_GetTemp(void);

#endif
