#include "timer.h"

uint8_t time_flag = 0;

void Timer_INTERRUPT(void)
{
	//Timer_InterruptClear();		// 清除定时器中断   现在编写的SoC定时中断只持续两个个周期有效，自己清除，即这句话可以不加
	
	/******以下为自己添加中断处理********/
	time_flag = 1;
}


// 定时器配置  注意先配置load 再配置ctrl 保证在使能后load值已经有效
void Timer_Config(uint8_t ctrl, uint32_t load)
{
	Timer -> load = load;		// 重加载值
	Timer -> ctrl = ctrl;		// 控制和状态寄存器
}


// 定时器中断清除
void Timer_InterruptClear(void)
{
	Timer -> clear = 1;	
}

// 不使能定时器
void Timer_DISENABLE(void)
{
	Timer -> ctrl = 0;
}

u32 Timer_GetValue(void)
{
	u32 value;
	value = Timer->value;
	return value;
}
