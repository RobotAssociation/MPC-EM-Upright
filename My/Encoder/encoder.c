#include "encoder.h"
#include "kalman_filter.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "LED.h"
#include "Interrupt.h"

float abs_4(float x)
{
	if(x<0)
		x=-x;
	
	return x;
}

float  SPEED = 10;       //速度给定
uint8_t MAX_SPEED = 0,MIN_SPEED =0;
uint8_t SpeedControlPeriod=0;
float   fCarSpeed;
float 	fSpeedControlOutOld=0,fSpeedControlOutNew=0;
float   g_fSpeedControlOut=0;


void Encoder_Init()
{
	 SIU.PCR[32].R = 0x0100;               // 设置PC[0]输入
	 SIU.PCR[33].R = 0x0100;               // 设置PC[1]输入
	 SIU.PCR[34].R = 0x0100;               // 设置PC[2]输入
	 SIU.PCR[35].R = 0x0100;               // 设置PC[3]输入
	 SIU.PCR[36].R = 0x0100;               // 设置PC[4]输入
	 SIU.PCR[37].R = 0x0100;               // 设置PC[5]输入
     SIU.PCR[38].R = 0x0100;               // 设置PC[6]输入
     SIU.PCR[39].R = 0x0100;               // 设置PC[7]输入
     SIU.PCR[40].R = 0x0100;               // 设置PC[8]输入
     SIU.PCR[41].R = 0x0100;               // 设置PC[9]输入
     SIU.PCR[42].R = 0x0100;               // 设置PC[10]输入
     SIU.PCR[43].R = 0x0100;               // 设置PC[11]输入   
     
	 SIU.PCR[48].R = 0x0100;               // 设置PD[0]输入
	 SIU.PCR[49].R = 0x0100;               // 设置PD[1]输入
	 SIU.PCR[50].R = 0x0100;               // 设置PD[2]输入
	 SIU.PCR[51].R = 0x0100;               // 设置PD[3]输入
	 SIU.PCR[52].R = 0x0100;               // 设置PD[4]输入
	 SIU.PCR[53].R = 0x0100;               // 设置PD[5]输入
     SIU.PCR[54].R = 0x0100;               // 设置PD[6]输入
     SIU.PCR[55].R = 0x0100;               // 设置PD[7]输入
     SIU.PCR[56].R = 0x0100;               // 设置PD[8]输入
     SIU.PCR[57].R = 0x0100;               // 设置PD[9]输入
     SIU.PCR[58].R = 0x0100;               // 设置PD[10]输入
     SIU.PCR[59].R = 0x0100;               // 设置PD[11]输入         
     
     
     SIU.PCR[44].R = 0x0100;               // 设置PC[12]输入 判断方向用的
     SIU.PCR[60].R = 0x0100;               // 设置PD[12]输入          
      
	 SIU.PCR[45].R = 0x0201;               // 设置PC[13]弱上拉输出  计数器清零用的
}

void SpeedControl(void)
{

    float iIncpid;
    float iError;
    static float LastError = 0.0;
    
    
   	int LeftMotor=0,RightMOtor=0;
   	float real_speed;
   	real_speed=SPEED*50;
	LeftMotor = Right_Counter; 
	RightMOtor = Left_Counter;

//	OutData[0] = LeftMotor;
//	OutData[1] = RightMOtor;
//	OutData[2] = real_speed;

	if(LeftMotor>3000)
		INTC.SSCIR[4].R = 2;  //触发软件中断4  ,车失控时让车轮停下来
					   
    CLEAR = 1;   //复位计数器与下面一条语句合起来          	
    CLEAR = 0;   //复位计数器  开始计数	
    if(Left_Dir == 1)             //也可用PWM的输出来判断方向
    {
    	 LeftMotor  = -LeftMotor;  
         RightMOtor = -RightMOtor ;     	
    }
    
    
  	fCarSpeed = (LeftMotor+ RightMOtor)/2.0 * CAR_SPEED_CONSTANT;
  	
  //	UART0_TX(abs_2((int)fCarSpeed));
	
	iError =SPEED - fCarSpeed;	   //CAR_SPEED_SETfDelta是速度给定，直立给先给0吧不知道对不对

// 			 S_P  	10				  S_I 30
//                  30                    45
//                  40                    55                 
    iIncpid = 5 * (iError - LastError) + 50 * iError;//增量计算PI  5/50    10/26先静态调节的效果是确定速度控制的参数大致范围
//前一项是I，后一项是P。先调I后调P，I的效果是回到原点的速度，大了会来回摆动
	LastError = iError;	 
	                                
	fSpeedControlOutOld = fSpeedControlOutNew;
	fSpeedControlOutNew = iIncpid ;
	//OutData[3] = fSpeedControlOutNew;
}


void SpeedControlOutput(void)
{
	float value;
	value=fSpeedControlOutNew - fSpeedControlOutOld;
	g_fSpeedControlOut = fSpeedControlOutOld+SpeedControlPeriod*value/20;
}	









