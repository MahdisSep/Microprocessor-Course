#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define RS 0x01
#define RW 0x02
#define EN 0x04
#define carriage 0x80

void lcd_init(void);
void lcd_command(int command);
void lcd_show(char value);
void delay(int ms);
void sysclk_show(void);
void msg_show();
char msg[] = "abcdefghijklmnopqrsz";

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
  GPIOA->MODER = 0x00000000; 
  
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;    // enable portB 
  GPIOB->MODER = 0x00000000;              // set portB as input for push button
}



int main(){
  gpio_init();          // enable necessary ports  
  lcd_init();
  while(1){
		lcd_command(0x80);
		sysclk_show();
		//msg_show();
		if((GPIOA->IDR &(1<<0)) == 0){
			lcd_show('b');
		}
		
	}
  
    return 0;
}
void msg_show(){
	int len = strlen(msg);
	int base = 1;
	for(int i = 0 ; i < len; i++){
		if(i < 16){
			lcd_show(msg[i]);
		}
		else {
			lcd_command(0x80);//berim aval khat aval
			for(int j = base ; j < base + 16 ; j++){
				lcd_show(msg[j]);
			}
			base++;
		}
	}
	
}

void sysclk_show(){
	SystemCoreClockUpdate();
	unsigned int copy = SystemCoreClock;
	//unsigned int copy = 124;
	unsigned int rev = 0;
	int count = 0 ;
	while(copy != 0){
		int remain = copy % 10;
		rev = rev * 10 + remain;
		copy  /= 10 ;
		count++;
	}
	for(int i = 0 ; i < count ; i++){
		int digit = rev % 10;
		lcd_show(digit + 48);
		rev /= 10 ;
	}
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
  delay(50);
  GPIOC->ODR &= ~EN;
  delay(50);
}

void delay(int ms){
  volatile int i = 0;
  for(i=0; i < ms*1000; i++);
}