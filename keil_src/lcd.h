#ifndef __LCD_H
#define __LCD_H

#include "code_def.h"

//  LCD 
typedef struct  
{										    
	uint16_t width;			
	uint16_t height;			
	uint16_t id;				
	uint8_t  dir;			
	uint16_t wramcmd;		
	uint16_t setxcmd;		
	uint16_t setycmd;		
}_lcd_dev; 	  

extern _lcd_dev lcddev;

extern uint16_t  POINT_COLOR;  
extern uint16_t  BACK_COLOR; 


// 字体大小 即字号
#define WORD_SIZE       12      // 12x12
// 字符显示模式
#define OVERLAY         1       // 叠加   即没有字的背景色
#define NO_OVERLAY      0       // 非叠加 


//      BASIC SIGNAL SET AND CLEAR
#define LCD_CS_SET         (LCD->LCD_CS        = 1) 	 
#define LCD_RS_SET         (LCD->LCD_RS        = 1) 
#define LCD_WR_SET         (LCD->LCD_WR        = 1) 
#define LCD_RD_SET         (LCD->LCD_RD        = 1) 
#define LCD_RST_SET        (LCD->LCD_RST       = 1)
#define LCD_BL_CTR_SET     (LCD->LCD_BL_CTR    = 1)
     
#define LCD_CS_CLR         (LCD->LCD_CS        = 0) 	 
#define LCD_RS_CLR         (LCD->LCD_RS        = 0) 
#define LCD_WR_CLR         (LCD->LCD_WR        = 0) 
#define LCD_RD_CLR         (LCD->LCD_RD        = 0) 
#define LCD_RST_CLR        (LCD->LCD_RST       = 0)
#define LCD_BL_CTR_CLR     (LCD->LCD_BL_CTR    = 0)
     
//      DATA     
#define LCD_DATA0_SET( x )   (LCD->LCD_DATA[0]   = (x))
#define LCD_DATA1_SET( x )   (LCD->LCD_DATA[1]   = (x)) 	  	 
#define LCD_DATA2_SET( x )   (LCD->LCD_DATA[2]   = (x)) 	 
#define LCD_DATA3_SET( x )   (LCD->LCD_DATA[3]   = (x)) 	 
#define LCD_DATA4_SET( x )   (LCD->LCD_DATA[4]   = (x)) 	 
#define LCD_DATA5_SET( x )   (LCD->LCD_DATA[5]   = (x)) 	 
#define LCD_DATA6_SET( x )   (LCD->LCD_DATA[6]   = (x)) 	 
#define LCD_DATA7_SET( x )   (LCD->LCD_DATA[7]   = (x)) 	 
#define LCD_DATA8_SET( x )   (LCD->LCD_DATA[8]   = (x)) 	 
#define LCD_DATA9_SET( x )   (LCD->LCD_DATA[9]   = (x)) 	 
#define LCD_DATA10_SET( x )  (LCD->LCD_DATA[10]  = (x)) 	 
#define LCD_DATA11_SET( x )  (LCD->LCD_DATA[11]  = (x)) 	 
#define LCD_DATA12_SET( x )  (LCD->LCD_DATA[12]  = (x)) 	 
#define LCD_DATA13_SET( x )  (LCD->LCD_DATA[13]  = (x)) 	 
#define LCD_DATA14_SET( x )  (LCD->LCD_DATA[14]  = (x)) 	 
#define LCD_DATA15_SET( x )  (LCD->LCD_DATA[15]  = (x)) 	 

