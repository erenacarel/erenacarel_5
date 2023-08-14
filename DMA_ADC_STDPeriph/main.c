// DMA ile ADC okumas� yap�lacak

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#define BufferLength 1

// �oklu ADC uygulamas�na binaen dizi olu�turuldu
// mesela 3 ADC okuma yap�l�rsa dizinin i�ine 3 yaz�l�r.
uint16_t adc_value[BufferLength]; // 1 tane okuma yaz�l�r.

GPIO_InitTypeDef GPIO_InitStruct;
ADC_InitTypeDef ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonStruct; // ADC'nin 2 tane
DMA_InitTypeDef DMA_InitStruct;

// GPIO ayarlar�na ba�lamak i�in konfig�rasyon yapar�m.
// A portunu se�tik. Bu da AHB1 clock hatt�na ba�l�

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// �nce ADC konfig�rasyonlar�m� yapar�m
// ADC kanallar� APB2 clock hatt�na ba�l�

void ADC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonStruct.ADC_Mode = ADC_Mode_Independent;
	/*
	 * Prescaler b�lme oran� demektir. ADC frekans� 36MHz'dir. APB2'ye giden
	 * frekans 84 MHz'dir. Bu frekans�n d��mesi i�in b�l�nme yap�lmas� gerekir.
	 * Yani 4' b�l�nme uygun olur. 2 yetmez.
	 */
	ADC_CommonStruct.ADC_Prescaler = ADC_Prescaler_Div4;
	// �oklu ADC okumas� yap�l�yorsa tercih edilebilir.
	ADC_CommonStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;

	ADC_CommonInit(&ADC_CommonStruct);

	// S�rekli �evrim modu. ADC'nin devaml� yap�lmas�n� istiyorsak bunu da aktif etmek gerekir.
	// Tek bir sefer okuma olsun ve dursun istiyorsak DISABLE yazmal�y�z.
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;

	// 12 bitlik okuman�n 16 bitlik registere yaz�ld���n� d���nelim.
	// sa�a yasl� m� sola yasl� m� oldu�unu d���n�yoruz.
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;

	// D��ar�dan tetikleme yap�l�rsa �evrim teti�ini soran fonksiyon
	ADC_InitStruct.ADC_ExternalTrigConv = 0;

	// ADC'yi harici bir tetikleme ile ba�latmam�z� sa�l�yor.
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

	// �evrim say�s�n� soruyor. 1 ADC okumas� varsa 1, 2 ise 2....
	ADC_InitStruct.ADC_NbrOfConversion = BufferLength;

	// ADC okuma hassasiyeti
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;

	// �oklu ADC okumas� yap�l�yorsa bu mod aktif edilmelidir.
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;

	ADC_Init(ADC1, &ADC_InitStruct); // Structure'yi initialize ediyorum.

	// �evresel birim oldu�undan aktif etmek gerekiyor.
	ADC_Cmd(ADC1, ENABLE);

	// Channel_0 GPIO_A pininin 0. pini demek, 1 adet okumadan dolay� rank 1
	// �rnekleme s�resini de 3 Cycle se�iyorum. Ne kadar az se�ersek o kadar k�sa s�rede okur.
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);

	// Her DMA bitti�inde istek yapmak gerekir yoksa d�zg�n ADC okuyamay�z.
	// Transferden sonra tekrar DMA yap�lmas� gerekir.
	// ADC'yi DMA ile kullanabilmek i�in istek yap�lmal�d�r.
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	// ADC ve DMA'y� birbirine ba�lamak gerekiyor. DMA da �evresel birim.
	ADC_DMACmd(ADC1, ENABLE);
}

void DMA_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_InitStruct.DMA_BufferSize = BufferLength;

	// DMA'n�n stream 0'�na ba�l�
	DMA_InitStruct.DMA_Channel = DMA_Channel_0;

	// DMA'y� nas�l kullanaca��m�z� belirtir.
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;

	// First Input First Output, ilk giren ilk ��ks�n
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;

	// FIFO ne kadar doluyken iletsin. Belirledi�imiz fonksiyon ile FIFO ta�ana kadar i�ine veri al�yor.
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;

	// Ald���m�z ADC'nin nerelere yaz�laca�� belirtilir.
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&adc_value;

	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

	// Veri yollanan/yaz�lan adres s�rekli de�i�iyor yani
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;

	// Hi� durmadan �evresel birimden ald��� veriyi RAM'e yollas�n.
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;

	// DMA'm�z veriyi ADC'nin data registerindan alacak olarak ayarland�.
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;

	// Memory ve Peripheral Burst ayn� olmal�d�r.
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	// Memory DataSize ile ayn� olur.
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;

	// Veri okunan adres hi� de�i�miyor, sabit, Disable
	DMA_InitStruct.DMA_PeripheralInc = DMA_MemoryInc_Disable;

	// �ncelik belirleme fonksiyonu
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;


	DMA_Init(DMA2_Stream0, &DMA_InitStruct);
	DMA_Cmd(DMA2_Stream0, ENABLE);


}

int main(void)
{
	GPIO_Config();
	ADC_Config();
	DMA_Config();

	// ADC'yi yaz�l�msal ba�lat�yoruz.
	// While i�inde yazarsak RAM'e CPU �zerinden yazar�z.
	ADC_SoftwareStartConv(ADC1);

  while (1)
  {

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
