#include "stm32l476xx.h"
#include "ADC.h"
#include "DAC.h"
#include "button.h"
#include "Systick_timer.h"
#include "sensor.h"
#include "encoder.h"


int main(void){

	ADC_Init();
	DAC_Init();
	SysTick_Init(2000); // 2k samples rate 50hz-500 freq
	//configure_Button_pin();
	configure_PB10(); // Trig #25
	
	configure_PB6(); // Echo pin #17
	
	TIM2_CH3_init();
	
	TIM4_CH1_init();
	
	PC2();
	PC3();
	configure_EXTI();
	
	//config_EXTI();
	
	while(1){
		
	}
}



