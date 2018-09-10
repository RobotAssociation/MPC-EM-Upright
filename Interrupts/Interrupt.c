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
/*                       初始化中断向量                     */
/************************************************************/
asm void initIrqVectors(void) {
  lis	   r3, __IVPR_VALUE@h   /* IVPR value is passed from link file */
  ori      r3, r3, __IVPR_VALUE@l 
  mtivpr   r3									 
}


/************************************************************/
/*                         配置中断                         */
/************************************************************/
void initINTC(void) 
{	
  INTC.MCR.B.HVEN = 0;       //配置为软件中断
  INTC.MCR.B.VTES = 0;       //配置向量表大小为4字节
  INTC.IACKR.R = (uint32_t) &IntcIsrVectorTable[0];    //设置中断向量表基地址
}
/************************************************************/
/*                      初始化PIT模块                       */
/************************************************************/
void initPIT(void) 
{
                            
  PIT.PITMCR.R = 0x00000000;       //使能PIT模块，并且在debug模式下继续运行
  PIT.CH[1].LDVAL.R = 320000;    //设置计数值为320000，系统时钟为64MHz，则计数时间为5ms
  PIT.CH[1].TCTRL.R = 0x000000003; //使能PIT1计数，并使能中断 
  INTC.PSR[60].R = 0x01;           //设置PIT1的中断优先级为1
}

/************************************************************/
/*                      初始化软件中断                      */
/************************************************************/
void initSwIrq4(void) 
{
  INTC.PSR[4].R = 2;		//软件中断4优先级为2
}

/************************************************************/
/*                        使能中断                          */
/************************************************************/
void enableIrq(void) 
{
  INTC.CPR.B.PRI = 0;          //设置当前的优先级
  asm(" wrteei 1");	    	   //使能外部中断
}

/************************************************************/
/*                     PIT1中断函数                         */
/************************************************************/

void PIT1inter(void) 
{
	static int vol_temp = 0;
	static int fValue = 0;
	
  
/*		if ((angle > 70)||(angle < -70)) 
		{ 
			INTC.SSCIR[4].R = 2;  //触发软件中断4，这里为了示例软件中断的用法
		} 
*/

		SpeedControlPeriod ++;	 //速度控制
		   
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
		PIT.CH[1].TFLG.B.TIF = 1;  //清除中断标志位
}



/************************************************************/
/*                     软件中断4函数                        */
/************************************************************/
void SwIrq4ISR(void) 
{
	INTC.SSCIR[4].R = 1;		//清除中断标志  
	while(1)
	{
		LED4 = 0;
		Motor_L_Front = 1;
		Motor_L_Back = 1;
		Motor_R_Front = 1;
		Motor_R_Back = 1;  	
	}
 }

