#include "MMA8451.h"
#include "LED.h" 
#include "IIC.h"



//初始化MMA845x.
//返回值:0,初始化成功;1,初始化失败.
void MMA845x_init()
{	
	uint8_t v=0;
	init_I2C();
	writebyte(MMA845x_IIC_ADDRESS,CTRL_REG1,ASLP_RATE_20MS+DATA_RATE_5MS);	
	writebyte(MMA845x_IIC_ADDRESS,XYZ_DATA_CFG_REG, FULL_SCALE_2G); //2G
//	writebyte(MMA845x_IIC_ADDRESS,MMA845x_HP_FILTER_CUTOFF, PULSE_LPF_EN_MASK );//enable Low-Pass-filtered	
	writebyte(MMA845x_IIC_ADDRESS,CTRL_REG1, (ACTIVE_MASK+ASLP_RATE_20MS+DATA_RATE_5MS)&(~FREAD_MASK)); //激活状态   14bit
    v=readbyte(MMA845x_IIC_ADDRESS,WHO_AM_I_REG);
	if(v == MMA8451Q_ID) 
   	 	LED0 = 1;

}

//读Z轴的数据  Z读到的数据
int MMA845x_Read_Z()
{
    uint8_t  z=0;
    int  wz=0; 
   	z = readbyte(MMA845x_IIC_ADDRESS,OUT_Z_MSB_REG); //
  	wz = ((readbyte(MMA845x_IIC_ADDRESS,OUT_Z_LSB_REG))|z<<8);  	///	
    if(z>0x7f) //补码求出加速度原始对应数值
	 {			          
		 wz=(~(wz>>2) + 1)&0X3FFF ; //移位取反加一再去掉无效字符 
		 wz=-wz;		    
	 }
    else
      {		
	    wz=(wz>>2)&0x3FFF ; 	//移位再去掉无效字符 
	   
      }   
     if(wz > 4096)           //如果不加这四句车在剧烈震动时，输出的角度为零
     	wz = 4096;
     if(wz < -4096)
     	wz = -4096; 
	return  wz;
}  

/**********************************
**函数名称：Z_Average
**输入 : m 要平均的次数
          
**输出：平均后的值 
**功能描述:	求平均值
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


//读Z轴的加速度  
float MMA8451x_GZ_Value()
{
    float G_Value=0,wz;
	wz=MMA845x_Read_Z();
    G_Value=wz*0.00025;
	return(G_Value);
}





