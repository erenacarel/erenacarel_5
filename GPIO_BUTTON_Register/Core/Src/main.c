#include "main.h"
void delay(volatile uint32_t);
enum mode{mode0, mode1, mode2, mode3, mode4, mode5};
enum mode state;
int main(void) {
	 /* Enable GPIOB clock */
	RCC->IOPENR |= (1U << 1);
	 /* Setup PB0 as output for led*/
	GPIOB->MODER &= ~(3U);
	GPIOB->MODER |= (1U);

	 /* Setup PB3 as input for button*/
	GPIOB->MODER &= ~(3U << 2*3);

	while(1) {
	 switch(state){
	 case 0:
		 //Led is of no toggling
		 // IDR: Input data register
		 // ODR: Output data register
		 GPIOB->ODR |= ~(1U);
		 delay(1000000);
		 if((GPIOB->IDR >> 3) & 1){
			 state ++ ; }
		 break;

	 case 1:
		 GPIOB->ODR ^= (1U);
		 delay(3200000);
		 if((GPIOB->IDR >> 3) & 1){
			 state ++ ; }
		 break;

	 case 2:
		 GPIOB->ODR ^= (1U);
		 delay(1600000);
		 if((GPIOB->IDR >> 3) & 1){
			 state ++ ; }
	 break;

	 case 3:
		 GPIOB->ODR ^= (1U);
		 delay(800000);
		 if((GPIOB->IDR >> 3) & 1){
			 state ++ ; }
		 break;

	 case 4:
		 GPIOB->ODR ^= (1U);
		 delay(160000);
		 if((GPIOB->IDR >> 3) & 1){
			 state ++ ; }
		 break;

	 case 5:
	 //Led is on no toggling
		 GPIOB->ODR |= (1U);
		 delay(1000000);
		 if((GPIOB->IDR >> 3) & 1){
			 state = 0 ; }
		 break;
	 }

}

	return 0;
}

void delay(volatile uint32_t s){
	for(; s>0; s--);
}


Buradaki kod çalışmamızda butonlardan değer okumayı denedik.
İlk olarak GPIOB hattının clock hattını aktif ettik. Bunu için
RCC sınıfından giriş/çıkış pinlerini aktive eden IOPENR elemanını
çağırdık. Ardından onun üzerine " |= " fonksiyonu ile ilave yaptık.
Ardından bir çıkış pini ayarladık, bu led veya başka bir şey de
olabilir. Bunu da "MODER" ile yaptık. Yine buton girişi için PB3
ayarlandı. "Swith-Case" döngüsüne girerek ve IDR ile data girişlerini
okurken, ODR ile data çıkışlarını okuyoruz. delay eklemesi yaparak
butonlarda gereksiz zıplamaların önüne geçmeyi hedefledik.










