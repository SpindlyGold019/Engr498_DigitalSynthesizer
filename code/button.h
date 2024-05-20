#ifndef UNTITLED_BUTTON_H
#define UNTITLED_BUTTON_H

#include "stm32l476xx.h"


// Modular function to initialize PA5 as an output pin, interfacing with the LD2 LED.
void configure_Button_pin();

void config_EXTI();

void EXTI15_10_IRQHandler(void);

// Modular function to turn on the LD2 LED.
//int button();



#endif //UNTITLED_BUTTON_H
