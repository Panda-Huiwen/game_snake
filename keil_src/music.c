#include "music.h"
#include "snake.h"

u8 sutFlag = 0;
u8 sutstate;


extern _snake_Info snake_Info;					//�ߵ�������Ϣ
extern u8 startGame ;
extern u8 keyFlag ;
void Music_INTERRUPT(void)
{
	sutFlag = 1;
	sutstate = SUT->SUT_STATE ;
		if(sutFlag == 1)
    {
       sutFlag = 0;
		if(sutstate !=0 )//��Ϸ��ʼ
		{
			if(snake_Info.sLife == 0 || startGame == 0)
			{
				startGame = 1;
			}
		}
		if(sutstate == 8)//��
		{
			//LCD_Fill(30,100,40,110,GREEN);
			if(snake_Info.sHeadDir == 3 || snake_Info.sHeadDir == 4)
			{
				keyFlag = 1;				
			}
		}		
		if(sutstate == 2)//��
		{
			if(snake_Info.sHeadDir == 1 || snake_Info.sHeadDir == 2)
			{
				keyFlag = 3;
			}
		}	
		if(sutstate == 4)//��
		{
			if(snake_Info.sHeadDir == 3 || snake_Info.sHeadDir == 4)
			{
				keyFlag = 2;
			}
		}	
		if(sutstate == 1)//��
		{
			if(snake_Info.sHeadDir == 1 || snake_Info.sHeadDir == 2)
			{
				keyFlag = 4;
			}
		}	
    }
			sutstate =0;
}


