/*
 * processing.h
 *
 *  Created on: 2018/06/16
 *      Author: devox
 */

#ifndef PROCESSING_H_
#define PROCESSING_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "Gen.h"

#define NUM_OF_VOICES 4

extern Gen synth[NUM_OF_VOICES];
extern DMA_HandleTypeDef hdma_spi3_tx;
extern uint16_t circularbuffer[AUDIO_BLOCK_SIZE * 2 * 2];
extern uint16_t adc_buff[ADC_SAMPLE_LENGTH];
extern uint16_t adcResult1;
extern void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
extern void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s);
extern void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle);
extern void InitSynthesizer();

#endif /* PROCESSING_H_ */