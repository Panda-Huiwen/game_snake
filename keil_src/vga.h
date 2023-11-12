#ifndef __VGA_H
#define __VGA_H

#include "code_def.h"

//color
#define WHITE  0XFF
#define BLACK  0X00
#define RED    0XE0
#define BLUE   0X03
#define GREEN  0X1C
#define YELLOW 0XFC
#define CHING  0X1F //青色
#define PEONY  0XE3 //深红色

void VGA_CLEAN(uint32_t data);

void VGA_SET_IMAGE(uint16_t x,uint16_t y,uint16_t color);//0<=x<=79 0<=y<=59

void VGA_DRAW_LINE(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);

void VGA_DrawRectangle(u16 sx, u16 sy, u16 ex, u16 ey, u8 width, u16 color);

void VGA_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);

void VGA_ShowString(u16 sx, u16 sy, u16 width, u16 height, u16 fc, u16 bc,u8 size, u8 *p);

void VGA_ShowChar(u16 sx, u16 sy, u8 num, u16 fc, u16 bc, u8 size, u8 mode);

uint32_t VGA_Pow(uint8_t m,uint8_t n);

void VGA_ShowNum(u16 x, u16 y, u16 fc, u16 bc ,u32 num, u8 len, u8 size);

u8 VGA_Rd_Color(u16 x,u16 y);

void VGA_Show_Pic(u16 pic[60][80]);
#endif
