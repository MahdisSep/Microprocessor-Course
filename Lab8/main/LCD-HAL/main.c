#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "LCD16x2Lib/LCD.h"
#include <stdio.h>
#include <string.h>
void SysTick_Handler(void);
void msg_show();
void sysclk_show();
void delay(int ms);

char msg[] = "you see how im wasting my time?";

void GPIO_init(){
		SystemCoreClockUpdate();
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
		RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 
    
    GPIOC->MODER &= ~(0x3 << (26));       // decrease the frequency push button
    GPIOC->MODER &= ~(0x3 << (16));       // increase the frequency push button

    SYSCFG->EXTICR[3] |= (0x2 << 4 );     // interrupt for increase
    EXTI->FTSR |= (1 << 13);                   
    EXTI->IMR |= (1 << 13);                    
    NVIC_EnableIRQ(EXTI15_10_IRQn);            
    
    SYSCFG->EXTICR[2] |= (0x2 << 0);      // interrupt for decrease
    EXTI->FTSR |= (1 << 8);                      
    EXTI->IMR |= (1 << 8);                       
    NVIC_EnableIRQ(EXTI9_5_IRQn);              

    HAL_Init();
}

void EXTI15_10_IRQHandler() {   // set pll N, M, P to increace the frequency in 6 different stages
    SystemCoreClockUpdate();
		FLASH->ACR = 2;
		switch (SystemCoreClock) {
    case 16000000:
        RCC->PLLCFGR = (16<<RCC_PLLCFGR_PLLM_Pos) | (208<<RCC_PLLCFGR_PLLN_Pos) | (3<<RCC_PLLCFGR_PLLP_Pos);
        break;
    case 26000000:
        RCC->PLLCFGR = (16<<RCC_PLLCFGR_PLLM_Pos) | (288<<RCC_PLLCFGR_PLLN_Pos) | (3<<RCC_PLLCFGR_PLLP_Pos);
        break;
		case 36000000:
        RCC->PLLCFGR = (16<<RCC_PLLCFGR_PLLM_Pos) | (368<<RCC_PLLCFGR_PLLN_Pos) | (3<<RCC_PLLCFGR_PLLP_Pos);
        break;
		case 46000000:
        RCC->PLLCFGR = (16<<RCC_PLLCFGR_PLLM_Pos) | (224<<RCC_PLLCFGR_PLLN_Pos) | (1<<RCC_PLLCFGR_PLLP_Pos);
        break;
		case 56000000:
        RCC->PLLCFGR = (16<<RCC_PLLCFGR_PLLM_Pos) | (264<<RCC_PLLCFGR_PLLN_Pos) | (1<<RCC_PLLCFGR_PLLP_Pos);
        break;
		case 66000000:
        RCC->PLLCFGR = (8<<RCC_PLLCFGR_PLLM_Pos) | (228<<RCC_PLLCFGR_PLLN_Pos) | (2<<RCC_PLLCFGR_PLLP_Pos);
        break;
}

		RCC->CR |= RCC_CR_PLLON; // (1<<24)
		while((RCC->CR & RCC_CR_PLLRDY) == 0); // wait for PLL READY
		RCC->CFGR |= (RCC_CFGR_SW_PLL | (4<<RCC_CFGR_PPRE1_Pos)); 
		while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // wait for setting the clock sourse
	
    EXTI->PR |= (1 << 13); 
    NVIC_ClearPendingIRQ(EXTI15_10_IRQn); 
} 

void EXTI9_5_IRQHandler() {     // set pll N, M, P to decreace the frequency in 6 different stages
	  SystemCoreClockUpdate();
		FLASH->ACR = 2;
		switch (SystemCoreClock) {
    case 26000000:
        RCC->PLLCFGR = (16<<RCC_PLLCFGR_PLLM_Pos) | (208<<RCC_PLLCFGR_PLLN_Pos) | (3<<RCC_PLLCFGR_PLLP_Pos);
        break;
    case 36000000:
        RCC->PLLCFGR = (16<<RCC_PLLCFGR_PLLM_Pos) | (208<<RCC_PLLCFGR_PLLN_Pos) | (3<<RCC_PLLCFGR_PLLP_Pos);
        break;
		case 46000000:
        RCC->PLLCFGR = (16<<RCC_PLLCFGR_PLLM_Pos) | (288<<RCC_PLLCFGR_PLLN_Pos) | (3<<RCC_PLLCFGR_PLLP_Pos);
        break;
		case 56000000:
        RCC->PLLCFGR = (16<<RCC_PLLCFGR_PLLM_Pos) | (368<<RCC_PLLCFGR_PLLN_Pos) | (3<<RCC_PLLCFGR_PLLP_Pos);
        break;
		case 66000000:
        RCC->PLLCFGR = (16<<RCC_PLLCFGR_PLLM_Pos) | (224<<RCC_PLLCFGR_PLLN_Pos) | (1<<RCC_PLLCFGR_PLLP_Pos);
				break;
		case 76000000:
        RCC->PLLCFGR = (16<<RCC_PLLCFGR_PLLM_Pos) | (264<<RCC_PLLCFGR_PLLN_Pos) | (1<<RCC_PLLCFGR_PLLP_Pos);
        break;
		
}

		RCC->CR |= RCC_CR_PLLON; // (1<<24)
		while((RCC->CR & RCC_CR_PLLRDY) == 0); // wait for PLL READY
		RCC->CFGR |= (RCC_CFGR_SW_PLL | (4<<RCC_CFGR_PPRE1_Pos)); 
		while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // wait for setting the clock sourse
	
    EXTI->PR |= (1 << 8); 
    NVIC_ClearPendingIRQ(EXTI9_5_IRQn); 
} 


int main(void)
{
	GPIO_init();
	LCD_Init();
	while(1){
	sysclk_show(); // function to show sysclk in the first line
	msg_show();    // function to show scrolling in the second line
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
			delay(300);
	}
	else{
			for(int i=0; i<= strlen(msg)-16; i++){   // operations to shift the msg to left
					strncpy(str_to_show, msg+base, 16);
					str_to_show[16] = '\0';
					LCD_Puts(0,1, str_to_show);
					delay(300);
					base++;
			}	
		}
}


void sysclk_show(){  // function to show sysclk in the first line
	SystemCoreClockUpdate();
	unsigned int copy = SystemCoreClock;
	char str[20];
	sprintf(str, "%d", copy); 
	LCD_Puts(0,0, str);
}

void delay(int ms){
	volatile int i = 0;
	for(i=0; i < ms*1000; i++);
}

void SysTick_Handler(void){
  HAL_IncTick();
}
