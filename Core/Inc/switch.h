#ifndef __SWITCH_H__
#define __SWITCH_H__

#include "main.h"

#define ADC_THRESHOLD_ON 800
#define ADC_THRESHOLD_OFF 0

extern volatile uint32_t counter_sw1;
extern volatile uint32_t counter_sw2;
extern volatile uint32_t counter_sw3;
extern volatile uint8_t current_switch;

void Switch_Init(void);
void controlLED(uint32_t adc_value);

#endif /* __SWITCH_H__ */
