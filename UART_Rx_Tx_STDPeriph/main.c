#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

// USB TTL gereklidir.
//char str[50];
char rx_buff[50];
int i = 0;

GPIO_InitTypeDef GPIO_InitStruct;
USART_InitTypeDef USART_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

//void delay(uint32_t time)
//{
//	while(time--);
//}

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Alternate Function ayarý kullanmak gerekir.
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // PA2 Tx
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // PA3 Rx

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; // USART_TX pini ve USART_RX pini
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void USART_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	// mesaj gönderileceði için Tx ayrýca mesaj alýmý da yapacaðýz
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	// Karþý taraf için saðlama
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART2, &USART_InitStruct);

	USART_Cmd(USART2, ENABLE);

	// Bir mesaj geldiðinde interrupt olsun
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);


}


void NVIC_Config()
{
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIC_InitStruct);
}

// Data gönderirken yine bu fonksiyon kullanýlmaya devam edecek
void USART_Puts(USART_TypeDef* USARTx, volatile char *s)
{
	while(*s)
	{
		while(!(USARTx->SR & 0x00000040));

		USART_SendData(USARTx, *s);
		*s++;
	}
}

// Data geldiðinde bu fonksiyona gidilsin
void USART2_IRQHandler()
{
	// bayraðýn durumuna baktýk
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
		char rx_buffer = USART2->DR;
		// char rx_buffer = USART_ReceiveData(USART2);
		if(rx_buff != '\n' && i < 50) // Enter'e basilana kadar ve 50 elemaný geçmeyene kadar
		{
			rx_buff[i] = rx_buffer;
			i++;
		}
		else
		{
			rx_buff[i] = rx_buffer; // Enter'a basýldýðýnda son gelen parametreyi(\n) yazmasýn. Onun yerine rx_buffer diyerek her mesajý alta alýyorum
			i = 0; // yeni mesaj gelmiyor artýk
			USART_Puts(USART2, rx_buff); // gelen mesajý geri gönderelim.

			// Her veri gönderildiðinde rx_buffer dizisi boþaltýlmalýdýr.
			// for döngüsü ile mesajý gönderdikten sonra tüm verileri 0'lýyoruz.
			for(int a = 0; a < 50; a++)
				rx_buff[a] = '\0';

		}
	}


}


int main(void)
{
	GPIO_Config();
	USART_Config();
	NVIC_Config();

  while (1)
  {
//	sprintf(str, "Heloo\n");
//	USART_Puts(USART2, str);
//	delay(1680000);
  }
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
  return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void)
{
  return -1;
}
