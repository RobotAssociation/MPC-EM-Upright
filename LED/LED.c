#include "LED.h"


void LED_Init(void)
{
//寄存器设置，参考MPC560XPRM寄存器参考手册.pdf第280页寄存器说明即可
//PCR:pad configuration register
//在100脚的封装中，是PCR[0..78]
//PCR[0:15]=port A;     PA0......PA15
//PCR[16:31]=port B;    PB0......PB15
//PCR[32:47]=port C     PC0......PC15
//PCR[48:63]=port D     PD0......PD15
//PCR[64:76]=port E     PE0......PE12
//PCR[77:78]=port H     PH9，PH10
//PB[4:7]和PC[0：11] 是ADC入口，在作为ADC时，要单独配置  
  SIU.PCR[12].R = 0x0220;               // 设置PA[12]为开漏输出
  SIU.PCR[13].R = 0x0220;               // 设置PA[13]为开漏输出
//  SIU.PCR[14].R = 0x0220;               // 设置PA[14]为开漏输出  被中断占用了
  SIU.PCR[15].R = 0x0220;               // 设置PA[15]为开漏输出  	
  
  SIU.PCR[72].R = 0x0220;               // 设置E8为开漏输出
  SIU.PCR[73].R = 0x0220;               // 设置E9为开漏输出
  SIU.PCR[74].R = 0x0220;               // 设置E10为开漏输出
  SIU.PCR[75].R = 0x0220;               // 设置E11为开漏输出    
  
  
  SIU.PCR[28].R = 0x0100;               //设置PB[12]为输入    档位4
  SIU.PCR[29].R = 0x0100;               //设置PB[13]为输入    档位3
  SIU.PCR[30].R = 0x0100;               //设置PB[14]为输入    档位2
  SIU.PCR[31].R = 0x0100;               //设置PB[15]为输入    档位1
  
  
  LED0 = 1;	
  LED1 = 1;		
//  LED2 = 1;	
  LED3 = 1;	
  
  
  LED4 = 1;	
  LED5 = 1;		
  LED6 = 1;	
  LED7 = 1;	  
  
  
  		
}