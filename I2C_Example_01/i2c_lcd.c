/*
 * i2c_lcd.c
 *
 *  Created on: 3 Nis 2022
 *      Author: ERENACAREL
 */

#include "i2c_lcd.h"

extern I2C_InitTypeDef I2C_InitStruct;  // I2C ile igili fonksiyonlar dahil edildi.
// ilgili fonksiyonlar�m�z: I2C_AcknowledgedAddress;  I2C_Ack;   I2C_OwnAddress1;   I2C_DutyCycle;     I2C_Mode;    I2C_ClockSpeed;
// bu fonksiyonlar main.c taraf�nda

uint8_t myDeviceAddress = 0x4E;
//uint8_t myDeviceAddress = 0x40;

unsigned char karakter_[8][8] =
{
// TR Karakterler
{0, 14, 16, 16, 17, 14, 4, 0},   // �
{0, 0, 12, 4, 4, 4, 14, 0},      // I
{10, 0, 14, 17, 17, 17, 14, 0},  // �
{0, 15, 16, 14, 1, 30, 4, 0},    // �
{10, 0, 17, 17, 17, 17, 14, 0},  // �

// �zel i�aretler
{2, 6, 14, 30, 14, 6, 2, 0},     // <
{0, 4, 4, 4, 4, 4, 4, 0},        // |
{0, 16, 8, 4, 2, 1, 0 ,0}        // \ //

};

void lcd_init(void)
{
	lcd_send_cmd(0x02);   // return to home
	lcd_send_cmd(0x28);   // Function set -> DB5 = 1 and DB3 = N = 1 2-line display mode.      2x16 kullan�labilir
	lcd_send_cmd(0x0D);   // Display ON/OFF Control DB3=1 and DB2 = D = 1 display is turned on
	// if DB1 = C = 1 cursor on
	lcd_send_cmd(0x80);  // imleci ilk sat�r�n ba��nda tut.

}

void lcd_send_cmd(char cmd)
{
	char data_u, data_l;
	uint8_t data_t[4];

	/*	Komut g�ndermek i�in genellikle RS = 0 ve E = 1 yap�l�r.
	 *  Ard�ndan y�ksek de�erlikli 4 bit g�nderilir.
	 *  Y�ksek de�erlikli 4 bit g�nderildikten sonra E = 0 yap�l�r.
	 *  G�nderilen y�ksek de�erlikli 4 bit temizlenir.
	 *  Ard�ndan E = 1 yap�l�r ve ilk 4 bit lcd'ye g�nderilir.
	 *  Ve E = 0 yap�larak, son g�nderilen d���k de�erli 4 bit porttan silinir.
	 */

	data_u = (cmd & 0xF0);  // cmd & 0x11110000 -> 0xabcdefgh & 0x11110000 = 0xabcd0000
	data_l = ((cmd << 4) & 0xF0); // (cmd << 4) & 0x11110000 -> 0xefgh0000 & 0x11110000 = 0xefgh0000

	data_t[0] = data_u | 0x0C;             // en = 1 & rs = 0    0xabcd0000 | 0x00001100 -> 0xabcd1100
	I2C_Write(myDeviceAddress, data_t[0]);
	data_t[1] = data_u | 0x08;             // en = 0 & rs = 0    0xabcd0000 | 0x00001000 -> 0xabcd1000
	I2C_Write(myDeviceAddress, data_t[1]);
	data_t[2] = data_l | 0x0C;             // en = 1 & rs = 0    0xefgh0000 | 0x00001100 -> 0xefgh1100
	I2C_Write(myDeviceAddress, data_t[2]);
	data_t[3] = data_l | 0x08;             // en = 0 & rs = 0    0xefgh0000 | 0x00001000 -> 0xefgh1000
	I2C_Write(myDeviceAddress, data_t[3]);

}

void lcd_send_data(char data)
{
	char data_u, data_l;
	uint8_t data_t[4];

	// �ncelikli ve d���k seviyeli olarak ikiye b�l�yor
	data_u = (data & 0xF0);  // data & 0x11110000 -> 0xabcdefgh & 0x11110000 = 0xabcd0000
	data_l = ((data << 4) & 0xF0); // (data << 4) & 0x11110000 -> 0xefgh0000 & 0x11110000 = 0xefgh0000

	data_t[0] = data_u | 0x0D;               // en = 1 & rs = 1   0xabcd0000 | 0x00001101 -> 0xabcd1101   // yazma yani g�nderme yap�yoruz
	I2C_Write(myDeviceAddress, data_t[0]);
	data_t[1] = data_u | 0x09;               // en = 0 & rs = 1   0xabcd0000 | 0x00001001 -> 0xabcd1001   // silme yap�yoruz
	I2C_Write(myDeviceAddress, data_t[1]);
	data_t[2] = data_l | 0x0D;               // en = 1 & rs = 1   0xefgh0000 | 0x00001100 -> 0xefgh1101   // d���k seviyeli bit g�nderme
	I2C_Write(myDeviceAddress, data_t[2]);
	data_t[3] = data_l | 0x09;               // en = 0 & rs = 1   0xefgh0000 | 0x00001000 -> 0xefgh1001   // sonra silme
	I2C_Write(myDeviceAddress, data_t[3]);

}


void lcd_send_string(char *str)
{
	while(*str)
		lcd_send_data(*str++);

}





































