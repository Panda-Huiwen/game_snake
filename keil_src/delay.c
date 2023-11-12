#include "delay.h"
#include "systick.h"
#include "timer.h"

volatile uint32_t DelayTimeTick = 0;
extern uint8_t time_flag;

// 延时  供LCD驱动使用
void delay_systick(uint32_t time)
{
	Set_SysTick_CTRL(0);
	Set_SysTick_LOAD(time);
	Set_SysTick_VALUE(0);
	Set_SysTick_CTRL(0x7);		// 选择系统时钟,使能SysTick中断，使能SysTick定时器
	while(!DelayTimeTick);
	DelayTimeTick = 0;
}


// 延时1微秒
void delay_systick_1us(void)
{
	Set_SysTick_CTRL(0);
	Set_SysTick_LOAD(SystemCoreClock/1000000);
	Set_SysTick_VALUE(0);
	Set_SysTick_CTRL(0x7);		// 选择系统时钟,使能SysTick中断，使能SysTick定时器
	while(!DelayTimeTick);
	DelayTimeTick = 0;
}


// 延时1毫秒
void delay_systick_1ms(void)
{
	Set_SysTick_CTRL(0);
	Set_SysTick_LOAD(SystemCoreClock/1000);
	Set_SysTick_VALUE(0);
	Set_SysTick_CTRL(0x7);
	while(!DelayTimeTick);
	DelayTimeTick = 0;
}


// 延时n微秒
void delay_systick_nus(uint32_t n)
{
	uint32_t i;
	
	for(i=0;i<n;i++)
	{
		delay_systick_1us();
	}
}


// 延时n毫秒
void delay_systick_nms(uint32_t n)
{
	uint32_t i;
	
	for(i=0;i<n;i++)
	{
		delay_systick_1ms();
	}
}

void delay_timer(uint32_t time)
{
	Timer_DISENABLE();
	Timer_Config(3,time);
	while(!time_flag);
	time_flag = 0;
	Timer_DISENABLE();
}
void delay_timer_1us(void)
{
	Timer_DISENABLE();
	Timer_Config(3,SystemCoreClock/1000000);
	while(!time_flag);
	time_flag = 0;
	Timer_DISENABLE();
}	

void delay_timer_1ms(void)
{
	Timer_DISENABLE();
	Timer_Config(3,SystemCoreClock/1000);
	while(!time_flag);
	time_flag = 0;
	Timer_DISENABLE();
}
void delay_timer_nus(uint32_t n)
{
	uint32_t i;
	
	for(i=0;i<n;i++)
	{
		delay_timer_1us();
	}
}
void delay_timer_nms(uint32_t n)
{
	uint32_t i;
	
	for(i=0;i<n;i++)
	{
		delay_timer_1ms();
	}
}
