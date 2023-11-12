#include "code_def.h"
#include "delay.h"

void Set_SysTick_CTRL(uint32_t ctrl)
{
	SysTick->CTRL = ctrl;
}

void Set_SysTick_LOAD(uint32_t load)
{
	SysTick->LOAD = load;
}

uint32_t Read_SysTick_VALUE(void)
{
	return(SysTick->VALUE);
}

void Set_SysTick_CALIB(uint32_t calib)
{
	SysTick->CALIB = calib;
}

void Set_SysTick_VALUE(uint32_t value)
{
	SysTick->VALUE = value;
}

// SysTick��ֹ
void SysTick_DISENABLE(void)
{
	Set_SysTick_CTRL(0);
}


// SysTick�жϴ�����
void SysTickHandler(void)
{
	DelayTimeTick++;
    SysTick_DISENABLE();					// ���жϺͽ�ֹSysTick
    SCB->ICSR = SCB->ICSR | (1 << 25);		// д1���SysTick����״̬
}
	

