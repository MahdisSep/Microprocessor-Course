#include <stdio.h>
#include "stm32f4xx.h"

int isPrime(int num);
int isPalindrome(int num);
void delay1();
void delay2();
void setout();
void showNum(int num);


int main() {
    //====================
    RCC->AHB1ENR = ((1U<<0) | (1U<<2) | (1U<<1));    // GPIO A,C enable
    //GPIOC->MODER &= (~(0x3<<20));                    // PC12-13: input  
		//GPIOC->MODER &= (~(0x3<<26));
	
		GPIOC->MODER = 0X00000000;
	  GPIOC->PUPDR = 0X00000000;
	
	  setout();
	//====================
    int num = 2, last = 0;
    int arr[1000];
    while(1){
        if(1){                 
            while(!isPrime(num) || !isPalindrome(num)){
                num++;
						}
            arr[last] = num;
            last++;
            showNum(num);
            num++;
        }
        else if((GPIOC->IDR & (1<<10)) == 0){              
            int index = last - 2;
            num = arr[index] + 1;
            last--;
            showNum(arr[index]);
        }
    }
    return 0;
}
void setout(){
		GPIOA->MODER |= (1<<0);
		GPIOA->MODER |= (1<<2);
		GPIOA->MODER |= (1<<4);
		GPIOA->MODER |= (1<<6);
		GPIOA->MODER |= (1<<8);
		GPIOA->MODER |= (1<<10);
		GPIOA->MODER |= (1<<12);	
}
int isPrime(int num) {
    int i;
    if (num < 2)
        return 0;

    for (i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}
int isPalindrome(int num) {
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
void delay1(){
  for (unsigned int d = 10000;d!=0;d--)
  {}
}
void delay2(){
  for (unsigned int d = 40000;d!=0;d--)
  {}
}
void showNum(int num){
    
    
    //GPIOA->MODER |= ((1<<0) | (1<<2) | (1<<4)| (1<<6) | (1<<8) | (1<<10) | (1<<12));      // PA0-6: output   
    //GPIOA->MODER &= (~(1<<1) & ~(1<<3) & ~(1<<5) & ~(1<<7) & ~(1<<9) & ~(1<<11) & ~(1<<13)); 

    int count = 0,copy = num;
    while (copy != 0) {
        copy /= 10;
        count++;
    }
    for(int i=0;i<count;i++){
        int digit = num%10;
        num/=10;
        
        switch (digit) {
            case 0:
                GPIOA->ODR |=(7<<5);
                delay1();
						    GPIOA->ODR &= 0xFF00;
                break;
            case 1:
                GPIOA->ODR |=(575<<5);
                delay1();
								GPIOA->ODR &= 0xFF00;
                break;
            case 2:
                GPIOA->ODR |=(75<<5);
                delay1();
								GPIOA->ODR &= 0xFF00;
                break;
            case 3:
                GPIOA->ODR |= (27<<5);
                delay1();
								GPIOA->ODR &= 0xFF00;
                break;
            case 4:
                GPIOA->ODR |=(563<<5);
                delay1();
								GPIOA->ODR &= 0xFF00;
                break;
            case 5:
                GPIOA->ODR |=(147<<5);
                delay1();
								GPIOA->ODR &= 0xFF00;
                break;
            case 6:
                GPIOA->ODR |=(131<<5);
                delay1();
								GPIOA->ODR &= 0xFF00;
                break;
            case 7:
                GPIOA->ODR |=(63<<5);
                delay1();
								GPIOA->ODR &= 0xFF00;
                break;
            case 8:
                GPIOA->ODR |=(3<<5);
                delay1();
								GPIOA->ODR &= 0xFF00;
                break;
            case 9:
                GPIOA->ODR |=(19<<5);
                delay1();
								GPIOA->ODR &= 0xFF00;
                break;
        }    
    }
    GPIOA->ODR = 0x00;
    delay2();
}