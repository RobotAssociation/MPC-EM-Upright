#ifndef __MOTOR_H_
#define __MOTOR_H_
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */



//通过改变TIM3->CCR2的值来改变占空比，
#define Motor_R_Front       EMIOS_0.CH[18].CBDR.R   //  PE2
#define Motor_R_Back        EMIOS_0.CH[19].CBDR.R   //  PE3
#define Motor_L_Front       EMIOS_0.CH[21].CBDR.R   //  PE5
#define Motor_L_Back        EMIOS_0.CH[20].CBDR.R   //  PE4 


void PWM_Init(void);

#endif