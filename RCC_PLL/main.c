#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

extern uint32_t SystemCoreClock; // system_stm32f4xx.c dosyasýndan alýyoruz. Ýlk deðeri elde etmiyoruz.

uint32_t systemClock; // main dýþýnda global deðiþken olduðu için 0 atanýr.

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

	while((RCC->CFGR & (1 << 1))); // Bayrak 1 olduðunda burayý tersleyip 0 yapýcam ve bu döngüden çýkacaðým.


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

  systemClock = SystemCoreClock; // Önce 168 MHz, Sonra 16 MHz, sonra 168 MHz

  while (1)
  {

  }
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
