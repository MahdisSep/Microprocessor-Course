#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "LCD16x2Lib/LCD.h"
#include <stdio.h>
#include <string.h>

#define MAX_COUNT 1000
char str[20];
int duty_cycle = 50;
volatile long int time = 0;
void delay(int ms);

void GPIO_init(){
		RCC->AHB1ENR |= ((1<<0) | (1<<1) | (1<<2));  // port A,B,C enable
		RCC->APB1ENR |= (1<<0);
	  GPIOA ->MODER |= (2 << 10);  // PA5: AF
		GPIOA -> AFR[0] |= (1<<20);
	
    GPIOC->MODER &= ~(0x3 << (26));       // decrease the frequency push button
    GPIOC->MODER &= ~(0x3 << (24));       // increase the frequency push button

    HAL_Init();
}

 void TIM2_IRQHandler(void) {
		TIM2->SR    = 0;  
		time++;
}

void right() { // increase
		duty_cycle += 10;
		TIM2->CCR1 = (MAX_COUNT*duty_cycle) / 100;
} 

void left() {  // decrease   
		duty_cycle -= 10;
		TIM2->CCR1 = (MAX_COUNT*duty_cycle) / 100;
} 


int main(void)
{
	GPIO_init();
	LCD_Init();
	
	sprintf(str, "%d", duty_cycle);
	LCD_Puts(0,1,str);

	TIM2->PSC = 16 - 1;
	TIM2->ARR = MAX_COUNT - 1;
	TIM2->CCMR1 &= ~(3<<0);
	TIM2->CCMR1 &= ~(7<<4);
	TIM2->CCMR1 |= (6<<4);
	TIM2->CCR1 = 500;
	TIM2->CCER |= (1<<0);
	TIM2->CR1 = 1;
	
	TIM2->DIER |= (1<<0);
	NVIC_EnableIRQ(TIM2_IRQn);
	__enable_irq(); 
	
	while(1){
			sprintf(str, "%d", time);
			LCD_Puts(0,0,str);
		
		if((GPIOC->IDR & (1<<13)) == 0){
			right();
			sprintf(str, "%d", duty_cycle);
			LCD_Puts(0,1,"    ");
			LCD_Puts(0,1,str);
			delay(100);
			
		}
		if((GPIOC->IDR & (1<<12)) == 0){
			left();
			sprintf(str, "%d", duty_cycle);
			LCD_Puts(0,1,"    ");
			LCD_Puts(0,1,str);
			delay(100);
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
