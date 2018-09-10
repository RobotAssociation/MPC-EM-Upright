#include "motor.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */



void PWM_Init(void)
{
	SIU.PCR[66].R = 0x0600;               // ����PE[2]ΪeMIOS_0��18ͨ��		
	SIU.PCR[67].R = 0x0600;               // ����PE[3]ΪeMIOS_0��19ͨ��
	SIU.PCR[68].R = 0x0600;               // ����PE[4]ΪeMIOS_0��20ͨ��		
	SIU.PCR[69].R = 0x0600;               // ����PE[5]ΪeMIOS_0��21ͨ��
//	SIU.PCR[70].R = 0x0600;               // ����PE[6]ΪeMIOS_0��21ͨ��
	
    EMIOS_0.MCR.B.GPRE  = 7;        // ���÷�Ƶϵ��Ϊ1,����ƵΪ64MHz
    EMIOS_0.MCR.B.GPREN = 1;         // ʹ��eMIOSʱ��
    EMIOS_0.MCR.B.GTBE  = 1;         // ʹ��ȫ��ʱ��
    EMIOS_0.MCR.B.FRZ   = 1;         // �ڶ���ģʽ��ֹͣͨ��
    		
 //����18ͨ��������100us��
    EMIOS_0.CH[18].CADR.R      = 0;      // �����ױ��ص�ƥ��ֵ
    EMIOS_0.CH[18].CBDR.R      = 1;      // ����β���ص�ƥ��ֵ   
    EMIOS_0.CH[18].CCR.B.BSL   = 0x0;     // ʹ��ͨ��23���ɵ�Bus A
    EMIOS_0.CH[18].CCR.B.EDPOL = 1;       // ���ü��ԣ�Aƥ��ʱ��1��Bƥ��ʱ��0
    EMIOS_0.CH[18].CCR.B.MODE  = 0x60;    // ѡ��OPWMBģʽ
//����19ͨ��������100us��
    EMIOS_0.CH[19].CADR.R      = 0;      // �����ױ��ص�ƥ��ֵ
    EMIOS_0.CH[19].CBDR.R      = 1;      // ����β���ص�ƥ��ֵ   
    EMIOS_0.CH[19].CCR.B.BSL   = 0x0;     // ʹ��Bus A
    EMIOS_0.CH[19].CCR.B.EDPOL = 1;       // ���ü��ԣ�Aƥ��ʱ��1��Bƥ��ʱ��0
    EMIOS_0.CH[19].CCR.B.MODE  = 0x60;    // ѡ��OPWMBģʽ 
  //����20ͨ��������100us��
    EMIOS_0.CH[20].CADR.R      = 0;      // �����ױ��ص�ƥ��ֵ
    EMIOS_0.CH[20].CBDR.R      = 1;      // ����β���ص�ƥ��ֵ   
    EMIOS_0.CH[20].CCR.B.BSL   = 0x0;     // ʹ��Bus A
    EMIOS_0.CH[20].CCR.B.EDPOL = 1;       // ���ü��ԣ�Aƥ��ʱ��1��Bƥ��ʱ��0
    EMIOS_0.CH[20].CCR.B.MODE  = 0x60;    // ѡ��OPWMBģʽ 
 //����21ͨ��������100us
    EMIOS_0.CH[21].CADR.R      = 0;      // �����ױ��ص�ƥ��ֵ
    EMIOS_0.CH[21].CBDR.R      = 1;      // ����β���ص�ƥ��ֵ   
    EMIOS_0.CH[21].CCR.B.BSL   = 0x0;     // ʹ��ͨ��23���ɵ�Bus A
    EMIOS_0.CH[21].CCR.B.EDPOL = 1;       // ���ü��ԣ�Aƥ��ʱ��1��Bƥ��ʱ��0
    EMIOS_0.CH[21].CCR.B.MODE  = 0x60;    // ѡ��OPWMBģʽ
 //����22ͨ��������100us
 //   EMIOS_0.CH[22].CADR.R      = 0;      // �����ױ��ص�ƥ��ֵ
  //  EMIOS_0.CH[22].CBDR.R      = 1;      // ����β���ص�ƥ��ֵ   
  //  EMIOS_0.CH[22].CCR.B.BSL   = 0x0;     // ʹ��ͨ��23���ɵ�Bus A
  //  EMIOS_0.CH[22].CCR.B.EDPOL = 1;       // ���ü��ԣ�Aƥ��ʱ��1��Bƥ��ʱ��0
  //  EMIOS_0.CH[22].CCR.B.MODE  = 0x60;    // ѡ��OPWMBģʽ
    

//����23ͨ������Ϊ18,19,20,21��ʱ��Bus A
    EMIOS_0.CH[23].CADR.R      = 800;    //��������Ϊ6400=100us  
    EMIOS_0.CH[23].CCR.B.MODE  = 0x50;   //����Ϊ����ģ��������ģʽMCB
    EMIOS_0.CH[23].CCR.B.BSL   = 0x3;    //ʹ��64MHz���ڲ���������Ϊʱ��Դ
    EMIOS_0.CH[23].CCR.B.UCPEN = 1;      //ʹ�ܷ�Ƶ��
    EMIOS_0.CH[23].CCR.B.FREN  = 1;      //�ڶ���ģʽ��ֹͣ����  			
}
















































