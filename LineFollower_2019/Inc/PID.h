#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"
#include "sensor.h"

float Kp = 55;
float Ki = 0.2;
float Kd = 0.5;

float Error = 0;
float Previous_Error = 0;
  
float P = 0;
float I = 0;
float D = 0;
float PID_Value = 0;

int PWM_L = 0; 
int PWM_R = 0;
//int PWM_LL = 0; 
//int PWM_RR = 0;

int Cross_Count = 0;


void SENSOR_Binarizing(int LMF, int RMF)
{
  int Sensor_Count;
  float N_Error, P_Error;
  Sensor_Count = 0;
  
  for(int i = 0; i < Number_of_sensors; i++)
  {
    if(ADC_Value_Con[i] > ADC_White[i] - ADC_Offset[i] ){
      binarized[i] = 1;
      Sensor_Count++;
    }
    else if(ADC_Value_Con[i] < ADC_Black[i] + ADC_Offset[i] )    
      binarized[i] = 0;
  }    
  
   N_Error = binarized[1]*3 + binarized[2]*2 + binarized[3]*1 ;
    P_Error = binarized[4] + binarized[5]*2 + binarized[6]*3;      
  
  /* 가중치 변환
  if(LMF == 0 && RMF == 0)
  {
    N_Error = binarized[1]*3 + binarized[2]*2 + binarized[3]*1 ;
    P_Error = binarized[4] + binarized[5]*2 + binarized[6]*3;    
  }

  else if(LMF == 1)
  {
    N_Error = binarized[1]*4 + binarized[2]*3 + binarized[3]*2 + binarized[4];
    P_Error = binarized[5]*1 + binarized[6]*2;
  }
  else if(RMF == 1)
  {       
    N_Error = binarized[1]*2 + binarized[2]*1;
    P_Error = binarized[3] + binarized[4]*2 + binarized[5]*3 + binarized[6]*4;
  }
  */
    
  if(Sensor_Count == 0) 
  {
    Error = 0;
    Sensor_Count++;         // 0으로 나누지 않게 0이어도 1로 변환
  }
  
  if(N_Error == 1 && P_Error ==1)
  {
    Error = 0;
  }
  
  else if(N_Error > P_Error)
  {
    Error = N_Error / Sensor_Count;
    Error = -Error;
  }
  
  else if(N_Error < P_Error)
  {
    Error = P_Error / Sensor_Count;
  }
  
  if(Sensor_Count >= 6) 
    Cross_Count = 1;  
}

void SENSOR_PID(int PWM_LL , int PWM_RR)
{  
  P = Error;
  I = I + Error;
  D = Error - Previous_Error;
  
  PID_Value = (Kp*P) + (Ki*I) + (Kd*D);
  
  Previous_Error = Error;
  
  PWM_L = PWM_LL + PID_Value;
  PWM_R = PWM_RR - PID_Value;
  
  if(PWM_L > 999) PWM_L = 999;   
  if(PWM_R > 999) PWM_R = 999;   
  if(PWM_L < 0) PWM_L  = 0;      
  if(PWM_R < 0) PWM_R  = 0;  
  
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, PWM_L); 
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, PWM_R); 
}