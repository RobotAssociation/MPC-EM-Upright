#include "ImageSample.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "Interrupt.h"
#include "LED.h"
/////////图像数据缓存数组//////////////
uint8_t Buffer1[IMAGE_ROW][IMAGE_COLUMN];
uint8_t Buffer2[IMAGE_ROW][IMAGE_COLUMN];

//指向当前采集数据采样缓存首地址的指针/
uint8_t *pucSample = &Buffer1[0][0];

//指向当前处理数据采样缓存首地址的指针/
uint8_t *pucProcess = &Buffer2[0][0];


////////用于图像采集和处理交换缓存/////
//（注意：在每次交换指针后保证puca_BufferTemp与puca_BufferSample相同）
uint8_t *pucTemp = &Buffer1[0][0];




uint8_t GetImage[IMAGE_ROW] =      //每行采集的行
{
	15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,
	90,95,100,105,110,115,120,125,130,135,140,145,150,
	155,160,165,170,175,180,185,190,195,200,205,210
};
/*
uint8_t GetImage[IMAGE_ROW] =      //每行采集的行
{
	7,10,13,15,18,21,24,27,30,33,36,39,42,45,48,
	51,54,57,60,63,66,69,72,75,78,81,84,87,
	90,93,95,98,101,104,107,120,123,126,129,132
};
*/
uint16_t c_InterruptLine = 0;          //行中断计数
uint8_t  c_TureLine = 0;               //实际采样行计数
uint8_t  c_jiange = 0;
uint8_t  c_hang = 0;                   //行中断里面用的记录每行点的个数的变量
uint16_t s_speed = 0;                  //读取的速度值


//*****************************************************************************
///////////////////////////////标志位定义//////////////////////////////////////		 
//*****************************************************************************
uint8_t SampleOver_Flag = 0;           //采样结束标志置位  
uint8_t SampleFlag = 0;                //奇偶场标志


void Camera_Init(void)
{
/////////摄像头信号输入口PA7~PA0//////////////////////////////    
	SIU.PCR[7].R = 0x0103;             //PA7输入
    SIU.PCR[6].R = 0x0103; 
    SIU.PCR[5].R = 0x0103; 
    SIU.PCR[4].R = 0x0103;
    SIU.PCR[3].R = 0x0103;
    SIU.PCR[2].R = 0x0103; 
    SIU.PCR[1].R = 0x0103; 
    SIU.PCR[0].R = 0x0103;              //PA0输入 	
    
    INTC.MCR.B.HVEN = 1;               //硬件向量模式  向量地址4个字节
    
    SIU.IRER.B.EIRE4    = 0;           //EIRQ[4]禁止
    SIU.IRER.B.EIRE8    = 0;           //EIRQ[8]禁止
    
    SIU.IREER.B.IREE4 = 1;             //中断下降沿使能寄存器--0口（PA14）上升沿有效,行中断
    SIU.IFEER.B.IFEE8 = 1;             //中断上升沿使能寄存器--1口（PC14）下降沿有效，场中断    
    
    SIU.PCR[14].R = 0X0103;            //配置PA[14]引脚为EIRQ4功能--IRQ0组
    SIU.PCR[46].R = 0X0103;            //配置PC[14]引脚为EIRQ8功能--IRQ1组 
    
    INTC.PSR[41].R = 0x05;             //配置中断优先级，必须的,行中断优先级高	
    INTC.PSR[42].R = 0x04;  
    SIU.IRER.B.EIRE4    = 1;           //使能中断
    SIU.IRER.B.EIRE8    = 1;           //使能中断                   	
}

