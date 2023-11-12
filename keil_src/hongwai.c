#include "hongwai.h"
#include "snake.h"

HwInformation hw_info;

extern _snake_Info snake_Info;					//�ߵ�������Ϣ
u8 keyFlag = 0;
u8 startGame = 0;

//���ⰴ���жϺ���
void HW_INTERRUPT(void)
{
	//LED_SetMode(0x02);
	hw_info.hw_flag = 1;//���ⰴ������
	hw_info.key_state = HW->HW_STATE;//��ȡ����ֵ
	if(hw_info.hw_flag == 1)
    {
        hw_info.hw_flag = 0;
		if(hw_info.key_state == 28)//��Ϸ��ʼ
		{
			if(snake_Info.sLife == 0 || startGame == 0)
			{
				startGame = 1;
			}
		}
		if(hw_info.key_state == 24)//��
		{
			//LCD_Fill(30,100,40,110,GREEN);
			if(snake_Info.sHeadDir == 3 || snake_Info.sHeadDir == 4)
			{
				keyFlag = 1;				
			}
		}		
		if(hw_info.key_state == 8)//��
		{
			if(snake_Info.sHeadDir == 1 || snake_Info.sHeadDir == 2)
			{
				keyFlag = 3;
			}
		}	
		if(hw_info.key_state == 82)//��
		{
			if(snake_Info.sHeadDir == 3 || snake_Info.sHeadDir == 4)
			{
				keyFlag = 2;
			}
		}	
		if(hw_info.key_state == 90)//��
		{
			if(snake_Info.sHeadDir == 1 || snake_Info.sHeadDir == 2)
			{
				keyFlag = 4;
			}
		}	
    }
    HW->HW_STATE = 0;     // �������״ֵ̬
}
