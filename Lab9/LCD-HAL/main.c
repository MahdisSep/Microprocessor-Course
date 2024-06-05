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
		TIM2->CCR1 = (TIM2->ARR * duty_cycle) / 100;
} 

void left() {  // decrease   
		duty_cycle -= 10;
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
	
	//TIM2->DIER |= (1<<0);
	//NVIC_EnableIRQ(TIM2_IRQn);
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
	
	
	
	while (1) {
        sprintf(str, "%ldms", time);
        LCD_Puts(0, 0, str);

        if (mode == 0) {  // Blinking mode
            sprintf(str, "Period:%dms", blink_period);
            LCD_Puts(0, 1, str);

            if ((GPIOC->IDR & (1 << 13)) == 0) {  // UP button pressed
                up();
            }
            if ((GPIOC->IDR & (1 << 12)) == 0) {  // DOWN button pressed
                down();
            }
						
        } 
				
				else {  // Brightness adjustment mode
            sprintf(str, "Duty cycle:%d", duty_cycle);
            LCD_Puts(0, 1, str);

            if ((GPIOC->IDR & (1 << 13)) == 0) {  // RIGHT button pressed
                right();
            }
            if ((GPIOC->IDR & (1 << 12)) == 0) {  // LEFT button pressed
                left();
            }
        }
				
				
				if ((GPIOC->IDR & (1 << 11)) == 0) {  // SELECT button pressed
						LCD_Puts(0, 1, "                ");
            mode = ~mode;
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
