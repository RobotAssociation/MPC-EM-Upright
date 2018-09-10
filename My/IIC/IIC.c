#include "IIC.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "LED.h"



/************************************************************/
/*                       初始化I2C模块                      */
/************************************************************/
void init_I2C(void)
{
	SIU.PCR[10].R = 0x0b23;        //设置PA[10]为SDA接口,使能上拉电阻
	SIU.PCR[11].R = 0x0b23;        //设置PA[11]为SCL接口,使能上拉电阻	

	I2C.IBFD.R=0x65;//SCL频率为100kHz
	//分频系数SCL Divider = MUL x {2 x (scl2tap + [(SCL_Tap -1) x tap2tap] + 2)}
	//MUL=2scl2tap=14, tap2tap=16,  SCL_Tap=10，分频系数为640
	//SCL频率为100kHz
	I2C.IBAD.R=0;         //设置单片机的从机地址为0
	I2C.IBCR.B.MDIS=0;    //使能I2C模块
}

/************************************************************/
/*                       启动I2C总线                        */
/************************************************************/
void I2C_start()
{
	while(I2C.IBSR.B.IBB)   //等待总线空闲
	{ }

	I2C.IBCR.B.MS=1;     //设置I2C为主模式
	I2C.IBCR.B.TX=1;     //设置I2C为发送模式
}

/************************************************************/
/*                   I2C发送一字节数据                      */
/************************************************************/
uint8_t I2C_send(uint8_t data)
{
	I2C.IBDR.R=data;   //赋值需要发送的数据
	
	while(I2C.IBSR.B.IBIF==0)  //等待数据发送完毕
	{	}
	I2C.IBSR.B.IBIF=1;

	if(I2C.IBSR.B.RXAK)	//判断是否有应答
		return 1;//没有应答
	else
	    return 0;//有应答
}

/************************************************************/
/*                   I2C接收一字节数据                      */
/************************************************************/
uint8_t I2C_receive(uint8_t txak)
{
	unsigned char temp;
	if(txak==1)   //不需要发送应答位
		I2C.IBCR.B.NOACK=1;
	else    //需要发送应答位
		I2C.IBCR.B.NOACK=0;
	
	I2C.IBCR.B.TX=0;     //设置I2C为接收模式
	
	temp=I2C.IBDR.R; //读取数据寄存器，启动一次接收
		
	while(I2C.IBSR.B.IBIF==0)  //等待接收完成
	{	}
	
	I2C.IBSR.B.IBIF=1;   //清除标志位
	
	temp= I2C.IBDR.R;    //读取接收到的数据，同时也会触发一次接收
	
	while(I2C.IBSR.B.IBIF==0)  //等待接收完成
	{	}
	
	I2C.IBSR.B.IBIF=1;  //清除标志位
	
	return temp;   //返回接收到的数据
}

/************************************************************/
/*                      I2C总线停止                         */
/************************************************************/
void I2C_stop(void)
{
	I2C.IBCR.B.MS=0 ;	//设置为从模式，停止总线
}

/************************************************************/
/*                   I2C总线重新启动                        */
/************************************************************/
void I2C_restart(void)
{
	I2C.IBCR.B.RSTA=1;
}

/************************************************************/
/*                   I2C写入从设备一个字节                  */
/************************************************************/
uint8_t writebyte(uint8_t SlaveAddress,uint8_t address, uint8_t thedata)
{
	I2C_start();		//启动  
	
	if(I2C_send(SlaveAddress))	//写入设备ID及写信号
	{
		I2C_stop();
		return 1;
	}	  
	if(I2C_send(address))	//X地址
	{
		I2C_stop();
		return 1;
	}	  	
	if(I2C_send(thedata))  //
	{
		I2C_stop();
		return 1;
	}	 
	I2C_stop();
	return 0;
}

/************************************************************/
/*                   I2C从设备读一个字节                  */
/************************************************************/
uint8_t readbyte(uint8_t SlaveAddress,uint8_t address)
{
    uint8_t data = 0;
	 
	I2C_start();		//启动 
	if(I2C_send(SlaveAddress))	//写入设备ID及写信号
	{
		I2C_stop();
		return ;
	}	 
	if(I2C_send(address))	//X地址
	{
		I2C_stop();
		return ;
	}
		
	I2C_restart();        //重新启动	  
		
	if(I2C_send(SlaveAddress+1))  //写入设备ID及读信
	{
		I2C_stop();
		return ;
	}
		 
	data = I2C_receive(1);     //读取数据
	I2C_stop();
	return data;	
}