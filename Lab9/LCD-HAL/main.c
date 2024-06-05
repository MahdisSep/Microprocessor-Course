#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "LCD16x2Lib/LCD.h"
#include <stdio.h>
#include <string.h>

#define MAX_COUNT 1000  //1000
#define STEP 100

char str[20];
int duty_cycle = 50;
volatile long int time = 0;
int mode = 1;
int blink_period = 100;

void delay(int ms);

void GPIO_init(){
		RCC->AHB1ENR |= ((1<<0) | (1<<1) | (1<<2));  // port A,B,C enable
		RCC->APB1ENR |= ((1<<0) | (1<<3)); //TIM2,5
	  GPIOA ->MODER |= (2 << 10);  // PA5: AF
		GPIOA -> AFR[0] |= (1<<20);
	
    GPIOC->MODER &= ~(0x3 << (26));       // decrease the frequency push button
    GPIOC->MODER &= ~(0x3 << (24));       // increase the frequency push button
		GPIOC->MODER &= ~(0x3 << 22); 
	
    HAL_Init();
}

 void TIM5_IRQHandler(void) {
		TIM5->SR = 0;  
		time++;
}

void right() { // increase
		duty_cycle += 10;
		if (duty_cycle >100) duty_cycle = 100;
		TIM2->CCR1 = (TIM2->ARR * duty_cycle) / 100;
} 

void left() {  // decrease   
		duty_cycle -= 10;
		if (duty_cycle < 0) duty_cycle = 0;
		TIM2->CCR1 = (TIM2->ARR * duty_cycle) / 100;
} 

void up() {
    blink_period += STEP;
		TIM2->CCR1 = (TIM2->ARR + STEP) * duty_cycle /100;
		TIM2->ARR = TIM2->ARR + STEP;                            // Adjust timer period for blinking
}
void down() {
    blink_period -= STEP;
		TIM2->CCR1 = (TIM2->ARR - STEP) * duty_cycle /100;
		TIM2->ARR = TIM2->ARR - STEP;                           // Adjust timer period for blinking
}


int main(void)
{
	GPIO_init();
	LCD_Init();
	
	TIM2->PSC = 1600 - 1;
	TIM2->ARR = MAX_COUNT - 1;
	TIM2->CCMR1 &= ~(3<<0);
	TIM2->CCMR1 &= ~(7<<4);
	TIM2->CCMR1 |= (6<<4);
	TIM2->CCR1 = 500;
	TIM2->CCER |= (1<<0);
	TIM2->CR1 = 1;

//===========================
	
	TIM5->PSC = 16 - 1;
	TIM5->ARR = MAX_COUNT - 1;
	
	TIM5->CCMR1 &= ~(3<<0);
	TIM5->CCMR1 &= ~(7<<4);
	TIM5->CCMR1 |= (6<<4);
	
	TIM5->CCR1 = 500;
	TIM5->CCER |= (1<<0);
	TIM5->CR1 = 1;
	
	TIM5->DIER |= (1<<0);
	NVIC_EnableIRQ(TIM5_IRQn);
	__enable_irq();	
	

	unsigned int order;

	 /* set up pin PA1 for analog input */
		 RCC->AHB1ENR |= 1;         /* enable GPIOA clock */
		 GPIOA->MODER |= (3 << 0); /* PA0 analog */
		 
	 /* setup ADC1 */
		 RCC->APB2ENR |= (1 << 8); /* enable ADC1 clock */
		 ADC1->SQR3 = 0; /* conversion sequence starts at ch 0 */
		 ADC1->SQR1 = 0; /* conversion sequence length 1 */
		 ADC1->CR2 |= (1<<0); /* enable ADC1 */
		 
		 while (1){
		
			   sprintf(str, "%ldms", time);
         LCD_Puts(0, 0, str);
			 
         //LCD_Puts(0, 1, "                ");
				 sprintf(str, "T:%dms", blink_period);
         LCD_Puts(0, 1, str);	
			   sprintf(str, "D:%d", duty_cycle);
         LCD_Puts(10, 1, str);
			 
			   ADC1->CR2 |= ADC_CR2_SWSTART;     /* start a conversion */
			   while((ADC1->SR & (1<<1))==0) {}  /* wait for conv complete */
			   order = ADC1->DR & 0x0FFF;        /* read conversion result */
				 
				 if(0<=order && order >300){
					 LCD_Puts(0, 1, "                ");
					 right();
				 }
				 else if(300<=order && order<900){
					 LCD_Puts(0, 1, "                ");
					 up();
				 }
				 else if(900<=order && order<1600){
					 LCD_Puts(0, 1, "                ");
					 down();
				 }
				 else if(1600<=order && order<2500){
					 LCD_Puts(0, 1, "                ");
					 left();
				 }
				 else if(2500<=order && order<3500){
					 //select : NOP
				 }
		 }
	

	return 0;
}
void delay(int ms){
	volatile int i = 0;
	for(i=0; i < ms*1000; i++);
}
void SysTick_Handler(void)
{
  HAL_IncTick();
}
