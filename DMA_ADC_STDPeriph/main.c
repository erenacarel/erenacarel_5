// DMA ile ADC okumasý yapýlacak

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#define BufferLength 1

// Çoklu ADC uygulamasýna binaen dizi oluþturuldu
// mesela 3 ADC okuma yapýlýrsa dizinin içine 3 yazýlýr.
uint16_t adc_value[BufferLength]; // 1 tane okuma yazýlýr.

GPIO_InitTypeDef GPIO_InitStruct;
ADC_InitTypeDef ADC_InitStruct;
ADC_CommonInitTypeDef ADC_CommonStruct; // ADC'nin 2 tane
DMA_InitTypeDef DMA_InitStruct;

// GPIO ayarlarýna baþlamak için konfigürasyon yaparým.
// A portunu seçtik. Bu da AHB1 clock hattýna baðlý

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// Önce ADC konfigürasyonlarýmý yaparým
// ADC kanallarý APB2 clock hattýna baðlý

void ADC_Config()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonStruct.ADC_Mode = ADC_Mode_Independent;
	/*
	 * Prescaler bölme oraný demektir. ADC frekansý 36MHz'dir. APB2'ye giden
	 * frekans 84 MHz'dir. Bu frekansýn düþmesi için bölünme yapýlmasý gerekir.
	 * Yani 4' bölünme uygun olur. 2 yetmez.
	 */
	ADC_CommonStruct.ADC_Prescaler = ADC_Prescaler_Div4;
	// Çoklu ADC okumasý yapýlýyorsa tercih edilebilir.
	ADC_CommonStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;

	ADC_CommonInit(&ADC_CommonStruct);

	// Sürekli çevrim modu. ADC'nin devamlý yapýlmasýný istiyorsak bunu da aktif etmek gerekir.
	// Tek bir sefer okuma olsun ve dursun istiyorsak DISABLE yazmalýyýz.
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;

	// 12 bitlik okumanýn 16 bitlik registere yazýldýðýný düþünelim.
	// saða yaslý mý sola yaslý mý olduðunu düþünüyoruz.
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;

	// Dýþarýdan tetikleme yapýlýrsa çevrim tetiðini soran fonksiyon
	ADC_InitStruct.ADC_ExternalTrigConv = 0;

	// ADC'yi harici bir tetikleme ile baþlatmamýzý saðlýyor.
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

	// Çevrim sayýsýný soruyor. 1 ADC okumasý varsa 1, 2 ise 2....
	ADC_InitStruct.ADC_NbrOfConversion = BufferLength;

	// ADC okuma hassasiyeti
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;

	// Çoklu ADC okumasý yapýlýyorsa bu mod aktif edilmelidir.
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;

	ADC_Init(ADC1, &ADC_InitStruct); // Structure'yi initialize ediyorum.

	// Çevresel birim olduðundan aktif etmek gerekiyor.
	ADC_Cmd(ADC1, ENABLE);

	// Channel_0 GPIO_A pininin 0. pini demek, 1 adet okumadan dolayý rank 1
	// Örnekleme süresini de 3 Cycle seçiyorum. Ne kadar az seçersek o kadar kýsa sürede okur.
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);

	// Her DMA bittiðinde istek yapmak gerekir yoksa düzgün ADC okuyamayýz.
	// Transferden sonra tekrar DMA yapýlmasý gerekir.
	// ADC'yi DMA ile kullanabilmek için istek yapýlmalýdýr.
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	// ADC ve DMA'yý birbirine baðlamak gerekiyor. DMA da çevresel birim.
	ADC_DMACmd(ADC1, ENABLE);
}

void DMA_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_InitStruct.DMA_BufferSize = BufferLength;

	// DMA'nýn stream 0'ýna baðlý
	DMA_InitStruct.DMA_Channel = DMA_Channel_0;

	// DMA'yý nasýl kullanacaðýmýzý belirtir.
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;

	// First Input First Output, ilk giren ilk çýksýn
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;

	// FIFO ne kadar doluyken iletsin. Belirlediðimiz fonksiyon ile FIFO taþana kadar içine veri alýyor.
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;

	// Aldýðýmýz ADC'nin nerelere yazýlacaðý belirtilir.
	DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&adc_value;

	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

	// Veri yollanan/yazýlan adres sürekli deðiþiyor yani
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;

	// Hiç durmadan çevresel birimden aldýðý veriyi RAM'e yollasýn.
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;

	// DMA'mýz veriyi ADC'nin data registerindan alacak olarak ayarlandý.
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;

	// Memory ve Peripheral Burst ayný olmalýdýr.
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	// Memory DataSize ile ayný olur.
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;

	// Veri okunan adres hiç deðiþmiyor, sabit, Disable
	DMA_InitStruct.DMA_PeripheralInc = DMA_MemoryInc_Disable;

	// Öncelik belirleme fonksiyonu
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;


	DMA_Init(DMA2_Stream0, &DMA_InitStruct);
	DMA_Cmd(DMA2_Stream0, ENABLE);


}

int main(void)
{
	GPIO_Config();
	ADC_Config();
	DMA_Config();

	// ADC'yi yazýlýmsal baþlatýyoruz.
	// While içinde yazarsak RAM'e CPU üzerinden yazarýz.
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
