#include "ADC.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "LED.h"

/************************************************************/
/*                     ��ʼ��ADCת��                        */
/************************************************************/
void ADC_Init(void)
{
  	SIU.PCR[22].R=0X2000;		//AD		//����PB[6]ΪANP2
	SIU.PCR[23].R=0X2000;					//����PB[7]ΪANP3
	SIU.PCR[24].R=0X2000;					//����PB[8]ΪANS0
	SIU.PCR[25].R=0X2000;					//����PB[9]ΪANS1
	SIU.PCR[26].R=0X2000;					//����PB[10]ΪANS2
	SIU.PCR[27].R=0X2000;					//����PB[11]ΪANS3
  	
  	ADC.MCR.R=0x20000000;		//��ʼ��ADCΪɨ��ģʽ ADʱ��Ϊ32MHz
	ADC.NCMR[0].R=0x0000000C;   //Select ANP2-ANP3
	ADC.NCMR[1].R=0x0000000F;   //Select ANS0-ANP3
 	ADC.CTR[0].R=0x00008606;    // Conversion times for 32MHz ADClock
	ADC.MCR.B.NSTART=1; 		//����ADת��	
}
///��ʱ����
void delayBanking(uint16_t time)
{
	while(time--)
	{
		

    	 asm
    	{
	     	nop;
    	}
   	}
}

/************************************************************/
/*                       ��ȡͨ��2��ֵ                      */
/************************************************************/
int AD_Value_CH2(void)
{
	while(ADC.CDR[2].B.VALID!=1);		//�ȴ�CH2ת�������Ч
	return (ADC.CDR[2].B.CDATA);    	//��ȡCH2��ת�����		
}
/************************************************************/
/*                     ��ȡͨ��2��ƽ��ֵ                    */
/************************************************************/
int AD_Average_CH2(int times)
{
	int temp_val=0;
	int t;
	for(t=0;t<times;t++)
	{
		temp_val+=AD_Value_CH2();	
	}
	delayBanking(5);
	return (temp_val/times);
}

/************************************************************/
/*                       ��ȡͨ��3��ֵ                      */
/************************************************************/
int AD_Value_CH3(void)
{
	while(ADC.CDR[3].B.VALID!=1);		//�ȴ�CH3ת�������Ч
	return (ADC.CDR[3].B.CDATA);    	//��ȡCH3��ת�����		
}
/************************************************************/
/*                     ��ȡͨ��3��ƽ��ֵ                    */
/************************************************************/
int AD_Average_CH3(int times)
{
	int temp_val=0;
	int t;
	for(t=0;t<times;t++)
	{
		temp_val+=AD_Value_CH3();	
	}
	delayBanking(5);
	return (temp_val/times);
}

/************************************************************/
/*                       ��ȡͨ��32��ֵ                      */
/************************************************************/
int AD_Value_CH32(void)
{
	while(ADC.CDR[32].B.VALID!=1);		//�ȴ�CH32ת�������Ч
	return (ADC.CDR[32].B.CDATA);    	//��ȡCH32��ת�����		
}
/************************************************************/
/*                     ��ȡͨ��32��ƽ��ֵ                    */
/************************************************************/
int AD_Average_CH32(int times)
{
	int temp_val=0;
	int t;
	for(t=0;t<times;t++)
	{
		temp_val+=AD_Value_CH32();	
	}
	delayBanking(5);
	return (temp_val/times);
}

/************************************************************/
/*                       ��ȡͨ��33��ֵ                      */
/************************************************************/
int AD_Value_CH33(void)
{
	while(ADC.CDR[33].B.VALID!=1);		//�ȴ�CH33ת�������Ч
	return (ADC.CDR[33].B.CDATA);    	//��ȡCH33��ת�����		
}
/************************************************************/
/*                     ��ȡͨ��33��ƽ��ֵ                    */
/************************************************************/
int AD_Average_CH33(int times)
{
	int temp_val=0;
	int t;
	for(t=0;t<times;t++)
	{
		temp_val+=AD_Value_CH33();	
	}
	delayBanking(5);
	return (temp_val/times);
}

/************************************************************/
/*                       ��ȡͨ��34��ֵ                      */
/************************************************************/
int AD_Value_CH34(void)
{
	while(ADC.CDR[34].B.VALID!=1);		//�ȴ�CH34ת�������Ч
	return (ADC.CDR[34].B.CDATA);    	//��ȡCH34��ת�����		
}
/************************************************************/
/*                     ��ȡͨ��34��ƽ��ֵ                    */
/************************************************************/
int AD_Average_CH34(int times)
{
	int temp_val=0;
	int t;
	for(t=0;t<times;t++)
	{
		temp_val+=AD_Value_CH34();	
	}
	delayBanking(5);
	return (temp_val/times);
}

/************************************************************/
/*                       ��ȡͨ��35��ֵ                      */
/************************************************************/
int AD_Value_CH35(void)
{
	while(ADC.CDR[35].B.VALID!=1);		//�ȴ�CH35ת�������Ч
	return (ADC.CDR[35].B.CDATA);    	//��ȡCH35��ת�����		
}
/************************************************************/
/*                     ��ȡͨ��35��ƽ��ֵ                    */
/************************************************************/
int AD_Average_CH35(int times)
{
	int temp_val=0;
	int t;
	for(t=0;t<times;t++)
	{
		temp_val+=AD_Value_CH35();	
	}
	delayBanking(5);
	return (temp_val/times);
}