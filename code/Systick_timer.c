#include "Systick_timer.h"
#include "DAC.h"
#include "ADC.h"
#include "sensor.h"
#include "encoder.h"
#include <math.h>

//-------------------------------------------------------------------------------------------
// Initialize SysTick	
//-------------------------------------------------------------------------------------------	
void SysTick_Init(uint32_t Reload){
	
		
	// 1. Disable SysTick Counter before all configurations are done
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		
	
	// 2. Program reload value by configuring SysTick Reload Value Register. 
	SysTick->LOAD = Reload - 1;    //Note: Time Interval = (Content of the register + 1) × Source_Clock_Period
	
	// 3. Clear reload value by configuring SysTick Current Value Register. 
	SysTick->VAL = 0;
								
	// 4. Enable SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// 5. Select processor clock
	// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
	// If CLKSOURCE = 1, the processor clock is used.
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;		
	
	// 6. Enable SysTick Timer once all configurations are done
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  
}

//-------------------------------------------------------------------------------------------
// SysTick Exception Handler
//-------------------------------------------------------------------------------------------
float phase = 0;
float pi =  3.14159265358979323846;
float amplitude = 1;
float phaseIncrement;
unsigned int actualY;
void SysTick_Handler(void){
	ADC1->CR |= ADC_CR_ADSTART;//start a conversion
    
    float volume = adc_result*0.0002442002442;// 1/4095 normalize to 0-1
//    volume = 1.0;
    
    extern float freq;
    float frequency = freq; // = get_frequency();
//    frequency = 15.0;
	if(frequency > 35.0){
		frequency = 35.0;
	} else if(frequency < 5.0){
		frequency = 5.0;
	}
	float normalF = ((frequency-5)*0.033) * 450.0 + 50.0; 
    phaseIncrement = normalF * 0.001;
	
    extern int wave; // = getWave();
    
    /* 0: Square
    *  1: Sawtooth
    *  2: Ramp
    *  3: Sin
    */
   
	if(wave == 0){ // Square
		if(phase > 0.5){
			actualY = 4095;
		} else {
			actualY = 0;
		}
        actualY = actualY * volume;
	} 
    else if(wave == 1) { // Sawtooh
//        float temp = -1.0f + (2.0f * phase);
//        float y = 2.0f * (fabsf(temp)- 0.5f);
//        
//        // Normalizes (Makes into an int without rounding to zero)
//        y = (y + 1);
//        y = y * 4095;
//        
//        actualY = (uint16_t) round(y*volume);
        
        float y = ((phase * 2) - 1) * -1;
        y = (y + 1) / 2;
        y = y * 4095;
		actualY = (uint16_t) round(y*volume);
        
    }
    else if(wave == 2){ // Ramp
        float y = (phase * 2) - 1;
        
//		float y = sinf(2 * pi * phase);
//		y = 2*y/(1+(float)fabs(y));
        y = (y + 1) / 2;
        y = y * 4095;
//		
////    volume = 1; // hardcode for testing
//	
		actualY = (uint16_t) round(y*volume);
	}
    else { // Sine
		
		float y = sinf(2 * pi * phase);
        y = (y + 1) / 2;
        y = y * 4095;
//		float volume = adc_result*0.0002442002442;// 1/4095 normalize to 0-1
//    volume = 1; // hardcode for testing
	
		actualY = (uint16_t) round(y*volume);
	}
    
    phase += phaseIncrement;
		if(phase > 1.0) {
			phase -= 1.0;
		}
    
    DAC_setValue(actualY);
}
