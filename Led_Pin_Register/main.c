#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

//extern uint32_t SystemCoreClock; // system_stm32f4xx.c dosyasýndan alýyoruz. Ýlk deðeri elde etmiyoruz.

//uint32_t systemClock; // main dýþýnda global deðiþken olduðu için 0 atanýr.

void RCC_Config(void)
{
	RCC->CR &= ~(1<<0); // HSI osilatörünün 1. bitini 0 yaparak kapatýyorum. 0. biti 1 yapýyorum, bunu tersliyorum ve 0 oluyor.
	RCC->CR |= 1 << 16; // HSI osilatörünü açtýk.
	while(!(RCC->CR & (1 << 17))); // ! iþareti terslemeye yarýyor. 17. bit 1 olunca... Wait HSE active
	RCC->CR |= 1 << 19;
	RCC->PLLCFGR = 0x000000000; // PLLCFGR reset // PLL P deðeri 2 olur.
	RCC->PLLCFGR |= ~(4 << 0); // PLL M deðerini CubeMx'den 4 yapmýþ oldum.
	RCC->PLLCFGR |= ~(168 << 6); // PLL N deðerini CubeMx'den 168 yapmýþ oldum.

	RCC->CR |= (1 << 24); // PLL ON
	while(!(RCC->CR & (1 << 25))); // Wait PLL active. Bayrak için active olmasý bekleniyor.

	RCC->CFGR &= ~(1 << 0);
	RCC->CFGR |= (1 << 1);  // System Clock is PLL.

	while(!(RCC->CFGR & (1 << 1))); // Bayrak 1 olduðunda burayý tersleyip 0 yapýcam ve bu döngüden çýkacaðým.
}

void GPIO_Config(void)
{
	RCC->AHB1ENR |= 1<<3;      // GPIOD Clock Enable

	GPIOD->MODER |= 1 << 24;   // GPIOD 12. pin output // GPIO MODER tarafý MODER12
	GPIOD->MODER &= ~(1<<25); // 24'ü 1, 25'i 0 yaptýk.
	GPIOD->MODER |= 1 << 26;  // GPIOD 13. pin output  // GPIO MODER tarafý MODER13
	GPIOD->MODER &= ~(1<<27); // 26'yý 1, 27'yi 0 yaptýk.
	GPIOD->MODER |= 1 << 28;  // GPIOD 14. pin output  // GPIO MODER tarafý MODER14
	GPIOD->MODER &= ~(1<<29); // 28'i 1, 29'u 0 yaptýk.
	GPIOD->MODER |= 1 << 30;  // GPIOD 15. pin output  // GPIO MODER tarafý MODER15
	GPIOD->MODER &= ~(1<<31); // 30'u 1, 31'i 0 yaptýk.

	//GPIOD->OSPEEDR |= 0xFF000000; // 24 ve 28. bit arasý hep 1

}

int main(void)
{
//  systemClock = SystemCoreClock; // Atama yaptým.
//
//  RCC_DeInit(); // HighSpeedInternal Osilatörü enable eder. HSI-ON(HSIEN) ve PLL-OFF
//                // Bu fonksiyon sayesinde clock ayarlarý restlenir.
//  SystemCoreClockUpdate(); // 16 000 000
//
//  systemClock = SystemCoreClock; // 168 MHz'den 16 MHz'ye geliriz.

  RCC_Config();
  SystemCoreClockUpdate();

  GPIO_Config();
//  systemClock = SystemCoreClock; // Önce 168 MHz, Sonra 16 MHz, sonra 168 MHz
                                 // Sistem gerçekten 168 MHz olmuþ mu ona bakýyor, gerek yok.
  while (1)
  {
	  GPIOD->ODR |= 1<<12;     // 12. pin set
	  GPIOD->ODR |= 1<<13;     // 13. pin set
	  GPIOD->ODR |= 1<<14;     // 14. pin set
	  GPIOD->ODR |= 1<<15;     // 15. pin set

	  //for(int i = 0; i < 1680000; i++);
	  GPIOD->ODR &= ~(1<<12);
	  GPIOD->ODR &= ~(1<<13);
	  GPIOD->ODR &= ~(1<<14);
	  GPIOD->ODR &= ~(1<<15);

	  for(int i = 0; i<1680000; i++);
  }
}

/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
