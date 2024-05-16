#include "stm32f4xx.h"
#include <stdio.h>

#define RS 0x01
#define RW 0x02
#define EN 0x04
#define carriage 0x80

void lcd_init(void);
void lcd_command(int command);
void lcd_show(char value);
int isPrime(int num);
int isPalindrome(int num);
void delay(int ms);

void gpio_init(){
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;   // enable portC
  GPIOC->MODER |= (1UL << 2*0);        // set portC as output for LCD
	GPIOC->MODER |= (1UL << 2*1);
	GPIOC->MODER |= (1UL << 2*2);
	GPIOC->MODER |= (1UL << 2*3);
	GPIOC->MODER |= (1UL << 2*4);
	GPIOC->MODER |= (1UL << 2*5);
	GPIOC->MODER |= (1UL << 2*6);
	GPIOC->MODER |= (1UL << 2*7);
	GPIOC->MODER |= (1UL << 2*8);
	GPIOC->MODER |= (1UL << 2*9);
	GPIOC->MODER |= (1UL << 2*10);
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    // enable portA
  GPIOA->MODER |= (1UL << 2*0);         // set portA pin 0-2 as output for keypad
	GPIOA->MODER |= (1UL << 2*1);
	GPIOA->MODER |= (1UL << 2*2);
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;    // enable portB 
	GPIOB->MODER = 0x00000000;	            // set portB as input for push button
}

int main(){
	gpio_init();          // enable necessary ports  
	lcd_init();
	lcd_command(0x80);    // ? 
	
	int num = 0 ;         // variable to keep the number

	while(1){
		// CHECK COL1
		GPIOA->ODR = 0;           //baghiee col 0 mikonim
		GPIOA->ODR |=(1<<0);      //A0 
		// CHECK NUMBERS IN COL1
		if((GPIOB->IDR &(1<<0))== 1){
			lcd_show('1');
			num = num * 10 + 1;
		}
		if((GPIOB->IDR &(1<<1))== 2){
			lcd_show('4');
			num = num * 10 + 4;
		}
		if((GPIOB->IDR &(1<<2))== 4){
			lcd_show('7');
			num = num * 10 + 7;
		}
		
		// CHECK COL2
		GPIOA->ODR = 0;            //baghiee col 0 mikonim
		GPIOA->ODR |=(1<<1);       //A1 
		// CHECK NUMBERS IN COL2
		if((GPIOB->IDR &(1<<0))== 1){
			lcd_show('2');
			num = num * 10 + 2;
		}
		if((GPIOB->IDR &(1<<1))== 2){
			lcd_show('5');
			num = num * 10 + 5;
		}
		if((GPIOB->IDR &(1<<2))== 4){
			lcd_show('8');
			num = num * 10 + 8;
		}
		if((GPIOB->IDR &(1<<5))== 32){
			lcd_show('0');
			num = num * 10 + 0;
		}
		
		// CHECK COL3
		GPIOA->ODR = 0;            //baghiee col 0 mikonim
		GPIOA->ODR |=(1<<2);       //A2
		// CHECK NUMBERS IN COL3
		if((GPIOB->IDR &(1<<0))== 1){
			lcd_show('3');
			num = num * 10 + 3;
		}
		if((GPIOB->IDR &(1<<1))== 2){
			lcd_show('6');
			num = num * 10 + 6;
		}
		if((GPIOB->IDR &(1<<2))== 4){
			lcd_show('9');
			num = num * 10 + 9;
		}
		
		// check push button 
		if((GPIOB->IDR &(1<<6))==0){
			break;
		}
	}
		lcd_show(' ');    
		//check pal and pr
		if(isPalindrome(num)){
			lcd_show('p');       // palindrome
			lcd_show('l');
			lcd_show(',');
			if(isPrime(num)){ 
				lcd_show('p');     // prime
				lcd_show('r');
			}
			else{
				lcd_show('N');     // not prime
				lcd_show('p');
				lcd_show('r');
			}
		}	
		else {
			lcd_show('N');       //not palindrome
			lcd_show('p');
			lcd_show('l');
			lcd_show(',');
			if(isPrime(num)){
				lcd_show('p');     // prime
				lcd_show('r');
			}
			else{
				lcd_show('N');     // not prime
				lcd_show('p');
				lcd_show('r');
			}
		}
    return 0;
}

//LCD
void lcd_init(){
	lcd_command(0x30); // 8 BIT MODE WITH 1 ROW   0110000
	lcd_command(0x06); // EREASE RAM AND DISPLAY  0000110
	lcd_command(0x01); //00000001
	lcd_command(carriage); // CARRIAGE RETURN
	lcd_command(0x0C); // TURN ON THE DISPLAY WITHOUT CAURSER  00001100
}

void lcd_command(int command){
	GPIOC->ODR &= ~(RS|RW);
	GPIOC->ODR &= (0X007);
	GPIOC->ODR |= (command << 3);
	GPIOC->ODR |= EN;
	delay(10);
	GPIOC->ODR &= ~EN;
	delay((command < 4)?20:10);
}

void lcd_show(char value){
	GPIOC->ODR |= RS;
	GPIOC->ODR &= ~(RW);
	GPIOC->ODR &= (0x007);
	GPIOC->ODR |= value << 3;
	GPIOC->ODR |= EN;	
	delay(200);
	GPIOC->ODR &= ~EN;
	delay(200);
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

