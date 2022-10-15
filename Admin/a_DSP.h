#ifndef _A_DSP_H_
#define _A_DSP_H_


#define FIR_EN 0

#include "a_include.h"
#include "fdacoefs.h"

extern arm_cfft_radix2_instance_f32 S_FFT;
extern arm_fir_instance_f32 S_FIR;

void FFT_GetValuesModulus(float32_t *InputValues, float32_t *OutputValues, uint16_t Length);

#if FIR_EN
void FIR(float32_t *InputValues, float32_t *OutputValues, uint16_t Length);
#endif

#endif // _A_DSP_H_
