#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_InitStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;
TIM_OCInitTypeDef TIMOC_InitStruct;
ADC_InitTypeDef ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonInitStruct;

uint16_t adc_value;
uint8_t pwm_value = 0;

uint32_t map(long A, long Bmin, long Bmax, long Cmin, long Cmax)
{
	return (A * Cmax) / Bmax;
}

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // Potansiyometre tak�l�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // Ledler tak�l�

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4); // Timer vb. �eyler i�in Alternate Function kullan�l�r.
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // Alternate Functionun Timer4 oldu�unu s�yl�yoruz.
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN; //  Analog Mode
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

}


void TIM_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // APB1 taraf� 42 MHz'dir. Timer'da bunun 2 kat�...
													 // ...84 MHz kullan�l�r. Hi� b�lmedik ve 84 MHz olarak kulland�k.
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_Period = 99 ; // Buras� di�er ad�yla AutoReload register de�erimizdir. // Yeni atad���m�z de�erler
	TIM_InitStruct.TIM_Prescaler = 83 ;
	TIM_InitStruct.TIM_RepetitionCounter = 0; // Ka� defa sayaca��n� s�ylemekte

	TIM_TimeBaseInit(TIM4, &TIM_InitStruct); // Timer y�kleme
	TIM_Cmd(TIM4, ENABLE); // �evresel birimi enable ettik.

//	TIMOC_InitStruct.TIM_OCIdleState = ;
	TIMOC_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;

	/* PWM1 ve PWM2 vard�r. Birincisinde %25 duty cycle verirsek, %25 HIGH olur, %75 LOW olur
	 * �kincisi olan PWM2'yi verirsek %25'lik duty cycle'de %25 LOW olur, %75 HIGH olur.
	 *
	 */
//	TIMOC_InitStruct.TIM_OCNIdleState = ;
	TIMOC_InitStruct.TIM_OCNPolarity =  TIM_OCNPolarity_High;
//	TIMOC_InitStruct.TIM_OCPolarity = ;
//	TIMOC_InitStruct.TIM_OutputNState = ;
	TIMOC_InitStruct.TIM_OutputState = ENABLE; // PWM ��k��� almak i�in aktive edilir.
	// TIMOC_InitStruct.TIM_Pulse = ; // Pulse s�rekli de�i�ece�inden while i�erisinde yazaca��z.

}

void ADC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4; // 84 MHz fazla, en fazla 36 MHz ile �al��t���ndan 4'e b�lmek uygundur.

	ADC_CommonInit(&ADC_CommonInitStruct);

	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;

	ADC_Init(ADC1, &ADC_InitStruct); // ADC Initialize
	ADC_Cmd(ADC1, ENABLE);


}

uint16_t Read_ADC()
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_56Cycles); // ADC1'de okuma, A portunun 0'�nc� pini
																			   // 1 tane ADC okumas�, ADC �rnekleme s�resini 56Cycle se�tik.
	ADC_SoftwareStartConv(ADC1);   // ADC yaz�l�msal olarak ba�lat�ld�.
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); // Yaz�l�msal olarak ba�lay�p 1 �evrim bitince EOC bayra�� kalk�yor.(�evrim bitti bayra��)
	return ADC_GetConversionValue(ADC1); // ADC de�eri art�k okunabildi�inden bu de�eri d�nd�rd�k.

}






int main(void)
{
	GPIO_Config();
	TIM_Config();
	ADC_Config();

  while (1)
  {
	adc_value = Read_ADC(); // return ADC_GetConversionValue(ADC1); olarak d�nd�r�len de�eri adc_value'ye atad�k.
	pwm_value = map(adc_value, 0, 4095, 0, 99);

  // PD 12
  TIMOC_InitStruct.TIM_Pulse = pwm_value;
  TIM_OC1Init(TIM4, &TIMOC_InitStruct); // Aktive ediyoruz
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);	// Timer'in output comparesini aktive ediyoruz. Timer4 Channnel1 Enable oldu

  // PD 13
  TIMOC_InitStruct.TIM_Pulse = pwm_value;
  TIM_OC2Init(TIM4, &TIMOC_InitStruct); // Aktive ediyoruz
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);	// Timer'in output comparesini aktive ediyoruz. Timer4 Channnel2 Enable oldu

  // PD 14
  TIMOC_InitStruct.TIM_Pulse = pwm_value;
  TIM_OC3Init(TIM4, &TIMOC_InitStruct); // Aktive ediyoruz
  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);	// Timer'in output comparesini aktive ediyoruz. Timer4 Channnel3 Enable oldu

  // PD 15
  TIMOC_InitStruct.TIM_Pulse = pwm_value;
  TIM_OC4Init(TIM4, &TIMOC_InitStruct); // Aktive ediyoruz
  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);	// Timer'in output comparesini aktive ediyoruz. Timer4 Channnel4 Enable oldu


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
