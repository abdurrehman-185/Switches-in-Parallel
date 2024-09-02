#include "adc.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

uint16_t raw[NUM_SAMPLES];
uint32_t potSum = 0;
uint8_t convCompleted = 0;
extern UART_HandleTypeDef hlpuart1;

void MX_ADC1_Init(void);
void MX_DMA_Init(void);

void ADC_Init(void)
{
    MX_DMA_Init();
    MX_ADC1_Init();
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)raw, NUM_SAMPLES);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    convCompleted = 1;
}

void calculate_and_print_average(void)
{
    uint16_t pot = potSum / NUM_SAMPLES;
    char msg[100];

    sprintf(msg, "Average POT: %hu\r\n", pot);
    HAL_UART_Transmit(&hlpuart1, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

    potSum = 0;
}

void MX_DMA_Init(void)
{
    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void MX_ADC1_Init(void)
{
    ADC_MultiModeTypeDef multimode = {0};
    ADC_ChannelConfTypeDef sConfig = {0};

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait = DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
    hadc1.Init.OversamplingMode = DISABLE;

    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    multimode.Mode = ADC_MODE_INDEPENDENT;
    if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
    {
        Error_Handler();
    }

    sConfig.Channel = ADC_CHANNEL_1;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}
