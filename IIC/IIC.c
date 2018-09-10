#include "IIC.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "LED.h"



/************************************************************/
/*                       ��ʼ��I2Cģ��                      */
/************************************************************/
void init_I2C(void)
{
	SIU.PCR[10].R = 0x0b23;        //����PA[10]ΪSDA�ӿ�,ʹ����������
	SIU.PCR[11].R = 0x0b23;        //����PA[11]ΪSCL�ӿ�,ʹ����������	

	I2C.IBFD.R=0x65;//SCLƵ��Ϊ100kHz
	//��Ƶϵ��SCL Divider = MUL x {2 x (scl2tap + [(SCL_Tap -1) x tap2tap] + 2)}
	//MUL=2�sscl2tap=14, tap2tap=16,  SCL_Tap=10����Ƶϵ��Ϊ640
	//SCLƵ��Ϊ100kHz
	I2C.IBAD.R=0;         //���õ�Ƭ���Ĵӻ���ַΪ0
	I2C.IBCR.B.MDIS=0;    //ʹ��I2Cģ��
}

/************************************************************/
/*                       ����I2C����                        */
/************************************************************/
void I2C_start()
{
	while(I2C.IBSR.B.IBB)   //�ȴ����߿���
	{ }

	I2C.IBCR.B.MS=1;     //����I2CΪ��ģʽ
	I2C.IBCR.B.TX=1;     //����I2CΪ����ģʽ
}

/************************************************************/
/*                   I2C����һ�ֽ�����                      */
/************************************************************/
uint8_t I2C_send(uint8_t data)
{
	I2C.IBDR.R=data;   //��ֵ��Ҫ���͵�����
	
	while(I2C.IBSR.B.IBIF==0)  //�ȴ����ݷ������
	{	}
	I2C.IBSR.B.IBIF=1;

	if(I2C.IBSR.B.RXAK)	//�ж��Ƿ���Ӧ��
		return 1;//û��Ӧ��
	else
	    return 0;//��Ӧ��
}

/************************************************************/
/*                   I2C����һ�ֽ�����                      */
/************************************************************/
uint8_t I2C_receive(uint8_t txak)
{
	unsigned char temp;
	if(txak==1)   //����Ҫ����Ӧ��λ
		I2C.IBCR.B.NOACK=1;
	else    //��Ҫ����Ӧ��λ
		I2C.IBCR.B.NOACK=0;
	
	I2C.IBCR.B.TX=0;     //����I2CΪ����ģʽ
	
	temp=I2C.IBDR.R; //��ȡ���ݼĴ���������һ�ν���
		
	while(I2C.IBSR.B.IBIF==0)  //�ȴ��������
	{	}
	
	I2C.IBSR.B.IBIF=1;   //�����־λ
	
	temp= I2C.IBDR.R;    //��ȡ���յ������ݣ�ͬʱҲ�ᴥ��һ�ν���
	
	while(I2C.IBSR.B.IBIF==0)  //�ȴ��������
	{	}
	
	I2C.IBSR.B.IBIF=1;  //�����־λ
	
	return temp;   //���ؽ��յ�������
}

/************************************************************/
/*                      I2C����ֹͣ                         */
/************************************************************/
void I2C_stop(void)
{
	I2C.IBCR.B.MS=0 ;	//����Ϊ��ģʽ��ֹͣ����
}

/************************************************************/
/*                   I2C������������                        */
/************************************************************/
void I2C_restart(void)
{
	I2C.IBCR.B.RSTA=1;
}

/************************************************************/
/*                   I2Cд����豸һ���ֽ�                  */
/************************************************************/
uint8_t writebyte(uint8_t SlaveAddress,uint8_t address, uint8_t thedata)
{
	I2C_start();		//����  
	
	if(I2C_send(SlaveAddress))	//д���豸ID��д�ź�
	{
		I2C_stop();
		return 1;
	}	  
	if(I2C_send(address))	//X��ַ
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
/*                   I2C���豸��һ���ֽ�                  */
/************************************************************/
uint8_t readbyte(uint8_t SlaveAddress,uint8_t address)
{
    uint8_t data = 0;
	 
	I2C_start();		//���� 
	if(I2C_send(SlaveAddress))	//д���豸ID��д�ź�
	{
		I2C_stop();
		return ;
	}	 
	if(I2C_send(address))	//X��ַ
	{
		I2C_stop();
		return ;
	}
		
	I2C_restart();        //��������	  
		
	if(I2C_send(SlaveAddress+1))  //д���豸ID������
	{
		I2C_stop();
		return ;
	}
		 
	data = I2C_receive(1);     //��ȡ����
	I2C_stop();
	return data;	
}