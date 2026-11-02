#include "a_key.h"

uint16_t key_judge[KEY_NUM]={0};
uint16_t key_state[KEY_NUM]={0};
uint16_t key_time[KEY_NUM]={0};
uint16_t key_flag[KEY_NUM]={0};


/*
 * TIM17 每 1 ms 调用一次，低电平表示按下；key_time 以扫描次数计时。
 * 状态 0 等待按下，状态 1 消抖，状态 2 等待释放，释放后置位 key_flag。
 * 主循环负责清除事件标志并执行操作；暂停扫描期间不累计按键时间。
 */
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
            // 连续经过超过 10 次扫描后复查电平，过滤短暂抖动。
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
            // 计时包含消抖阶段；不足 400 次扫描时释放才产生本轮事件。
            // 超时仅回到状态 0，持续按住会再次进入检测，不实现独立长按事件。
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
