#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "i2c_lcd.h"

GPIO_InitTypeDef GPIO_InitStruct;
I2C_InitTypeDef I2C_InitStruct;

void delay(uint32_t time)
{
	while(time--);
}

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);  // SCL Pin
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);  // SDA Pin

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOB, &GPIO_InitStruct);
}


void I2C_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

	I2C_InitStruct.I2C_Ack = ENABLE;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;

	I2C_Init(I2C1, &I2C_InitStruct);
	I2C_Cmd(I2C1, ENABLE);

}

void I2C_Write(uint8_t address, uint8_t data)
{
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));   // I2C bayraðý uygun olana kadar bekle // Flag_Busy durumu 0 olduðunda döngüye devam et.
	I2C_GenerateSTART(I2C1, ENABLE);  // I2C baþlatýyorum.
	while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)));  // Master_Mode durumu 1 olduðunda döngüden çýksýn
	I2C_Send7bitAddress(I2C1, address, I2C_Direction_Transmitter); // 7 bitlik adresi hatta gönderiyorum.
	while(!(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)));  // Master byte transferi gerçekleþtiyse 1'se, döngüden çýk.
	I2C_SendData(I2C1, data);   // Datamý gönderebiliyorum.
	I2C_GenerateSTOP(I2C1, ENABLE);  // Yeni mesajlara açýk olmak için haberleþmeyi durduruyorum.

}



int main(void)
{
	GPIO_Config();
	I2C_Config();
	lcd_init();
	//lcd_send_cmd(0x01); // clear display
	//delay(21000000); // 168 MHz / 8 = 210000000  delay 8 cycle sürüyor. Bir 0 eksik yazdýk 10ms oldu
	lcd_send_cmd(0x80); // 1. satýrýn baþý
	lcd_send_string("Eren");
  while (1)
  {
	  lcd_send_cmd(0x80); // 1. satýrýn baþý
	  lcd_send_string("Eren");
//	  delay(42000000);
//	  lcd_send_cmd(0xC0);   // 2. satýrýn baþý
//	  lcd_send_string("Acarel");
//	  delay(42000000);
//	  lcd_send_cmd(0x01);
//	  delay(21000000);
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
