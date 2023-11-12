//======================================================================================================
// 日期     : 16:36 2022/4/22
// 作者     : fly dragon
// 文件名   : cm0_lcd.c
// 功能描述 : CM0的LCD接口     寄存器操作
//              X:0~240  Y:0~320
// 修改记录 :   
// 备注     :
//======================================================================================================

#include "lcd.h"
#include "stdlib.h"
#include <stdint.h>
#include <stdio.h>
//#include "font.h"
#include "delay.h"

extern const unsigned char asc2_1206[95][12];

uint16_t POINT_COLOR = BLACK_LCD;
uint16_t BACK_COLOR  = WHITE_LCD;       // 屏幕背景色   

_lcd_dev lcddev;
uint16_t ans = 0;
//void MakeData( uint16_t data ) {
//    LCD->LCD_DATA[0] = ( data >> 0 ) & 1 ;
//    LCD->LCD_DATA[1] = ( data >> 1 ) & 1 ;
//    LCD->LCD_DATA[2] = ( data >> 2 ) & 1 ;
//    LCD->LCD_DATA[3] = ( data >> 3 ) & 1 ;
//    LCD->LCD_DATA[4] = ( data >> 4 ) & 1 ;
//    LCD->LCD_DATA[5] = ( data >> 5 ) & 1 ;
//    LCD->LCD_DATA[6] = ( data >> 6 ) & 1 ;
//    LCD->LCD_DATA[7] = ( data >> 7 ) & 1 ;
//    LCD->LCD_DATA[8] = ( data >> 8 ) & 1 ;
//    LCD->LCD_DATA[9] = ( data >> 9 ) & 1 ;
//    LCD->LCD_DATA[10] = ( data >> 10 ) & 1;
//    LCD->LCD_DATA[11] = ( data >> 11 ) & 1;
//    LCD->LCD_DATA[12] = ( data >> 12 ) & 1;
//    LCD->LCD_DATA[13] = ( data >> 13 ) & 1;
//    LCD->LCD_DATA[14] = ( data >> 14 ) & 1;
//    LCD->LCD_DATA[15] = ( data >> 15 ) & 1;
//}

void MakeData( uint16_t data ) {
    LCD->LCD_DATA = data;
}

uint16_t ReadData() {
	uint16_t ans ;
    ans = LCD->LCD_DATA;
	return ans;
}

//  WRITE REG FUNCTION
void LCD_WR_REG( uint16_t data ) {
  	LCD_RS_CLR; 
 	LCD_CS_CLR; 
	delay_systick(500);
	MakeData( data );
	LCD_WR_CLR;
	delay_systick(500);
	LCD_WR_SET; 
 	LCD_CS_SET;         
}

//  WRITE DATA
void LCD_WR_DATAX( uint16_t data ) {
  	LCD_RS_SET;
	LCD_CS_CLR;
	delay_systick(500);
	MakeData( data );
	LCD_WR_CLR;
	delay_systick(500);
	LCD_WR_SET;
	LCD_CS_SET;
}

uint16_t LCD_RD_DATA(void)
{										   
	uint16_t t;

	LCD_RS_SET;
	LCD_CS_CLR;
	LCD_RD_CLR;			   
	t=ReadData();
	//delay(500);  
	LCD_RD_SET;
	LCD_CS_SET; 

	return t;  
}

//  WRITE REG VALUE
//  LCD_Reg : NUMBER OF REG
//  LCD_RegValue : VALUE NEEDED TO BE WRITTEN
void LCD_WriteReg( uint16_t LCD_Reg, uint16_t LCD_RegValue ) {
	LCD_WR_REG( LCD_Reg );  
	LCD_WR_DATA( LCD_RegValue );	    		 
} 

uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{										   
 	LCD_WR_REG(LCD_Reg);   
	delay_systick(500);
	return LCD_RD_DATA(); 
}

//  BEGIN TO WRITE GRAM
void LCD_WriteRAM_Prepare( void ) {
    LCD_WR_REG( lcddev.wramcmd );
}

