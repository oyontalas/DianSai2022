#include "common.h"



uint8_t ShowFlag = 0;
uint8_t text[128] = {0};

arm_cfft_radix2_instance_f32 S;

void IT_start(void)
{
    // HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_Base_Start_IT(&htim15);
    HAL_TIM_Base_Start_IT(&htim17);

    HAL_UART_Receive_IT(&huart1, &Uart_Rx_byte, 1);
    // HAL_TIM_Base_Start_IT(&htim3);
}
void IT_stop(void)
{
    // HAL_TIM_Base_Stop_IT(&htim3);
    HAL_TIM_Base_Stop_IT(&htim15);
    HAL_TIM_Base_Stop_IT(&htim17);
    HAL_UART_AbortReceive_IT(&huart1);
}

void ADMIN_Init(void)
{

    // 开启定时器
    HAL_TIM_Base_Start(&htim1);
    HAL_TIM_Base_Start(&htim2);
    // HAL_TIM_Base_Start(&htim15);
    // HAL_TIM_Base_Start(&htim17);

    // 开启ADC
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Buffer, ADC_BUFFER_SIZE);

    Lcd_Init();
    Lcd_Clear(BLACK);
    snprintf(text, 320 / 8, "HELLO!");
    Gui_DrawFont_GBK16(LCD_X_SIZE / 2 - 3 * 8, LCD_Y_SIZE / 2 - 8, RED, BLACK, text);

    // IT_start();

    // HAL_ADCEx_Calibration_Start(&hadc,ADC_SINGLE_ENDED);
    // HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Buffer,ADC_BUFFER_SIZE);
    // HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_Value,3);
    // HAL_Delay(50); //必须加的延时，等待DMA给数组传输数据后再显示

    // IT_stop();
    // Gui_DrawFont_GBK16(8*3,16*1,RED,BLACK,"STU Code:123456789");
    // Gui_DrawFont_GBK16(180,40,RED,GRAY0,"Time:");
    // Gui_DrawFont_GBK16(180,60,RED,GRAY0,"Freq:");
    IT_start();
    // 初始化FFT
    arm_cfft_radix2_init_f32(&S, ADC_BUFFER_SIZE, 0, 1);
}

// uint16_t data[5]={90,90,90,90,90};

