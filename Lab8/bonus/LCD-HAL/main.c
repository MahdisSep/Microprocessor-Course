#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "LCD16x2Lib/LCD.h"
#include <stdio.h>
#include <string.h>

void SysTick_Handler(void);
void print_msg();
void delay(int ms);
void GPIO_init();
void print_persian_msg(void);

char msg[] = "Hello World!! This is LAB 8...";

int main(void)
{
	
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN; 
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 

		GPIO_init();
		LCD_Init();
		print_persian_msg();
		
		
		
		return 0;
}

void print_msg(){
	
	int min;
	if (strlen(msg) < 16) {
			min = strlen(msg);
	} else {
			min = 16;
	}
	
	char str[17];
	int startPoint = 0;
	if(strlen(msg) <= 16){
			LCD_Puts(0,1, msg);
			delay(300);
	}
	else{
			for(int i=0; i<= strlen(msg)-16; i++){
					strncpy(str, msg+startPoint , 16);
					str[16] = '\0';
					LCD_Puts(0,1, str);
					delay(300);
					startPoint ++;
			}	
		}
}


void print_persian_msg(){
	
	//Define Custom Chars
	uint8_t Mim[8] = {
  0x00,
  0x00,
  0x07,
  0x05,
  0x1F,
  0x10,
  0x10,
  0x10
};
	
	uint8_t Te[8] = {
  0x00,
  0x14,
  0x01,
  0x01,
  0x1F,
  0x00,
  0x00,
  0x00
};
	
	uint8_t Noon_big[8] = {
  0x00,
  0x00,
  0x00,
  0x04,
  0x11,
  0x11,
  0x1F,
  0x00
};
	
	uint8_t Fe[8] = {
  0x02,
  0x00,
  0x07,
  0x05,
  0x1F,
  0x00,
  0x00,
  0x00
};
	
	uint8_t Alef[8] = {
  0x00,
  0x04,
  0x04,
  0x04,
  0x04,
  0x00,
  0x00,
  0x00
};
	
	uint8_t Re[8] = {
  0x00,
  0x00,
  0x00,
  0x00,
  0x04,
  0x04,
  0x0C,
  0x00
};
	
	uint8_t Sin[8] = {
  0x00,
  0x00,
  0x00,
  0x15,
  0x1F,
  0x00,
  0x00,
  0x00
};

	uint8_t Ye[8] = {
  0x00,
  0x00,
  0x00,
  0x00,
  0x13,
  0x12,
  0x0C,
  0x00
};
	
uint8_t Lam[8] = {
   0x00,
  0x01,
  0x01,
  0x01,
  0x1F,
  0x00,
  0x00,
  0x00
};

	
		//Print On LCD
	  LCD_CreateChar(0, Mim);
    LCD_CreateChar(1, Te);
		LCD_CreateChar(2, Noon_big);
		LCD_CreateChar(3, Fe);
		LCD_CreateChar(4, Alef);
		LCD_CreateChar(5, Re);
		LCD_CreateChar(6, Sin);
		LCD_CreateChar(7, Lam);

		//matn
    //LCD_PutCustom(15, 0, 0);
    //LCD_PutCustom(14, 0, 1);
    //LCD_PutCustom(13, 0, 2);
		
		//farsi
		LCD_PutCustom(15, 0, 6);
		LCD_PutCustom(14, 0, 7);
		LCD_PutCustom(13, 0, 4);
		LCD_PutCustom(12, 0, 0);
		//LCD_PutCustom(11, 0, 7);
		
		delay(200);
}

void GPIO_init(){
	SystemCoreClockUpdate();
	HAL_Init();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
}


void delay(int ms){
	volatile int i = 0;
	for(i=0; i < ms*1000; i++);
}


void SysTick_Handler(void){
  HAL_IncTick();
}
