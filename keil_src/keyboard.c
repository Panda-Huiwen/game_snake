#include "keyboard.h"

KeyInformation key_info;

// �����жϴ���������������κ�һ�������£��ͽ����ж�
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
    KEY -> KEY_STATE = 0;     // ����������״ֵ̬
}