void ADMIN_Mainloop(void)
{  
    uint32_t i, j, k;

    // static uint8_t second,date;

    // switch (ShowFlag)
    // {
    // // 显示波形
    // case 0:
    //     // static uint8_t Case0Flag;
    //     // if(Case0Flag){
    //     IT_stop();
    //     // 采用频率
    //     snprintf(text, 320 / 8, "SampFreq:%.1fHz WaveFreq:%.1fHz", freq, base_freq);
    //     Gui_DrawFont_GBK16(8 * 0, 16 * 0, RED, BLACK, text);
    //     Gui_ShowWave(ADC_Values, ADC_BUFFER_SIZE / 4, 0.2, 0.1, 0.95, 0.9);
    //     IT_start();
    //     // }
    //     // else{
    //     //     Case0Flag = 1;
    //     // }

    //     break;
    // // 显示频谱
    // case 1:
    //     // static uint8_t Case1Flag;
    //     // if(Case1Flag){

    //     IT_stop();
    //     Gui_ShowWave(ADC_Values, ADC_BUFFER_SIZE / 4, 0.2, 0.1, 0.95, 0.9);
    //     snprintf(text, 320 / 8, "SampFreq:%.1fHz WaveFreq:%.1fHz", freq, base_freq);
    //     Gui_DrawFont_GBK16(8 * 0, 16 * 0, RED, BLACK, text);
    //     // 显示频谱
    //     Gui_ShowWave(FFT_OutputValue, ADC_BUFFER_SIZE / 2, 0.2, 0.1, 0.95, 0.9);
    //     IT_start();
    //     // }
    //     // else{
    //     //     Case1Flag = 1;
    //     // }
    //     break;
    // }

    // if(second != RTC_Time.Seconds){
    //     second = RTC_Time.Seconds;
    //     sprintf(text,"Time: %02d:%02d:%02d",RTC_Time.Hours,RTC_Time.Minutes,RTC_Time.Seconds);
    //     IT_stop();
    //     Gui_DrawFont_GBK16(8*3,16*3,RED,GRAY0,text);
    //     IT_start();
    // }
    // if(date != RTC_Date.Date){
    //     date = RTC_Date.Date;
    //     sprintf(text,"Date:20%02d-%02d-%02d",RTC_Date.Year,RTC_Date.Month,RTC_Date.Date);
    //     IT_stop();
    //     Gui_DrawFont_GBK16(8*3,16*5,RED,GRAY0,text);
    //     IT_start();
    // }

    // if(ADC_GetTemp_En){
    // ADC_GetTemp_En=0;
    // ADC_GetTemp();
    // IT_stop();
    // FLASH_save_10_data(ADC_Temp,RTC_Time);
    // IT_start();
    // sprintf(text,"TEMP:%.2f C",ADC_Buffer[0]*3.3/4096);
    // HAL_UART_Transmit(&huart1,(uint8_t *)text, strlen(text), 50);
    // if(ADC_Flag){

    // Gui_box2(10,10,300,220,1);
    // 将ADC_Buffer里面的数据传入到ADC_Values里面

    // HAL_ADC_Stop_DMA(&hadc1);

    // HAL_Delay(500);
    // 调用库，对ADC_Values进行傅里叶变换，得到频谱

    // uint16_t FFT_Values[ADC_BUFFER_SIZE];
    // arm_cfft_fast_instance_f32 S;
    // arm_rfft_fast_init_f32(&S, ADC_BUFFER_SIZE);
    // arm_rfft_fast_f32(&S, (float32_t*)ADC_Values, (float32_t*)FFT_Values, 0);
    // // 将FFT_Values里面的数据传入到FFT_Values里面
    // uint16_t FFT_Values2[ADC_BUFFER_SIZE];
    // memcpy(FFT_Values2,FFT_Values,sizeof(FFT_Values));
    // // 调用库，对FFT_Values2进行傅里叶逆变换，得到原始信号
    // arm_rfft_fast_instance_f32 S2;
    // arm_rfft_fast_init_f32(&S2, ADC_BUFFER_SIZE);
    // arm_rfft_fast_f32(&S2, (float32_t*)FFT_Values2, (float32_t*)ADC_Values, 1);
    // // 显示频谱
    // Gui_ShowWave((uint16_t*)FFT_Values2,ADC_BUFFER_SIZE,0.2,0.1,0.95,0.9);
    // Gui_ShowWave((uint16_t*)ADC_Values,ADC_BUFFER_SIZE,0.2,0.1,0.95,0.9);
    // Gui_ShowWave((uint16_t*)FFT_Values,ADC_BUFFER_SIZE,0.2,0.1,0.95,0.9);
    // Gui_ShowWave((uint16_t*)FFT_Values2,ADC_BUFFER_SIZE,0.2,0.1,0.95,0.9);
    // HAL_Delay(1000);
    // 显示原始信号

    // Gui_DrawHistogram((uint16_t*)data,5);
    // HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Buffer,ADC_BUFFER_SIZE);
    // ADC_Flag=0;

    // }
    // }

    // FLASH_uart_tx();

    for (uint8_t i = 0; i < KEY_NUM; i++)
    {
        if (key_flag[i])
        {
            key_flag[i] = 0;

            switch (i)
            {
            case 0:
                ShowFlag = 0;
                break;
            case 1:
                // ADC_GetTemp_Show_En=0;
                // IT_stop();
                // sprintf(text,"              ");
                // Gui_DrawFont_GBK16(8*3,16*7,RED,GRAY0,text);
                // IT_start();
                ShowFlag = 1;
                break;
            case 2:
                if (htim15.Instance->ARR < 20480)
                {
                    htim15.Instance->ARR = (htim15.Instance->ARR + 1) * 2 - 1;
                }
                else if (htim15.Instance->PSC < 20480)
                {
                    htim15.Instance->PSC = (htim15.Instance->PSC + 1) * 2 - 1;
                }
                HAL_Delay(100);

                // 开启ADC
                // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Buffer, ADC_BUFFER_SIZE);

                break;
            case 3:
                if (htim15.Instance->ARR > 10)
                {
                    htim15.Instance->ARR = (htim15.Instance->ARR + 1) / 2 - 1;
                }
                else if (htim15.Instance->PSC > 10)
                {
                    htim15.Instance->PSC = (htim15.Instance->PSC + 1) / 2 - 1;
                }
                HAL_Delay(100);

                // hadc1.Instance->

                // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Buffer, ADC_BUFFER_SIZE);

                // Gui_DrawFont_GBK16(180,20,RED,GRAY0,"KEY4");

                break;
            }
            uint32_t ADC_Values[ADC_BUFFER_SIZE] = {0};
            float_t FFT_Value[ADC_BUFFER_SIZE];
            uint32_t FFT_OutputValue[ADC_BUFFER_SIZE] = {0};
            float Vol_Value[ADC_BUFFER_SIZE * 2] = {0};
            uint16_t max_index = 0;

            for (j = 0; j < ADC_BUFFER_SIZE; j++)
            {
                ADC_Values[j] = (uint32_t)ADC_Buffer[j];
            }

            for (j = 0; j < ADC_BUFFER_SIZE; j++)
            {
                Vol_Value[2 * j] = ADC_Values[j] * 3.3 / 4096;
                Vol_Value[2 * j + 1] = 0;
                ADC_Values[j] = (uint32_t)(Vol_Value[2 * j] * 1000);
            }
            // float testInput_f32_10khz[2 * ADC_BUFFER_SIZE] = {0}; // FFT输入数组

            // int num;
            // for (num = 0; num < ADC_BUFFER_SIZE; num++)
            // {
            //     testInput_f32_10khz[num * 2] = Vol_Value[num];
            //     testInput_f32_10khz[num * 2 + 1] = 0;
            // }

            arm_cfft_radix2_f32(&S, Vol_Value);                       // FFT变换
            arm_cmplx_mag_f32(Vol_Value, FFT_Value, ADC_BUFFER_SIZE); //求复数模
            FFT_Value[0] /= ADC_BUFFER_SIZE;                          //直流分量除以采样点数
            for (j = 0; j < ADC_BUFFER_SIZE; j++)
                FFT_Value[j] /= (ADC_BUFFER_SIZE / 2); //其他分量除以（采样点数/2）

            for (j = 0; j < ADC_BUFFER_SIZE; j++)
                FFT_OutputValue[j] = (uint32_t)(FFT_Value[j] * 1000);
            // FFT_OutputValue最大值的下标

            for (j = 0; j < ADC_BUFFER_SIZE; j++)
            {
                if (FFT_OutputValue[j] > FFT_OutputValue[max_index])
                    max_index = j;
            }
            // 采用频率
            float freq = (float)(1.0 * 160 * 1000 * 1000 / (htim15.Instance->ARR + 1) / (htim15.Instance->PSC + 1));
            // 基波频率
            float base_freq = (float)(max_index * freq / ADC_BUFFER_SIZE);
            switch (ShowFlag)
            {
            case 0:
                IT_stop();

                Gui_ShowWave(ADC_Values, ADC_BUFFER_SIZE / 2, 0.2, 0.1, 0.95, 0.9);
                snprintf(text, 320 / 8, "SaFreq:%.1fHz WaFreq:%.1fHz", freq, base_freq);
                Gui_DrawFont_GBK16(8 * 0, 16 * 0, RED, BLACK, text);
                IT_start();
                break;
            case 1:
                IT_stop();

                // 显示频谱
                Gui_ShowWave(FFT_OutputValue, ADC_BUFFER_SIZE / 2, 0.2, 0.1, 0.95, 0.9);
                snprintf(text, 320 / 8, "SaFreq:%.1fHz WaFreq:%.1fHz", freq, base_freq);
                Gui_DrawFont_GBK16(8 * 0, 16 * 0, RED, BLACK, text);
                IT_start();
                break;
            }
        }
    }

    // HAL_ADC_Start(&hadc1);
    // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Buffer, ADC_BUFFER_SIZE);
    HAL_Delay(100);
}
