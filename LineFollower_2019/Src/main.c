/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sensor.h"
#include "PID.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//#define asdf Left_mark_flag == 1 && Right_mark_flag == 0 

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//피비 
int Stop_Count = 0;
int Left_mark_flag = 0 , Right_mark_flag = 0;

int Buzzer_Count = 500;
int temp1, temp2;
float Past_Val = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//피비피
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim); 

void Catch_Stop(void);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
  
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  //메인
  /* USER CODE END 1 */
  
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  
  HAL_TIM_Base_Start(&htim1);//Start TIM1 for PWM
  
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);//PWM_L 시작
  HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);//PWM_R 시작
  
  HAL_TIM_Base_Start_IT(&htim1) ; //TIMER IT for SENSOR_PID
  HAL_TIM_Base_Start_IT(&htim2) ;//TIMER IT for MT_method_L
  HAL_TIM_Base_Start_IT(&htim3) ;//TIMER IT for MT_method_R
  HAL_TIM_Base_Start_IT(&htim4) ;//TIMER IT for MOTOR_PID 
  
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); //DIR_L = 1
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); //DIR_R = 1
  
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0); //PWM_L 수정
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0); //PWM_R 수정 
 
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) //와일
  {  
    if(count == 0)
    {
      ADC_Conversion();
      for(i = 0; i<Number_of_sensors;i++)
        ADC_Black[i] = ADC_Value_Con[i];
    }
    else if(count == 1)
    {
      for(num = 0; num < 500; num++)
      {
        ADC_Conversion();
        HAL_Delay(1);
        ADC_Set_White();
        ADC_Set_Black();
      }
      ADC_Set_Offset();
      count++;
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
      HAL_Delay(100);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
    }
   
    else if(count == 3 || count == 4)
    {
      ADC_Conversion();
      SENSOR_Binarizing(Left_mark_flag, Right_mark_flag);
      Led_On();
      Catch_Stop();
    } 
    
    else if(count ==5)
    {
      count =3 ;
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void Catch_Stop(void)
{
  // 정지 마크 검출  
  if(Left_mark_flag == 1 && Right_mark_flag == 1)
  {
    if(Cross_Count == 0)
    {
      Stop_Count++;
      Left_mark_flag = 0;
      Right_mark_flag  = 0;
    }
    else
    {
      Cross_Count = 0;
      Left_mark_flag = 0;
      Right_mark_flag  = 0;
    }
  }    
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) //외인
{
/*EXTERNAL_INTERRUPT_BEGIN*/ 
  
  /*LEFT MARK CHECKING BEGIN*/ 
  if (GPIO_Pin == GPIO_PIN_13)
  {
    if(Left_mark_flag == 0) 
    {
       if(Buzzer_Count < 100)
      {
        Left_mark_flag = 1;
      }
        
      else
      {
        Left_mark_flag = 1;
        Right_mark_flag = 0;
      }
    }
    else Left_mark_flag = 0;
    
      Buzzer_Count = 0;
  }
  /*LEFT MARK CHECKING END*/ 
  
  /*RIGHT MARK CHECKING BEGIN*/ 
  if (GPIO_Pin == GPIO_PIN_14)
  {
    if(Right_mark_flag == 0) 
    {
      if(Buzzer_Count < 100)
      {
        Right_mark_flag = 1;
      }
        
      else
      {
        Right_mark_flag = 1;
        Left_mark_flag = 0;
      }
    }
    
    else Right_mark_flag = 0;
    
      Buzzer_Count = 0;
  }
  /*RIGHT MARK CHECKING END*/ 
  
  if (GPIO_Pin == GPIO_PIN_11)
  {
    count++;
  }
  
/*EXTERNAL_INTERRUPT_END*/ 
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //타인
{
/*TIMER_INTERRUPT_BEGIN*/   
  if(htim -> Instance == TIM1) //Sensor_PID
  {   
    if(count == 3)
    {
      if(Stop_Count != 2)
      {
        if(Left_mark_flag == 1)    
        { 
          Kp = 60;
          SENSOR_PID(410,900);
        }
        
        else if(Right_mark_flag == 1)    
          {  
            Kp = 60;
            SENSOR_PID(910,400);
          }
     
        else
        {
          /*if(Error != 0)
          { 
            if(Buzzer_Count > 40)
            {
              Ki = 0.000;
              SENSOR_PID(815 , 800);
            }
          }
          
          else  
          { 
             Ki = 0.000;
             SENSOR_PID(815 , 800);
          }
          
          I = 0;
        }*/
          if(Buzzer_Count > 40)
            {
              Ki = 0.000;
              I = 0;
              SENSOR_PID(815 , 800);
            }
        }
      }
    
      else if(Stop_Count == 2)
      {
        if(Buzzer_Count > 150)
        {
          __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
          __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);
        }
        else
          SENSOR_PID(815 , 800);
      }
    }
    
     else if(count == 4)
    {
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0); //PWM_L 수정
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0); //PWM_R 수정 
      Left_mark_flag = 0;
      Right_mark_flag = 0;
      Stop_Count = 0;
    }
    
    
     if(Buzzer_Count < 250){
      Buzzer_Count++;
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
    }
    else HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);

  }
  
/*TIMER_INTERRUPT_END*/ 
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
