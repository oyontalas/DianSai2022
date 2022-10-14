#include "a_rtc.h"

RTC_TimeTypeDef RTC_Time={0};
RTC_DateTypeDef RTC_Date={0};


void RTC_Scan(void){
    HAL_RTC_GetTime(&hrtc, &RTC_Time,RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &RTC_Date, RTC_FORMAT_BIN);
}