//  LCD WRITE GRAM
void LCD_WriteRAM( uint16_t RGB_Code ) {
    LCD_WR_DATA( RGB_Code );
}

void opt_delay( uint8_t t ) {
    while ( t-- );
}

//  LCD START DISPLAY
void LCD_DisplayOn( void ) {
    LCD_WR_REG( 0x29 );
}

//  LCD END DISPLAY
void LCD_DisplayOff( void ) {
    LCD_WR_REG( 0x28 );
}

//  LCD SET CURSOR POSTION
void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos ) {
    LCD_WR_REG( lcddev.setxcmd ); 
	LCD_WR_DATA( Xpos >> 8 );
    LCD_WR_DATA( Xpos & 0XFF ); 			 
	LCD_WR_REG( lcddev.setycmd ); 
	LCD_WR_DATA( Ypos >> 8 );
    LCD_WR_DATA( Ypos & 0XFF ); 
}

//  SET LCD SCANNING DIRECTION

void LCD_Scan_Dir(u8 dir)
{
    u16 regval = 0;
    u16 dirreg = 0;
    u16 temp;
 
    //横屏时，对1963不改变扫描方向, 其他IC改变扫描方向！竖屏时1963改变方向, 其他IC不改变扫描方向
    if ((lcddev.dir == 1 && lcddev.id != 0X1963) || (lcddev.dir == 0 && lcddev.id == 0X1963))
    {
        switch (dir)   //方向转换
        {
            case 0:
                dir = 6;
                break;
 
            case 1:
                dir = 7;
                break;
 
            case 2:
                dir = 4;
                break;
 
            case 3:
                dir = 5;
                break;
 
            case 4:
                dir = 1;
                break;
 
            case 5:
                dir = 0;
                break;
 
            case 6:
                dir = 3;
                break;
 
            case 7:
                dir = 2;
                break;
        }
    }
 
    switch (dir)
    {
        case L2R_U2D://从左到右,从上到下
            regval |= (0 << 7) | (0 << 6) | (0 << 5);
            break;
 
        case L2R_D2U://从左到右,从下到上
            regval |= (1 << 7) | (0 << 6) | (0 << 5);
            break;
 
        case R2L_U2D://从右到左,从上到下
            regval |= (0 << 7) | (1 << 6) | (0 << 5);
            break;
 
        case R2L_D2U://从右到左,从下到上
            regval |= (1 << 7) | (1 << 6) | (0 << 5);
            break;
 
        case U2D_L2R://从上到下,从左到右
            regval |= (0 << 7) | (0 << 6) | (1 << 5);
            break;
 
        case U2D_R2L://从上到下,从右到左
            regval |= (0 << 7) | (1 << 6) | (1 << 5);
            break;
 
        case D2U_L2R://从下到上,从左到右
            regval |= (1 << 7) | (0 << 6) | (1 << 5);
            break;
 
        case D2U_R2L://从下到上,从右到左
            regval |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
    }
 
    if (lcddev.id == 0X5510)
	{dirreg = 0X3600;}
    else dirreg = 0X36;
 
    if (lcddev.id == 0X9341 || lcddev.id == 0X7789)   //9341 & 7789 要设置BGR位
    {
        regval |= 0X08;
    }
 
    LCD_WriteReg(dirreg, regval);
 
    if (lcddev.id != 0X1963)   //1963不做坐标处理
    {
        if (regval & 0X20)
        {
            if (lcddev.width < lcddev.height)   //交换X,Y
            {
                temp = lcddev.width;
                lcddev.width = lcddev.height;
                lcddev.height = temp;
            }
        }
        else
        {
            if (lcddev.width > lcddev.height)   //交换X,Y
            {
                temp = lcddev.width;
                lcddev.width = lcddev.height;
                lcddev.height = temp;
            }
        }
    }
 
    //设置显示区域(开窗)大小
    if (lcddev.id == 0X5510)
    {
        LCD_WR_REG(lcddev.setxcmd);
        LCD_WR_DATA(0);
        LCD_WR_REG(lcddev.setxcmd + 1);
        LCD_WR_DATA(0);
        LCD_WR_REG(lcddev.setxcmd + 2);
        LCD_WR_DATA((lcddev.width - 1) >> 8);
        LCD_WR_REG(lcddev.setxcmd + 3);
        LCD_WR_DATA((lcddev.width - 1) & 0XFF);
        LCD_WR_REG(lcddev.setycmd);
        LCD_WR_DATA(0);
        LCD_WR_REG(lcddev.setycmd + 1);
        LCD_WR_DATA(0);
        LCD_WR_REG(lcddev.setycmd + 2);
        LCD_WR_DATA((lcddev.height - 1) >> 8);
        LCD_WR_REG(lcddev.setycmd + 3);
        LCD_WR_DATA((lcddev.height - 1) & 0XFF);
    }
    else
    {
        LCD_WR_REG(lcddev.setxcmd);
        LCD_WR_DATA(0);
        LCD_WR_DATA(0);
        LCD_WR_DATA((lcddev.width - 1) >> 8);
        LCD_WR_DATA((lcddev.width - 1) & 0XFF);
        LCD_WR_REG(lcddev.setycmd);
        LCD_WR_DATA(0);
        LCD_WR_DATA(0);
        LCD_WR_DATA((lcddev.height - 1) >> 8);
        LCD_WR_DATA((lcddev.height - 1) & 0XFF);
    }
}


