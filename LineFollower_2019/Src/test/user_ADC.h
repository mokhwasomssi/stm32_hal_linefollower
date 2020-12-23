#include "main.h"

#define a 0.3

void ADC_Case(void);
void Led_On(void);
void ADC_Set_Black(void);
void ADC_Set_White(void);
void ADC_Set_Offset(void);
void ADC_Conversion(void);

// for ADC
uint32_t ADC_Value[4];             // DMA
uint16_t ADC_Value_Con[8];         // seperate data
uint16_t ADC_Output[8] = {0};         // for LPF
char ADCcase = 0x00;                    // check Line

uint32_t ADC_Black[8] = {0};       // store black point       
uint32_t ADC_White[8] = {0};       // store white point
uint32_t ADC_Offset[8] = {0};      // offset

int i = 0, j = 0, num = 0;
int count = 0;

void ADC_Conversion(void)
{
  // DMA Start
  HAL_ADC_Start(&hadc2);
  HAL_ADC_Start(&hadc2);
  HAL_ADC_Start(&hadc2);
  HAL_ADC_Start(&hadc2);
  HAL_ADCEx_MultiModeStart_DMA(&hadc1,(uint32_t*) ADC_Value,4);

  // DMA °ª ºÐ¹è
  ADC_Value_Con[5] = ADC_Value[0] & 0x0000FFFF;
  ADC_Value_Con[4] = ADC_Value[1] & 0x0000FFFF;
  ADC_Value_Con[7] = ADC_Value[2] & 0x0000FFFF;
  ADC_Value_Con[6] = ADC_Value[3] & 0x0000FFFF;
  ADC_Value_Con[3] = ADC_Value[3] >> 16;
  ADC_Value_Con[0] = ADC_Value[2] >> 16;
  ADC_Value_Con[1] = ADC_Value[1] >> 16;
  ADC_Value_Con[2] = ADC_Value[0] >> 16;

  // LPF
  for( i = 0; i < 8 ; i++){
  ADC_Output[i] = ( 1.0 - a ) * ADC_Value_Con[i] + a * ADC_Output[i];
  }
}

void ADC_Case(void)
{
  if(count < 2)
  {
  if(ADC_Output[0] > 2 * ADC_Black[0])    ADCcase |= 0x80;  // 0b 1000 0000
  else ADCcase &= 0x7F;                         // 0b 0111 1111
    
  if(ADC_Output[1] > 2 * ADC_Black[1])    ADCcase |= 0x40;  // 0b 0100 0000
  else ADCcase &= 0xBF;                         // 0b 1011 1111
    
  if(ADC_Output[2] > 2 * ADC_Black[2])     ADCcase |= 0x20; // 0b 0010 0000
  else ADCcase &= 0xDF;                         // 0b 1101 1111        
    
  if(ADC_Output[3] > 2 * ADC_Black[3])    ADCcase |= 0x10;  // 0b 0001 0000
  else ADCcase &= 0xEF;                         // 0b 1110 1111        
  
  if(ADC_Output[4] > 2 * ADC_Black[4])    ADCcase |= 0x08;  // 0b 0000 1000
  else ADCcase &= 0xF7;                         // 0b 1111 0111
  
  if(ADC_Output[5] > 2 * ADC_Black[5])    ADCcase |= 0x04;  // 0b 0000 0100
  else ADCcase &= 0xFB;                         // 0b 1111 1011
  
  if(ADC_Output[6] > 2 * ADC_Black[6])     ADCcase |= 0x02; // 0b 0000 0010
  else ADCcase &= 0xFD;                         // 0b 1111 1101
  
  if(ADC_Output[7] > 2 * ADC_Black[7])    ADCcase |= 0x01;  // 0b 0000 0001
  else ADCcase &= 0xFE;                         // 0b 1111 1110 
  }
  
  else if(count >= 2)
  {
  if(ADC_Output[0] > ADC_White[0] - ADC_Offset[0])    ADCcase |= 0x80;               // 0b 1000 0000
  else if(ADC_Output[0] < ADC_Black[0] + ADC_Offset[0])     ADCcase &= 0x7F;       // 0b 0111 1111
    
  if(ADC_Output[1] > ADC_White[1] - ADC_Offset[1])    ADCcase |= 0x40;               // 0b 0100 0000
  else if(ADC_Output[1] < ADC_Black[1] + ADC_Offset[1]) ADCcase &= 0xBF;           // 0b 1011 1111
    
  if(ADC_Output[2] > ADC_White[2] - ADC_Offset[2])     ADCcase |= 0x20;              // 0b 0010 0000
  else if(ADC_Output[2] < ADC_Black[2] + ADC_Offset[2]) ADCcase &= 0xDF;           // 0b 1101 1111        
    
  if(ADC_Output[3] > ADC_White[3] - ADC_Offset[3])    ADCcase |= 0x10;               // 0b 0001 0000
  else if(ADC_Output[3] < ADC_Black[3] + ADC_Offset[3]) ADCcase &= 0xEF;           // 0b 1110 1111        
  
  if(ADC_Output[4] > ADC_White[4] - ADC_Offset[4])    ADCcase |= 0x08;               // 0b 0000 1000
  else if(ADC_Output[4] < ADC_Black[4] + ADC_Offset[4]) ADCcase &= 0xF7;           // 0b 1111 0111
  
  if(ADC_Output[5] > ADC_White[5] - ADC_Offset[5])    ADCcase |= 0x04;               // 0b 0000 0100
  else if(ADC_Output[5] < ADC_Black[5] + ADC_Offset[5]) ADCcase &= 0xFB;           // 0b 1111 1011
  
  if(ADC_Output[6] > ADC_White[6] - ADC_Offset[6])     ADCcase |= 0x02;              // 0b 0000 0010
  else if(ADC_Output[6] < ADC_Black[6] + ADC_Offset[6]) ADCcase &= 0xFD;           // 0b 1111 1101
  
  if(ADC_Output[7] > ADC_White[7] - ADC_Offset[7])    ADCcase |= 0x01;               // 0b 0000 0001
  else if(ADC_Output[7] < ADC_Black[7] + ADC_Offset[7]) ADCcase &= 0xFE;           // 0b 1111 1110
  }
}

