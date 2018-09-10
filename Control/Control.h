#ifndef __CONTROL_H_
#define __CONTROL_H_




#define M    667;   //角度控制值的放大倍数		7200-500=6700
                                     //                                 800                     
#define MOTOR_OUT_DEAD_VAL     0    //当寄存器值相差  时车轮刚好能动   20
#define	MOTOR_OUT_MAX		   799
#define	MOTOR_OUT_MIN		   -799



extern int AngleControlOut; 
extern int LeftMotorOut,RightMotorOut;




int MotorOutput(void);

#endif