#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_InitStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;
TIM_OCInitTypeDef TIMOC_InitStruct;

/*
 * PD12 de TIMCH1 var
 * PD13 de TIMCH2 var
 * PD14 de TIMCH3 var
 * PD15 de TIMCH4 var
 */

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // GPIO Pinleri AHB1'e baðlý, Block diyagram resminden bak.

	/*
	 * Alternate Function olarak kullandýðýmýz için bunun da configurasyonu yapýlmalýdýr.
	 */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); // Alternate Function ve Timer4 olarak ayarlama yapýlýyor.
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // Timer olarak kullanýldýðý için AF modu seçildi
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD, &GPIO_InitStruct);

}

// TIM4 APB1 Clock hattýna baðlýymýþ

void TIM_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // APB1 tarafý 42 MHz'dir. Timer'da bunun 2 katý...
													 // ...84 MHz kullanýlýr. Hiç bölmedik ve 84 MHz olarak kullandýk.
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = 9999 ;
	TIM_InitStruct.TIM_Prescaler = 8399 ;
	TIM_InitStruct.TIM_RepetitionCounter = 0; // Kaç defa sayacaðýný söylemekte

	TIM_TimeBaseInit(TIM4, &TIM_InitStruct); // Timer yükleme
	TIM_Cmd(TIM4, ENABLE); // Çevresel birimi enable ettik.

//	TIMOC_InitStruct.TIM_OCIdleState = ;
	TIMOC_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;

	/* PWM1 ve PWM2 vardýr. Birincisinde %25 duty cycle verirsek, %25 HIGH olur, %75 LOW olur
	 * Ýkincisi olan PWM2'yi verirsek %25'lik duty cycle'de %25 LOW olur, %75 HIGH olur.
	 *
	 */
//	TIMOC_InitStruct.TIM_OCNIdleState = ;
	TIMOC_InitStruct.TIM_OCNPolarity =  TIM_OCNPolarity_High;
//	TIMOC_InitStruct.TIM_OCPolarity = ;
//	TIMOC_InitStruct.TIM_OutputNState = ;
	TIMOC_InitStruct.TIM_OutputState = ENABLE; // PWM çýkýþý almak için aktive edilir.
	// TIMOC_InitStruct.TIM_Pulse = ; // Pulse sürekli deðiþeceðinden while içerisinde yazacaðýz.
}



int main(void)
{
	GPIO_Config();
	TIM_Config();
  while (1)
  {
	  // PD 12
	  TIMOC_InitStruct.TIM_Pulse = 9999; // D portunun 12. pinini %100 duty cycle ile çalýþtýrýyoruz.
	  TIM_OC1Init(TIM4, &TIMOC_InitStruct); // Aktive ediyoruz
	  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);	// Timer'in output comparesini aktive ediyoruz. Timer4 Channnel1 Enable oldu

	  // PD 13
	  TIMOC_InitStruct.TIM_Pulse = 7499; // D portunun 13. pinini %75duty cycle ile çalýþtýrýyoruz.
	  TIM_OC2Init(TIM4, &TIMOC_InitStruct); // Aktive ediyoruz
	  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);	// Timer'in output comparesini aktive ediyoruz. Timer4 Channnel2 Enable oldu

	  // PD 14
	  TIMOC_InitStruct.TIM_Pulse = 4999; // D portunun 14. pinini %50 duty cycle ile çalýþtýrýyoruz.
	  TIM_OC3Init(TIM4, &TIMOC_InitStruct); // Aktive ediyoruz
	  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);	// Timer'in output comparesini aktive ediyoruz. Timer4 Channnel3 Enable oldu

	  // PD 15
	  TIMOC_InitStruct.TIM_Pulse = 2499; // D portunun 15. pinini %25 duty cycle ile çalýþtýrýyoruz.
	  TIM_OC4Init(TIM4, &TIMOC_InitStruct); // Aktive ediyoruz
	  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);	// Timer'in output comparesini aktive ediyoruz. Timer4 Channnel4 Enable oldu


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
