#include "usart.h"
#include "Interrupt.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "LED.h"
#include "Control.h"
#include "encoder.h"
#include "ImageSample.h"
#include "Direction.h"

unsigned char receive;
unsigned char PD=0;
char Fasongwancheng = 1;
 
 
unsigned char canshu=50; 
unsigned char canshu11=80; 
/************************************************************/
/*                       ��ʼ��uart0                        */
/************************************************************/
void LINFlex0_init(void)
{
//����LINFlex
  SIU.PCR[18].R = 0x0600;    // ��PB[2]����ΪLINFlex_0.TxDA
  SIU.PCR[19].R = 0x0100;    // ��PB[3]����ΪLINFlex_0.RxDA
  LINFLEX_0.LINCR1.B.INIT   = ENABLE;   // �����ʼ��
  LINFLEX_0.LINCR1.B.SLEEP  = DISABLE;  // ��ֹ˯��ģʽ
  LINFLEX_0.LINCR1.B.BF     = DISABLE;  // ���ID��ƥ�䲻�����ж�

  LINFLEX_0.UARTCR.B.UART   = 1;        // ����UARTģʽ
  LINFLEX_0.UARTCR.B.RXEN   = ENABLE;   // �������
  LINFLEX_0.UARTCR.B.TXEN   = ENABLE;   // ������
  LINFLEX_0.UARTCR.B.WL     = 1;        // 8λ����λ
//  LINFLEX_0.UARTCR.B.OP     = 1;      // żУ��
  LINFLEX_0.UARTCR.B.PCE    = DISABLE;  // ��ֹ��żУ��
  LINFLEX_0.UARTCR.B.TDFL   = 0;        // ���ͻ�����Ϊ1���ֽ�
  LINFLEX_0.UARTCR.B.RDFL   = 0;        // ���ջ�����Ϊ1���ֽ�

//���ò�����Ϊ9600
//������=����ʱ��1/(16*LFDIV)
//DIV_M����LFDIV���������֡�(DIV_F/16)����LFDIV��С�����֡�
//LINFLEX_0.LINIBRR.B.DIV_M = 1666;     // Baud Rate = 2400, In Case fipg_clock_lin = 64 MHz
//LINFLEX_0.LINFBRR.B.DIV_F = 11;       // Baud Rate = 2400, In Case fipg_clock_lin = 64 MHz
//LINFLEX_0.LINIBRR.B.DIV_M = 833;      // Baud Rate = 4800, In Case fipg_clock_lin = 64 MHz
//LINFLEX_0.LINFBRR.B.DIV_F = 11;       // Baud Rate = 4800, In Case fipg_clock_lin = 64 MHz
 LINFLEX_0.LINIBRR.B.DIV_M = 416;      // Baud Rate = 9600, In Case fipg_clock_lin = 64 MHz
 LINFLEX_0.LINFBRR.B.DIV_F = 11;       // Baud Rate = 9600, In Case fipg_clock_lin = 64 MHz
//LINFLEX_0.LINIBRR.B.DIV_M = 208;      // Baud Rate = 19200, In Case fipg_clock_lin = 64 MHz
//LINFLEX_0.LINFBRR.B.DIV_F = 5;        // Baud Rate = 19200, On Case fipg_clock_lin = 64 MHz
//LINFLEX_0.LINIBRR.B.DIV_M = 104;      // Baud Rate = 38400, In Case fipg_clock_lin = 64 MHz
//LINFLEX_0.LINFBRR.B.DIV_F = 3;        // Baud Rate = 38400, In Case fipg_clock_lin = 64 MHz
// LINFLEX_0.LINIBRR.B.DIV_M = 69;       // Baud Rate = 57600, In Case fipg_clock_lin = 64 MHz
// LINFLEX_0.LINFBRR.B.DIV_F = 7;        // Baud Rate = 57600, In Case fipg_clock_lin = 64 MHz
//LINFLEX_0.LINIBRR.B.DIV_M = 34;       // Baud Rate = 115200, In Case fipg_clock_lin = 64 MHz
//LINFLEX_0.LINFBRR.B.DIV_F = 12;       // Baud Rate = 115200, In Case fipg_clock_lin = 64 MHz

//�����жϣ�ʹ���жϹ���
  LINFLEX_0.LINIER.B.DRIE   = ENABLE;   // ���ݽ�������ж�
//LINFLEX_0.LINIER.B.DTIE   = ENABLE;   // ���ݷ�������ж�
//LINFLEX_0.LINIER.B.DBFIE  = ENABLE;   // ���ݻ��������ж�
//LINFLEX_0.LINIER.B.DBEIE  = ENABLE;   // ���ݻ��������ж�

//�����жϣ���ֹ�жϹ���
//LINFLEX_0.LINIER.R        = 0;        // ��ֹ�����ж�
//LINFLEX_0.LINIER.B.DRIE   = DISABLE;  // ���ݽ�������ж�
//LINFLEX_0.LINIER.B.DTIE   = DISABLE;  // ���ݷ�������ж�
//LINFLEX_0.LINIER.B.DBFIE  = DISABLE;  // ���ݻ��������ж�
//LINFLEX_0.LINIER.B.DBEIE  = DISABLE;  // ���ݻ��������ж�

  LINFLEX_0.UARTSR.B.DRF    = 1;        // ���������ɱ�־
  LINFLEX_0.UARTSR.B.DTF    = 1;        // ���������ɱ�־
  INTC.PSR[79].R = 0x03;                //����UART0���ж����ȼ�Ϊ3

  LINFLEX_0.LINCR1.B.INIT   = DISABLE;  // ��Ϊ����ģʽ
}