void Led_On(void)
{
  // LED ON/OFF
  if(count == 0){
  if(ADC_Value_Con[0] > ADC_Black[0] * 2)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

  if(ADC_Value_Con[1] > ADC_Black[1] * 2)    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);    

  if(ADC_Value_Con[2] > ADC_Black[2] * 2)    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);

  if(ADC_Value_Con[3] > ADC_Black[3] * 2)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

  if(ADC_Value_Con[4] > ADC_Black[4] * 2)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

  if(ADC_Value_Con[5] > ADC_Black[5] * 2)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  if(ADC_Value_Con[6] > ADC_Black[6] * 2)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
  
  if(ADC_Value_Con[7] > ADC_Black[7] * 2)    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);  
  }
  
  else if(count != 0){
  if(ADC_Value_Con[0] > ADC_White[0] - ADC_Offset[0])    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

  if(ADC_Value_Con[1] > ADC_White[1] - ADC_Offset[1])    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);    

  if(ADC_Value_Con[2] > ADC_White[2] - ADC_Offset[2])    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);

  if(ADC_Value_Con[3] > ADC_White[3] - ADC_Offset[3])    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);

  if(ADC_Value_Con[4] > ADC_White[4] - ADC_Offset[4])    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);

  if(ADC_Value_Con[5] > ADC_White[5] - ADC_Offset[5])    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  if(ADC_Value_Con[6] > ADC_White[6] - ADC_Offset[6])    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
  
  if(ADC_Value_Con[7] > ADC_White[7] - ADC_Offset[7])    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
  else    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);  
  }
}
void ADC_Set_Black(void)
{
  HAL_Delay(100);
  while(num < 128)
  {
    ADC_Conversion();
    for(j=0;j<8;j++){
      ADC_Black[j] += ADC_Value_Con[j];
    }
    HAL_Delay(10);
    num++;
  }
  for(j=0;j<8;j++){
    ADC_Black[j] = ADC_Black[j] >> 7;
  }
}

void ADC_Set_White(void)
{   
  num = 0;
  HAL_Delay(100);
  
  if(count != 0){
    while(num < 128){
      ADC_Conversion();
      
      for(j=0;j<8;j++){
        ADC_White[j] += ADC_Value_Con[j];
      }
      
      HAL_Delay(10);
      num++;
    }
    
    for(j=0;j<8;j++){
      ADC_White[j] = ADC_White[j] >> 7;
    }
    
    count++;
  }
}

void ADC_Set_Offset(void)
{
  for(i = 0; i < 8; i++)
  {
    ADC_Offset[i] = (ADC_White[i] - ADC_Black[i]) * 2 / 10; 
  }
}