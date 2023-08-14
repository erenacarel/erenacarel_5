#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <stdio.h>

GPIO_InitTypeDef GPIO_InitStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;
TIM_OCInitTypeDef TIMOC_InitStruct;

//stm32f4xx_it.c dosyasýndadýr, iki kez tanýmlama yapmamak için orayý yorum yapýyorum.
// 1 ms'de bir buraya gelsin

uint32_t delay_counter;

void SysTick_Handler()
{
	if(delay_counter > 0)
	{
		delay_counter--; // Buna deðer atayabilmek için delay_ms(uint32_t time) fonksiyonunu oluþturduk.

	}
}

void delay_ms(uint32_t time)
{
	delay_counter = time;
	while(delay_counter);
}

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); // Timer vb. þeyler için Alternate Function kullanýlýr.
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD, &GPIO_InitStruct);

}


void TIM_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // APB1 tarafý 42 MHz'dir. Timer'da bunun 2 katý...
													 // ...84 MHz kullanýlýr. Hiç bölmedik ve 84 MHz olarak kullandýk.
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = 99 ; // Yeni atadýðýmýz deðerler
	TIM_InitStruct.TIM_Prescaler = 83 ;
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

	SysTick_Config(SystemCoreClock/1000); // 1 ms'de bir kesmeye giriyor. O kesme fonksiyonunun adý SysTick_Handler()

}




int main(void)
{
	GPIO_Config();
	TIM_Config();


  while (1)
  {
	for(int i = 0; i <= 100; i++)
	{

		TIMOC_InitStruct.TIM_Pulse = i;
		TIM_OC1Init(TIM4, &TIMOC_InitStruct);
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		delay_ms(20);

		TIMOC_InitStruct.TIM_Pulse = i;
		TIM_OC2Init(TIM4, &TIMOC_InitStruct);
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		delay_ms(20);

		TIMOC_InitStruct.TIM_Pulse = i;
		TIM_OC3Init(TIM4, &TIMOC_InitStruct);
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		delay_ms(20);

		TIMOC_InitStruct.TIM_Pulse = i;
		TIM_OC4Init(TIM4, &TIMOC_InitStruct);
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		delay_ms(20);

	}

	for(int i = 100; i >= 0; i--) // Burayý <= yapýnca sýfýrlanýp tekrar yanarken, >= yapýnca yavaþça sönüyor
	{
		TIMOC_InitStruct.TIM_Pulse = i;
		TIM_OC1Init(TIM4, &TIMOC_InitStruct);
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		delay_ms(20);

		TIMOC_InitStruct.TIM_Pulse = i;
		TIM_OC2Init(TIM4, &TIMOC_InitStruct);
		TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		delay_ms(20);

		TIMOC_InitStruct.TIM_Pulse = i;
		TIM_OC3Init(TIM4, &TIMOC_InitStruct);
		TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		delay_ms(20);

		TIMOC_InitStruct.TIM_Pulse = i;
		TIM_OC4Init(TIM4, &TIMOC_InitStruct);
		TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
		delay_ms(20);

	}




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
