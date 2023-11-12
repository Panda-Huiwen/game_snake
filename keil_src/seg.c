#include "seg.h"
#include <stdint.h>
#include <string.h>


/* ����4λ�������ʾ������ 0-F*/
void SEG4_SetData(u8 data_left1,u8 data_left2,u8 data_left3,u8 data_left4)
{
	SEG4 -> disp_left1 = data_left1;
	SEG4 -> disp_left2 = data_left2;
	SEG4 -> disp_left3 = data_left3;
	SEG4 -> disp_left4 = data_left4;
}


/* ����4λ�����С������ʾ��λ��--4bit��ʾ������������
	1000����ߵڶ�����0100��ð�ţ�0010����ߵ�3����0001����ߵ�4*/
void SEG4_SetDP(u8 set_dp_position)
{
	SEG4 -> dp_position = set_dp_position;
}


/* 4λ�������ʾ */
void SEG4_Display(u8 data_left1,u8 data_left2,u8 data_left3,u8 data_left4,u8 set_dp_position)
{
	SEG4_SetData(data_left1,data_left2,data_left3,data_left4);
	SEG4_SetDP(set_dp_position);
}


void Sz_Dsiplay(u16 data)
{
    u8 thousand;    // ǧλ
    u8 hundred;     // ��λ
    u8 ten;         // ʮλ
    u8 unit;        // ��λ

    thousand = data/1000;
    hundred = data/100%10;
    ten = data/10%100%10;
    unit = data%10;
    
    SEG4_Display(thousand,hundred,ten,unit,1);
}
