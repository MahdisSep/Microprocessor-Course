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
int main(void)
{

	GPIO_init();
	LCD_Init();
	sysclk_show();
	msg_show();

	

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
