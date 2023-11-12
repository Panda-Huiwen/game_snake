#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "code_def.h"

void SysTickHandler(void);
void Set_SysTick_CTRL(uint32_t ctrl);
void Set_SysTick_LOAD(uint32_t load);
uint32_t Read_SysTick_VALUE(void);
void Set_SysTick_VALUE(uint32_t value);
void Set_SysTick_CALIB(uint32_t calib);	
void SysTick_DISENABLE(void);

#endif


