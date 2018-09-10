#ifndef _LED_H_
#define _LED_H_
//#include "MPC5604B_M27V.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */

#define  LED0	SIU.GPDO[12].B.PDO   //在核心板上
#define  LED1	SIU.GPDO[13].B.PDO
//#define  LED2	SIU.GPDO[14].B.PDO
#define  LED3	SIU.GPDO[15].B.PDO


#define  LED4	SIU.GPDO[72].B.PDO   //在主板上
#define  LED5	SIU.GPDO[73].B.PDO
#define  LED6	SIU.GPDO[74].B.PDO
#define  LED7	SIU.GPDO[75].B.PDO



///////////////拔码开关///////////////
# define   Switch4   SIU.GPDI[28].R
# define   Switch3   SIU.GPDI[29].R
# define   Switch2   SIU.GPDI[30].R
# define   Switch1   SIU.GPDI[31].R

void LED_Init(void);



#endif