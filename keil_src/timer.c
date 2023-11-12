#include "timer.h"

uint8_t time_flag = 0;

void Timer_INTERRUPT(void)
{
	//Timer_InterruptClear();		// �����ʱ���ж�   ���ڱ�д��SoC��ʱ�ж�ֻ����������������Ч���Լ����������仰���Բ���
	
	/******����Ϊ�Լ�����жϴ���********/
	time_flag = 1;
}


// ��ʱ������  ע��������load ������ctrl ��֤��ʹ�ܺ�loadֵ�Ѿ���Ч
void Timer_Config(uint8_t ctrl, uint32_t load)
{
	Timer -> load = load;		// �ؼ���ֵ
	Timer -> ctrl = ctrl;		// ���ƺ�״̬�Ĵ���
}


// ��ʱ���ж����
void Timer_InterruptClear(void)
{
	Timer -> clear = 1;	
}

// ��ʹ�ܶ�ʱ��
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
