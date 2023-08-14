#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

/* STDPeriph Lib ile yap�ld�
 * Devremizde PA4 ve PA5 pinine ledin + ucu ba�land�.
 * - ucuna da Gnd ba�land�.
 *
 */

GPIO_InitTypeDef GPIO_InitStruct;
DAC_InitTypeDef DAC_InitStruct;

int i = 0;

void delay(uint32_t time)
{
	while(time--);
}

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;      // DAC1 4. pine ba�l�
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;      // DAC2 5. pine ba�l�
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd =GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);

}

void DAC2_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None; // Bir dalga olu�turmayaca��m.
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_None;               // Tetiklemesiz kullan�m i�in.
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;   // G�r�lt� �nleyici a�t�k.

	DAC_Init(DAC_Channel_1, &DAC_InitStruct);
	DAC_Init(DAC_Channel_2, &DAC_InitStruct);

	// �evresel birimleri �al��t�rmak i�in Cmd'de aktif edilmelidir.

	DAC_Cmd(DAC_Channel_1, ENABLE);  // Art�k DAC'� kullanabilirim.
	DAC_Cmd(DAC_Channel_2, ENABLE);  // Art�k DAC'� kullanabilirim.
}


int main(void)
{
	GPIO_Config();
	DAC2_Config();
  while (1)
  {
	for(; i<255; i++)
	{
		DAC_SetChannel1Data(DAC_Align_8b_R, i);
		delay(10000);
		DAC_SetChannel2Data(DAC_Align_8b_R, i);
		delay(10000);
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
