/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Left_Mark_Detect_Pin GPIO_PIN_13
#define Left_Mark_Detect_GPIO_Port GPIOC
#define Left_Mark_Detect_EXTI_IRQn EXTI15_10_IRQn
#define Right_Mark_Detect_Pin GPIO_PIN_14
#define Right_Mark_Detect_GPIO_Port GPIOC
#define Right_Mark_Detect_EXTI_IRQn EXTI15_10_IRQn
#define LED6_Pin GPIO_PIN_0
#define LED6_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOB
#define Switch_Pin GPIO_PIN_11
#define Switch_GPIO_Port GPIOB
#define Switch_EXTI_IRQn EXTI15_10_IRQn
#define DIR_L_Pin GPIO_PIN_12
#define DIR_L_GPIO_Port GPIOB
#define DIR_R_Pin GPIO_PIN_13
#define DIR_R_GPIO_Port GPIOB
#define PWM_L_Pin GPIO_PIN_14
#define PWM_L_GPIO_Port GPIOB
#define PWM_R_Pin GPIO_PIN_15
#define PWM_R_GPIO_Port GPIOB
#define Encoder_L_B_Pin GPIO_PIN_8
#define Encoder_L_B_GPIO_Port GPIOA
#define Encoder_L_B_EXTI_IRQn EXTI9_5_IRQn
#define LED2_Pin GPIO_PIN_11
#define LED2_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_12
#define LED3_GPIO_Port GPIOA
#define ST_LINK_SWDIO_Pin GPIO_PIN_13
#define ST_LINK_SWDIO_GPIO_Port GPIOA
#define ST_LINK_SWCLK_Pin GPIO_PIN_14
#define ST_LINK_SWCLK_GPIO_Port GPIOA
#define Encoder_L_A_Pin GPIO_PIN_15
#define Encoder_L_A_GPIO_Port GPIOA
#define Encoder_L_A_EXTI_IRQn EXTI15_10_IRQn
#define LED4_Pin GPIO_PIN_4
#define LED4_GPIO_Port GPIOB
#define LED5_Pin GPIO_PIN_5
#define LED5_GPIO_Port GPIOB
#define Encoder_R_A_Pin GPIO_PIN_6
#define Encoder_R_A_GPIO_Port GPIOB
#define Encoder_R_A_EXTI_IRQn EXTI9_5_IRQn
#define Encoder_R_B_Pin GPIO_PIN_7
#define Encoder_R_B_GPIO_Port GPIOB
#define Encoder_R_B_EXTI_IRQn EXTI9_5_IRQn
#define LED7_Pin GPIO_PIN_8
#define LED7_GPIO_Port GPIOB
#define LED8_Pin GPIO_PIN_9
#define LED8_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define a 0.3
#define Number_of_sensors 8
#define FILTERDATA 5

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
