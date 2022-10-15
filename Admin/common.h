
#ifndef _COMMON_H_
#define _COMMON_H_

/*  BEGIN ----------------------------------------------------------*/

#include "a_include.h"
#include "a_interrupt.h"
// #include "a_rtc.h"
#include "a_lcd.h"
#include "a_adc.h"
// #include "a_flash.h"

#include "GUI.h"

#include "a_DSP.h"

void IT_start(void);
void IT_stop(void);
void ADMIN_Init(void);
void ADMIN_Mainloop(void);

/*   END  ----------------------------------------------------------*/



#endif