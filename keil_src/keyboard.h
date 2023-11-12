#include "code_def.h"

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

typedef struct{
	uint16_t key_flag;		// 红外中断标志 1--表示按下中断
	uint16_t key_state;		// 红外的状态值，用于指示那些按键被按下，赋值0x00清除状态
}KeyInformation;

#endif
