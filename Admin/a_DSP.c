#include "a_DSP.h"

arm_cfft_radix2_instance_f32 S_FFT;
arm_fir_instance_f32 S_FIR;

/*
 * 将 Length 个实数采样点转换为频谱幅值，结果写入 OutputValues。
 * 两个数组均需容纳 Length 个 float32_t；Length 必须为库支持的 FFT 长度。
 * 第 k 个输出对应 k*Fs/Length；本函数不负责加窗、去直流或频率估计。
 * FFT 在临时复数数组内原地计算，保留 InputValues 供时域绘图使用。
 */
void FFT_GetValuesModulus(float32_t *InputValues, float32_t *OutputValues, uint16_t Length)
{
	uint16_t i;
	// CMSIS 复数输入按 [实部0, 虚部0, 实部1, 虚部1, ...] 交错存放。
	float32_t *ComplexInputValues = (float32_t *)malloc(Length * 2 * sizeof(float32_t));

	if (ComplexInputValues != NULL)
	{
		for (i = 0; i < Length; i++)
		{
			ComplexInputValues[2 * i] = InputValues[i];
			ComplexInputValues[2 * i + 1] = 0;
		}
		// 参数 0 选择正变换，1 启用位反转排序，使输出频点按自然顺序排列。
		arm_cfft_radix2_init_f32(&S_FFT, Length, 0, 1);
		arm_cfft_radix2_f32(&S_FFT, ComplexInputValues);			 // FFT变换
		arm_cmplx_mag_f32(ComplexInputValues, OutputValues, Length); //求复数模
		OutputValues[0] /= Length;									 //直流分量除以采样点数
		// TODO：单边谱的直流和 Nyquist 点应单独归一化；此循环还会重复缩放直流。
		for (i = 0; i < Length; i++)
			OutputValues[i] /= (Length / 2); // 按普通交流频点的 2/N 比例缩放
		free(ComplexInputValues);
	}
	else
	{
		while (1)
			;
	}
}

#if FIR_EN
// FIR 默认关闭。每次调用重新初始化状态，未保留跨数据块的滤波历史。
void FIR(float32_t *InputValues, float32_t *OutputValues, uint16_t Length)
{
	// uint32_t timeTick = HAL_GetTick();		//函数进入时间戳
	// uint32_t timePass = 0;					//计算用时
	// uint32_t blockSize = 128;				//块长度，原始数据长度为1280
	// uint32_t numOfBlock = 1280 / blockSize;	//
	uint16_t i;
	// 状态区长度 = 当前块长度 + 滤波器抽头数 - 1。
	float32_t *pState = (float32_t *)malloc((Length + BL - 1) * sizeof(float32_t));

	if (pState != NULL)
	{
		// TODO：B 当前为 real64_T 系数，指针强转不会转换数值，启用前需改为 float32_t 系数。
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