/*****************************************************************************/
//函数名：HREF_ISR                                                                 
//功  能：行中断函数，用于采集每行的像素点灰度值                                                                   
//参  数：无                                                                    
//返回值：无                                                                    
//说  明：采集                                                                   
/*****************************************************************************/
void HREF_ISR(void)
{
	uint8_t *pucSampleTemp = (void*)0; //赋初值空指针，指向实际采样点
	
 	SIU.ISR.B.EIF4=1;                    //清行中断标志位 放在最后面有问题？   	
	c_InterruptLine++;


	
	if(c_InterruptLine < ROW_START || c_InterruptLine > ROW_MAX)   //	SampleFlag = 0 || 
	{
		return;	     //不是有效行，直接跳出行中断
	}	
	if (c_InterruptLine == GetImage[c_TureLine])   //是所需采集行
	{
		pucSampleTemp = pucSample + c_TureLine*IMAGE_COLUMN;//指向每行的首地址

		*(pucSampleTemp+0) = IMAGE_DATAPORT; 
		*(pucSampleTemp+0) = IMAGE_DATAPORT;
		*(pucSampleTemp+1) = IMAGE_DATAPORT;
		*(pucSampleTemp+1) = IMAGE_DATAPORT;
		*(pucSampleTemp+2) = IMAGE_DATAPORT;
		*(pucSampleTemp+2) = IMAGE_DATAPORT;   
		*(pucSampleTemp+3) = IMAGE_DATAPORT;
		*(pucSampleTemp+3) = IMAGE_DATAPORT;
		*(pucSampleTemp+4) = IMAGE_DATAPORT;
		*(pucSampleTemp+4) = IMAGE_DATAPORT;
		*(pucSampleTemp+5) = IMAGE_DATAPORT;
		*(pucSampleTemp+5) = IMAGE_DATAPORT;
		*(pucSampleTemp+6) = IMAGE_DATAPORT;
		*(pucSampleTemp+6) = IMAGE_DATAPORT;	
		*(pucSampleTemp+7) = IMAGE_DATAPORT;
		*(pucSampleTemp+7) = IMAGE_DATAPORT;
		*(pucSampleTemp+8) = IMAGE_DATAPORT;
		*(pucSampleTemp+8) = IMAGE_DATAPORT;
		*(pucSampleTemp+9) = IMAGE_DATAPORT;
		*(pucSampleTemp+9) = IMAGE_DATAPORT;

		*(pucSampleTemp+10) = IMAGE_DATAPORT; 
		*(pucSampleTemp+10) = IMAGE_DATAPORT;
		*(pucSampleTemp+11) = IMAGE_DATAPORT;
		*(pucSampleTemp+11) = IMAGE_DATAPORT;
		*(pucSampleTemp+12) = IMAGE_DATAPORT;
		*(pucSampleTemp+12) = IMAGE_DATAPORT;   
		*(pucSampleTemp+13) = IMAGE_DATAPORT;
		*(pucSampleTemp+13) = IMAGE_DATAPORT;
		*(pucSampleTemp+14) = IMAGE_DATAPORT;
		*(pucSampleTemp+14) = IMAGE_DATAPORT;
		*(pucSampleTemp+15) = IMAGE_DATAPORT;
		*(pucSampleTemp+15) = IMAGE_DATAPORT;
		*(pucSampleTemp+16) = IMAGE_DATAPORT;
		*(pucSampleTemp+16) = IMAGE_DATAPORT;	
		*(pucSampleTemp+17) = IMAGE_DATAPORT;
		*(pucSampleTemp+17) = IMAGE_DATAPORT;
		*(pucSampleTemp+18) = IMAGE_DATAPORT;
		*(pucSampleTemp+18) = IMAGE_DATAPORT;
		*(pucSampleTemp+19) = IMAGE_DATAPORT;
		*(pucSampleTemp+19) = IMAGE_DATAPORT;

		*(pucSampleTemp+20) = IMAGE_DATAPORT; 
		*(pucSampleTemp+20) = IMAGE_DATAPORT;
		*(pucSampleTemp+21) = IMAGE_DATAPORT;
		*(pucSampleTemp+21) = IMAGE_DATAPORT;
		*(pucSampleTemp+22) = IMAGE_DATAPORT;
		*(pucSampleTemp+22) = IMAGE_DATAPORT;   
		*(pucSampleTemp+23) = IMAGE_DATAPORT;
		*(pucSampleTemp+23) = IMAGE_DATAPORT;
		*(pucSampleTemp+24) = IMAGE_DATAPORT;
		*(pucSampleTemp+24) = IMAGE_DATAPORT;
		*(pucSampleTemp+25) = IMAGE_DATAPORT;
		*(pucSampleTemp+25) = IMAGE_DATAPORT;
		*(pucSampleTemp+26) = IMAGE_DATAPORT;
		*(pucSampleTemp+26) = IMAGE_DATAPORT;	
		*(pucSampleTemp+27) = IMAGE_DATAPORT;
		*(pucSampleTemp+27) = IMAGE_DATAPORT;
		*(pucSampleTemp+28) = IMAGE_DATAPORT;
		*(pucSampleTemp+28) = IMAGE_DATAPORT;
		*(pucSampleTemp+29) = IMAGE_DATAPORT;
		*(pucSampleTemp+29) = IMAGE_DATAPORT;
		
		*(pucSampleTemp+30) = IMAGE_DATAPORT; 
		*(pucSampleTemp+30) = IMAGE_DATAPORT;
		*(pucSampleTemp+31) = IMAGE_DATAPORT;
		*(pucSampleTemp+31) = IMAGE_DATAPORT;
		*(pucSampleTemp+32) = IMAGE_DATAPORT;
		*(pucSampleTemp+32) = IMAGE_DATAPORT;   
		*(pucSampleTemp+33) = IMAGE_DATAPORT;
		*(pucSampleTemp+33) = IMAGE_DATAPORT;
		*(pucSampleTemp+34) = IMAGE_DATAPORT;
		*(pucSampleTemp+34) = IMAGE_DATAPORT;
		*(pucSampleTemp+35) = IMAGE_DATAPORT;
		*(pucSampleTemp+35) = IMAGE_DATAPORT;
		*(pucSampleTemp+36) = IMAGE_DATAPORT;
		*(pucSampleTemp+36) = IMAGE_DATAPORT;	
		*(pucSampleTemp+37) = IMAGE_DATAPORT;
		*(pucSampleTemp+37) = IMAGE_DATAPORT;
		*(pucSampleTemp+38) = IMAGE_DATAPORT;
		*(pucSampleTemp+38) = IMAGE_DATAPORT;
		*(pucSampleTemp+39) = IMAGE_DATAPORT;
		*(pucSampleTemp+39) = IMAGE_DATAPORT;
////////////采三次，延时，将中间数据压缩////////////		
		*(pucSampleTemp+40) = IMAGE_DATAPORT; 
		*(pucSampleTemp+40) = IMAGE_DATAPORT;
		*(pucSampleTemp+40) = IMAGE_DATAPORT;
		*(pucSampleTemp+41) = IMAGE_DATAPORT;
		*(pucSampleTemp+41) = IMAGE_DATAPORT;
		*(pucSampleTemp+41) = IMAGE_DATAPORT;
		*(pucSampleTemp+42) = IMAGE_DATAPORT;
		*(pucSampleTemp+42) = IMAGE_DATAPORT;
		*(pucSampleTemp+42) = IMAGE_DATAPORT;   
		*(pucSampleTemp+43) = IMAGE_DATAPORT;
		*(pucSampleTemp+43) = IMAGE_DATAPORT;
		*(pucSampleTemp+43) = IMAGE_DATAPORT;
		*(pucSampleTemp+44) = IMAGE_DATAPORT;
		*(pucSampleTemp+44) = IMAGE_DATAPORT;
		*(pucSampleTemp+44) = IMAGE_DATAPORT;
		*(pucSampleTemp+45) = IMAGE_DATAPORT;
		*(pucSampleTemp+45) = IMAGE_DATAPORT;
		*(pucSampleTemp+45) = IMAGE_DATAPORT;
		*(pucSampleTemp+46) = IMAGE_DATAPORT;
		*(pucSampleTemp+46) = IMAGE_DATAPORT;
		*(pucSampleTemp+46) = IMAGE_DATAPORT;	
		*(pucSampleTemp+47) = IMAGE_DATAPORT;
		*(pucSampleTemp+47) = IMAGE_DATAPORT;
		*(pucSampleTemp+47) = IMAGE_DATAPORT;
		*(pucSampleTemp+48) = IMAGE_DATAPORT;
		*(pucSampleTemp+48) = IMAGE_DATAPORT;
		*(pucSampleTemp+48) = IMAGE_DATAPORT;
		*(pucSampleTemp+49) = IMAGE_DATAPORT;
		*(pucSampleTemp+49) = IMAGE_DATAPORT;
		*(pucSampleTemp+49) = IMAGE_DATAPORT;
		
		*(pucSampleTemp+50) = IMAGE_DATAPORT; 
		*(pucSampleTemp+50) = IMAGE_DATAPORT;
		*(pucSampleTemp+50) = IMAGE_DATAPORT;
		*(pucSampleTemp+51) = IMAGE_DATAPORT;
		*(pucSampleTemp+51) = IMAGE_DATAPORT;
		*(pucSampleTemp+51) = IMAGE_DATAPORT;
		*(pucSampleTemp+52) = IMAGE_DATAPORT;		
		*(pucSampleTemp+52) = IMAGE_DATAPORT;
		*(pucSampleTemp+52) = IMAGE_DATAPORT;   
		*(pucSampleTemp+53) = IMAGE_DATAPORT;
		*(pucSampleTemp+53) = IMAGE_DATAPORT;
		*(pucSampleTemp+53) = IMAGE_DATAPORT;
		*(pucSampleTemp+54) = IMAGE_DATAPORT;
		*(pucSampleTemp+54) = IMAGE_DATAPORT;
		*(pucSampleTemp+54) = IMAGE_DATAPORT;
		*(pucSampleTemp+55) = IMAGE_DATAPORT;
		*(pucSampleTemp+55) = IMAGE_DATAPORT;
		*(pucSampleTemp+55) = IMAGE_DATAPORT;
		*(pucSampleTemp+56) = IMAGE_DATAPORT;
		*(pucSampleTemp+56) = IMAGE_DATAPORT;
		*(pucSampleTemp+56) = IMAGE_DATAPORT;	
		*(pucSampleTemp+57) = IMAGE_DATAPORT;
		*(pucSampleTemp+57) = IMAGE_DATAPORT;
		*(pucSampleTemp+57) = IMAGE_DATAPORT;
		*(pucSampleTemp+58) = IMAGE_DATAPORT;
		*(pucSampleTemp+58) = IMAGE_DATAPORT;
		*(pucSampleTemp+58) = IMAGE_DATAPORT;
		*(pucSampleTemp+59) = IMAGE_DATAPORT;
		*(pucSampleTemp+59) = IMAGE_DATAPORT;
		*(pucSampleTemp+59) = IMAGE_DATAPORT;
		
		*(pucSampleTemp+60) = IMAGE_DATAPORT; 
		*(pucSampleTemp+60) = IMAGE_DATAPORT;
		*(pucSampleTemp+60) = IMAGE_DATAPORT;
		*(pucSampleTemp+61) = IMAGE_DATAPORT;
		*(pucSampleTemp+61) = IMAGE_DATAPORT;
		*(pucSampleTemp+61) = IMAGE_DATAPORT;
		*(pucSampleTemp+62) = IMAGE_DATAPORT;
		*(pucSampleTemp+62) = IMAGE_DATAPORT;
		*(pucSampleTemp+62) = IMAGE_DATAPORT;   
		*(pucSampleTemp+63) = IMAGE_DATAPORT;
		*(pucSampleTemp+63) = IMAGE_DATAPORT;
		*(pucSampleTemp+63) = IMAGE_DATAPORT;
		*(pucSampleTemp+64) = IMAGE_DATAPORT;
		*(pucSampleTemp+64) = IMAGE_DATAPORT;
		*(pucSampleTemp+64) = IMAGE_DATAPORT;
		*(pucSampleTemp+65) = IMAGE_DATAPORT;
		*(pucSampleTemp+65) = IMAGE_DATAPORT;
		*(pucSampleTemp+65) = IMAGE_DATAPORT;
		*(pucSampleTemp+66) = IMAGE_DATAPORT;
		*(pucSampleTemp+66) = IMAGE_DATAPORT;
		*(pucSampleTemp+66) = IMAGE_DATAPORT;	
		*(pucSampleTemp+67) = IMAGE_DATAPORT;
		*(pucSampleTemp+67) = IMAGE_DATAPORT;
		*(pucSampleTemp+67) = IMAGE_DATAPORT;
		*(pucSampleTemp+68) = IMAGE_DATAPORT;
		*(pucSampleTemp+68) = IMAGE_DATAPORT;
		*(pucSampleTemp+68) = IMAGE_DATAPORT;
		*(pucSampleTemp+69) = IMAGE_DATAPORT;
		*(pucSampleTemp+69) = IMAGE_DATAPORT;
		*(pucSampleTemp+69) = IMAGE_DATAPORT;
		
		*(pucSampleTemp+70) = IMAGE_DATAPORT; 
		*(pucSampleTemp+70) = IMAGE_DATAPORT;
		*(pucSampleTemp+70) = IMAGE_DATAPORT;
		*(pucSampleTemp+71) = IMAGE_DATAPORT;
		*(pucSampleTemp+71) = IMAGE_DATAPORT;
		*(pucSampleTemp+71) = IMAGE_DATAPORT;		
		*(pucSampleTemp+72) = IMAGE_DATAPORT;
		*(pucSampleTemp+72) = IMAGE_DATAPORT;
		*(pucSampleTemp+72) = IMAGE_DATAPORT;		   
		*(pucSampleTemp+73) = IMAGE_DATAPORT;
		*(pucSampleTemp+73) = IMAGE_DATAPORT;
		*(pucSampleTemp+73) = IMAGE_DATAPORT;		
		*(pucSampleTemp+74) = IMAGE_DATAPORT;
		*(pucSampleTemp+74) = IMAGE_DATAPORT;
		*(pucSampleTemp+74) = IMAGE_DATAPORT;		
		*(pucSampleTemp+75) = IMAGE_DATAPORT;
		*(pucSampleTemp+75) = IMAGE_DATAPORT;
		*(pucSampleTemp+75) = IMAGE_DATAPORT;		
		*(pucSampleTemp+76) = IMAGE_DATAPORT;
		*(pucSampleTemp+76) = IMAGE_DATAPORT;
		*(pucSampleTemp+76) = IMAGE_DATAPORT;			
		*(pucSampleTemp+77) = IMAGE_DATAPORT;
		*(pucSampleTemp+77) = IMAGE_DATAPORT;
		*(pucSampleTemp+77) = IMAGE_DATAPORT;		
		*(pucSampleTemp+78) = IMAGE_DATAPORT;
		*(pucSampleTemp+78) = IMAGE_DATAPORT;
		*(pucSampleTemp+78) = IMAGE_DATAPORT;		
		*(pucSampleTemp+79) = IMAGE_DATAPORT;
		*(pucSampleTemp+79) = IMAGE_DATAPORT;
		*(pucSampleTemp+79) = IMAGE_DATAPORT;
		
		*(pucSampleTemp+80) = IMAGE_DATAPORT; 
		*(pucSampleTemp+80) = IMAGE_DATAPORT;
		*(pucSampleTemp+80) = IMAGE_DATAPORT;		
		*(pucSampleTemp+81) = IMAGE_DATAPORT;
		*(pucSampleTemp+81) = IMAGE_DATAPORT;
		*(pucSampleTemp+81) = IMAGE_DATAPORT;		
		*(pucSampleTemp+82) = IMAGE_DATAPORT;
		*(pucSampleTemp+82) = IMAGE_DATAPORT;
		*(pucSampleTemp+82) = IMAGE_DATAPORT;		   
		*(pucSampleTemp+83) = IMAGE_DATAPORT;	
		*(pucSampleTemp+83) = IMAGE_DATAPORT;
		*(pucSampleTemp+83) = IMAGE_DATAPORT;		
		*(pucSampleTemp+84) = IMAGE_DATAPORT;
		*(pucSampleTemp+84) = IMAGE_DATAPORT;
		*(pucSampleTemp+84) = IMAGE_DATAPORT;		
		*(pucSampleTemp+85) = IMAGE_DATAPORT;
		*(pucSampleTemp+85) = IMAGE_DATAPORT;
		*(pucSampleTemp+85) = IMAGE_DATAPORT;		
		*(pucSampleTemp+86) = IMAGE_DATAPORT;
		*(pucSampleTemp+86) = IMAGE_DATAPORT;	
		*(pucSampleTemp+86) = IMAGE_DATAPORT;		
		*(pucSampleTemp+87) = IMAGE_DATAPORT;
		*(pucSampleTemp+87) = IMAGE_DATAPORT;
		*(pucSampleTemp+87) = IMAGE_DATAPORT;		
		*(pucSampleTemp+88) = IMAGE_DATAPORT;
		*(pucSampleTemp+88) = IMAGE_DATAPORT;
		*(pucSampleTemp+88) = IMAGE_DATAPORT;		
		*(pucSampleTemp+89) = IMAGE_DATAPORT;
		*(pucSampleTemp+89) = IMAGE_DATAPORT;
		*(pucSampleTemp+89) = IMAGE_DATAPORT;
		
		*(pucSampleTemp+90) = IMAGE_DATAPORT; 
		*(pucSampleTemp+90) = IMAGE_DATAPORT;
		*(pucSampleTemp+90) = IMAGE_DATAPORT;
		*(pucSampleTemp+91) = IMAGE_DATAPORT;
		*(pucSampleTemp+91) = IMAGE_DATAPORT;
		*(pucSampleTemp+91) = IMAGE_DATAPORT;
		*(pucSampleTemp+92) = IMAGE_DATAPORT;
		*(pucSampleTemp+92) = IMAGE_DATAPORT;
		*(pucSampleTemp+92) = IMAGE_DATAPORT;   
		*(pucSampleTemp+93) = IMAGE_DATAPORT;
		*(pucSampleTemp+93) = IMAGE_DATAPORT;
		*(pucSampleTemp+93) = IMAGE_DATAPORT;
		*(pucSampleTemp+94) = IMAGE_DATAPORT;
		*(pucSampleTemp+94) = IMAGE_DATAPORT;
		*(pucSampleTemp+94) = IMAGE_DATAPORT;
		*(pucSampleTemp+95) = IMAGE_DATAPORT;
		*(pucSampleTemp+95) = IMAGE_DATAPORT;
		*(pucSampleTemp+95) = IMAGE_DATAPORT;
		*(pucSampleTemp+96) = IMAGE_DATAPORT;
		*(pucSampleTemp+96) = IMAGE_DATAPORT;
		*(pucSampleTemp+96) = IMAGE_DATAPORT;	
		*(pucSampleTemp+97) = IMAGE_DATAPORT;
		*(pucSampleTemp+97) = IMAGE_DATAPORT;
		*(pucSampleTemp+97) = IMAGE_DATAPORT;
		*(pucSampleTemp+98) = IMAGE_DATAPORT;
		*(pucSampleTemp+98) = IMAGE_DATAPORT;
		*(pucSampleTemp+98) = IMAGE_DATAPORT;
		*(pucSampleTemp+99) = IMAGE_DATAPORT;
		*(pucSampleTemp+99) = IMAGE_DATAPORT;
		*(pucSampleTemp+99) = IMAGE_DATAPORT;
		
//////////采三次结束////////////////////////////////		

		*(pucSampleTemp+100) = IMAGE_DATAPORT; 
		*(pucSampleTemp+100) = IMAGE_DATAPORT;
		*(pucSampleTemp+101) = IMAGE_DATAPORT;
		*(pucSampleTemp+101) = IMAGE_DATAPORT;
		*(pucSampleTemp+102) = IMAGE_DATAPORT;
		*(pucSampleTemp+102) = IMAGE_DATAPORT;   
		*(pucSampleTemp+103) = IMAGE_DATAPORT;
		*(pucSampleTemp+103) = IMAGE_DATAPORT;
		*(pucSampleTemp+104) = IMAGE_DATAPORT;
		*(pucSampleTemp+104) = IMAGE_DATAPORT;
		*(pucSampleTemp+105) = IMAGE_DATAPORT;
		*(pucSampleTemp+105) = IMAGE_DATAPORT;
		*(pucSampleTemp+106) = IMAGE_DATAPORT;
		*(pucSampleTemp+106) = IMAGE_DATAPORT;	
		*(pucSampleTemp+107) = IMAGE_DATAPORT;
		*(pucSampleTemp+107) = IMAGE_DATAPORT;
		*(pucSampleTemp+108) = IMAGE_DATAPORT;
		*(pucSampleTemp+108) = IMAGE_DATAPORT;
		*(pucSampleTemp+109) = IMAGE_DATAPORT;
		*(pucSampleTemp+109) = IMAGE_DATAPORT;
		
		*(pucSampleTemp+110) = IMAGE_DATAPORT; 
		*(pucSampleTemp+110) = IMAGE_DATAPORT;
		*(pucSampleTemp+111) = IMAGE_DATAPORT;
		*(pucSampleTemp+111) = IMAGE_DATAPORT;
		*(pucSampleTemp+112) = IMAGE_DATAPORT;
		*(pucSampleTemp+112) = IMAGE_DATAPORT;   
		*(pucSampleTemp+113) = IMAGE_DATAPORT;
		*(pucSampleTemp+113) = IMAGE_DATAPORT;
		*(pucSampleTemp+114) = IMAGE_DATAPORT;
		*(pucSampleTemp+114) = IMAGE_DATAPORT;
		*(pucSampleTemp+115) = IMAGE_DATAPORT;
		*(pucSampleTemp+115) = IMAGE_DATAPORT;
		*(pucSampleTemp+116) = IMAGE_DATAPORT;
		*(pucSampleTemp+116) = IMAGE_DATAPORT;	
		*(pucSampleTemp+117) = IMAGE_DATAPORT;
		*(pucSampleTemp+117) = IMAGE_DATAPORT;
		*(pucSampleTemp+118) = IMAGE_DATAPORT;
		*(pucSampleTemp+118) = IMAGE_DATAPORT;
		*(pucSampleTemp+119) = IMAGE_DATAPORT;
		*(pucSampleTemp+119) = IMAGE_DATAPORT;
		
		*(pucSampleTemp+120) = IMAGE_DATAPORT; 
		*(pucSampleTemp+120) = IMAGE_DATAPORT;
		*(pucSampleTemp+121) = IMAGE_DATAPORT;
		*(pucSampleTemp+121) = IMAGE_DATAPORT;
		*(pucSampleTemp+122) = IMAGE_DATAPORT;
		*(pucSampleTemp+122) = IMAGE_DATAPORT;   
		*(pucSampleTemp+123) = IMAGE_DATAPORT;
		*(pucSampleTemp+123) = IMAGE_DATAPORT;
		*(pucSampleTemp+124) = IMAGE_DATAPORT;
		*(pucSampleTemp+124) = IMAGE_DATAPORT;
		*(pucSampleTemp+125) = IMAGE_DATAPORT;
		*(pucSampleTemp+125) = IMAGE_DATAPORT;
		*(pucSampleTemp+126) = IMAGE_DATAPORT;
		*(pucSampleTemp+126) = IMAGE_DATAPORT;	
		*(pucSampleTemp+127) = IMAGE_DATAPORT;
		*(pucSampleTemp+127) = IMAGE_DATAPORT;
		*(pucSampleTemp+128) = IMAGE_DATAPORT;
		*(pucSampleTemp+128) = IMAGE_DATAPORT;
		*(pucSampleTemp+129) = IMAGE_DATAPORT;
		*(pucSampleTemp+129) = IMAGE_DATAPORT;
		
		*(pucSampleTemp+130) = IMAGE_DATAPORT; 
		*(pucSampleTemp+130) = IMAGE_DATAPORT;
		*(pucSampleTemp+131) = IMAGE_DATAPORT;
		*(pucSampleTemp+131) = IMAGE_DATAPORT;
		*(pucSampleTemp+132) = IMAGE_DATAPORT;
		*(pucSampleTemp+132) = IMAGE_DATAPORT;   
		*(pucSampleTemp+133) = IMAGE_DATAPORT;
		*(pucSampleTemp+133) = IMAGE_DATAPORT;
		*(pucSampleTemp+134) = IMAGE_DATAPORT;
		*(pucSampleTemp+134) = IMAGE_DATAPORT;
		*(pucSampleTemp+135) = IMAGE_DATAPORT;
		*(pucSampleTemp+135) = IMAGE_DATAPORT;
		*(pucSampleTemp+136) = IMAGE_DATAPORT;
		*(pucSampleTemp+136) = IMAGE_DATAPORT;	
		*(pucSampleTemp+137) = IMAGE_DATAPORT;
		*(pucSampleTemp+137) = IMAGE_DATAPORT;
		*(pucSampleTemp+138) = IMAGE_DATAPORT;
		*(pucSampleTemp+138) = IMAGE_DATAPORT;
		*(pucSampleTemp+139) = IMAGE_DATAPORT;
		*(pucSampleTemp+139) = IMAGE_DATAPORT;

		c_TureLine++;                  //实际采样行计数器加一	
	}   				
}


/*****************************************************************************/
//函数名：VSYNC_ISR                                                                  
//功  能：场中断，交换采集和处理的指针，清零行中断计数器，速度采集                                                                   
//参  数：无                                                                    
//返回值：无                                                                    
//说  明：                                                                   
/*****************************************************************************/
void VSYNC_ISR(void)
{
	///////////////////////////////////
	////交换图像采集和处理缓存/////////
	///////////////////////////////////
	pucSample = pucProcess;
	pucProcess = pucTemp;
	pucTemp = pucSample;
    LED3=~LED3;
    UART0_TX(0x00);    
    UART0_TX(c_InterruptLine); 
	if(c_InterruptLine >= 120)	
	{
		SampleOver_Flag = 1;           //采样结束标志置位					
	}

	c_TureLine =0;
	c_InterruptLine =0;	
		
	SIU.ISR.B.EIF8=1;                    //清行中断标志位		
}























































