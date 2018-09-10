#ifndef __ENCODER_H_
#define __ENCODER_H_
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */



#define Right_Counter    SIU.PGPDI1.B.PCL
#define Left_Counter     SIU.PGPDI1.B.PDL
#define Right_Dir        SIU.PGPDI1.B.PCH
#define Left_Dir         SIU.PGPDI1.B.PDH
#define CLEAR            SIU.GPDO[45].R 


#define OPTICAL_ENCODE_CONSTANT   512   //光电编码盘的刻槽数量
#define SPEED_CONTROL_PERIOD	      100   //速度控制周期，ms
#define CAR_SPEED_CONSTANT        0.02      //单位转化比例值 

extern float SPEED;
extern uint8_t MAX_SPEED,MIN_SPEED ;
extern uint8_t SpeedControlPeriod;
extern float fCarSpeed;
extern float fSpeedControlOutOld,fSpeedControlOutNew;
extern float g_fSpeedControlOut;


void Encoder_Init();
void SpeedControl(void);
void SpeedControlOutput(void);

float abs_4(float x);

#endif