#include "ImageSample.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "Interrupt.h"
#include "LED.h"
/////////ͼ�����ݻ�������//////////////
uint8_t Buffer1[IMAGE_ROW][IMAGE_COLUMN];
uint8_t Buffer2[IMAGE_ROW][IMAGE_COLUMN];

//ָ��ǰ�ɼ����ݲ��������׵�ַ��ָ��/
uint8_t *pucSample = &Buffer1[0][0];

//ָ��ǰ�������ݲ��������׵�ַ��ָ��/
uint8_t *pucProcess = &Buffer2[0][0];


////////����ͼ��ɼ��ʹ���������/////
//��ע�⣺��ÿ�ν���ָ���֤puca_BufferTemp��puca_BufferSample��ͬ��
uint8_t *pucTemp = &Buffer1[0][0];




uint8_t GetImage[IMAGE_ROW] =      //ÿ�вɼ�����
{
	15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,
	90,95,100,105,110,115,120,125,130,135,140,145,150,
	155,160,165,170,175,180,185,190,195,200,205,210
};
/*
uint8_t GetImage[IMAGE_ROW] =      //ÿ�вɼ�����
{
	7,10,13,15,18,21,24,27,30,33,36,39,42,45,48,
	51,54,57,60,63,66,69,72,75,78,81,84,87,
	90,93,95,98,101,104,107,120,123,126,129,132
};
*/
uint16_t c_InterruptLine = 0;          //���жϼ���
uint8_t  c_TureLine = 0;               //ʵ�ʲ����м���
uint8_t  c_jiange = 0;
uint8_t  c_hang = 0;                   //���ж������õļ�¼ÿ�е�ĸ����ı���
uint16_t s_speed = 0;                  //��ȡ���ٶ�ֵ


//*****************************************************************************
///////////////////////////////��־λ����//////////////////////////////////////		 
//*****************************************************************************
uint8_t SampleOver_Flag = 0;           //����������־��λ  
uint8_t SampleFlag = 0;                //��ż����־


void Camera_Init(void)
{
/////////����ͷ�ź������PA7~PA0//////////////////////////////    
	SIU.PCR[7].R = 0x0103;             //PA7����
    SIU.PCR[6].R = 0x0103; 
    SIU.PCR[5].R = 0x0103; 
    SIU.PCR[4].R = 0x0103;
    SIU.PCR[3].R = 0x0103;
    SIU.PCR[2].R = 0x0103; 
    SIU.PCR[1].R = 0x0103; 
    SIU.PCR[0].R = 0x0103;              //PA0���� 	
    
    INTC.MCR.B.HVEN = 1;               //Ӳ������ģʽ  ������ַ4���ֽ�
    
    SIU.IRER.B.EIRE4    = 0;           //EIRQ[4]��ֹ
    SIU.IRER.B.EIRE8    = 0;           //EIRQ[8]��ֹ
    
    SIU.IREER.B.IREE4 = 1;             //�ж��½���ʹ�ܼĴ���--0�ڣ�PA14����������Ч,���ж�
    SIU.IFEER.B.IFEE8 = 1;             //�ж�������ʹ�ܼĴ���--1�ڣ�PC14���½�����Ч�����ж�    
    
    SIU.PCR[14].R = 0X0103;            //����PA[14]����ΪEIRQ4����--IRQ0��
    SIU.PCR[46].R = 0X0103;            //����PC[14]����ΪEIRQ8����--IRQ1�� 
    
    INTC.PSR[41].R = 0x05;             //�����ж����ȼ��������,���ж����ȼ��ߣH	
    INTC.PSR[42].R = 0x04;  
    SIU.IRER.B.EIRE4    = 1;           //ʹ���ж�
    SIU.IRER.B.EIRE8    = 1;           //ʹ���ж�                   	
}

/*****************************************************************************/
//��������HREF_ISR                                                                 
//��  �ܣ����жϺ��������ڲɼ�ÿ�е����ص�Ҷ�ֵ                                                                   
//��  ������                                                                    
//����ֵ����                                                                    
//˵  �����ɼ�                                                                   
/*****************************************************************************/
void HREF_ISR(void)
{
	uint8_t *pucSampleTemp = (void*)0; //����ֵ��ָ�룬ָ��ʵ�ʲ�����
	
 	SIU.ISR.B.EIF4=1;                    //�����жϱ�־λ ��������������⣿   	
	c_InterruptLine++;


	
	if(c_InterruptLine < ROW_START || c_InterruptLine > ROW_MAX)   //	SampleFlag = 0 || 
	{
		return;	     //������Ч�У�ֱ���������ж�
	}	
	if (c_InterruptLine == GetImage[c_TureLine])   //������ɼ���
	{
		pucSampleTemp = pucSample + c_TureLine*IMAGE_COLUMN;//ָ��ÿ�е��׵�ַ

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
////////////�����Σ���ʱ�����м�����ѹ��////////////		
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
		
//////////�����ν���////////////////////////////////		

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

		c_TureLine++;                  //ʵ�ʲ����м�������һ	
	}   				
}


/*****************************************************************************/
//��������VSYNC_ISR                                                                  
//��  �ܣ����жϣ������ɼ��ʹ����ָ�룬�������жϼ��������ٶȲɼ�                                                                   
//��  ������                                                                    
//����ֵ����                                                                    
//˵  ����                                                                   
/*****************************************************************************/
void VSYNC_ISR(void)
{
	///////////////////////////////////
	////����ͼ��ɼ��ʹ�����/////////
	///////////////////////////////////
	pucSample = pucProcess;
	pucProcess = pucTemp;
	pucTemp = pucSample;
    LED3=~LED3;
    UART0_TX(0x00);    
    UART0_TX(c_InterruptLine); 
	if(c_InterruptLine >= 120)	
	{
		SampleOver_Flag = 1;           //����������־��λ					
	}

	c_TureLine =0;
	c_InterruptLine =0;	
		
	SIU.ISR.B.EIF8=1;                    //�����жϱ�־λ		
}























































