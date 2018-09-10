#include "ADC.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "LED.h"

/************************************************************/
/*                     初始化ADC转换                        */
/************************************************************/
void ADC_Init(void)
{
  	SIU.PCR[22].R=0X2000;		//AD		//设置PB[6]为ANP2
	SIU.PCR[23].R=0X2000;					//设置PB[7]为ANP3
	SIU.PCR[24].R=0X2000;					//设置PB[8]为ANS0
	SIU.PCR[25].R=0X2000;					//设置PB[9]为ANS1
	SIU.PCR[26].R=0X2000;					//设置PB[10]为ANS2
	SIU.PCR[27].R=0X2000;					//设置PB[11]为ANS3
  	
  	ADC.MCR.R=0x20000000;		//初始化ADC为扫描模式 AD时钟为32MHz
	ADC.NCMR[0].R=0x0000000C;   //Select ANP2-ANP3
	ADC.NCMR[1].R=0x0000000F;   //Select ANS0-ANP3
 	ADC.CTR[0].R=0x00008606;    // Conversion times for 32MHz ADClock
	ADC.MCR.B.NSTART=1; 		//启动AD转换	
}
///延时函数
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
/*                       获取通道2的值                      */
/************************************************************/
int AD_Value_CH2(void)
{
	while(ADC.CDR[2].B.VALID!=1);		//等待CH2转换结果有效
	return (ADC.CDR[2].B.CDATA);    	//读取CH2的转换结果		
}
/************************************************************/
/*                     获取通道2的平均值                    */
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
/*                       获取通道3的值                      */
/************************************************************/
int AD_Value_CH3(void)
{
	while(ADC.CDR[3].B.VALID!=1);		//等待CH3转换结果有效
	return (ADC.CDR[3].B.CDATA);    	//读取CH3的转换结果		
}
/************************************************************/
/*                     获取通道3的平均值                    */
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
/*                       获取通道32的值                      */
/************************************************************/
int AD_Value_CH32(void)
{
	while(ADC.CDR[32].B.VALID!=1);		//等待CH32转换结果有效
	return (ADC.CDR[32].B.CDATA);    	//读取CH32的转换结果		
}
/************************************************************/
/*                     获取通道32的平均值                    */
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
/*                       获取通道33的值                      */
/************************************************************/
int AD_Value_CH33(void)
{
	while(ADC.CDR[33].B.VALID!=1);		//等待CH33转换结果有效
	return (ADC.CDR[33].B.CDATA);    	//读取CH33的转换结果		
}
/************************************************************/
/*                     获取通道33的平均值                    */
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
/*                       获取通道34的值                      */
/************************************************************/
int AD_Value_CH34(void)
{
	while(ADC.CDR[34].B.VALID!=1);		//等待CH34转换结果有效
	return (ADC.CDR[34].B.CDATA);    	//读取CH34的转换结果		
}
/************************************************************/
/*                     获取通道34的平均值                    */
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
/*                       获取通道35的值                      */
/************************************************************/
int AD_Value_CH35(void)
{
	while(ADC.CDR[35].B.VALID!=1);		//等待CH35转换结果有效
	return (ADC.CDR[35].B.CDATA);    	//读取CH35的转换结果		
}
/************************************************************/
/*                     获取通道35的平均值                    */
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