void LCD_DrawPoint(u16 x,u16 y, u16 color)
{
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD_WR_DATA(color); 
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{	   
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);  			 
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF);
	LCD_RS_CLR;
 	LCD_CS_CLR; 
	MakeData(lcddev.wramcmd); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET; 
	LCD_WR_DATA(color);		
}

void LCD_Display_Dir(uint8_t dir)
{
	if(dir==0)			
	{
		lcddev.dir=0;	
		lcddev.width=240;
		lcddev.height=320;
		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 
		if(lcddev.id==0X6804||lcddev.id==0X5310)
		{
			lcddev.width=320;
			lcddev.height=480;
		}
		
	}else 				
	{	  				
		lcddev.dir=1;	
		lcddev.width=320;
		lcddev.height=240;
		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	
}	 

void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{    
	// uint8_t hsareg,heareg,vsareg,veareg;
	// uint16_t hsaval,heaval,vsaval,veaval; 
	uint16_t twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(sx>>8); 
	LCD_WR_DATA(sx&0XFF);	 
	LCD_WR_DATA(twidth>>8); 
	LCD_WR_DATA(twidth&0XFF);  
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(sy>>8); 
	LCD_WR_DATA(sy&0XFF); 
	LCD_WR_DATA(theight>>8); 
	LCD_WR_DATA(theight&0XFF); 
	
}

void LCD_Init(void)
{ 
    // 这个在这加，初始化时就不用加了
    LCD->LCD_RST = 0;
	delay_systick_nms(50);
	LCD->LCD_RST = 1;

	delay_systick_nms(50); // delay 50 ms 
	LCD_WriteReg(0x0000,0x0001);
	delay_systick_nms(50); // delay 50 ms 
	uint16_t idx = LCD_ReadReg(0x0000);
  	lcddev.id = 0x9341;
	if(lcddev.id==0X9341)	//9341
	{	 
		LCD_WR_REG(0xCF);  
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0xC1); 
		LCD_WR_DATAX(0X30); 
		LCD_WR_REG(0xED);  
		LCD_WR_DATAX(0x64); 
		LCD_WR_DATAX(0x03); 
		LCD_WR_DATAX(0X12); 
		LCD_WR_DATAX(0X81); 
		LCD_WR_REG(0xE8);  
		LCD_WR_DATAX(0x85); 
		LCD_WR_DATAX(0x10); 
		LCD_WR_DATAX(0x7A); 
		LCD_WR_REG(0xCB);  
		LCD_WR_DATAX(0x39); 
		LCD_WR_DATAX(0x2C); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x34); 
		LCD_WR_DATAX(0x02); 
		LCD_WR_REG(0xF7);  
		LCD_WR_DATAX(0x20); 
		LCD_WR_REG(0xEA);  
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_REG(0xC0);    //Power control 
		LCD_WR_DATAX(0x1B);   //VRH[5:0] 
		LCD_WR_REG(0xC1);    //Power control 
		LCD_WR_DATAX(0x01);   //SAP[2:0];BT[3:0] 
		LCD_WR_REG(0xC5);    //VCM control 
		LCD_WR_DATAX(0x30); 	 //3F
		LCD_WR_DATAX(0x30); 	 //3C
		LCD_WR_REG(0xC7);    //VCM control2 
		LCD_WR_DATAX(0XB7); 
		LCD_WR_REG(0x36);    // Memory Access Control 
		//LCD_WR_DATAX(0x48); 
		LCD_WR_DATAX(0x48);
		LCD_WR_REG(0x3A);   
		LCD_WR_DATAX(0x55); 
		LCD_WR_REG(0xB1);   
		LCD_WR_DATAX(0x00);   
		LCD_WR_DATAX(0x1A); 
		LCD_WR_REG(0xB6);    // Display Function Control 
		LCD_WR_DATAX(0x0A); 
		LCD_WR_DATAX(0xA2); 
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
		LCD_WR_DATAX(0x00); 
		LCD_WR_REG(0x26);    //Gamma curve selected 
		LCD_WR_DATAX(0x01); 
		LCD_WR_REG(0xE0);    //Set Gamma 
		LCD_WR_DATAX(0x0F); 
		LCD_WR_DATAX(0x2A); 
		LCD_WR_DATAX(0x28); 
		LCD_WR_DATAX(0x08); 
		LCD_WR_DATAX(0x0E); 
		LCD_WR_DATAX(0x08); 
		LCD_WR_DATAX(0x54); 
		LCD_WR_DATAX(0XA9); 
		LCD_WR_DATAX(0x43); 
		LCD_WR_DATAX(0x0A); 
		LCD_WR_DATAX(0x0F); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x00); 		 
		LCD_WR_REG(0XE1);    //Set Gamma 
		LCD_WR_DATAX(0x00); 
		LCD_WR_DATAX(0x15); 
		LCD_WR_DATAX(0x17); 
		LCD_WR_DATAX(0x07); 
		LCD_WR_DATAX(0x11); 
		LCD_WR_DATAX(0x06); 
		LCD_WR_DATAX(0x2B); 
		LCD_WR_DATAX(0x56); 
		LCD_WR_DATAX(0x3C); 
		LCD_WR_DATAX(0x05); 
		LCD_WR_DATAX(0x10); 
		LCD_WR_DATAX(0x0F); 
		LCD_WR_DATAX(0x3F); 
		LCD_WR_DATAX(0x3F); 
		LCD_WR_DATAX(0x0F); 
		LCD_WR_REG(0x2B); 
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x01);
		LCD_WR_DATAX(0x3f);
		LCD_WR_REG(0x2A); 
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0x00);
		LCD_WR_DATAX(0xef);	 
		LCD_WR_REG(0x11); //Exit Sleep
		delay_systick_nms(120);
		LCD_WR_REG(0x29); //display on	
	}
	LCD_Display_Dir(0);		 	// horizon or vertical
	LCD -> LCD_BL_CTR = 1;					// Back Light
	LCD_Clear(WHITE_LCD);
}  		  

