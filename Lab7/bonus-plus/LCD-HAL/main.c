#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "LCD16x2Lib/LCD.h"

void delay(int ms);
int isPalindrome(int num);
int isPrime(int num);

void GPIO_init(){
	SystemCoreClockUpdate();
	HAL_Init();	
	RCC->AHB1ENR = ((1<<0) | (1<<1) | (1<<2));
	
  GPIOA->MODER |= (1UL << 2*4); // output KEYPAD
	GPIOA->MODER |= (1UL << 2*5);
	GPIOA->MODER |= (1UL << 2*6);
	
	
	GPIOC->MODER  &= ~(0X3 << 26); // pc13: INPUT
	
	GPIOC->MODER  &= ~(0X3 << 0);
	GPIOC->MODER  &= ~(0X3 << 2);
	GPIOC->MODER  &= ~(0X3 << 4);
	GPIOC->MODER  &= ~(0X3 << 6);
}

int main(void)
{
	GPIO_init();
	LCD_Init();
	
	int num = 0 ;         // variable to keep the number
	int i = 0;
	while(1){
		
		// CHECK COL1
		GPIOA->ODR &= ~(0X3 << 4);           //baghiee col 0 mikonim
		GPIOA->ODR &= ~(0X3 << 5);
		GPIOA->ODR &= ~(0X3 << 6);
		GPIOA->ODR |=(1<<4);      //A0 
		// CHECK NUMBERS IN COL1
		if((GPIOC->IDR &(1<<0))== 1){
			LCD_Puts(i, 0, "1");
			delay(300);
			num = num * 10 + 1;
			i++;
		}
		if((GPIOC->IDR &(1<<1))== 2){
			LCD_Puts(i, 0, "4");
			delay(300);
			num = num * 10 + 4;
			i++;
		}
		if((GPIOC->IDR &(1<<2))== 4){
			LCD_Puts(i, 0, "7");
			delay(300);
			num = num * 10 + 7;
			i++;
		}
		
		// CHECK COL2
		GPIOA->ODR &= ~(0X3 << 4);           
		GPIOA->ODR &= ~(0X3 << 5);
		GPIOA->ODR &= ~(0X3 << 6);            //baghiee col 0 mikonim
		GPIOA->ODR |=(1<<5);                    //A1 
		// CHECK NUMBERS IN COL2
		if((GPIOC->IDR &(1<<0))== 1){
			LCD_Puts(i, 0, "2");
			delay(300);
			num = num * 10 + 2;
			i++;
		}
		if((GPIOC->IDR &(1<<1))== 2){
			LCD_Puts(i, 0, "5");
			delay(300);
			num = num * 10 + 5;
			i++;
		}
		if((GPIOC->IDR &(1<<2))== 4){
			LCD_Puts(i, 0, "8");
			delay(300);
			num = num * 10 + 8;
			i++;
		}
		if((GPIOC->IDR &(1<<3))== 8){
			LCD_Puts(i, 0, "0");
			delay(300);
			num = num * 10 + 0;
			i++;
		}
		
		// CHECK COL3
		GPIOA->ODR &= ~(0X3 << 4);           //baghiee col 0 mikonim
		GPIOA->ODR &= ~(0X3 << 5);
		GPIOA->ODR &= ~(0X3 << 6);
		GPIOA->ODR |=(1<<6);       //A2
		// CHECK NUMBERS IN COL3
		if((GPIOC->IDR &(1<<0))== 1){
			LCD_Puts(i, 0, "3");
			delay(300);
			num = num * 10 + 3;
			i++;
		}
		if((GPIOC->IDR &(1<<1))== 2){
			LCD_Puts(i, 0, "6");
			delay(300);
			num = num * 10 + 6;
			i++;
		}
		if((GPIOC->IDR &(1<<2))== 4){
			LCD_Puts(i, 0, "9");
			delay(300);
			num = num * 10 + 9;
			i++;
		}
		
		// check push button 
		if((GPIOC->IDR &(1<<13))==0){
			break;
		}
	}
	
			if(isPalindrome(num)){
				if(isPrime(num)){ 
					LCD_Puts(0, 1, "pl,pr");
				}
				else{
					LCD_Puts(0, 1, "pl,Npr");
				}
		}	
		else {
			if(isPrime(num)){
					LCD_Puts(0, 1, "Npl,pr");
			}
			else{
					LCD_Puts(0, 1, "Npl,Npr");
			}
		}
	
	return 0;
}

int isPrime(int num) {   // check if num is prime
    int i;
    if (num < 2)
        return 0;
    for (i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

int isPalindrome(int num) {  // check if num is palindrome by making the revesed
    int reversedNum = 0;
    int originalNum = num;
    while (num > 0) {
        reversedNum = reversedNum * 10 + (num % 10);
        num /= 10;
    }
    if (originalNum == reversedNum)
        return 1;
    else
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
