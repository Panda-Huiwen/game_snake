#ifndef _TIMER_H
#define _TIMER_H

#include "code_def.h"

void Timer_Config(uint8_t ctrl, uint32_t load);
void Timer_InterruptClear(void);
void Timer_DISENABLE(void);
u32 Timer_GetValue(void);

#endif
