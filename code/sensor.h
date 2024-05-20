#ifndef __STM32L476G_sensor_H
#define __STM32L476G_sensor_H

#include "stm32l476xx.h"

void configure_PB10(void);

void configure_PB6(void);

void TIM2_CH3_init(void);

void TIM4_CH1_init(void);

void TIM4_IRQHandler(void);

float get_frequency(void);


#endif /* __STM32L476G_sensor_H */