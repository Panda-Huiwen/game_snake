#include "keyboard.h"

KeyInformation key_info;

// 按键中断处理函数，矩阵键盘任何一个被按下，就进入中断
void KEYBOARD_INTERRUPT(void)
{
    key_info.key_flag = 1;  
    key_info.key_state = KEY -> KEY_STATE;
	if(key_info.key_flag == 1)
    {
		key_info.key_flag = 0;
		if(key_info.key_state == 0x0001)
		{
			LED_SetMode(0X01);
		}
    }
    KEY -> KEY_STATE = 0;     // 清除矩阵键盘状态值
}

