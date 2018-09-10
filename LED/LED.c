#include "LED.h"


void LED_Init(void)
{
//�Ĵ������ã��ο�MPC560XPRM�Ĵ����ο��ֲ�.pdf��280ҳ�Ĵ���˵������
//PCR:pad configuration register
//��100�ŵķ�װ�У���PCR[0..78]
//PCR[0:15]=port A;     PA0......PA15
//PCR[16:31]=port B;    PB0......PB15
//PCR[32:47]=port C     PC0......PC15
//PCR[48:63]=port D     PD0......PD15
//PCR[64:76]=port E     PE0......PE12
//PCR[77:78]=port H     PH9��PH10
//PB[4:7]��PC[0��11] ��ADC��ڣ�����ΪADCʱ��Ҫ��������  
  SIU.PCR[12].R = 0x0220;               // ����PA[12]Ϊ��©���
  SIU.PCR[13].R = 0x0220;               // ����PA[13]Ϊ��©���
//  SIU.PCR[14].R = 0x0220;               // ����PA[14]Ϊ��©���  ���ж�ռ����
  SIU.PCR[15].R = 0x0220;               // ����PA[15]Ϊ��©���  	
  
  SIU.PCR[72].R = 0x0220;               // ����E8Ϊ��©���
  SIU.PCR[73].R = 0x0220;               // ����E9Ϊ��©���
  SIU.PCR[74].R = 0x0220;               // ����E10Ϊ��©���
  SIU.PCR[75].R = 0x0220;               // ����E11Ϊ��©���    
  
  
  SIU.PCR[28].R = 0x0100;               //����PB[12]Ϊ����    ��λ4
  SIU.PCR[29].R = 0x0100;               //����PB[13]Ϊ����    ��λ3
  SIU.PCR[30].R = 0x0100;               //����PB[14]Ϊ����    ��λ2
  SIU.PCR[31].R = 0x0100;               //����PB[15]Ϊ����    ��λ1
  
  
  LED0 = 1;	
  LED1 = 1;		
//  LED2 = 1;	
  LED3 = 1;	
  
  
  LED4 = 1;	
  LED5 = 1;		
  LED6 = 1;	
  LED7 = 1;	  
  
  
  		
}