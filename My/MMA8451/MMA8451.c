#include "MMA8451.h"
#include "LED.h" 
#include "IIC.h"



//��ʼ��MMA845x.
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ��.
void MMA845x_init()
{	
	uint8_t v=0;
	init_I2C();
	writebyte(MMA845x_IIC_ADDRESS,CTRL_REG1,ASLP_RATE_20MS+DATA_RATE_5MS);	
	writebyte(MMA845x_IIC_ADDRESS,XYZ_DATA_CFG_REG, FULL_SCALE_2G); //2G
//	writebyte(MMA845x_IIC_ADDRESS,MMA845x_HP_FILTER_CUTOFF, PULSE_LPF_EN_MASK );//enable Low-Pass-filtered	
	writebyte(MMA845x_IIC_ADDRESS,CTRL_REG1, (ACTIVE_MASK+ASLP_RATE_20MS+DATA_RATE_5MS)&(~FREAD_MASK)); //����״̬   14bit
    v=readbyte(MMA845x_IIC_ADDRESS,WHO_AM_I_REG);
	if(v == MMA8451Q_ID) 
   	 	LED0 = 1;

}

//��Z�������  Z����������
int MMA845x_Read_Z()
{
    uint8_t  z=0;
    int  wz=0; 
   	z = readbyte(MMA845x_IIC_ADDRESS,OUT_Z_MSB_REG); //
  	wz = ((readbyte(MMA845x_IIC_ADDRESS,OUT_Z_LSB_REG))|z<<8);  	///	
    if(z>0x7f) //����������ٶ�ԭʼ��Ӧ��ֵ
	 {			          
		 wz=(~(wz>>2) + 1)&0X3FFF ; //��λȡ����һ��ȥ����Ч�ַ� 
		 wz=-wz;		    
	 }
    else
      {		
	    wz=(wz>>2)&0x3FFF ; 	//��λ��ȥ����Ч�ַ� 
	   
      }   
     if(wz > 4096)           //����������ľ䳵�ھ�����ʱ������ĽǶ�Ϊ��
     	wz = 4096;
     if(wz < -4096)
     	wz = -4096; 
	return  wz;
}  

/**********************************
**�������ƣ�Z_Average
**���� : m Ҫƽ���Ĵ���
          
**�����ƽ�����ֵ 
**��������:	��ƽ��ֵ
***********************************/
s16 Z_Average(u8 times)
{
	u8 i = 0;
	s16 k = 0 ;
	s32 all=0;
	for(i=0;i<times;i++)
	{
		all += MMA845x_Read_Z();	
	}
	k = (s16)(all /times);
	return (k);
}


//��Z��ļ��ٶ�  
float MMA8451x_GZ_Value()
{
    float G_Value=0,wz;
	wz=MMA845x_Read_Z();
    G_Value=wz*0.00025;
	return(G_Value);
}





