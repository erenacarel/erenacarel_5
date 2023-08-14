#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_InitStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;
TIM_OCInitTypeDef TIMOC_InitStruct;

uint32_t delay_count;

// Her 1 ms'de bir bu fonksiyona girecek
void SysTick_Handler(void)
{
	delay_count--;
}

void Delay_ms(uint32_t time)
{
	delay_count = time;
	while(time);
}


void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Alternate Function'da TIM4 olarak kullanýyoruz.
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD, &GPIO_InitStruct);

}


void TIM_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = 19999;
	TIM_InitStruct.TIM_Prescaler = 83;
	TIM_InitStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM4, &TIM_InitStruct);

	TIM_Cmd(TIM4, ENABLE);

	TIMOC_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIMOC_InitStruct.TIM_OutputNState = ENABLE;
	TIMOC_InitStruct.TIM_OCPolarity = TIM_OCPolarity_High;

	SysTick_Config(SystemCoreClock/1000); // 1 ms'de bir kesmeye giriyor. O kesme fonksiyonunun adý SysTick_Handler()


}

void Delay(uint32_t time)
{
	while(time--);
	// 1 tanesi 8 cycle'de gerçekleþir, o yüzden 21000000 yazýyoruz.
}


int main(void)
{
	GPIO_Config();
	TIM_Config();

  while (1)
  {
	  for(int i = 500; i <= 2000; i++)
	  {
		  TIMOC_InitStruct.TIM_Pulse = i;
		  TIM_OC1Init(TIM4, &TIMOC_InitStruct);
		  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		  Delay_ms(1);
	  }
	  for(int i = 2000; i >=500; i--)
	  {
		  TIMOC_InitStruct.TIM_Pulse = i;
		  TIM_OC1Init(TIM4, &TIMOC_InitStruct);
		  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		  Delay_ms(1);
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
