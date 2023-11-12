#ifndef __CODE_DEF_H
#define __CODE_DEF_H
//�ṹ�廯�Ĵ����������Ĵ�����ַ����Ϊ�ṹ��ָ��
//(1)��д����ṹ�壺�Ҷ�����һ���ṹ�壬�ṹ����ı������ڴ��ʲô
#include <stdint.h>

/* �����������ƶ��� */
typedef uint32_t    u32;
typedef uint16_t    u16;
typedef uint8_t     u8;
typedef int32_t     s32;
typedef int16_t     s16;
typedef int8_t      s8;

//INTERRUPT DEF
#define NVIC_CTRL_ADDR (*(volatile unsigned *)0xe000e100)
	
/* SysTick Definitions */
typedef struct{
    volatile uint32_t CTRL;             // ���ƺ�״̬�Ĵ���        0xe000e010
    volatile uint32_t LOAD;             // �ؼ��ؼĴ���            0xe000e014
    volatile uint32_t VALUE;            // ��ǰֵ�Ĵ���            0xe000e018
    volatile uint32_t CALIB;            // У׼�Ĵ���              0xe000e01c
}SysTick_Type;

#define SysTick_BASE                 0xe000e010
#define SysTick                     ((SysTick_Type *)SysTick_BASE)

/* SCB Definitions */
typedef struct
{
  volatile uint32_t CPUID;                  
  volatile uint32_t ICSR;                   
  volatile uint32_t VTOR;                   
  volatile uint32_t AIRCR;                  
  volatile uint32_t SCR;                    
  volatile uint32_t CCR;                    
  volatile uint8_t  SHP[12];                
  volatile uint32_t SHCSR;                  
  volatile uint32_t CFSR;                   
  volatile uint32_t HFSR;                   
  volatile uint32_t DFSR;                   
  volatile uint32_t MMFAR;                  
  volatile uint32_t BFAR;                   
  volatile uint32_t AFSR;                   
  volatile uint32_t PFR[2];                 
  volatile uint32_t DFR;                    
  volatile uint32_t ADR;                    
  volatile uint32_t MMFR[4];                
  volatile uint32_t ISAR[5];                
  volatile uint32_t RESERVED0[5];
  volatile uint32_t CPACR;                  
}SCB_Type;

#define SCB_BASE                    0xe000ed00
#define SCB                         ((SCB_Type *)SCB_BASE)

/* LED Definitions--LED */
typedef struct{
    volatile uint32_t WaterLight_MODE;            // LED״̬ 1--��  0--��    0x40000000        W
	volatile uint32_t WaterLight_SPEED;
}LED_Type;

#define LED_BASE                    0x40000000
#define LED                         ((LED_Type *)LED_BASE)

/* VGA Definitions----VGA */
typedef struct{
    volatile uint32_t VGA_CLEAR;     //
	volatile uint32_t VGA_IMAGE;
	volatile uint32_t VGA_RDCOLOR; 	
}VGA_Type;

#define VGA_BASE             0x40000010
#define VGA                 ((VGA_Type *)VGA_BASE)

/* KEYBOARD Definitions----KEYBOARD */
typedef struct{
    volatile uint32_t KEY_STATE;     //
}KEY_Type;

#define KEY_BASE             0x40000020
#define KEY                 ((KEY_Type *)KEY_BASE)

/* HW Definitions----HW */
typedef struct{
    volatile uint32_t HW_STATE;     //
}HW_Type;

#define HW_BASE             0x40000030
#define HW                 ((HW_Type *)HW_BASE)

void LED_SetMode(uint32_t mode);
void LED_SetSpeed(uint32_t speed);

/* SEG4 Definitions--4λ����� */
typedef struct{
    volatile uint32_t disp_left1;       // ��ߵ�һλ��ʾ����   0x4000_0040      W
    volatile uint32_t disp_left2;       //                      0x4000_0044      W
    volatile uint32_t disp_left3;       //                      0x4000_0048      W
    volatile uint32_t disp_left4;       //                      0x4000_004C      W
    volatile uint32_t dp_position;      // С����λ��           0x4000_0050      W
}SEG4_Type;

#define SEG4_BASE                   0x40000040
#define SEG4                        ((SEG4_Type *)SEG4_BASE)

/* Timer Definitions--��ʱ�� */
typedef struct{
    volatile uint32_t load;             // ���ƺ�״̬�Ĵ���     0x4000_0060      W
    volatile uint32_t value;             // �ؼ��ؼĴ���         0x4000_0064      W
    volatile uint32_t ctrl;            // ��ǰֵ�Ĵ���         0x4000_0068      R
    volatile uint32_t clear;            // ����жϼĴ���       0x4000_006C      W //δ�õ�
}Timer_Type;

#define Timer_BASE                  0x40000060
#define Timer                       ((Timer_Type *)Timer_BASE)

/* LCD Definitions----LCD */
typedef struct {
    volatile uint32_t LCD_CS;           //                      0x4000_0070
    volatile uint32_t LCD_RS;           //                      0x4000_0074
    volatile uint32_t LCD_WR;           //                      0x4000_0078
    volatile uint32_t LCD_RD;           //                      0x4000_007C
    volatile uint32_t LCD_RST;          //                      0x4000_0080
    volatile uint32_t LCD_BL_CTR;       //                      0x4000_0084
    volatile uint32_t LCD_DATA;     //                      0x4000_0088
}LCD_Type;

#define LCD_BASE                    0x40000100
#define LCD                         ((LCD_Type *)LCD_BASE)

/* sut Definitions----sut */
typedef struct{
    volatile uint32_t SUT_STATE;     //
}SUT_Type;

#define SUT_BASE             0x40000070
#define SUT                 ((SUT_Type *)SUT_BASE)



/* LCD Definitions----LCD */
typedef struct {
    volatile uint32_t MUSIC_CTRL;           //                      0x4000_0090
    volatile uint32_t MUSIC_RESET;          //                      0x4000_0094
    volatile uint32_t MUSIC_SD_ADDR;        //                      0x4000_0098
    volatile uint32_t MUSIC_SD_SECTOR;      //                      0x4000_009C

}MUSIC_Type;

#define MUSIC_BASE                    0x40000090
#define MUSIC                         ((MUSIC_Type *)MUSIC_BASE)

#endif
