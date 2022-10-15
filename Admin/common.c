#include "common.h"

uint8_t ShowFlag = 1000;
uint8_t text[128] = {0};

void IT_start(void)
{
    // HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_Base_Start_IT(&htim15);
    HAL_TIM_Base_Start_IT(&htim16);
    HAL_TIM_Base_Start_IT(&htim17);

    HAL_UART_Receive_IT(&huart1, &Uart_Rx_byte, 1);
    // HAL_TIM_Base_Start_IT(&htim3);
}
void IT_stop(void)
{
    // HAL_TIM_Base_Stop_IT(&htim3);
    HAL_TIM_Base_Stop_IT(&htim15);
    HAL_TIM_Base_Stop_IT(&htim16);
    HAL_TIM_Base_Stop_IT(&htim17);
    HAL_UART_AbortReceive_IT(&huart1);
}

void ADMIN_Init(void)
{

    // 开启定时器
    // HAL_TIM_Base_Start(&htim1);
    // HAL_TIM_Base_Start(&htim2);
    // HAL_TIM_Base_Start(&htim15);
    // HAL_TIM_Base_Start(&htim17);
    // 开启PWM
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

    // 开启ADC
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Buffer, ADC_BUFFER_SIZE);

    Lcd_Init();
    Lcd_Clear(BLACK);
    snprintf(text, 320 / 8, "HELLO!");
    Gui_DrawFont_GBK16(LCD_X_SIZE / 2 - 3 * 8, LCD_Y_SIZE / 2 - 8, WHITE, BLACK, text);

    // IT_start();

    // HAL_ADCEx_Calibration_Start(&hadc,ADC_SINGLE_ENDED);
    // HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_Buffer,ADC_BUFFER_SIZE);
    // HAL_ADC_Start_DMA(&hadc1,(uint32_t *)&ADC_Value,3);
    // HAL_Delay(50); //必须加的延时，等待DMA给数组传输数据后再显示

    // IT_stop();
    // Gui_DrawFont_GBK16(8*3,16*1,RED,BLACK,"STU Code:123456789");
    // Gui_DrawFont_GBK16(180,40,RED,GRAY0,"Time:");
    // Gui_DrawFont_GBK16(180,60,RED,GRAY0,"Freq:");

    // 初始化FFT

    IT_start();
}

// uint16_t data[5]={90,90,90,90,90};

