#include "main.h"
#include "adc.h"
#include "dma.h"
//#include "gpio.h".

#ifndef __SENSOR_H_
#define __SENSOR_H_

int i = 0;
int num = 0;


int count = 0;

// for ADC
uint32_t ADC_Value[4];             // DMA
uint16_t ADC_Value_Con[8];         // seperate data
uint16_t ADC_Output[8] = {0};         // for LPF

uint32_t ADC_Black[8] = {0};       // store black point       
uint32_t ADC_White[8] = {0};       // store white point
uint32_t ADC_Offset[8] = {0};      // offset

uint32_t binarized[8] = {0};

void ADC_Conversion(void)
{
    // DMA Start
    HAL_ADC_Start(&hadc2);
    HAL_ADCEx_MultiModeStart_DMA(&hadc1,(uint32_t*) ADC_Value,4);

    // DMA °ª ºÐ¹è
    ADC_Value_Con[5] = ADC_Value[0] & 0x0000FFFF;
    ADC_Value_Con[4] = ADC_Value[1] & 0x0000FFFF;
    ADC_Value_Con[7] = ADC_Value[2] & 0x0000FFFF;
    ADC_Value_Con[6] = ADC_Value[3] & 0x0000FFFF;
    ADC_Value_Con[2] = ADC_Value[3] >> 16;
    ADC_Value_Con[3] = ADC_Value[2] >> 16;
    ADC_Value_Con[0] = ADC_Value[1] >> 16;
    ADC_Value_Con[1] = ADC_Value[0] >> 16;

    // LPF
    for( i = 0; i < 8 ; i++){
    ADC_Output[i] = ( 1.0 - a ) * ADC_Value_Con[i] + a * ADC_Output[i];
    }
}


void ADC_Set_Black(void)
{
  for(i=0;i<Number_of_sensors;i++){
    if(ADC_Black[i] > ADC_Value_Con[i]){
      ADC_Black[i] = ADC_Value_Con[i];
    }
  }
}

void ADC_Set_White(void)
{   
  for(i=0;i<Number_of_sensors;i++){
    if(ADC_White[i] < ADC_Value_Con[i]){
      ADC_White[i] = ADC_Value_Con[i];
    }
  }
}

void ADC_Set_Offset(void)
{
  for(i = 0; i < 8; i++)
  {
    ADC_Offset[i] = (ADC_White[i] - ADC_Black[i]) * 2 / 10; 
  }
}

void Led_On(void)
{
  if(binarized[0] == 1)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

  if(binarized[1] == 1)    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);    

  if(binarized[2] == 1)    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);

  if(binarized[3] == 1)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

  if(binarized[4] == 1)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

  if(binarized[5] == 1)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  if(binarized[6] == 1)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
  
  if(binarized[7] == 1)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);  
}
#else
#endif
