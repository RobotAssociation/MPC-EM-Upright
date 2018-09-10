#ifndef _DIRECTION_H_
#define _DIRECTION_H_
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */



extern 	float Proportion;
extern 	float Derivative;


extern float DirectionControlOut;
extern float DirectionControlOutNew,DirectionControlOutOld;
extern unsigned char DirectionControlPeriod;



void DirectionControl(void);
void DirectionControlOutput(void);
void Direction(void);          //路径处理函数
void AD_zhi(void);




#endif