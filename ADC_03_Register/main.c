#include "stm32f4xx.h"
#include "stm32f4_discovery.h"


// 8 bitlik de�er okuyaca��m�z i�in bunu olu�turuyoruz
uint8_t adc_value;

// Bu 8 bitlik de�eri okuyaca��m�z ADC fonksiyonu olu�turuyoruz.
uint8_t Read_ADC()
{
	uint8_t value;
	ADC1->CR2 |= 0x40000000;         // ADC1'in yaz�l�msal olarak �evrimi ba�lad�. 30. bit SWSSTART biti 1 yap�ld�. 13.13.3
	while(!(ADC1->SR & 0x00000002)); // EOC flag� kalkt� m� onu bekleyece�iz. EOC flag� kalkt�ysa ADC �evrimi biter. Normalde bayrak kalkmam��ken 0
	                                 // 1 olana kadar flag kalkana kadar bekle, bayrak kalkt���
								     // anda ��kmas� i�in ! koyduk. Bayrak kalkmam��ken 0, bayrak kalkana kadar bekle
								     // bayrak kalkt���nda 1 olacak ve 1'i de ! ile tersliyorum. while'den ��kar�z.
	value = ADC1->DR; // Datalar�n tutuldu�u bu registerdeki de�eri alaca��m.
	return value;
}


void CLK_Config()
{
	RCC->CR |= 0x00000000;
	RCC->CR |= 0x00030000;         // HSEON and HSEONRDY enable   // Datasheet 7.3.1
	while(!(RCC->CR & 0x00020000)); // HSEON Ready Flag wait
	RCC->CR |= 0x00080000;         // CSS Enable Clock Security   // Datasheet 7.3.2
	RCC->PLLCFGR |= 0x00000000;    // PLLCFGR reset
	RCC->PLLCFGR |= 0x00400000;    // PLL e HSE se�tik
	RCC->PLLCFGR |= 0x00000004;    // PLL M=4
	RCC->PLLCFGR |= 0x00005A00;    // PLL N=168
	RCC->PLLCFGR |= 0x00000000;    // PLL P=2
	RCC->CFGR |= 0x00000000;       // AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;       // APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;       // APB1 Prescaler = 4
	RCC->CIR |= 0x00080000;        // HSERDY Flag Clear
	RCC->CIR |= 0x00800000;        // CSS Flag Center

	RCC->CR |= (1<<24);
	while(!(RCC->CR & (1<<25)));

	RCC->CFGR |= 0x00000003; // PLL 11 yazd�k

	while(!(RCC->CR & (1<<1)));

}

void GPIO_Config()
{
	RCC->AHB1ENR |= 0x00000001; // GPIOA Clock Enable               // Datasheet 7.3.10
	RCC->AHB1ENR |= 0x00000008; // GPIOD Clock Enable
	GPIOA->MODER |= 0x00000003; // Pin 0 Analog Mode  00..0011      // Datasheet 8.4.1
	GPIOA->OSPEEDR |= 0x00000003; // Pin 0 100 MHz
	//
	GPIOD->MODER |= 0x55000000;    // 12, 13, 14, 15 digital output
	GPIOD->OTYPER |= 0x00000000;   // 12, 13, 14, 15 push pull
	GPIOD->OSPEEDR |= 0xFF000000;  // 24'den 31. pine 11111111 en h�zl� ayae
	GPIOD->PUPDR |= 0x00000000;    // no pull

}

void ADC_Config()
{
	RCC->APB2ENR |= 0x00000100; // APB2 clock hatt� aktif edilmelidir.  // 7.3.14
	ADC1->CR1 |= 0x02000000;    // Control registerimizde 8 bit ��z�n�rl�kte �al��t�k ve
								// 25 ile 24. bitlere 10 yazd�k.  Datasheet 13.13.2
	ADC1->CR2 |= 0x00000001;    // ADC Enable                     Datasheet 13.13.3
	ADC1->SMPR2	|= 0x00000003;	// 0. kanalda 56 cycle ile �al���yoruz. 011 yazmak gerekiyor. 13.13.4
	ADC->CCR |= 0x00010000;     // Div 4, 17 ve 16. bitlere 01 yazmak gerekiyor. // Datasheet 13.13.16
}

void delay(uint32_t time)
{
	while(time--);
}

int main(void)
{
	//RCC_Config();
	CLK_Config();
	GPIO_Config();
	ADC_Config();
	SystemCoreClockUpdate();

  while (1)
  {
	  GPIOD->ODR |= 1<<15;
	  adc_value = Read_ADC();
	  if(adc_value <= 100)
	  {
		  //GPIOD->ODR &= ~(0x0000F000);
		  GPIOD->ODR |= 1<<12;
		  delay(6720000);
		  GPIOD->ODR |= 1<<13;
		  delay(6720000);
	  }

	  else{
		  //GPIOD->ODR &= ~(1<<1);
		  GPIOD->ODR &= ~(1<<12);
		  delay(6720000);
		  GPIOD->ODR &= ~(1<<13);
		  delay(6720000);
	  }
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
