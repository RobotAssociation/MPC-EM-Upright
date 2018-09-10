#include "Interrupt.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "LED.h"
#include "Kalman_Filter.h"
#include "ImageProcess.h"
#include "Control.h"
#include "encoder.h"
#include "motor.h"
#include "Direction.h"

extern IVOR4Handler();
extern uint32_t __IVPR_VALUE;    /* Interrupt Vector Prefix vaue from link file*/
extern const vuint32_t IntcIsrVectorTable[];

uint8_t MS5_EventCount = 0;


//uint8_t Pit2 = 0;
//uint32_t SWirq4Ctr = 0;	     /* Counter for software interrupt 4 */

/************************************************************/
/*                       ��ʼ���ж�����                     */
/************************************************************/
asm void initIrqVectors(void) {
  lis	   r3, __IVPR_VALUE@h   /* IVPR value is passed from link file */
  ori      r3, r3, __IVPR_VALUE@l 
  mtivpr   r3									 
}


/************************************************************/
/*                         �����ж�                         */
/************************************************************/
void initINTC(void) 
{	
  INTC.MCR.B.HVEN = 0;       //����Ϊ����ж�
  INTC.MCR.B.VTES = 0;       //�����������СΪ4�ֽ�
  INTC.IACKR.R = (uint32_t) &IntcIsrVectorTable[0];    //�����ж����������ַ
}
/************************************************************/
/*                      ��ʼ��PITģ��                       */
/************************************************************/
void initPIT(void) 
{
                            
  PIT.PITMCR.R = 0x00000000;       //ʹ��PITģ�飬������debugģʽ�¼�������
  PIT.CH[1].LDVAL.R = 320000;    //���ü���ֵΪ320000��ϵͳʱ��Ϊ64MHz�������ʱ��Ϊ5ms
  PIT.CH[1].TCTRL.R = 0x000000003; //ʹ��PIT1��������ʹ���ж� 
  INTC.PSR[60].R = 0x01;           //����PIT1���ж����ȼ�Ϊ1
}

/************************************************************/
/*                      ��ʼ������ж�                      */
/************************************************************/
void initSwIrq4(void) 
{
  INTC.PSR[4].R = 2;		//����ж�4���ȼ�Ϊ2
}

/************************************************************/
/*                        ʹ���ж�                          */
/************************************************************/
void enableIrq(void) 
{
  INTC.CPR.B.PRI = 0;          //���õ�ǰ�����ȼ�
  asm(" wrteei 1");	    	   //ʹ���ⲿ�ж�
}

/************************************************************/
/*                     PIT1�жϺ���                         */
/************************************************************/

void PIT1inter(void) 
{
	static int vol_temp = 0;
	static int fValue = 0;
	
  
/*		if ((angle > 70)||(angle < -70)) 
		{ 
			INTC.SSCIR[4].R = 2;  //��������ж�4������Ϊ��ʾ������жϵ��÷�
		} 
*/

		SpeedControlPeriod ++;	 //�ٶȿ���
		   
		Filter(); 
	
		if(SpeedControlPeriod%2 == 0)
		{
			DirectionControl();
			DirectionControlOutput();	
		}
			
		if(SpeedControlPeriod == 20)
		{
    		SpeedControl();
    		vol_temp = (fSpeedControlOutNew - fSpeedControlOutOld) /20.0;
		} 
		  
		g_fSpeedControlOut = vol_temp*SpeedControlPeriod + fSpeedControlOutOld;  
		
		if(SpeedControlPeriod == 20)
		{
			SpeedControlPeriod = 0;
		}
		
	       
		 MotorOutput();
		PIT.CH[1].TFLG.B.TIF = 1;  //����жϱ�־λ
}



/************************************************************/
/*                     ����ж�4����                        */
/************************************************************/
void SwIrq4ISR(void) 
{
	INTC.SSCIR[4].R = 1;		//����жϱ�־  
	while(1)
	{
		LED4 = 0;
		Motor_L_Front = 1;
		Motor_L_Back = 1;
		Motor_R_Front = 1;
		Motor_R_Back = 1;  	
	}
 }

