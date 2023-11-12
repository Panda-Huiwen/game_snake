#include "code_def.h"

void LED_SetMode(uint32_t mode)
{
	LED->WaterLight_MODE = mode;
}

void LED_SetSpeed(uint32_t speed)
{
	LED->WaterLight_SPEED = speed;
}	
