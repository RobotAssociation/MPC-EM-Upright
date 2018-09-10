#include "L3G4200D.h"
#include "IIC.h"

 //************初始化L3G4200D*********************************
void Init_L3G4200D(void)
{
    init_I2C();
	writebyte(L3G4200_Addr,CTRL_REG11, 0xE9);   //y,z axis disable  x axis enable
	writebyte(L3G4200_Addr,CTRL_REG22, 0x00);
	writebyte(L3G4200_Addr,CTRL_REG33, 0x08);
//	writebyte(L3G4200_Addr,CTRL_REG44, 0x30);	//+-2000ds
//	writebyte(L3G4200_Addr,CTRL_REG44, 0x10);	//+-500dps
	writebyte(L3G4200_Addr,CTRL_REG44, 0x00);	//+-250dps		
	writebyte(L3G4200_Addr,CTRL_REG55, 0x03);    //enable Low-Pass-filtered
}	
//******读取L3G4200D数据****************************************
int READ_L3G4200D(void)
{
   unsigned char BUF[2];
   int T_X=0; 
   BUF[0]= readbyte(L3G4200_Addr,OUT_X_L);
   BUF[1]= readbyte(L3G4200_Addr,OUT_X_H);
   T_X = (BUF[1]<<8)|BUF[0];
   
  // BUF[2]=Single_Read(L3G4200_Addr,OUT_Y_L);
  // BUF[3]=Single_Read(L3G4200_Addr,OUT_Y_H);
  // T_Y=	(BUF[3]<<8)|BUF[2];
  

 //  BUF[4]=Single_Read(L3G4200_Addr,OUT_Z_L);
 //  BUF[5]=Single_Read(L3G4200_Addr,OUT_Z_H);
 //  T_Z=	(BUF[5]<<8)|BUF[4];
   if(T_X > 32768)
   {
   		T_X = T_X - 65535 ;   //理论上减65535 
   }
   return (T_X + 90); //  - 60  为陀螺仪的零点偏移量  43
}