void LCD_Clear(uint16_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			
	if((lcddev.id==0X6804)&&(lcddev.dir==1))
	{						    
 		lcddev.dir=0;	 
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 			
		LCD_SetCursor(0x00,0x0000);		  
 		lcddev.dir=1;	 
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  	 
 	}else LCD_SetCursor(0x00,0x0000);	 
	LCD_WriteRAM_Prepare();     			  	  
	for(index=0;index<totalpoint;index++)LCD_WR_DATA(color);	
}  


/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：sx,sy   起始坐标
                ex,ey   终止坐标
                color   要填充的颜色d
      返回值：  无
******************************************************************************/
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{          
	uint16_t i,j;
	uint16_t xlen=0;
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
	 	LCD_SetCursor(sx,i);      				
		LCD_WriteRAM_Prepare();     				  
		for(j=0;j<xlen;j++)LCD_WR_DATA(color);		    
	}
}  

void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	width=ex-sx+1; 			
	height=ey-sy+1;			
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	 
		LCD_WriteRAM_Prepare();     
		for(j=0;j<width;j++)LCD_WR_DATA(color[i*width+j]);
	}	  
} 


/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                // color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t; 
	int xerr=0, yerr=0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x2 - x1; //计算坐标增量 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	if(delta_x>0)
		incx=1; //设置单步方向 
	else if(delta_x==0)
		incx=0;//垂直线 
	else 
	{	
		incx=-1;
		delta_x = -delta_x;
	} 
	if(delta_y>0)
		incy=1; 
	else if(delta_y==0)
		incy=0;//水平线 
	else
	{
		incy=-1;
		delta_y = -delta_y;
	} 
	if( delta_x>delta_y)
		distance = delta_x; //选取基本增量坐标轴 
	else 
		distance = delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol, color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符      要么输入字符，要么输入字符对应的ascii值
                fc 字的颜色
                bc 字的背景色
                size 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
