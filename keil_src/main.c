#include "code_def.h"
#include "delay.h"
#include "systick.h"
#include "vga.h"
#include "hongwai.h"
#include "snake.h"
#include "keyboard.h"
#include "seg.h"
#include "timer.h"
#include "lcd.h"
#include "music.h"

extern uint32_t VGA_clear_finish;
extern uint8_t time_flag;
extern uint32_t music_init_flag;

int main()
{
	NVIC_CTRL_ADDR	=	0x1f;
	SysTick_DISENABLE();
	VGA_CLEAN(0X00);
	LED_SetSpeed(0x003d7840);
	while(!VGA_clear_finish);
	VGA_clear_finish = 0;
	
		Snake_Game( );
	
//	while(1);
	
	//LCD_Init();
	//LCD_Fill(50,50,100,100,RED_LCD);
	//LED_SetMode(0x01);
	//re_data = VGA_Rd_Color(1,1);
	//Sz_Dsiplay(1234);
//	LED_SetSpeed(0x017d7840);
//	//LED_SetMode(0x01);
//	//delay_nus(100);
//	//LED_SetMode(0x02);
//	VGA_SET_IMAGE(0,0,0xE0);
//	VGA_SET_IMAGE(79,0,0x1C);
//	VGA_SET_IMAGE(0,59,0x03);
//	VGA_SET_IMAGE(79,59,0xE0);
//	VGA_DRAW_LINE(5,5,10,5,0xE0);
//	VGA_DRAW_LINE(5,10,5,15,0xE0);
//	VGA_DrawRectangle(20,20,30,30,2,0xE0);
//	VGA_Fill(40,40,50,50,0xE0);
	//while(1){}
	//VGA_ShowString(1,1,60,24,12,"GAME OVER");
	//LED_SetMode(0x01);
//	Timer_Config(3,500000000);
//	while(time_flag == 0);
//	if(time_flag == 1)
//	{
//		LED_SetMode(0x01);
//	}
	//while(1){};
	//VGA_ShowNum(1,1,WHITE,BLACK,0,1,12);

//	while(!music_init_flag);
//	music_init_flag = 0;
//	Music_SetAddr(24832,31009);
//	Music_Ctrl(1);
//	delay_systick_nms(10000);
//	Music_Ctrl(0);
//	delay_systick_nms(10000);
//	Music_Ctrl(1);
//	while(1){};
	
//	delay_timer_nms(10000);
//	LED_SetMode(0x03);
//	delay_timer_nms(10000);
//	LED_SetMode(0x00);
}

//int main()
//{
//	SysTick_DISENABLE();
//	LED_SetSpeed(0x017d7840);
//	LED_SetMode(0x01);
//	delay_nms(10000);
//	LED_SetMode(0x02);
//	while(1){}
//}
