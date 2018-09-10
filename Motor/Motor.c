#include "motor.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */



void PWM_Init(void)
{
	SIU.PCR[66].R = 0x0600;               // 设置PE[2]为eMIOS_0的18通道		
	SIU.PCR[67].R = 0x0600;               // 设置PE[3]为eMIOS_0的19通道
	SIU.PCR[68].R = 0x0600;               // 设置PE[4]为eMIOS_0的20通道		
	SIU.PCR[69].R = 0x0600;               // 设置PE[5]为eMIOS_0的21通道
//	SIU.PCR[70].R = 0x0600;               // 设置PE[6]为eMIOS_0的21通道
	
    EMIOS_0.MCR.B.GPRE  = 7;        // 设置分频系数为1,不分频为64MHz
    EMIOS_0.MCR.B.GPREN = 1;         // 使能eMIOS时钟
    EMIOS_0.MCR.B.GTBE  = 1;         // 使能全局时基
    EMIOS_0.MCR.B.FRZ   = 1;         // 在冻结模式下停止通道
    		
 //设置18通道，周期100us，
    EMIOS_0.CH[18].CADR.R      = 0;      // 设置首边沿的匹配值
    EMIOS_0.CH[18].CBDR.R      = 1;      // 设置尾边沿的匹配值   
    EMIOS_0.CH[18].CCR.B.BSL   = 0x0;     // 使用通道23生成的Bus A
    EMIOS_0.CH[18].CCR.B.EDPOL = 1;       // 设置极性：A匹配时置1，B匹配时清0
    EMIOS_0.CH[18].CCR.B.MODE  = 0x60;    // 选择OPWMB模式
//设置19通道，周期100us，
    EMIOS_0.CH[19].CADR.R      = 0;      // 设置首边沿的匹配值
    EMIOS_0.CH[19].CBDR.R      = 1;      // 设置尾边沿的匹配值   
    EMIOS_0.CH[19].CCR.B.BSL   = 0x0;     // 使用Bus A
    EMIOS_0.CH[19].CCR.B.EDPOL = 1;       // 设置极性：A匹配时置1，B匹配时清0
    EMIOS_0.CH[19].CCR.B.MODE  = 0x60;    // 选择OPWMB模式 
  //设置20通道，周期100us，
    EMIOS_0.CH[20].CADR.R      = 0;      // 设置首边沿的匹配值
    EMIOS_0.CH[20].CBDR.R      = 1;      // 设置尾边沿的匹配值   
    EMIOS_0.CH[20].CCR.B.BSL   = 0x0;     // 使用Bus A
    EMIOS_0.CH[20].CCR.B.EDPOL = 1;       // 设置极性：A匹配时置1，B匹配时清0
    EMIOS_0.CH[20].CCR.B.MODE  = 0x60;    // 选择OPWMB模式 
 //设置21通道，周期100us
    EMIOS_0.CH[21].CADR.R      = 0;      // 设置首边沿的匹配值
    EMIOS_0.CH[21].CBDR.R      = 1;      // 设置尾边沿的匹配值   
    EMIOS_0.CH[21].CCR.B.BSL   = 0x0;     // 使用通道23生成的Bus A
    EMIOS_0.CH[21].CCR.B.EDPOL = 1;       // 设置极性：A匹配时置1，B匹配时清0
    EMIOS_0.CH[21].CCR.B.MODE  = 0x60;    // 选择OPWMB模式
 //设置22通道，周期100us
 //   EMIOS_0.CH[22].CADR.R      = 0;      // 设置首边沿的匹配值
  //  EMIOS_0.CH[22].CBDR.R      = 1;      // 设置尾边沿的匹配值   
  //  EMIOS_0.CH[22].CCR.B.BSL   = 0x0;     // 使用通道23生成的Bus A
  //  EMIOS_0.CH[22].CCR.B.EDPOL = 1;       // 设置极性：A匹配时置1，B匹配时清0
  //  EMIOS_0.CH[22].CCR.B.MODE  = 0x60;    // 选择OPWMB模式
    

//设置23通道，作为18,19,20,21的时基Bus A
    EMIOS_0.CH[23].CADR.R      = 800;    //计数周期为6400=100us  
    EMIOS_0.CH[23].CCR.B.MODE  = 0x50;   //设置为缓冲模数计数器模式MCB
    EMIOS_0.CH[23].CCR.B.BSL   = 0x3;    //使用64MHz的内部计数器作为时钟源
    EMIOS_0.CH[23].CCR.B.UCPEN = 1;      //使能分频器
    EMIOS_0.CH[23].CCR.B.FREN  = 1;      //在冻结模式下停止计数  			
}
















































