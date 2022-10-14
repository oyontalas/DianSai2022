#ifndef _A_ADC_H_
#define _A_ADC_H_

#include "a_include.h"

#define ADC_BUFFER_SIZE ((uint32_t)512)

extern uint16_t ADC_Buffer[ADC_BUFFER_SIZE];
extern uint16_t ADC_Value;
extern uint8_t ADC_GetTemp_En;
extern uint8_t ADC_GetTemp_Show_En;

extern float ADC_Temp;

void ADC_GetTemp(void);

#endif