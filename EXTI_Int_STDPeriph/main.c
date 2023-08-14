#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);


	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void EXTI_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, GPIO_PinSource0); // External Interrupt'larý aktif etmek için External hattýný da aktive ettik.
							                                      // A portunun 0. pinini External Interrupt olarak kullanýrýz.
	EXTI_InitStruct.EXTI_Line = EXTI_Line0; // 0. hatta baðlýyýz.
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; // 0 ile 15 arasý
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; //
	// Bu iki ifade birden fazla interrupta öncelik belirlemek için kullanýlýr.

	NVIC_Init(&NVIC_InitStruct);
}

void delay(uint32_t time)
{
	while(time--);
}

void EXTI0_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		GPIO_ResetBits(GPIOD, GPIO_Pin_All);
		delay(16800000);
		GPIO_SetBits(GPIOD, GPIO_Pin_All);
		delay(16800000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_All);
		delay(16800000);
		GPIO_SetBits(GPIOD, GPIO_Pin_All);
		delay(16800000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_All);
		delay(16800000);

		EXTI_ClearITPendingBit(EXTI_Line0); // Bayraklarý temizledik.
	}
}



int main(void)
{
	GPIO_Config();
	EXTI_Config();
  while (1)
  {
	  GPIO_SetBits(GPIOD, GPIO_Pin_All);



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

/* Clock hatlarý aktif olmalý ki çevresel birimler de aktif olsun
 * External Interrupt'ýn clock hattý da aktif olmalýdýr. External Interrupt SYSCFG olarak geçmektedir.
 * NVIC doðrudan iþlemcimize baðlý bu yüzden doðrudan baðlý olduðu bir clock hattý yok.
 * Aktif edilmesine gerek yok.
 * GPIO configürasyonlarýndan sonra interrupt configürasyonlarý yapýlmalýdýr.
 * Interrupt'dan sonra interrupt yöneticisinin konfigürasyonlarý yapýlmalýdýr.
 *
 * External interrupt'ýn bayraðýný temizlemek gerekir.
 *
 */
