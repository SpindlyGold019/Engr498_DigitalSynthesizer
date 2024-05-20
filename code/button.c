#include "button.h"
#include "ADC.h"
#include "Systick_timer.h"

volatile long counter = 0;

void configure_Button_pin(){
    // 1. Enable the clock to GPIO Port A	
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;   
		
	// 2. Configure GPIO Mode to 'Input': Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOC->MODER &= ~(3UL<<(2*13));  	// Input (00)
	
	// 3. Configure GPIO Push-Pull to 'No Pull-up or Pull-down': No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR  &= ~(3<<(2*13));  // No pull-up, no pull-down
}

void config_EXTI(){
	
	//1. Enable the EXTI15_10 interrupt (including EXTI13) in NVIC using a function from CMSIS's core_cm4.h.
	NVIC_EnableIRQ(EXTI15_10_IRQn); 
	
	//2. Configure the SYSCFG module to link EXTI line 13 to GPIO PC13
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;							// Enable the clock to SYSCFG
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;     	// Clear the EXTI13 bits in SYSCFG's EXTICR4 register.
	SYSCFG->EXTICR[3] |=  SYSCFG_EXTICR4_EXTI13_PC; 	// Set PC13 (0010) as the EXTI13 source in SYSCFG_EXTICR4.

	// 3. Enable (unmask) the EXTI13 interrupt by setting its corresponding bit in the EXTI's IMR.
	EXTI->IMR1 |= (1<<13);     //Interrupt Mask Register (IMR): 0 = marked, 1 = not masked (i.e., enabled)
	
	//4. Enable interrupt trigger for both rising (button release) and falling (button press) edges.
	EXTI->RTSR1 |= (1<<13);  //Rising trigger selection register (RTSR):0 = disabled, 1 = enabled
	EXTI->FTSR1 &= ~(1<<13);  //Falling trigger selection register (FTSR): 0 = disabled, 1 = enabled
	
}


//int button(){
//	
//	if((GPIOC->IDR & GPIO_IDR_ID13) == GPIO_IDR_ID13){
//		return 1;
//	}
//	return 0;
//}



// ISR (interrupt handler) for EXTI15_10. Interrupt handlers are initially defined in startup_stml476xx.s.
void EXTI15_10_IRQHandler(void) {  
	// PR (Pending Register): Check if the interrupt is triggered by EXTI13, as EXTI 10-15 share this interrupt vector.	
	if ((EXTI->PR1 & EXTI_PR1_PIF13) == EXTI_PR1_PIF13) {
		// cleared by writing a 1 to this bit
		EXTI->PR1 |= EXTI_PR1_PIF13;
		
			
	}
}