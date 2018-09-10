#include "Direction.h"
#include "ImageProcess.h"
#include "Kalman_Filter.h"
#include "Visual_data.h"
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */
#include "encoder.h"
#include "usart.h"
#include "LED.h"
#include "ADC.h"


float Proportion = 1.5;  //1.2  4.2
float Derivative = 0;           //方向PD控制


float DirectionControlOut = 0;
float DirectionControlOutNew = 0 ;
float DirectionControlOutOld = 0 ;
unsigned char DirectionControlPeriod = 0;
float error_direction=0.0;		//实际速度与控制速度的偏差

//原方向控制
#define   N   10
uint16_t  AD_data[4][N];
uint16_t  AD_value[6]={0};
#define   ad_left_offset     0
#define   ad_right_offset    0
float  g_fleftVoltageSigma=0, g_frightVoltageSigma=0;
float ad_he, ad_cha;

void AD_zhi()
{
 	int i=0,j=0,k=0;
 	uint16_t AD_temp=0,AD_sum[4];
 	
 	for(j=0;j<N;j++)
	{
      
      AD_data[0][j]=AD_Average_CH2(5);         //每个通道取N次值  此时N=10
      AD_data[1][j]=AD_Average_CH3(5);
      AD_data[2][j]=AD_Average_CH32(5);
      AD_data[3][j]=AD_Average_CH33(5);
      
	}
 	
 	for(i=0;i<6;i++)
	{
		for(j=0;j<N;j++)
		{
		    for(k=0;k<N-j;k++)
		    {
		       if(AD_data[i][k]>AD_data[i][k+1])
		       {
		       	AD_temp=AD_data[i][k];
		       	AD_data[i][k]=AD_data[i][k+1];
		       	AD_data[i][k+1]=AD_temp;
		       }
		    }
		}
	}
 
 	for(i=0;i<6;i++)
	{
		AD_sum[i]=AD_data[i][1]+AD_data[i][2]+AD_data[i][3]+AD_data[i][4]+AD_data[i][5]+AD_data[i][6]+AD_data[i][7]+AD_data[i][8]; 
        
	}
	
//	AD_value[0] = AD_sum[0]/8; 
//    AD_value[1] = AD_sum[1]/8;
    AD_value[2] = AD_sum[2]/8;      //右侧电感
    AD_value[3] = AD_sum[3]/8;      //左侧电感
 		
 	
//	OutData[0]=AD_value[0];
//	OutData[1]=AD_value[1];
	OutData[2]=AD_value[2];
	OutData[3]=AD_value[3];	
}


uint16_t abs(int  x)
	{
	    if(x<0)  return - x;
	    else     return x;
	}

float abs_f(float x)
	{
	    if(x<0)  return - x;
	    else     return x;
	}



void DirectionControl(void)
{
	float fvalue;
	static float last_direction=0.0;
	float n_left,n_right;
 	
 	AD_zhi();
	if(AD_value[3]>ad_left_offset)       n_left=AD_value[3]-ad_left_offset;
	else                                 n_left=0;
	if(AD_value[2]>ad_right_offset)	     n_right=AD_value[2]-ad_right_offset;
	else                                 n_right=0;
	
		
	ad_he=n_left+n_right;
	ad_cha=n_left-n_right;
	
	DirectionControlOutOld=DirectionControlOutNew;
	DirectionControlOutNew=Derivative*(error_direction - last_direction)+Proportion*ad_cha;
	last_direction=error_direction;   
    
}



void DirectionControlOutput(void)
{
	float Value;
	Value=DirectionControlOutNew-DirectionControlOutOld;
	DirectionControlOut=DirectionControlOutOld+DirectionControlPeriod*Value /2.0;         		                
}