void ADMIN_Mainloop(void)
{
    uint32_t i, j, k;

    for (uint8_t i = 0; i < KEY_NUM; i++)
    {
        if (key_flag[i])
        {
            key_flag[i] = 0;

            switch (i)
            {
            case 0:
                // if (ShowFlag == 0)
                //     ShowFlag = 2;
                // else if (ShowFlag == 2)
                //     ShowFlag = 0;
                // else
                //     ShowFlag = 0;

                ShowFlag = 0;

                // PWM让BUZZER叫一声
                Buzzer_Flag = 1;
                HAL_Delay(100);

                break;
            case 1:
                // if (ShowFlag == 1)
                //     ShowFlag = 3;
                // else if (ShowFlag == 3)
                //     ShowFlag = 1;
                // else
                //     ShowFlag = 1;

                    ShowFlag = 1;

                // PWM让BUZZER叫一声
                Buzzer_Flag = 1;
                HAL_Delay(100);

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
                // PWM让BUZZER叫一声
                Buzzer_Flag = 1;
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
                // PWM让BUZZER叫一声
                Buzzer_Flag = 1;
                HAL_Delay(100);

                // hadc1.Instance->

                // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Buffer, ADC_BUFFER_SIZE);

                // Gui_DrawFont_GBK16(180,20,RED,GRAY0,"KEY4");

                break;
            }
            // uint32_t ADC_Values[ADC_BUFFER_SIZE] = {0};
            // float32_t_t FFT_Value[ADC_BUFFER_SIZE];
            // uint32_t FFT_OutputValue[ADC_BUFFER_SIZE] = {0};
            // float32_t Vol_Value[ADC_BUFFER_SIZE * 2] = {0};
            // uint16_t max_index = 0;

            // for (j = 0; j < ADC_BUFFER_SIZE; j++)
            // {
            //     ADC_Values[j] = (uint32_t)ADC_Buffer[j];
            // }

            // for (j = 0; j < ADC_BUFFER_SIZE; j++)
            // {
            //     Vol_Value[2 * j] = ADC_Values[j] * 3.3 / 4096;
            //     Vol_Value[2 * j + 1] = 0;
            //     ADC_Values[j] = (uint32_t)(Vol_Value[2 * j] * 1000);
            // }
            // // float32_t testInput_f32_10khz[2 * ADC_BUFFER_SIZE] = {0}; // FFT输入数组

            // // int num;
            // // for (num = 0; num < ADC_BUFFER_SIZE; num++)
            // // {
            // //     testInput_f32_10khz[num * 2] = Vol_Value[num];
            // //     testInput_f32_10khz[num * 2 + 1] = 0;
            // // }

            // arm_cfft_radix2_f32(&S, Vol_Value);                       // FFT变换
            // arm_cmplx_mag_f32(Vol_Value, FFT_Value, ADC_BUFFER_SIZE); //求复数模
            // FFT_Value[0] /= ADC_BUFFER_SIZE;                          //直流分量除以采样点数
            // for (j = 0; j < ADC_BUFFER_SIZE; j++)
            //     FFT_Value[j] /= (ADC_BUFFER_SIZE / 2); //其他分量除以（采样点数/2）

            // for (j = 0; j < ADC_BUFFER_SIZE; j++)
            //     FFT_OutputValue[j] = (uint32_t)(FFT_Value[j] * 1000);
            // // FFT_OutputValue最大值的下标

            // for (j = 0; j < ADC_BUFFER_SIZE; j++)
            // {
            //     if (FFT_OutputValue[j] > FFT_OutputValue[max_index])
            //         max_index = j;
            // }

            uint32_t ADC_Values[ADC_BUFFER_SIZE] = {0};
            float32_t Vol_Value[ADC_BUFFER_SIZE] = {0};
            float32_t FFT_Value[ADC_BUFFER_SIZE] = {0};
            float32_t FIR_Value[ADC_BUFFER_SIZE] = {0};

            uint16_t max_index = 0;
            // 将ADC_Buffer的值赋给ADC_Values
            for (j = 0; j < ADC_BUFFER_SIZE; j++)
            {
                ADC_Values[j] = (uint32_t)ADC_Buffer[j];
            }

            for (j = 0; j < ADC_BUFFER_SIZE; j++)
            {
                Vol_Value[j] = ADC_Values[j] * 3.3 / 4096;
            }
            FFT_GetValuesModulus(Vol_Value, FFT_Value, ADC_BUFFER_SIZE);

            // FFT_OutputValue最大值的下标
            for (j = 0; j < ADC_BUFFER_SIZE; j++)
            {
                if (FFT_Value[j] > FFT_Value[max_index])
                    max_index = j;
            }

            // 采用频率
            float32_t freq = (float32_t)(1.0 * 160 * 1000 * 1000 / (htim15.Instance->ARR + 1) / (htim15.Instance->PSC + 1));
            // 基波频率
            float32_t base_freq = (float32_t)(max_index * freq / ADC_BUFFER_SIZE);
            switch (ShowFlag)
            {
            case 0:
                IT_stop();

                Gui_ShowWave(Vol_Value, ADC_BUFFER_SIZE / 2, 0.2, 0.1, 0.95, 0.9);
                snprintf(text, 320 / 8, "SaFreq:%.1fHz BaFreq:%.1fHz", freq, base_freq);
                Gui_DrawFont_GBK16(8 * 0, 16 * 0, RED, BLACK, text);
                IT_start();
                break;
            case 1:
                IT_stop();

                // 显示频谱
                Gui_ShowWave(FFT_Value, ADC_BUFFER_SIZE / 2, 0.2, 0.1, 0.95, 0.9);

                snprintf(text, 320 / 8, "SaFreq:%.1fHz BaFreq:%.1fHz", freq, base_freq);
                Gui_DrawFont_GBK16(8 * 0, 16 * 0, RED, BLACK, text);
                IT_start();
                break;
            // case 2:
            //     FIR(Vol_Value, FIR_Value, ADC_BUFFER_SIZE);

            //     IT_stop();
            //     // 显示滤波后的波形
            //     Gui_ShowWave(FIR_Value, ADC_BUFFER_SIZE / 2, 0.2, 0.1, 0.95, 0.9);
            //     snprintf(text, 320 / 8, "SaFreq:%.1fHz BaFreq:%.1fHz", freq, base_freq);
            //     Gui_DrawFont_GBK16(8 * 0, 16 * 0, RED, BLACK, text);
            //     IT_start();
            //     break;
            // case 3:
            //     FIR(Vol_Value, FIR_Value, ADC_BUFFER_SIZE);
            //     // 将FIR_Value的值赋给Vol_Value
            //     memcpy(Vol_Value, FIR_Value, sizeof(float32_t) * ADC_BUFFER_SIZE);
            //     // 将滤波后的波形进行FFT变换
            //     FFT_GetValuesModulus(Vol_Value, FFT_Value, ADC_BUFFER_SIZE);

            //     IT_stop();

            //     // 显示频谱
            //     Gui_ShowWave(FFT_Value, ADC_BUFFER_SIZE / 2, 0.2, 0.1, 0.95, 0.9);
            //     snprintf(text, 320 / 8, "SaFreq:%.1fHz BaFreq:%.1fHz", freq, base_freq);
            //     Gui_DrawFont_GBK16(8 * 0, 16 * 0, RED, BLACK, text);
            //     IT_start();
            //     break;
            }
        }
    }

    // HAL_ADC_Start(&hadc1);
    // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Buffer, ADC_BUFFER_SIZE);
    HAL_Delay(100);
}