/*void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t size,uint8_t mode)
{  							  
    uint8_t temp,t1,t;
	uint16_t x0=x;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		
    num=num-' ';    //得到偏移后的值
 	// num=num-0;
	for(t=0;t<csize;t++)
	{   temp=asc2_1206[num][t];     //默认给16 为减少存储容量，只给一个
		// if(size==12)temp=asc2_1206[num][t]; 	 	
		// else if(size==16)temp=asc2_1608[num][t];	
		// else if(size==24)temp=asc2_2412[num][t];	
		// else return;								
		for(t1=0;t1<8;t1++)
		{	
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,fc);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,bc);     
            else LCD_Fast_DrawPoint(x,y,BACK_COLOR);        
			temp<<=1;
			x++;
			if(x>=lcddev.width)return;		
			if((x-x0)==size)
			{
				x=x0;
				y++;
				if(y>=lcddev.height)return;	
				break;
			}
		}  	 
	}  	    	   	 	  
}   */

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode)
{
    u8 temp, t1, t;
    u16 y0 = y;
    u8 csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);  //得到字体一个字符对应点阵集所占的字节数
    num = num - ' ';    //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）

    for (t = 0; t < csize; t++)
    {
        if (size == 12)temp = asc2_1206[num][t];        //调用1206字体
        //else if (size == 16)temp = asc2_1608[num][t];   //调用1608字体
        //else if (size == 24)temp = asc2_2412[num][t];   //调用2412字体
        else return;                                    //没有的字库

        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)LCD_Fast_DrawPoint(x, y, POINT_COLOR);
            else if (mode == 0)LCD_Fast_DrawPoint(x, y, BACK_COLOR);

            temp <<= 1;
            y++;

            if (y >= lcddev.height)return;      //超区域了

            if ((y - y0) == size)
            {
                y = y0;
                x++;

                if (x >= lcddev.width)return;   //超区域了

                break;
            }
        }
    }
}



/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;	 
    while(n--)result*=m;    
    return result;
}			 
	 
     
     
/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                size 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/     
/*void LCD_ShowIntNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t size,uint8_t mode)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x,y+(size/2)*t,' ',fc,bc,size,mode);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x,y+(size/2)*t,temp+'0',fc,bc,size,mode);     
	}
} */



/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数=整数部分位数+2 例 12.5 则len=4   因为显示两位小数
                fc 字的颜色
                bc 字的背景色
                size 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
/*void LCD_ShowFloatNum(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t size,uint8_t mode)
{   
    uint8_t t,temp,d_size;
	uint16_t num1;
	d_size=size/2;
	num1=num*100;       // 设置显示小数位
	for(t=0;t<len;t++)
	{
		temp=(num1/LCD_Pow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x,y+(len-2)*d_size,'.',fc,bc,size,mode);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x,y+t*d_size,temp+48,fc,bc,size,mode);
	}
} */  



