#ifdef    __INTERRUPT_H_
#define   __INTERRUPT_H_
///#include "MPC5604B_0M27V_0102.h" /* Use proper include file */   //�����������û��Ҫ��������

//extern uint8_t Pit2; 
extern uint8_t MS5_EventCount;
//extern IVOR4Handler();
//extern __IVPR_VALUE;
//extern IntcIsrVectorTable[];
asm void initIrqVectors(void);

#endif 
