#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

int i=0;

void delay(uint32_t time)
{
	while(time--);
}

void RCC_Config()
{
	RCC->CR |= 0x00010000;    // HSEON enable. Datasheet 7.3.1
	/*
	 * Daha öncesinde HSERDY bitini de aktif ettik. Bu readable(okunabilen) olduðundan yapýlan pek doðru deðil.
	 * Düzelttik onu.
	 */
	while((RCC->CR & 0x00020000));   // HSEON Read Flag wait
	RCC->CR |= 0x00080000;           // CSS Enable
	RCC->PLLCFGR |= 0x00040000;      // PLL'e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;      // PLL M=4
	RCC->PLLCFGR |= 0x00005A00;      // PLL N=168
	RCC->PLLCFGR |= 0x00000000;      // PLL P=4
	RCC->CFGR |= 0x00000000;         // AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;         // APB2 Prescaler = 2
	RCC->CFGR |= 0x00001000;         // APB2 Prescaler = 4
	RCC->CIR |= 0x00080000;          // HSERDY Flag Clear
	RCC->CIR |= 0x00800000;          // CSS Flag Clear

}

void DAC1_Config()
{
	RCC->AHB1ENR |= 0x00000001;      // Datasheet 6.3.10 Bizim led GPIOA 4. pinde GPIO pinlerinin aktifliði için AHB1ENR ilk biti 1 olmalý
	RCC->APB1ENR |= 0x20000000;      // Datasheet 6.3.13  29. bit 1 olmalý
	// DAC'ýn clock ayarý aktif olduðuna göre artýk DAC'ýn ayarlarýna geçilebilir.
	DAC->CR |= 0x00000001;   // Datasheet 14.5.1  channel 1 için 0. biti yani EN1 bitini 1 yapýyoruz.
	DAC->SWTRIGR |= 0x00000000;    // Datasheet 14.5.2 DAC Channel için software trigger disable. Channel1 için yaptýk.
	DAC->DHR12R1 |= 0x00000000;    // Datasheet 14.5.3
	// DAC->DHR12R1 |= 0x00000FFF;  yazsaydým ilk datasý 4095 olurdu yani 3.3 volt çýkacaktý, biz ilk baþlangýç deðerini 0 yaptýk.
	// bu yüzden 0 volt ile karþýlaþacaðýz.

}


int main(void)
{

	RCC_Config();
	DAC1_Config();
  while (1)
  {
	  for(i=0; i<4096; i++)
	  {
		  DAC->DHR12R1 = i;
		  delay(168000);
	  }
	  i = 0;
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
