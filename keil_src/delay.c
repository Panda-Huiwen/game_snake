#include "delay.h"
#include "systick.h"
#include "timer.h"

volatile uint32_t DelayTimeTick = 0;
extern uint8_t time_flag;

// ��ʱ  ��LCD����ʹ��
void delay_systick(uint32_t time)
{
	Set_SysTick_CTRL(0);
	Set_SysTick_LOAD(time);
	Set_SysTick_VALUE(0);
	Set_SysTick_CTRL(0x7);		// ѡ��ϵͳʱ��,ʹ��SysTick�жϣ�ʹ��SysTick��ʱ��
	while(!DelayTimeTick);
	DelayTimeTick = 0;
}


// ��ʱ1΢��
void delay_systick_1us(void)
{
	Set_SysTick_CTRL(0);
	Set_SysTick_LOAD(SystemCoreClock/1000000);
	Set_SysTick_VALUE(0);
	Set_SysTick_CTRL(0x7);		// ѡ��ϵͳʱ��,ʹ��SysTick�жϣ�ʹ��SysTick��ʱ��
	while(!DelayTimeTick);
	DelayTimeTick = 0;
}


// ��ʱ1����
void delay_systick_1ms(void)
{
	Set_SysTick_CTRL(0);
	Set_SysTick_LOAD(SystemCoreClock/1000);
	Set_SysTick_VALUE(0);
	Set_SysTick_CTRL(0x7);
	while(!DelayTimeTick);
	DelayTimeTick = 0;
}


// ��ʱn΢��
void delay_systick_nus(uint32_t n)
{
	uint32_t i;
	
	for(i=0;i<n;i++)
	{
		delay_systick_1us();
	}
}


// ��ʱn����
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
