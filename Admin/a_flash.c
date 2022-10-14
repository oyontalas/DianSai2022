#include "a_flash.h"



void FLASH_save_10_data(float temp,RTC_TimeTypeDef RTC_Time){
    static uint8_t record_num;
    uint16_t value;
    
    temp*=1000;
    value = (uint16_t)temp;	
    Flash_WriteData(PARA_ADDR+0+record_num*32, (uint64_t *)&value, 1);						
    value=RTC_Time.Hours;
    Flash_WriteData(PARA_ADDR+8+record_num*32, (uint64_t *)&value, 1);
    value=RTC_Time.Minutes;
    Flash_WriteData(PARA_ADDR+16+record_num*32, (uint64_t *)&value, 1);		
    value=RTC_Time.Seconds;
    Flash_WriteData(PARA_ADDR+24+record_num*32, (uint64_t *)&value, 1);		
    record_num++;
    if(record_num==10)record_num = 0;	
}



uint64_t FLASH_read_data[4]={0};
float FLASH_read(uint8_t record_num){
    float output;
    Flash_ReadData(PARA_ADDR+record_num*32,FLASH_read_data,4);
    output=1.0*FLASH_read_data[0]/1000;
    return output;
}




uint8_t FLASH_uart_tx_En[10]={0};
void FLASH_uart_tx(){
    for (uint8_t i=0;i<10;i++){
        if(FLASH_uart_tx_En[i]){
            FLASH_uart_tx_En[i]=0;
            uint8_t text[128];
            float temp=FLASH_read(i);
            sprintf(text,"TEMP %d:%.2fC Time: %02lld:%02lld:%02lld\n",i,temp,FLASH_read_data[1],FLASH_read_data[2],FLASH_read_data[3]);
            HAL_UART_Transmit(&huart1,(uint8_t *)text, strlen(text), 50); 
        }
    }
}
