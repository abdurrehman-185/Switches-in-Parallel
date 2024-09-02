#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "main.h"

#define NUM_SAMPLES 3000

extern uint16_t raw[NUM_SAMPLES];
extern uint32_t potSum;
extern uint8_t convCompleted;
extern ADC_HandleTypeDef hadc1; // Added extern for hadc1

void ADC_Init(void);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
void calculate_and_print_average(void);
void MX_ADC1_Init(void);   // Added function prototype
void MX_DMA_Init(void);    // Added function prototype

#endif /* INC_ADC_H_ */
