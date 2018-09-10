#ifndef __USART_H_
#define __USART_H_
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */






#define   DISABLE             0         // Bit-Field Is Disabled
#define   ENABLE              1         // Bit-Field Is Enabled


# define THRESHOLD 145    //112


extern unsigned char PD;
extern char Fasongwancheng; 


extern unsigned char canshu;
extern unsigned char canshu11;

void LINFlex0_init(void);
void UART0_TX(unsigned char data);
void ImageShow(uint8_t ucShowFlag);

//void send_string(unsigned char *putchar);




#endif