/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                size 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
/*void LCD_ShowString(uint16_t x,uint16_t y,const u8 *p,uint16_t fc,uint16_t bc,uint16_t size,uint16_t mode)
{       
    // 这个是硬木课堂的
	// uint8_t x0=x;
	// width+=x;
	// height+=y;
    // while((*p<='~')&&(*p>=' '))
    // {       
        // if(x>=width){x=x0;y+=size;}
        // if(y>=height)break;
        // LCD_ShowChar(x,y,*p,size,0);
        // x+=size/2;
        // p++;
    // }  
    
    // 这个是中景园的
    while((*p<='~')&&(*p>=' '))
	{       
		LCD_ShowChar(x,y,*p,fc,bc,size,mode);
		y+=size/2;
		p++;
	} 
}*/


/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                size 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/

void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u8 width, u8 dir, u16 color)
{
	u8 i = 0;
	
	if(width == 0)
	{
		LCD_DrawLine(x1,y1,x2,y1, color);
		LCD_DrawLine(x1,y1,x1,y2, color);
		LCD_DrawLine(x1,y2,x2,y2, color);
		LCD_DrawLine(x2,y1,x2,y2, color);
	}
	else if( width > 0)
	{
		if(dir == 0)
		{
			for(i=0; i<width; i++)
			{								
				LCD_DrawLine(x1,y1,x2,y1, color);
				LCD_DrawLine(x1,y1,x1,y2, color);
				LCD_DrawLine(x1,y2,x2,y2, color);
				LCD_DrawLine(x2,y1,x2,y2, color);
				x1--; y1--; x2++; y2++;
			}
		}
		else
		{			
			for(i=0; i<width; i++)
			{
				x1++; y1++; x2--; y2--;
				LCD_DrawLine(x1,y1,x2,y1, color);
				LCD_DrawLine(x1,y1,x1,y2, color);
				LCD_DrawLine(x1,y2,x2,y2, color);
				LCD_DrawLine(x2,y1,x2,y2, color);				
			}			
		}	
	}
}

//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
u16 r=0,g=0,b=0;
u16 LCD_ReadPoint(u16 x,u16 y)
{
	
 	//u16 r=0,g=0,b=0;
	if( x>=lcddev.width || y>=lcddev.height )
		return 0;	//超过了范围,直接返回	
	
	LCD_SetCursor(x,y);	    
	LCD_WR_REG(0X2E);//9341/6804/3510/1963 发送读GRAM指令

 	r = LCD_RD_DATA();								//dummy Read	    
	//delay_nus(2);	  
 	r = LCD_RD_DATA();  		  						//实际坐标颜色
 	//delay_nus(2);	  
	
	b = LCD_RD_DATA(); 
	g = r & 0XFF;		//对于9341/5310/5510,第一次读取的是RG的值,R在前,G在后,各占8位
	g <<= 8;

	return ( ( (r>>11)<<11 ) | ( (g>>10)<<5 ) | (b>>11) );			//ILI9341/NT35310/NT35510需要公式转换一下

}

void Set_Dir(u8 dir)
{
	if((dir>>4)%4)
	{
		lcddev.width=320;
		lcddev.height=240;
	}else
	{
		lcddev.width=240;
		lcddev.height=320;
	}
}

//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size)
{
    u8 t, temp;
    u8 enshow = 0;

    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                LCD_ShowChar(x + (size / 2)*t, y, ' ', size, 0);
                continue;
            }
            else enshow = 1;

        }

        LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size, 0);
    }
}

//显示字符串
//x,y:起点坐标
//width,height:区域大小
//size:字体大小
//*p:字符串起始地址
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p)
{
    u8 x0 = x;
    width += x;
    height += y;

    while ((*p <= '~') && (*p >= ' '))   //判断是不是非法字符!
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }

        if (y >= height)break; //退出

        LCD_ShowChar(x, y, *p, size, 0);
        x += size / 2;
        p++;
    }
}
