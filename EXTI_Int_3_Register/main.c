#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

void CLK_Config()
{
	RCC->CR |= 0x00030000;         // HSEON and HSEONRDY enable   // Datasheet 7.3.1
	while(!(RCC->CR & 0x0002000)); // HSEON Ready Flag wait
	RCC->CR |= 0x00080000;         // CSS Enable Clock Security   // Datasheet 7.3.2
	RCC->PLLCFGR |= 0x00400000;    // PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;    // PLL M=4
	RCC->PLLCFGR |= 0x00005A00;    // PLL N=168
	RCC->PLLCFGR |= 0x00000000;    // PLL P=2
	RCC->CFGR |= 0x00000000;       // AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;       // APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;       // APB1 Prescaler = 4
	RCC->CIR |= 0x00080000;        // HSERDY Flag Clear
	RCC->CIR |= 0x00800000;        // CSS Flag Center
}

void GPIO_Config()
{
	RCC->AHB1ENR = 0x00000009; // GPIOA and GPIOD active  Datasheet 6.3.10
	GPIOD->MODER = 0x55000000; // GPIOD pin12...15 output Datasheet 8.4.1
	GPIOD->OTYPER = 0x00000000; // GPIOD push pull
	GPIOD->OSPEEDR = 0xFF000000; // very high
	GPIOD->PUPDR = 0x00000000; // no pull

}

void EXTI_Config()
{
	RCC->APB2ENR = 0x00004000; // SYSCFG active
	NVIC_EnableIRQ(EXTI0_IRQn); // EXTI0 için interrupt aktif ettim.
	NVIC_EnableIRQ(EXTI1_IRQn); // EXTI1 için interrupt aktif ettim.
	NVIC_EnableIRQ(EXTI2_IRQn); // EXTI2 için interrupt aktif ettim.

	NVIC_SetPriority(EXTI0_IRQn, 0); // Öncelik EXTI0'ýn olsun
	NVIC_SetPriority(EXTI1_IRQn, 1);
	NVIC_SetPriority(EXTI2_IRQn, 2);

	// NVIC'leri ve öncelikleri tanýmlamýþ oldum.
	// Ardýndan interrup registerleri gözden geçirilmelidir.
	// Datasheet 12.3.1 buradaki ilk 3 pini 1 yapacaðýz,

	EXTI->IMR = 0x00000007; // PA0, PA1, PA2 interrupt için aktif

	// Datasheet 12.3.2 de EMR registeri var, biz interrupt olarak kullaným yaptýðýmýz için burayý atlýyoruz.
	// Event mode'da çalýþmýyoruz.

	EXTI->RTSR = 0x0000007; // pinleri yükselen kenarda yaptýk, ilk 3 pin 1. Datasheet 12.3.5

	// Pending bayrak inme kalkmasý, kesmeye girme çýkmayla ilgilidir.

}

void delay(uint32_t time)
{
	while(time--);
}

void EXTI0_IRQHandler()    // Birinci interrupt, buna basýlýnca olacaklar.
{
	if(EXTI->PR & 0x00000001) // Buradaki 1. bitte deðer varsa içeri gir.
	{
		GPIOD->ODR = 0x00001000; // ilk ledi yak. moder12
		delay(33600000);

		EXTI->PR = 0x00000001; // bayrak kalkmýþtý, artýk inmesini saðlýyoruz.
	}
}

void EXTI1_IRQHandler()
{
	if(EXTI->PR & 0x00000002) // 2. bitin dolu olup olmadýðýna bakýyoruz.
	{
		GPIOD->ODR = 0x00002000; // 2. ledi yakýyorum.
		delay(3360000);

		EXTI->PR = 0x00000002;
	}
}

void EXTI2_IRQHandler()
{
	if(EXTI->PR & 0x00000004)  // 3. bitin dolu olup olmadýðýna bakýyoruz.
	{
		GPIOD->ODR = 0x0004000;  // 3. ledi yakýyorum.
		delay(3360000);

		EXTI->PR = 0x00000004;
	}
}

int main(void)
{
	CLK_Config();
	GPIO_Config();
	EXTI_Config();
	EXTI0_IRQHandler();
	EXTI1_IRQHandler();
	EXTI2_IRQHandler();

  while (1)
  {
	  GPIOD->ODR &= 0x0000F000; // bütün pinler yanýyor.
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
