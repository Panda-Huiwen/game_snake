#ifndef __DELAY_H
#define __DELAY_H

//#include "code_def.h"
#include <stdint.h>

#define SystemCoreClock			50000000		// œµÕ≥ ±÷”25M	

extern volatile uint32_t DelayTimeTick;

void delay_systick(uint32_t time);
void delay_systick_1us(void);
void delay_systick_1ms(void);
void delay_systick_nus(uint32_t n);
void delay_systick_nms(uint32_t n);

void delay_timer(uint32_t time);
void delay_timer_1us(void);
void delay_timer_1ms(void);
void delay_timer_nus(uint32_t n);
void delay_timer_nms(uint32_t n);

#endif
