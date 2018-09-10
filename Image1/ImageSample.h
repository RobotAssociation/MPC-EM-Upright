#ifndef __IMAGESAMPLE_H_
#define __IMAGESAMPLE_H_
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */



#define IMAGE_DATAPORT  SIU.PGPDI0.B.PAL //���ݲɼ��ڣ����� SIU.PGPDI1.B.PDL//50  


//*****************************************************************************
///////////////////////////////////////�궨��//////////////////////////////////
//*****************************************************************************
# define IMAGE_ROW        40           //��������
# define IMAGE_COLUMN     140         //��������

# define ROW_START        10           //������ʼ��
# define ROW_MAX          220          //���������� 220 

extern uint8_t SampleOver_Flag;           //����������־��λ  
extern uint8_t SampleFlag;                //��ż����־

extern uint16_t c_InterruptLine;            //���жϼ���
extern uint8_t c_TureLine;


extern uint8_t *pucSample;
extern uint8_t *pucProcess;
extern uint8_t *pucTemp;

void Camera_Init(void);

#endif