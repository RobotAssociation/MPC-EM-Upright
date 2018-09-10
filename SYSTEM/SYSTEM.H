#ifndef _SYSTEM_H_
#define _SYSTEM_H_
//#include "MPC5604B_M27V.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */

void SWT_DisableWatchDog(void);
void SYSTEM_Init(void);
void delay_ms(int ms);


#endif



