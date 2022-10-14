#include "a_adc.h"



uint16_t   ADC_Buffer[ADC_BUFFER_SIZE];
uint16_t ADC_Value=0;
uint8_t ADC_GetTemp_En=0;
uint8_t ADC_GetTemp_Show_En=0;
float ADC_Temp=0.0;





void ADC_GetTemp(void){
	// HAL_ADC_Start(&hadc5);	//启动ADC转换
	// HAL_ADC_PollForConversion(&hadc5,10);	//等待转换完成，10ms表示超时时间
	// AD_Value = HAL_ADC_GetValue(&hadc5);	//读取ADC转换数据（12位数据）

	// double adcx = 80.0/(TEMP110_CAL_ADDR- TEMP30_CAL_ADDR);
	// double temp = adcx*((ADC_Value[0]+ADC_Value[1]+ADC_Value[2]+ADC_Value[3]+ADC_Value[4])/5.0*3.0 - TEMP30_CAL_ADDR)+30;
	
    // uint16_t ad1 = ADC_Value[0];					//Tem温度值处理与读取//推荐设置25.4采样周期
	// uint16_t vdda_mV=__HAL_ADC_CALC_VREFANALOG_VOLTAGE(ADC_Value[2],ADC_RESOLUTION_12B);//需要首先获得VDD ADC参考电压用于后续Tem温度的计算
	// ADC_Temp = __HAL_ADC_CALC_TEMPERATURE(vdda_mV,ad1,ADC_RESOLUTION_12B);//使用VDD值以及读取的Tem原始值，计算得到实际温度值。
		
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_Value,1);
	HAL_Delay(50);    //必须加的延时，等待DMA给数组传输数据后再显示
	ADC_Temp = (ADC_Value*3300/4096-760)/2.5+25;
}