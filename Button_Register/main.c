#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

int count = 0;

void delay(uint32_t time)
{
	while(time--);
}

void GPIO_Config()
{
	RCC->CR |= 0x00030000; // HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000)); // HSEON ready flag wait
	RCC->CR |= 0x00080000; // CSS Enable
	RCC->PLLCFGR |= 0x00000000; // PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004; // PLL M = 4
	RCC->PLLCFGR |= 0x000000A8; // 0x0005A000 // PLL N = 168
	RCC->PLLCFGR |= 0x00000000; // PLL P = 2
	RCC->CFGR |= 0x00000000; // AHB Prescaler = 1
	RCC->CFGR |= 0x0008000; // APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400; // APB1 Prescaler = 4
	RCC->CIR |= 0x00080000; // HSERDY Flag clear
	RCC->CIR |= 0x00080000; // CSS Flag clear

	RCC->AHB1ENR = 0x00000009; // GPIOA ve GPIOD enable

	GPIOD->MODER = 0x55000000; // 12, 13, 14, 15. pin digital output
	GPIOD->OTYPER = 0x00000000; // 12, 13, 14, 15 pins push pull
	GPIOD->OSPEEDR = 0xFF000000; // 12, 13, 14, 15 pins 100MHz
	GPIOD->PUPDR = 0x00000000; // 12, 13, 14, 15 pins no pull

}

int main(void)
{
	GPIO_Config();
  while (1)
  {
	  if(GPIOA->IDR & 0x00000001)
	  {   // butona basmaya devam ediyor mu, butona basmaya devam edene kadar bekle.
		  while(GPIOA->IDR & 0x00000001);
		  delay(1680000);

		  count++;
		  // 100 ms'lik bir iþlemi tamamladýðýmý varsayýyorum.
	  }

	  if(count%2 == 0)
		  GPIOD->ODR = 0x00000000; // 12,13,14,15 pinleri 0 oldu.
	  else // butona basýldýysa ledleri yak
		  GPIOD->ODR = 0x0000F000; // 12,13,14,15 pinleri 1 oldu.
	      // GPIOD -> ODR = 0x0000A00;
  }
}


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
