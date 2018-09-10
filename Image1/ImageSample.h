#ifndef __IMAGESAMPLE_H_
#define __IMAGESAMPLE_H_
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */



#define IMAGE_DATAPORT  SIU.PGPDI0.B.PAL //数据采集口，假设 SIU.PGPDI1.B.PDL//50  


//*****************************************************************************
///////////////////////////////////////宏定义//////////////////////////////////
//*****************************************************************************
# define IMAGE_ROW        40           //采样行数
# define IMAGE_COLUMN     140         //采样列数

# define ROW_START        10           //采样开始行
# define ROW_MAX          220          //采样结束行 220 

extern uint8_t SampleOver_Flag;           //采样结束标志置位  
extern uint8_t SampleFlag;                //奇偶场标志

extern uint16_t c_InterruptLine;            //行中断计数
extern uint8_t c_TureLine;


extern uint8_t *pucSample;
extern uint8_t *pucProcess;
extern uint8_t *pucTemp;

void Camera_Init(void);

#endif