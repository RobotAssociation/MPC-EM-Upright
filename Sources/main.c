#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "LED.h"
#include "SYSTEM.h"
#include "Interrupt.h"
#include "usart.h"
#include "Visual_data.h"
#include "Kalman_Filter.h"
#include "ImageSample.h"
#include "ImageProcess.h"
#include "Motor.h"
#include "ADC.h"
#include "MMA8451.h"
#include "L3G4200D.h"
#include "encoder.h"






void main (void) 
{	
	SWT_DisableWatchDog(); 
	SYSTEM_Init();
	Encoder_Init(); 
	LED_Init();
	PWM_Init();
	ADC_Init();
	
	MMA845x_init();
	Init_L3G4200D();
	LINFlex0_init();
	initIrqVectors();		
	initINTC();
			
	initPIT();		  	
	initSwIrq4();		
			   

	enableIrq();



	for(;;)
	{ 
	
		Visual_Data(OutData);//数据发回上位机 
		LED0=~LED0;
	}
}


