#include "common.h"

// 显示模式：0 为时域波形，1 为频谱；按下 KEY1/KEY2 后选择模式。
// 注意：初值 1000 超出 uint8_t 范围，截断后的值不对应上述模式。
uint8_t ShowFlag = 1000;
uint8_t text[128] = {0};

void IT_start(void)
{
    // TIM15 通过 TRGO 触发 ADC；TIM16 更新声光状态；TIM17 扫描按键。
    // HAL_TIM_Base_Start_IT(&htim3);
    HAL_TIM_Base_Start_IT(&htim15);
    HAL_TIM_Base_Start_IT(&htim16);
    HAL_TIM_Base_Start_IT(&htim17);

    HAL_UART_Receive_IT(&huart1, &Uart_Rx_byte, 1);
    // HAL_TIM_Base_Start_IT(&htim3);
}
void IT_stop(void)
{
    // 绘图期间暂停采样触发、声光状态更新和按键扫描，并中止串口接收。
    // 这里只停指定外设，不是全局关中断，也没有关闭 ADC 的 DMA 通道。
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

    // 先准备 DMA 循环缓冲区；启动 TIM15 后，ADC 才按外部触发节拍采样。
    // DMA 使用半字传输，长度单位为采样点；uint32_t * 转换仅适配 HAL 接口。
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

    // 屏幕初始化完成后开始采样和按键扫描；FFT 在处理按键事件时初始化。

    IT_start();
}

// uint16_t data[5]={90,90,90,90,90};

void ADMIN_Mainloop(void)
{
    uint32_t i, j, k;

    // 中断只置位 key_flag，主循环消费事件并执行耗时的 FFT 和 SPI 绘图。
    // 数据处理也在事件分支内，因此没有按键事件时不会自动刷新画面。
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
                // KEY3：优先增大 ARR，再增大 PSC，使采样周期按倍数增加。
                // 定时器分频使用寄存器值加 1，因此修改时需要先加 1 再减 1。
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
                // KEY4：优先减小 ARR，再减小 PSC，提高采样率。
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
            // 复制到局部数组，供后续电压换算与 FFT 使用。
            // 注意：此时 DMA 仍在写入，复制结果不保证是一帧连续、完整的新数据；
            // 调整采样率后也未等待整帧更新，后续需结合 DMA 完成事件同步。
            for (j = 0; j < ADC_BUFFER_SIZE; j++)
            {
                ADC_Values[j] = (uint32_t)ADC_Buffer[j];
            }

            // 按 3.3 V 参考电压、12 位 ADC 换算为伏特，实际参考电压变化会影响幅值。
            for (j = 0; j < ADC_BUFFER_SIZE; j++)
            {
                Vol_Value[j] = ADC_Values[j] * 3.3 / 4096;
            }
            FFT_GetValuesModulus(Vol_Value, FFT_Value, ADC_BUFFER_SIZE);

            // 搜索幅值最大的频点。当前搜索包含直流和镜像频谱，不能保证对应基波。
            for (j = 0; j < ADC_BUFFER_SIZE; j++)
            {
                if (FFT_Value[j] > FFT_Value[max_index])
                    max_index = j;
            }

            // 采样率 Fs = TIM15 时钟 / ((ARR + 1) * (PSC + 1))，当前时钟为 160 MHz。
            float32_t freq = (float32_t)(1.0 * 160 * 1000 * 1000 / (htim15.Instance->ARR + 1) / (htim15.Instance->PSC + 1));
            // 频点间隔为 Fs/N；谱峰下标乘以该间隔得到频率估计值。
            float32_t base_freq = (float32_t)(max_index * freq / ADC_BUFFER_SIZE);
            switch (ShowFlag)
            {
            case 0:
                IT_stop();

                // 时域仅绘制前半帧；后四个参数为绘图区在屏幕宽高中的比例。
                Gui_ShowWave(Vol_Value, ADC_BUFFER_SIZE / 2, 0.2, 0.1, 0.95, 0.9);
                snprintf(text, 320 / 8, "SaFreq:%.1fHz BaFreq:%.1fHz", freq, base_freq);
                Gui_DrawFont_GBK16(8 * 0, 16 * 0, RED, BLACK, text);
                IT_start();
                break;
            case 1:
                IT_stop();

                // 实数输入的幅度谱具有镜像对称性，显示下标 0 到 N/2-1 的频点。
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