/************************************************************/
/*                    UART0����һ�ֽ�                       */
/************************************************************/
void UART0_TX(unsigned char data)
{
  LINFLEX_0.BDRL.B.DATA0 = data;        // д���跢�͵�����
  while(LINFLEX_0.UARTSR.B.DTF == 0) {} // �ȴ��������
  LINFLEX_0.UARTSR.B.DTF = 1;           // ���������ɱ�־λ
}

/************************************************************/
/*                    UART0����һ�ֽ�                       */
/************************************************************/
unsigned char UART0_RX(void)
{
  unsigned char datain;
  datain=LINFLEX_0.BDRM.B.DATA4;        // ��ȡ���յ�������
  LINFLEX_0.UARTSR.B.DRF = 1;           // ���������ɱ�־λ
  return datain;
}


/************************************************************/
/*                    UART0�жϺ���                         */
/************************************************************/
void UART0iner(void) 
{
	receive=UART0_RX();
	if(receive==0xff)
	{
	    PD=1;	
	}
	if(receive==0xfe)
	{
	    PD=2;	
	}
    if(receive==0xfd)
	{
	    PD=3;	
	}
	if(receive==0xfc)
	{
	    PD=4;	
	}
    if(receive==0xfb)
	{
	    PD=5;	
	}
	
	
		
	if(PD==1)
	{
	    if(receive!=0xff)
	    {
	    	Proportion=receive/10.0;
	        UART0_TX(receive);
	    }
	    LED3=0;
	   // LED4=0;    
	}
	
	if(PD==2)
    {
        if(receive!=0xfe)
        {
        	canshu=receive;
	        UART0_TX(receive);
        }
	    LED3=1;
	    LED4=0;    
	}
	
    if(PD==3)
	{
	    if(receive!=0xfd)
	    {
	        Derivative = receive/10.0;
	        //Angle_offset=receive/100.0;
	        UART0_TX(receive);
	    }
	   // LED4=1;
	}
	
	if(PD==4)
    {
        if(receive!=0xfc)
        {
            canshu11 = receive;
	        UART0_TX(receive); 
        }
       // LED3=0;
	}
	
    if(PD==5)
	{
	    if(receive!=0xfb)
	    {
	        SPEED = receive;
	        UART0_TX(receive);   
	    }
	   // LED3=1;
	}	
}






























