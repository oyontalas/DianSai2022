
#include "a_interrupt.h" 


uint8_t Uart_Rx_byte=0;
uint8_t Uart_Rx_bytes[128]={0};
uint16_t Uart_Rx_size=0;
cJSON           *root;

uint8_t ADC_Flag=0;

double a;
// // printf重定向
// #ifdef __GNUC__
// #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
// #else
// #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
// #endif
// PUTCHAR_PROTOTYPE
// {
//     HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF); //阻塞方式打印
//     return ch;
// }

// 每次中断接收一个字节，以换行作为 JSON 报文结束标记；命令分支目前未启用。
// 当前在中断内解析报文；扩展通信功能时应将解析移到主循环，缩短中断执行时间。
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart){
    if(huart->Instance==USART1){
        if(Uart_Rx_byte=='\n'){

            // strcpy(pdata,Uart_Rx_bytes);
            // change_pcode_en=1;
            root = cJSON_Parse(Uart_Rx_bytes);
            // uint8_t *cmd=getStr("cmd");
            // if (is(cmd, "test"))
            // {
            //     uint8_t text[128];
            //     a=getNum("data");
            //     sprintf(text,"USART_IT test ok.data is %.2f\n",a);
		    //     HAL_UART_Transmit(&huart1,(uint8_t *)text, strlen(text), 50);

            // }
            // else if (is(cmd, "temp"))
            // {
            //     uint8_t num = (uint8_t)getNum("data");
            //     for (uint8_t i = 0; i <10;i++){
            //         if(num==i)FLASH_uart_tx_En[i]=1;
            //     }
            // }
            // else if (is(cmd, "time"))
            // {
            //     RTC_Scan();
            //     if(!isnan(getNum("Minutes")))   RTC_Time.Minutes =(uint8_t)getNum("Minutes");
            //     if(!isnan(getNum("Hours")))     RTC_Time.Hours=(uint8_t)getNum("Hours");
            //     if(!isnan(getNum("Seconds")))   RTC_Time.Seconds =(uint8_t)getNum("Seconds");
            //     if(!isnan(getNum("Date")))      RTC_Date.Date =(uint8_t)getNum("Date");
            //     if(!isnan(getNum("Month")))     RTC_Date.Month =(uint8_t)getNum("Month");
            //     if(!isnan(getNum("Year")))      RTC_Date.Year =(uint8_t)getNum("Year");

            //     // HAL_RTC_SetTime(&hrtc, &RTC_Time, RTC_FORMAT_BIN);
	        //     // HAL_RTC_SetDate(&hrtc, &RTC_Date, RTC_FORMAT_BIN);	
                
            //     uint8_t text[128];
            //     sprintf(text,"time set ok.\n");
		    //     HAL_UART_Transmit(&huart1,(uint8_t *)text, strlen(text), 50);
            // }
            // TODO：解析树包含子节点，应使用 cJSON_Delete() 完整释放。
            free(root);
            Uart_Rx_byte=0;
            memset(Uart_Rx_bytes,0,128);
            Uart_Rx_size=0;
        }
        else
            // TODO：写入前需检查容量，并为 JSON 字符串的结束符预留空间。
            Uart_Rx_bytes[Uart_Rx_size++]=Uart_Rx_byte;
        // HAL 单字节接收完成后需重新挂接，否则后续字节不会继续触发接收回调。
        HAL_UART_Receive_IT(&huart1,&Uart_Rx_byte,1);
    }
}



uint8_t LED_Red_Flag=0;
uint8_t LED_Blue_Flag=0;
uint8_t LED_Green_Flag=0;
uint8_t Buzzer_Flag=0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

    // TIM16/TIM17 的周期均为 1 ms；TIM15 的 ADC 触发由硬件完成，不在此逐点采样。
    if(htim->Instance==TIM16){
        // ADC_GetTemp_En=1;
        if(LED_Red_Flag){
            htim2.Instance->CCR1+=10;
            if(htim2.Instance->CCR1>=1000-10){
                htim2.Instance->CCR1=0;
                LED_Red_Flag=0;
                }
        }
        if(LED_Blue_Flag){
            htim2.Instance->CCR2+=10;
            if(htim2.Instance->CCR2>=1000-10){
                htim2.Instance->CCR2=0;
                LED_Blue_Flag=0;
                }
        }
        if(LED_Green_Flag){
            htim2.Instance->CCR3+=10;
            if(htim2.Instance->CCR3>=1000-10){
                htim2.Instance->CCR3=0;
                LED_Green_Flag=0;

                }
        }
        if(Buzzer_Flag){
            // 主循环写 1 启动提示音，随后兼作毫秒节拍计数，约 100 ms 后关闭。
            // 若 TIM16 被暂停，提示音的实际持续时间会延长。
            htim1.Instance->CCR1=1000-10;
            Buzzer_Flag++;
            if(Buzzer_Flag>=100){
                htim1.Instance->CCR1=0;
                Buzzer_Flag=0;
            }
            // htim1.Instance->CCR1+=10;
            // if(htim1.Instance->CCR1>=1000-10){
            //     htim1.Instance->CCR1=0;
            //     Buzzer_Flag=0;
            //     }
        }  

    }
    else if(htim->Instance==TIM17){
        KEY_scan();
        
        
        // RTC_Scan();
    }
    // else if (htim->Instance==TIM15){
    //     static uint16_t i;
    //     if (!ADC_Flag)
    //     {
    //         if (i < ADC_BUFFER_SIZE)
    //         {
    //             HAL_ADC_Start(&hadc1);	//启动ADC转换
	//             HAL_ADC_PollForConversion(&hadc1,1);	//等待转换完成，10ms表示超时时间
    //             ADC_Buffer[i++] = (uint16_t)HAL_ADC_GetValue(&hadc1);
    //         }
    //         else
    //         {
    //             i = 0;
    //             ADC_Flag = 1;
    //         }
            	
    //     }
    // }

}
