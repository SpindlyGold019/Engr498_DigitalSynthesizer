#include "stm32l4xx.h"  // Include the STM32 library
unsigned int wave = 0;


void PC3() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; //enable clock
	GPIOC->MODER &= ~GPIO_MODER_MODER3; //input mode
	GPIOC->OTYPER &= ~GPIO_PUPDR_PUPD3; //no pull up pull down
}
void PC2() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN; //enable clock
	GPIOC->MODER &= ~GPIO_MODER_MODER2; //input mode
	GPIOC->OTYPER &= ~GPIO_PUPDR_PUPD2; //no pull up pull down
}

int sw2(){
	if ((GPIOC->IDR & 1UL<<2) == 1UL<<2) { //if bit is high
			return 1; //negative logic so the button is not pressed
		} else
		return 0;
}

int sw3(){
	if ((GPIOC->IDR & 1UL<<3) == 1UL<<3) { //if bit is high
			return 1; //negative logic so the button is not pressed
		} else
		return 0;
}

void configure_EXTI(void){

	//1. Enable the EXTI2/3 interrupt (including EXTI13) in NVIC using a function from CMSIS's core_cm4.h.
  NVIC_EnableIRQ(EXTI2_IRQn);
	//NVIC_EnableIRQ(EXTI3_IRQn);	
	
	//2. Configure the SYSCFG module to link EXTI 2/3 to pc2/3
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;							// Enable the clock to SYSCFG
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI2;     	
	SYSCFG->EXTICR[0] |=  SYSCFG_EXTICR1_EXTI2_PC; 	
	
	//SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
	//SYSCFG->EXTICR[0] |=  SYSCFG_EXTICR1_EXTI3_PC;

	// 3. Enable (unmask) the EXTI13 interrupt by setting its corresponding bit in the EXTI's IMR.
	//EXTI->IMR1 |= (1<<3) | (1<<2);     //Interrupt Mask Register (IMR): 0 = marked, 1 = not masked (i.e., enabled)
	EXTI->IMR1 |= 1<<2;
	//4. Enable interrupt trigger for both rising (button release) and falling (button press) edges.
	EXTI->RTSR1 |= (1<<2);  //Rising trigger selection register (RTSR):0 = disabled, 1 = enabled
	//EXTI->FTSR1 |= (1<<3);  //Falling trigger selection register (FTSR): 0 = disabled, 1 = enabled
}

void EXTI3_IRQHandler(void) {  //a
	// PR (Pending Register): Check if the interrupt is triggered by EXTI3
	if ((EXTI->PR1 & EXTI_PR1_PIF3) == EXTI_PR1_PIF3) {
		// cleared by writing a 1 to this bit
		EXTI->PR1 |= (1<<3);
		if (sw3()) //check if push button is pressed
		{	
			//turn_on_LED();
		}
	}
}

//volatile int rot_old_state = 0;
//volatile int rot_new_state = 0;

int rot_get_state(void) {
	volatile int a = (GPIOC->IDR & 1UL<<3);
	volatile int b = (GPIOC->IDR & 1UL<<2) << 1;
	
    return a & b;
}

void cw(){
	wave++;
	if (wave > 3){
		wave = 0;
	}
}

void ccw(){
	wave--;
//	if (wave == 0xFFFFFFFF){
    if (wave > 4){
		wave = 3;
	}
}

void EXTI2_IRQHandler(void) {  
	// PR (Pending Register): Check if the interrupt is triggered by EXTI3
	if ((EXTI->PR1 & EXTI_PR1_PIF2) == EXTI_PR1_PIF2) {
		// cleared by writing a 1 to this bit
		//EXTI->PR1 |= (1<<2);
        //EXTI->PR1 |= (1<<3);
        EXTI->PR1 |= EXTI_PR1_PIF2;
		
//		rot_new_state = rot_get_state();
        uint8_t rotationDir = rot_get_state(); // Get the rotation direction of the rotary encoder knob.

        // Check transition
        /* 8 and 8 for the second half of the `||` statement as the state of new and old 
         * gets stuck to those values when continously turning in one direction. First 
         * half is for turning in that direction once. Same with 0 and 0 for the else if.
         */
//        if ((rot_old_state == 0 && rot_new_state == 8) || (rot_old_state == 8 && rot_new_state == 8)) {        // 3 -> 2 transition
//            ccw	();
//        } else if ((rot_old_state == 8 && rot_new_state == 0) || (rot_old_state == 0 && rot_new_state == 0)) { // 2 -> 0 transition
//            cw();
//        }
        
        if (rotationDir == 8) { // Counter Clockwise
            ccw();
        } else if (rotationDir == 0) { // Clockwise
            cw();
        }

//        rot_old_state = rot_new_state;
		
		
	}
}

//int getWave(){
//	return wave;
//}