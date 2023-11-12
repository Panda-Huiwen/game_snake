#include "vga.h"
#include "font.h"

void VGA_CLEAN(uint32_t data)
{
	VGA->VGA_CLEAR = data;
}

void VGA_SET_IMAGE(uint16_t x,uint16_t y,uint16_t color)
{
	uint32_t data;
	data = (uint32_t)(x + (y << 7) + (color << 13));
	VGA->VGA_IMAGE = data;
}

//1.(sx = ex , sy < ey) 2.(sx < ex , sy = ey)
void VGA_DRAW_LINE(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{
	int i,j;
	if(sy == ey)
	{
		for(i = 0;i <= (ex-sx);i++)
		{
			 VGA_SET_IMAGE(sx+i,sy,color);
		}
	}
	if(sx == ex)
	{
		for(j = 0;j <= (ey-sy);j++)
		{
			 VGA_SET_IMAGE(sx,sy+j,color);
		}
	}
}

//sx<=ex sy<=ey
void VGA_DrawRectangle(u16 sx, u16 sy, u16 ex, u16 ey, u8 width, u16 color)
{
	u8 i = 0;
	if(width == 0)
	{
		VGA_DRAW_LINE(sx,sy,ex,sy, color);
		VGA_DRAW_LINE(sx,sy,sx,ey, color);
		VGA_DRAW_LINE(sx,ey,ex,ey, color);
		VGA_DRAW_LINE(ex,sy,ex,ey, color);
	}
	else if( width > 0)
	{
		for(i=0; i<width; i++)
		{								
			VGA_DRAW_LINE(sx,sy,ex,sy, color);
			VGA_DRAW_LINE(sx,sy,sx,ey, color);
			VGA_DRAW_LINE(sx,ey,ex,ey, color);
			VGA_DRAW_LINE(ex,sy,ex,ey, color);
			sx--; sy--; ex++; ey++;
		}
	}
}
//��ɨ��
void VGA_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{
	int i,len;
	len = ey - sy;
	for(i = 0;i <= len;i++)
	{
		VGA_DRAW_LINE(sx,sy+i,ex,sy+i,color);
	}
}

u8 VGA_Rd_Color(u16 x,u16 y)
{
	u32 send_data;
	u32 re_data_reg;
	u8  re_data;
	send_data = (uint32_t)(x + (y << 7));
	VGA->VGA_RDCOLOR = send_data;
	re_data_reg = VGA->VGA_RDCOLOR;
	re_data = re_data_reg & 0xff;
	return re_data;
}

//��ʾ�ַ���
//sx,sy:�������
//width,height:�����С
//size:�����С
//*p:�ַ�����ʼ��ַ
void VGA_ShowString(u16 sx, u16 sy, u16 width, u16 height, u16 fc, u16 bc, u8 size, u8 *p)
{
    u8 x0 = sx;
    width += sx;
    height += sy;

    while ((*p <= '~') && (*p >= ' '))   //�ж��ǲ��ǷǷ��ַ�!
    {
        if (sx >= width)
        {
            sx = x0;
            sy += size;
        }

        if (sy >= height)break; //�˳�

        VGA_ShowChar(sx, sy, *p, fc, bc, size, 0);
        sx += size / 2;
        p++;
    }
}

//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void VGA_ShowChar(u16 sx, u16 sy, u8 num, u16 fc, u16 bc ,u8 size, u8 mode)
{
    u8 temp, t1, t;
    u16 y0 = sy;
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    num = num - ' ';    //�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩

    for (t = 0; t < csize; t++)
    {
        if (size == 12)temp = asc2_1206[num][t];        //����1206����
        //else if (size == 16)temp = asc2_1608[num][t];   //����1608����
        //else if (size == 24)temp = asc2_2412[num][t];   //����2412����
        else return;                                    //û�е��ֿ�

        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)VGA_SET_IMAGE(sx, sy, fc);
            else if (mode == 0)VGA_SET_IMAGE(sx, sy, bc);
			else VGA_SET_IMAGE(sx,sy,BLACK); 

            temp <<= 1;
            sy++;

            if (sy >= 59)return;      //��������

            if ((sy - y0) == size)
            {
                sy = y0;
                sx++;

                if (sx >= 79)return;   //��������

                break;
            }
        }
    }
}

uint32_t VGA_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;	 
    while(n--)result*=m;    
    return result;
}			

//��ʾ����,��λΪ0,����ʾ
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//color:��ɫ
//num:��ֵ(0~4294967295);
void VGA_ShowNum(u16 x, u16 y, u16 fc, u16 bc , u32 num, u8 len, u8 size)
{
    u8 t, temp;
    u8 enshow = 0;

    for (t = 0; t < len; t++)
    {
        temp = (num / VGA_Pow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                VGA_ShowChar(x + (size / 2)*t, y, ' ', fc, bc ,size, 0);
                continue;
            }
            else enshow = 1;

        }

        VGA_ShowChar(x + (size / 2)*t, y, temp + '0', fc, bc , size, 0);
    }
}

void VGA_Show_Pic(u16 pic[60][80])
{
	int i,j;
	for(j = 0;j < 60;j++)
	{
		for(int i = 0;i < 80;i++)
		{
			VGA_SET_IMAGE(i,j,pic[j][i]);
		}
	}
}

uint32_t VGA_clear_finish;

void VGA_CLEAR_FINISH(void)
{
	VGA_clear_finish = 1; 
}
