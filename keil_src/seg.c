#include "seg.h"
#include <stdint.h>
#include <string.h>


/* 设置4位数码管显示的数字 0-F*/
void SEG4_SetData(u8 data_left1,u8 data_left2,u8 data_left3,u8 data_left4)
{
	SEG4 -> disp_left1 = data_left1;
	SEG4 -> disp_left2 = data_left2;
	SEG4 -> disp_left3 = data_left3;
	SEG4 -> disp_left4 = data_left4;
}


/* 设置4位数码管小数点显示的位置--4bit表示，从左往右数
	1000：左边第二个，0100：冒号，0010：左边第3个，0001，左边第4*/
void SEG4_SetDP(u8 set_dp_position)
{
	SEG4 -> dp_position = set_dp_position;
}


/* 4位数码管显示 */
void SEG4_Display(u8 data_left1,u8 data_left2,u8 data_left3,u8 data_left4,u8 set_dp_position)
{
	SEG4_SetData(data_left1,data_left2,data_left3,data_left4);
	SEG4_SetDP(set_dp_position);
}


void Sz_Dsiplay(u16 data)
{
    u8 thousand;    // 千位
    u8 hundred;     // 百位
    u8 ten;         // 十位
    u8 unit;        // 个位

    thousand = data/1000;
    hundred = data/100%10;
    ten = data/10%100%10;
    unit = data%10;
    
    SEG4_Display(thousand,hundred,ten,unit,1);
}
