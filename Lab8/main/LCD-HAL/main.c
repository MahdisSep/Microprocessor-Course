#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "LCD16x2Lib/LCD.h"
#include <stdio.h>
#include <string.h>
void SysTick_Handler(void);
void msg_show();
void sysclk_show();
void delay(int ms);

char msg[] = "abcdefghijklmnopqrstuwxyz";

void GPIO_init(){
	SystemCoreClockUpdate();
	HAL_Init();
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
}

void EXTI15_10_IRQHandler() { 
    SystemCoreClockUpdate();
		//SystemCoreClock += 100;
	
    EXTI->PR |= (1 << 13); 
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn); 
} 

void EXTI9_5_IRQHandler() { 
    SystemCoreClockUpdate();
		//SystemCoreClock -= 100;
	
    EXTI->PR |= (1 << 8); 
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn); 
} 


int main(void)
{
	//==============================
	
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN; 
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 
 
	  //GPIOA->MODER |= 1 << 10;            // PA5: OUTPUT
  	GPIOC->MODER &= 3 << 26;            // PC13: INPUT
		GPIOC->MODER &= 3 << 16;            // PC8: INPUT
	  
	  SYSCFG->EXTICR[3] |= (2 << 4);      // EXTI13: PC13
    EXTI->FTSR |= (1 << 13);            // falling 
    EXTI->IMR |= (1 << 13);             // clear mask
    NVIC_EnableIRQ(EXTI15_10_IRQn); 
		
		SYSCFG->EXTICR[2] |= (2 << 0);      // EXTI13: PC13
    EXTI->FTSR |= (1 << 8);            // falling 
    EXTI->IMR |= (1 << 8);             // clear mask
    NVIC_EnableIRQ(EXTI9_5_IRQn); 
	

	//==============================
	GPIO_init();
	LCD_Init();
	while(1){
	sysclk_show();
	msg_show();
	}

	return 0;
}
void msg_show(){
	int min;
	if (strlen(msg) < 16) {
			min = strlen(msg);
	} else {
			min = 16;
	}
	
	char str_to_show[17];
	int base = 0;
	if(strlen(msg) <= 16){
			LCD_Puts(0,1, msg);
			delay(500);
	}
	else{
			for(int i=0; i<= strlen(msg)-16; i++){
					strncpy(str_to_show, msg+base, 16);
					str_to_show[16] = '\0';
					LCD_Puts(0,1, str_to_show);
					delay(500);
					base++;
			}	
		}
}


void sysclk_show(){
	SystemCoreClockUpdate();
	unsigned int copy = SystemCoreClock;
	char str[20];
	sprintf(str, "%d", copy); // tmp -> copy
	LCD_Puts(0,0, str);
}


void delay(int ms){
	volatile int i = 0;
	for(i=0; i < ms*1000; i++);
}


void SysTick_Handler(void){
  HAL_IncTick();
}
