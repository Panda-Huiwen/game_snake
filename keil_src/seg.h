#ifndef _SEG_H
#define _SEG_H

#include "code_def.h"

void SEG4_SetData(u8 data_left1,u8 data_left2,u8 data_left3,u8 data_left4);
void SEG4_SetDP(u8 set_dp_position);
void SEG4_Display(u8 data_left1,u8 data_left2,u8 data_left3,u8 data_left4,u8 set_dp_position);
void Sz_Dsiplay(u16 data);
#endif
