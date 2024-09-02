#include "switch.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef hlpuart1;

volatile uint32_t counter_sw1 = 0;
volatile uint32_t counter_sw2 = 0;
volatile uint32_t counter_sw3 = 0;
volatile uint8_t current_switch = 1;
uint32_t switchOnTime = 0;
uint32_t switchDuration = 0;

void Switch_Init(void)
{
}

void controlLED(uint32_t adc_value)
{
    static uint8_t led_state = 0;
    char msg[100];

    if (adc_value >= ADC_THRESHOLD_ON && led_state == 0) {

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
        led_state = 1;
        switchOnTime = HAL_GetTick();  // Record the time when the switch was turned on

        if (current_switch == 1) {
            counter_sw1++;
            sprintf(msg, "SW1 ON, LED ON (%lu times)\r\n", counter_sw1);
            current_switch = 2;
        } else if (current_switch == 2) {
            counter_sw2++;
            sprintf(msg, "SW2 ON, LED ON (%lu times)\r\n", counter_sw2);
            current_switch = 3;
        } else if (current_switch == 3) {
            counter_sw3++;
            sprintf(msg, "SW3 ON, LED ON (%lu times)\r\n", counter_sw3);
            current_switch = 1;
        }

        HAL_UART_Transmit(&hlpuart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

    } else if (adc_value == ADC_THRESHOLD_OFF && led_state == 1) {

        switchDuration = HAL_GetTick() - switchOnTime;  // Calculate how long the switch was on

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
        led_state = 0;

        sprintf(msg, "LED OFF, Switch was on for %lu ms\r\n", switchDuration);
        HAL_UART_Transmit(&hlpuart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
    }
}
