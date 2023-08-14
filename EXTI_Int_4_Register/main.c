#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

/*
 *  Burada baðlantý þemalarýndan bahsedeyim.
 *  Buton 1'i PA1'e baðladým. Diðer bacaðýný 3V yaptým.
 *  3V taktýðým bacaðýn altýndan GND'ye pull down direnci baðladým.
 *
 *  Buton 2'yi PA2'ye baðladým. Diðer bacaðýný 3V yaptým.
 *  3V taktýðým bacaðýn altýndan GND'ye pull down direnci baðladým.
 *
 */

void delay(uint32_t time)
{
	while(time--);
}

void GPIO_Config()
{
	RCC->AHB1ENR = 0x00000009; // 0000... 1001 Datasheet 6.3.10 RCC peripheral clock register
	GPIOD->MODER = 0x55000000; //MODER input output yapmaya karar verir. GPIO modu yaptýk.
	GPIOD->OTYPER = 0x00000000;
	GPIOD->OSPEEDR = 0xFF000000;

}

void EXTI_Config()
{
	// External Interrupt pini APB2 saatine baðlý Bunu da aktif etmek gerekli
	RCC->APB2ENR = 0x00004000; // Burada SYSCFG biti bizim interruptýmýz, bunu aktif etmek gerkli, 14. bit 1 olmalý

	// 3 butonumuz var, biri PA0, biri PA1, diðeri PA2
	NVIC_EnableIRQ(EXTI0_IRQn); // PA0 birinci hat aktif
	NVIC_EnableIRQ(EXTI1_IRQn); // PA1 ikinci hat aktif
	NVIC_EnableIRQ(EXTI2_IRQn); // PA2 ikinci hat aktif

	SYSCFG->EXTICR[0] = 0x00000000;   // Datasheet 9.2.3 PA0 pini için yaptýk. PA pinini hep 0000 diye belirtmiþ.
	SYSCFG->EXTICR[1] = 0x00000000;   // PA1 biti aktif
	SYSCFG->EXTICR[2] = 0x00000000;   // PA2 biti aktif

	// Priority tanýmlamak gerekir. Öncelik sýrasý
	NVIC_SetPriority(EXTI0_IRQn, 0);
	NVIC_SetPriority(EXTI1_IRQn, 2);
	NVIC_SetPriority(EXTI2_IRQn, 1);

	// Þimdi EXTI IMR registerini ayarlamalýyýz. EXTI_IMR Datasheet 12.3.1
	// IMR ile interrupt olduðunu belirtiyoruz.
	// EMR ile de event olduðunu belirtiriz.
	// IMR'de ilk 3 biti kullanacaðýz. PA0, PA1, PA2
	EXTI->IMR = 0x00000007;
	EXTI->RTSR = 0x00000007; // ilk üç biti 1

}

// Interrupt bayraðý için pending registeri vardýr.
void EXTI0_IRQHandler()
{
	if(EXTI->PR & (1<<0)) // 0. bit 1 oldu mu, olduysa interrupt olmuþtur.
	{
		int i = 0;
		do{
			GPIOD->ODR = 0x00001000;
			delay(6720000);
			GPIOD->ODR = 0x00000000;
			delay(6720000);
			i++;
		}while(i<5);

		EXTI->PR = (1<<0); // 0. biti 1 yap.
	}
}

void EXTI1_IRQHandler()
{
	if(EXTI->PR & (1<<1)) // 1. bit 1 oldu mu, olduysa interrupt olmuþtur.
	{
		int i = 0;
		do{
			GPIOD->ODR = 0x00002000;
			delay(6720000);
			GPIOD->ODR = 0x00000000;
			delay(6720000);
			i++;
		}while(i<5);

		EXTI->PR = (1<<1); // 1. biti 1 yap.
	}
}

void EXTI2_IRQHandler()
{
	if(EXTI->PR & (1<<2)) // 2. bit 1 oldu mu, olduysa interrupt olmuþtur.
	{
		int i = 0;
		do{
			GPIOD->ODR = 0x00004000;
			delay(6720000);
			GPIOD->ODR = 0x00000000;
			delay(6720000);
			i++;
		}while(i<5);

		EXTI->PR = (1<<2); // 2. biti 1 yap.
	}
}

//void EXTI3_IRQHandler()
//{
//	if(EXTI->PR & (1<<3)) // 2. bit 1 oldu mu, olduysa interrupt olmuþtur.
//	{
//		int i = 0;
//		do{
//			GPIOD->ODR = 0x00008000;
//			delay(6720000);
//			GPIOD->ODR = 0x00000000;
//			delay(6720000);
//			i++;
//		}while(i<5);
//
//		EXTI->PR = (1<<3); // 2. biti 1 yap.
//	}
//}


int main(void)
{
	GPIO_Config();
	EXTI_Config();
//	EXTI0_IRQHandler();
//	EXTI1_IRQHandler();
//	EXTI2_IRQHandler();

  while (1)
  {

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
