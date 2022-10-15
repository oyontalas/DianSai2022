#ifndef _A_LCD_H_
#define _A_LCD_H_


#include "a_include.h"
#include "GUI.h"


void Gui_ShowWave(float *InputWaveData, uint16_t c_len, float begin_x, float begin_y, float end_x, float end_y);
void Gui_DrawHistogram(uint16_t *amplitudeData,uint16_t h_len, float begin_x, float begin_y, float end_x, float end_y);



#endif