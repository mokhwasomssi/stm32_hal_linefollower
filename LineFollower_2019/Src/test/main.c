#include "main.h"
#include "peripheral.h"
#include "user_ADC.h"


/* USER CODE BEGIN PV */
//피비 

 int En_L_counts = 0;
 int En_R_counts = 0;
 int En_L_counts_Save = 0; 
 int En_R_counts_Save = 0;
 
 int Delta_T_counts_L = 0;
 int Delta_T_counts_R = 0;
 
 int TIM2_IT_reset_L = 0;
 int TIM3_IT_reset_R = 0;
 
 float MT_method_L = 0;
 float MT_method_R = 0;
 
 int PWM_L = 0;
 int PWM_R = 0;
 
 uint16_t PWM_Past_L = 0; 
 uint16_t PWM_Past_R = 0; 
 
float error_total_L = 0; //for intergral
float error_total_R = 0;

float error_past_L=0; //for differential
float error_past_R=0;

int Ref_RPM_L = 0; // command value
int Ref_RPM_R = 0;

int Count_L = 0, Count_R = 0;
char ADC_case;

// user function
void Forward(void);
void Left_turn(void);
void Right_turn(void);

int fputc(int ch, FILE *f);

void PID(int RPM_L ,int RPM_R ) ;

void Motor_Set(void);



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) //외인
{
  if (GPIO_Pin == GPIO_PIN_11)
  {
    count++;
  }
  
  if (GPIO_Pin == GPIO_PIN_15) 
  {
    En_L_counts++;
  }
  
  if (GPIO_Pin == GPIO_PIN_8) 
  {
    En_L_counts++;
  }
  
  if( ( ( GPIO_Pin == GPIO_PIN_15) || (GPIO_Pin == GPIO_PIN_8) ) && (TIM2_IT_reset_L == 1 )  )
  {
    Delta_T_counts_L = TIM2 -> CNT;
    MT_method_L = (float)(60 * 1000000 * En_L_counts_Save) / ( 156 * (10000 + Delta_T_counts_L) );  
    TIM2_IT_reset_L = 0 ;
    TIM2 -> CNT = 0;
  }

  /*Encoder_R_A*/
  
  if (GPIO_Pin == GPIO_PIN_6) 
  {
    En_R_counts++;
  }
  
  if (GPIO_Pin == GPIO_PIN_7) 
  {
    En_R_counts++;
  }
 

  if( ( (GPIO_Pin == GPIO_PIN_6) || (GPIO_Pin == GPIO_PIN_7) ) && (TIM3_IT_reset_R == 1 ) ) 
  {
    Delta_T_counts_R = TIM3 -> CNT;
    MT_method_R = (float)(60 * 1000000 * En_R_counts_Save) / ( 156 * (10000 + Delta_T_counts_R) );  
    TIM3_IT_reset_R = 0 ;
    TIM3 -> CNT = 0;
  }
 
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //타인
{
  if(htim -> Instance == TIM2) 
  {
    En_L_counts_Save = En_L_counts;
    En_L_counts = 0;
    TIM2_IT_reset_L = 1;
  }
  
   if(htim -> Instance == TIM3) 
  {
    En_R_counts_Save = En_R_counts;
    En_R_counts = 0;
    TIM3_IT_reset_R = 1;
  }
  
  if( htim -> Instance == TIM4 )
  {
    if(count >= 2) 
    {
      PID(Ref_RPM_L ,Ref_RPM_R);
    }
  }
  
}

int main(void)
{
  /* USER CODE BEGIN 1 */
  //메인
  
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_ADC2_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  
  
  HAL_TIM_Base_Start(&htim1);//TIM1 시작
   
  HAL_TIM_Base_Start_IT(&htim2) ;//TIM2 인터럽트
  HAL_TIM_Base_Start_IT(&htim3) ;//TIM3 인터럽트
  HAL_TIM_Base_Start_IT(&htim4) ;//TIM4 인터럽트

  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);//PWM_L 시작
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);//PWM_R 시작

   Forward();
   
   // reset B11 for use interrupt
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
  
  ADC_Set_Black();
  
  
  while (1)
  {  
    if(count == 1)
    {
      ADC_Set_White();
      ADC_Set_Offset();
    }
    ADC_Conversion();
    ADC_Case();
    Motor_Set();
    Led_On();
  }
}

/* USER CODE BEGIN 4 */
//함수
void Forward(void) //직진
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); //DIR_L = 1
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); //DIR_R = 1
}

void Left_turn(void)//좌회전
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);//DIR_L = 0 
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);//DIR_R = 1
}

void Right_turn(void)//우회전
{
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);//DIR_L = 1
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);//DIR_R = 0
}

int fputc(int ch, FILE *f) //UART1
{
    uint8_t temp[1]={ch};
    HAL_UART_Transmit(&huart1, temp, 1, 2);
  return(ch);
}

void PID(int RPM_L ,int RPM_R ) //피아이디
{
  float error_L = RPM_L -  MT_method_L;
  float error_R = RPM_R -  MT_method_R;

    PWM_L = (int)(PWM_Past_L + 0.015 *error_L + 0.05 * error_total_L + 0.02 * ( error_L - error_past_L ) );
    PWM_R = (int)(PWM_Past_R + 0.015 *error_R + 0.051 * error_total_R+ 0.02 * ( error_R - error_past_R ) );
    
    if (PWM_L >= 999) PWM_L = 999;
    if (PWM_R >= 999) PWM_R = 999;
    
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, PWM_L); //PWM_L 수정
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, PWM_R); //PWM_R 수정 
    
    PWM_Past_L = PWM_L;
    PWM_Past_R = PWM_R;
    
    error_past_L= error_L;
    error_past_R= error_R;
    
    error_total_L += 0.001 * error_L;
    error_total_R += 0.001 * error_R;
}


void Motor_Set(void) //케이스
{  
  switch(ADCcase){
  case 0x18:                                    // 0b 0001 1000       
    Ref_RPM_L = 400;
    Ref_RPM_R = 400;
    break;
    
  case 0x10:                                    // 0b 0001 0000
    Ref_RPM_L = 0;
    Ref_RPM_R = 400;
    break;
    
  case 0x08:                                    // 0b 0000 1000
    Ref_RPM_L = 400;
    Ref_RPM_R = 0;
    break;
                                                                
  case 0xDB:                                    // 0b 1101 1011
                                      
    /*HAL_Delay(1100);
    //PWM_L = 300;
    //PWM_R = 300;
     

    ADCcase = 0;
    HAL_Delay(150);*/
    break;
    
  case 0xD8:                                    // 0b 1101 1000
    Count_L++;
    if(Count_R != 0)    Count_R = 0;
    break;
    
  case 0x1B:                                    // 0b 0001 1011
    Count_L++;
    if(Count_R != 0)    Count_R = 0;
    break;
    
  case 0xFF:                                    // 0b 1111 1111
    break;
    
  default:
    //PWM_L = 0;
    //PWM_R = 0;
    break;
  }
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, PWM_L);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, PWM_R); 
}