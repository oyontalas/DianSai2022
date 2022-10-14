
#ifndef _A_KEY_H_
#define _A_KEY_H_

#include "a_include.h"

// 按键数量
#define KEY_NUM 4


extern uint16_t key_judge[KEY_NUM];
extern uint16_t key_state[KEY_NUM];
extern uint16_t key_time[KEY_NUM];
extern uint16_t key_flag[KEY_NUM];

void KEY_scan();


#endif


