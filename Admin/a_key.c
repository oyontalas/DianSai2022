#include "a_key.h"

uint16_t key_judge[KEY_NUM]={0};
uint16_t key_state[KEY_NUM]={0};
uint16_t key_time[KEY_NUM]={0};
uint16_t key_flag[KEY_NUM]={0};


void KEY_scan(){
    key_state[0]=HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin);
    key_state[1]=HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin);
    key_state[2]=HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin);
    key_state[3]=HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin);
    for(uint8_t i=0;i<KEY_NUM;i++){
        switch(key_judge[i]){
            case 0:
            if(key_state[i]==0){
                key_judge[i]=1;
                key_time[i]=0;

            }
            break;
            case 1:
            key_time[i]++;
            if(key_time[i]>10){
                if(key_state[i]==0){
                    key_judge[i]=2;
                }
                else{
                    key_judge[i]=0;
                }
            }
            break;
            case 2:
            key_time[i]++;
            if(key_time[i]<400){
                if(key_state[i]==1){
                    key_judge[i]=0;
                    key_flag[i]=1;
                }
            }
            else{
                key_judge[i]=0;
            }
            break;
            
        }
    }
}
