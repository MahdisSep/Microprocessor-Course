#include "stm32f4xx.h"
int isPalindrome(int num);
int isPrime(int num);
void showNumber(int num);
void delay(int q);

int main(void){
	RCC->AHB1ENR = ((1<<0) | (1<<1)); // port A,B enable
	GPIOB->MODER = 0X00000000;         // port B: input
	GPIOA->MODER |= ((1<<10) | (1<<12) | (1<<14)  | (1<<16)  | (1<<18) | (1<<20) | (1<<22) | (1<<24) | (1<<26) | (1<<28));   // port A: output

	int num=1;
	while(1){

		if((GPIOB->IDR & (1<<0)) == 0){
			num++;
			while(1){
        if(isPalindrome(num) && isPrime(num))
          break;
        num+=1;
			}
		showNumber(num);
	  delay(1);
		}
		else if((GPIOB->IDR & (1<<1)) == 0){
			num-=1;
			while(1){
				if(isPalindrome(num) && isPrime(num))
					break;
				num-=1;
			}
	  showNumber(num);
	  delay(1);
		}
  }
  return 0;
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

void showNumber(int num){
while (num != 0) {
    int remain=num % 10;
    num /= 10;
		int digit = remain;
		switch (digit) {
				case 0:
						GPIOA->ODR |= (7 << 5);     // 0
						break;
				case 1:
						GPIOA->ODR |= (575 << 5);   // 1
						break;
				case 2:
						GPIOA->ODR |= (75 << 5);    // 2
						break;
				case 3:
						GPIOA->ODR |= (27 << 5);    // 3
						break;
				case 4:
						GPIOA->ODR |= (563 << 5);   // 4
						break;
				case 5:
						GPIOA->ODR |= (147 << 5);   // 5
						break;
				case 6:
						GPIOA->ODR |= (131 << 5);   // 6
						break;
				case 7:
						GPIOA->ODR |= (63 << 5);    // 7
						break;
				case 8:
						GPIOA->ODR |= (3 << 5);     // 8
						break;
				default:
						GPIOA->ODR |= (19 << 5);    // 9
						break;
		}
		delay(1);
		GPIOA->ODR &= ~(0x3FF << 5);
		delay(1);
	}
return ;
}

void delay(int q){
   for(unsigned int d=q*500000; d!=0; d--)
    {}
}	
