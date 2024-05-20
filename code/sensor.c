#include "stm32l476xx.h"
//set a GPIO to TIM2 CH3
//AF2

volatile uint32_t arr = 65535;


void configure_PB10(){
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	GPIOB->MODER &= ~GPIO_MODER_MODE10;
	GPIOB->MODER |= GPIO_MODER_MODE10_1; //AF mode
	
	GPIOB->AFR[1] &= ~GPIO_AFRL_AFSEL3; 
	GPIOB->AFR[1] |= 1<<8; //sel tim2 ch3 AF1
	
	
	GPIOB->OTYPER &= ~(1<<10);
	GPIOB->PUPDR &= ~(2<<20);
}

void configure_PB6(){
	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	GPIOB->MODER &= ~GPIO_MODER_MODE6;
	GPIOB->MODER |= GPIO_MODER_MODE6_1; //AF mode
	
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6; 
	GPIOB->AFR[0] |= 2<<24; //sel tim4 ch1
	
	
	GPIOB->OTYPER &= ~(1<<6);
	GPIOB->PUPDR &= ~(2<<12);
}


void TIM2_CH3_init(){
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	
	TIM2->CR1 &= ~TIM_CR1_DIR;
	
	TIM2->PSC |= 40-1;
	TIM2->ARR &= 0x0;
	TIM2->ARR = 3000-1;//30ms period
	
	TIM2->CCR3 = 1; //10us pulse
	
	TIM2->CCMR2 &= ~TIM_CCMR2_CC3S; //for all these registers we need to make sure we set the channel 2 bits
	
	TIM2->CCMR2 &= ~TIM_CCMR2_OC3M;
	TIM2->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
	
	TIM2->CCER |= TIM_CCER_CC3E;
	
	TIM2->CR1 |= TIM_CR1_CEN;
	
}


void TIM4_CH1_init(){
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	TIM4->CR1 &= ~TIM_CR1_DIR;
	
	//TIM4->PSC |= 4-1;
	//TIM4->PSC |= 400-1;
	TIM4->PSC |= 4-1;
	TIM4->ARR &= 0x0;
	//TIM4->ARR = 10000-1;//1s
	//TIM4->ARR = 1000-1;//100ms
	TIM4->ARR = arr-1;//10ms
	//TIM4->ARR = 100-1;//10ms period
	
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM4->CCMR1 |= 1; //input on TI1
	
	TIM4->CCMR1 &= ~(15<<4);//disable digital filter IC1F
	
	TIM4->CCMR1 &= ~(3<<2);//disable prescalar
	
	TIM4->CCER |= 5<<1;//capture both edges
	
	TIM4->CCER |= 1;
	
	TIM4->DIER |= 3;
	
	NVIC_EnableIRQ(TIM4_IRQn);
	
	TIM4->CR1 |= TIM_CR1_CEN;
	
}

volatile uint32_t ccrP = 0;
volatile uint32_t ccrO = 0;
volatile uint32_t overflows = 0;
volatile uint32_t pulse_width = 0;
volatile float time;
float freq;

void TIM4_IRQHandler(void){
     if ((TIM4->SR & TIM_SR_UIF) == TIM_SR_UIF){
         TIM4->SR &= (~1);
         overflows++;
     }
     
     if((TIM4->SR & 2) == 2){
         ccrO = ccrP;
         ccrP = TIM4->CCR1;
         
         pulse_width = ((ccrP - ccrO));
         time = (pulse_width * 0.0343)/2;
         overflows = 0;
     } 
     freq = time;
 
}//end of interrupt

//float get_frequency(void) {
//	//frequency +=1;
//  //return 35.0;
//	return freq;
//}