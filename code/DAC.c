#include "DAC.h"
#include "stm32l4xx.h"

void DAC_Init(void) {
  // Enable DAC clock
  RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN; //enable clock to DAC

  // Enable GPIOA clock
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //enable clock to GPIO pin PA4 for DAC output

 
  GPIOA->MODER &= ~(0x3UL << 8U);//clear mode
  GPIOA->MODER |= GPIO_MODER_MODE4_0; //analog mode

  DAC1->CR &= ~DAC_CR_EN1; //disable dac
	
	DAC1->CR |= 7<<3; //set software trigger
	
	DAC1->CR |= DAC_CR_CEN1; //calibrate and wait
	int i;
	for (i = 0; i <1000; i++){}
		
	DAC->CR &= ~DAC_CR_CEN1; //turn off calibration

  DAC1->MCR |= 2; //no output buffer

  DAC1->CR |= DAC_CR_EN1; //enable the DAC
}

void DAC_setValue(volatile uint16_t value) {
  // Write digital value to DAC_DHR12R1 register (12-bit right-aligned data)
  DAC1->DHR12R1 = value;
	DAC1->SWTRIGR |= 1<<0; //trigger the DAC
}