#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
MakeData(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 

//      SCANNING DIRECTION
#define L2R_U2D  0 // LEFT TO RIGHT, UP TO DOWN //从左到右,从上到下（正向）
#define L2R_D2U  1 // LEFT TO RIGHT, DOWN TO UP //从左到右,从下到上
#define R2L_U2D  2 // RIGHT TO LEFT, UP TO DOWN //从右到左,从上到下
#define R2L_D2U  3 // RIGHT TO LEFT, DOWN TO UP //从右到左,从下到上（旋转180度）

#define U2D_L2R  4 // UP TO DOWN, LEFT TO RIGHT //从上到下,从左到右
#define U2D_R2L  5 // UP TO DOWN, RIGHT TO LEFT //从上到下,从右到左（旋转90度）
#define D2U_L2R  6 // DOWN TO UP, LEFT TO RIGHT //从下到上,从左到右（旋转270度）
#define D2U_R2L  7 // DOWN TO UP, RIGHT TO LEFT //从下到上,从右到左

//#define DFT_SCAN_DIR    R2L_U2D // DEFAULT
#define DFT_SCAN_DIR L2R_U2D

#define LCD_WORD_COLOR				RED
#define LCD_BACKGROUND_COLOR		WHITE

//  PEN COLOR
#define WHITE_LCD         	 0xFFFF
#define BLACK_LCD         	 0x0000	  
#define BLUE_LCD         	 0x001F  
#define BRED_LCD             0XF81F
#define GRED_LCD 			 0XFFE0
#define GBLUE_LCD			 0X07FF
#define RED_LCD           	 0xF800
#define MAGENTA_LCD       	 0xF81F
#define GREEN_LCD         	 0x07E0
#define CYAN_LCD          	 0x7FFF
#define YELLOW_LCD        	 0xFFE0
#define BROWN_LCD 			 0XBC40 
#define BRRED_LCD 			 0XFC07 
#define GRAY_LCD  			 0X8430  

//  GUI COLOR ( COLOR OF PANEL )
#define DARKBLUE      	 0X01CF	
#define LIGHTBLUE      	 0X7D7C	 
#define GRAYBLUE       	 0X5458 


#define LIGHTGREEN     	 0X841F 
#define LGRAY 			 0XC618 // BACKGROUND COLOR OF WINDOW

#define LGRAYBLUE        0XA651 // MIDDLE LAYER COLOR
#define LBBLUE           0X2B12 // COLOR OF SWITCHED

//  LCD RESOLUTION
#define SSD_HOR_RESOLUTION		800		// LCD HORIZON RESOLUTION
#define SSD_VER_RESOLUTION		480		// LCD VERTICAL RESOLUTION

//  LCD DRIVING PARAMETER
#define SSD_HOR_PULSE_WIDTH		1		
#define SSD_HOR_BACK_PORCH		46		
#define SSD_HOR_FRONT_PORCH		210		

#define SSD_VER_PULSE_WIDTH		1		
#define SSD_VER_BACK_PORCH		23		
#define SSD_VER_FRONT_PORCH		22		

//  AUTO CALCULATE
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)  	

void LCD_Init(void);									
void LCD_DisplayOn(void);													
void LCD_DisplayOff(void);													
void LCD_Clear(uint16_t Color);	 											
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);							
//void LCD_DrawPoint(uint16_t x,uint16_t y);	
void LCD_DrawPoint(u16 x,u16 y, u16 color);
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);							
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); 										
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);						 		
//void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);					
//void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);	
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u8 width, u8 dir, u16 color);
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);		   	
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);

void LCD_ShowNum(u16 x, u16 y, u32 num, u8 len, u8 size); 
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode); 
//void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t size,uint8_t mode);
//void LCD_ShowIntNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t size,uint8_t mode);					
//void LCD_ShowFloatNum(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t size,uint8_t mode);				
//void LCD_ShowString(uint16_t x,uint16_t y,const u8 *p,uint16_t fc,uint16_t bc,uint16_t size,uint16_t mode);	
void LCD_ShowString(u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p);

void LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t LCD_ReadReg(uint16_t LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);
void LCD_SSD_BackLightSet(uint8_t pwm);							
void LCD_Scan_Dir(uint8_t dir);									
void LCD_Display_Dir(uint8_t dir);
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
void Set_Dir(u8 dir);

#endif


