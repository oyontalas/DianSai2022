#include "a_DSP.h"

// FFT变换，返回频率模值
// 输入参数：输入数据，输出数据，数据长度
// 输出参数：无
// 返回参数：无

arm_cfft_radix2_instance_f32 S_FFT;
arm_fir_instance_f32 S_FIR;

void FFT_GetValuesModulus(float32_t *InputValues, float32_t *OutputValues, uint16_t Length)
{
	uint16_t i;
	float32_t *ComplexInputValues = (float32_t *)malloc(Length * 2 * sizeof(float32_t));

	if (ComplexInputValues != NULL)
	{
		for (i = 0; i < Length; i++)
		{
			ComplexInputValues[2 * i] = InputValues[i];
			ComplexInputValues[2 * i + 1] = 0;
		}
		arm_cfft_radix2_init_f32(&S_FFT, Length, 0, 1);
		arm_cfft_radix2_f32(&S_FFT, ComplexInputValues);			 // FFT变换
		arm_cmplx_mag_f32(ComplexInputValues, OutputValues, Length); //求复数模
		OutputValues[0] /= Length;									 //直流分量除以采样点数
		for (i = 0; i < Length; i++)
			OutputValues[i] /= (Length / 2); //其他分量除以（采样点数/2）
		free(ComplexInputValues);
	}
	else
	{
		while (1)
			;
	}
}

#if FIR_EN
void FIR(float32_t *InputValues, float32_t *OutputValues, uint16_t Length)
{
	// uint32_t timeTick = HAL_GetTick();		//函数进入时间戳
	// uint32_t timePass = 0;					//计算用时
	// uint32_t blockSize = 128;				//块长度，原始数据长度为1280
	// uint32_t numOfBlock = 1280 / blockSize;	//
	uint16_t i;
	float32_t *pState = (float32_t *)malloc((Length + BL - 1) * sizeof(float32_t));

	if (pState != NULL)
	{
		arm_fir_init_f32(&S_FIR, BL, (float32_t *)B, pState, Length);
		arm_fir_f32(&S_FIR, InputValues, OutputValues, Length);
		free(pState);
	}
	else
	{
		while (1)
			;
	}

	// timePass = HAL_GetTick() - timeTick;
}
#endif