#include "Control.h"
#include "encoder.h"
#include "motor.h"
#include "kalman_filter.h"
#include "Direction.h"






int AngleControlOut=0;
int LeftMotorOut,RightMotorOut;

                  
int MotorOutput(void)
{

	
	AngleControlOut = (-47 - angle)*82 + (0-Gyro_AngleSpeed)*0.68; //直立控制 
	
	//OutData[2] = AngleControlOut;

	LeftMotorOut  = (float)(AngleControlOut - g_fSpeedControlOut + DirectionControlOut);   //- DirectionControlOut
	RightMotorOut = (float)(AngleControlOut - g_fSpeedControlOut - DirectionControlOut);   //+ DirectionControlOut   



					
	if(LeftMotorOut > MOTOR_OUT_MAX)          //限制输出，防止超过PWM范围
		LeftMotorOut = MOTOR_OUT_MAX;
	if(LeftMotorOut < MOTOR_OUT_MIN)
		LeftMotorOut = MOTOR_OUT_MIN;
    if(RightMotorOut > MOTOR_OUT_MAX)
		RightMotorOut = MOTOR_OUT_MAX;
	if(RightMotorOut < MOTOR_OUT_MIN)
		RightMotorOut = MOTOR_OUT_MIN;
	
	
 	if(LeftMotorOut > 0)
	{
		Motor_L_Back = 0;
		Motor_L_Front = LeftMotorOut;  
	}else
	{
		Motor_L_Front =0;
		LeftMotorOut = -LeftMotorOut;
		Motor_L_Back  =	 LeftMotorOut;	
	}
	
 	if(RightMotorOut > 0)
	{
		Motor_R_Back = 0;
		Motor_R_Front = RightMotorOut;  
	}else
	{
		Motor_R_Front =0;
		RightMotorOut = -RightMotorOut;
		Motor_R_Back  =	 RightMotorOut;	
	}			
}





















