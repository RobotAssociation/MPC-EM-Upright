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

float  SPEED = 10;       //�ٶȸ���
uint8_t MAX_SPEED = 0,MIN_SPEED =0;
uint8_t SpeedControlPeriod=0;
float   fCarSpeed;
float 	fSpeedControlOutOld=0,fSpeedControlOutNew=0;
float   g_fSpeedControlOut=0;


void Encoder_Init()
{
	 SIU.PCR[32].R = 0x0100;               // ����PC[0]����
	 SIU.PCR[33].R = 0x0100;               // ����PC[1]����
	 SIU.PCR[34].R = 0x0100;               // ����PC[2]����
	 SIU.PCR[35].R = 0x0100;               // ����PC[3]����
	 SIU.PCR[36].R = 0x0100;               // ����PC[4]����
	 SIU.PCR[37].R = 0x0100;               // ����PC[5]����
     SIU.PCR[38].R = 0x0100;               // ����PC[6]����
     SIU.PCR[39].R = 0x0100;               // ����PC[7]����
     SIU.PCR[40].R = 0x0100;               // ����PC[8]����
     SIU.PCR[41].R = 0x0100;               // ����PC[9]����
     SIU.PCR[42].R = 0x0100;               // ����PC[10]����
     SIU.PCR[43].R = 0x0100;               // ����PC[11]����   
     
	 SIU.PCR[48].R = 0x0100;               // ����PD[0]����
	 SIU.PCR[49].R = 0x0100;               // ����PD[1]����
	 SIU.PCR[50].R = 0x0100;               // ����PD[2]����
	 SIU.PCR[51].R = 0x0100;               // ����PD[3]����
	 SIU.PCR[52].R = 0x0100;               // ����PD[4]����
	 SIU.PCR[53].R = 0x0100;               // ����PD[5]����
     SIU.PCR[54].R = 0x0100;               // ����PD[6]����
     SIU.PCR[55].R = 0x0100;               // ����PD[7]����
     SIU.PCR[56].R = 0x0100;               // ����PD[8]����
     SIU.PCR[57].R = 0x0100;               // ����PD[9]����
     SIU.PCR[58].R = 0x0100;               // ����PD[10]����
     SIU.PCR[59].R = 0x0100;               // ����PD[11]����         
     
     
     SIU.PCR[44].R = 0x0100;               // ����PC[12]���� �жϷ����õ�
     SIU.PCR[60].R = 0x0100;               // ����PD[12]����          
      
	 SIU.PCR[45].R = 0x0201;               // ����PC[13]���������  �����������õ�
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
		INTC.SSCIR[4].R = 2;  //��������ж�4  ,��ʧ��ʱ�ó���ͣ����
					   
    CLEAR = 1;   //��λ������������һ����������          	
    CLEAR = 0;   //��λ������  ��ʼ����	
    if(Left_Dir == 1)             //Ҳ����PWM��������жϷ���
    {
    	 LeftMotor  = -LeftMotor;  
         RightMOtor = -RightMOtor ;     	
    }
    
    
  	fCarSpeed = (LeftMotor+ RightMOtor)/2.0 * CAR_SPEED_CONSTANT;
  	
  //	UART0_TX(abs_2((int)fCarSpeed));
	
	iError =SPEED - fCarSpeed;	   //CAR_SPEED_SETfDelta���ٶȸ�����ֱ�����ȸ�0�ɲ�֪���Բ���

// 			 S_P  	10				  S_I 30
//                  30                    45
//                  40                    55                 
    iIncpid = 5 * (iError - LastError) + 50 * iError;//��������PI  5/50    10/26�Ⱦ�̬���ڵ�Ч����ȷ���ٶȿ��ƵĲ������·�Χ
//ǰһ����I����һ����P���ȵ�I���P��I��Ч���ǻص�ԭ����ٶȣ����˻����ذڶ�
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









