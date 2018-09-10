#include "Kalman_Filter.h"
#include "Visual_data.h"
#include "ADC.h"
#include "MMA8451.h"
#include "L3G4200D.h"

//
//#define Official  enable
#define Balance   enable       //这三个只能选一个
//#define Kalman    enable

int OutData[4] = {0};
#define aa 0.98  //互补滤波用的



float Gyro_offset;
float Gyro,Acc_z;
float Acc_z_Angle,Gyro_AngleSpeed;
float angle_dot,angle;
float CarAngleIntegral;
extern float Gyro_offset;





/************************************************************************************/
/*******************************求arccos(x)的函数************************************/
/*********************************利用泰勒展开式求***********************************/
/************************************************************************************/
float acos(double x)
{
	float a = 0.0;
	a = (5.0*x*x*x*x*x*x*x)/112.0 - (3.0*x*x*x*x*x)/40.0 - (x*x*x)/6.0 - x - 1.5708;
	return a;		
}
/************************************************************************************/
/*******************************求arcsin(x)的函数************************************/
/*********************************利用泰勒展开式求***********************************/
/************************************************************************************/
float asin(double x)
{
	float a = 0.0;
	a = x + x*x*x/6 + (3*x*x*x*x*x)/40 + (5*x*x*x*x*x*x*x)/112;
	return a;		
}

/**************Kalman滤波****************/
float angle, angle_dot;         //外部需要引用的变量
//static const char C_0 = 1;
//float Q_angle=0.0001, Q_gyro=0.003, R_angle=0.5,dt=0.005;

static const char C_0 = 1;
float Q_angle=0.0001, Q_gyro=0.0035, R_angle=0.5,dt=0.0053;

static float P[2][2] = {{ 1, 0 },{ 0, 1 }}; 
static float Pdot[4] ={0,0,0,0};
float q_bias, angle_err, PCt_0, PCt_1, E, K_0, K_1, t_0, t_1;
/**************Kalman滤波****************/
void Kalman_Filter(float angle_m,float gyro_m)  //gyro_m:gyro_measure
{
    angle+=(gyro_m-q_bias) * dt;
    angle_err = angle_m - angle;   
	 
    Pdot[0]=Q_angle - P[0][1] - P[1][0];
    Pdot[1]=- P[1][1];
    Pdot[2]=- P[1][1];
    Pdot[3]=Q_gyro;
    
    P[0][0] += Pdot[0] * dt;
    P[0][1] += Pdot[1] * dt;
    P[1][0] += Pdot[2] * dt;
    P[1][1] += Pdot[3] * dt;
     
    PCt_0 = C_0 * P[0][0];
    PCt_1 = C_0 * P[1][0];
    
    E = R_angle + C_0 * PCt_0;
    
    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;
    
    t_0 = PCt_0;
    t_1 = C_0 * P[0][1];

    P[0][0] -= K_0 * t_0;
    P[0][1] -= K_0 * t_1;
    P[1][0] -= K_1 * t_0;
    P[1][1] -= K_1 * t_1;
      
    angle   += K_0 * angle_err;
    q_bias  += K_1 * angle_err;
    angle_dot = gyro_m-q_bias;
}

/******************************官方滤波****************************/
void Official_filter()
{	
	float fDeltaValue;
	angle = CarAngleIntegral;
	fDeltaValue = (Acc_z_Angle - angle) / 2 ; //b=3不要太小
	CarAngleIntegral += (Gyro_AngleSpeed + fDeltaValue) * 0.005;
}
/******************************官方滤波****************************/

/******************************互补滤波************************/
void Balance_Filter()
{
	angle = aa*(angle + Gyro_AngleSpeed * 0.005) +  (1-aa) * Acc_z_Angle;
}
/******************************互补滤波************************/


/*************************外部调用滤波*****************************/
int Filter()
{
 
 // Gyro = Get_Adc_Average_ch1(20);
 // Gyro = Gyro - Gyro_offset;
 // Gyro_AngleSpeed =  Gyro * 7;    //(3300/4096)/（0.67*9.1） = 0.13214   7
  //OutData[0] = Gyro_AngleSpeed;
  	
  //Acc_z = Get_Adc_Average_ch2(20);    //180/(430-190)=0.75
 // Acc_z -= Acc_z_offset;
 // Acc_z_Angle = Acc_z*0.75;
 // OutData[1] = Acc_z_Angle*10;
  
  
  Gyro = READ_L3G4200D();
                                //官方滤波  乘以2.82
  Gyro_AngleSpeed = Gyro * 0.0095;  //或卡尔曼滤波 乘以2.75      0.0245 
 //  OutData[0] = Gyro_AngleSpeed;                                                 //互补滤波 //0.0253872是为了调整调整陀螺仪与加速度对应
 
  Acc_z = (float)MMA845x_Read_Z(); 
  Acc_z_Angle = asin(Acc_z/4096);
  Acc_z_Angle *= -57.296 ;               //180/3.14159 = 57.29578 
  
 // OutData[1] = Acc_z_Angle * 10;
  

  #ifdef Kalman
  //	OutData[0] = angle_dot;
  #else		
 // 	OutData[0] = Gyro_AngleSpeed; 
  #endif      
  
  
 /****************官方滤波****************/
 #ifdef Official
 
  	Official_filter();
  
 #endif 
/****************官方滤波****************/


/**************Kalman滤波****************/
#ifdef Kalman

  Kalman_Filter(Acc_z_Angle,Gyro_AngleSpeed);

#endif  
/**************Kalman滤波****************/ 


/******************************互补滤波************************/
#ifdef Balance

	Balance_Filter();
		
#endif
/******************************互补滤波************************/


 // OutData[3] = angle*10;
 // Visual_Data(OutData);//数据发回上位机  		
}













