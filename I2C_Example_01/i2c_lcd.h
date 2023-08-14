/*
 * i2c_lcd.h
 *
 *  Created on: 3 Nis 2022
 *      Author: ERENACAREL
 */

#include "stm32f4xx.h"

#ifndef I2C_LCD_H_
#define I2C_LCD_H_

void lcd_init(void);             // initialize lcd 						// ilk kullanýma hazýr hale getirdik
void lcd_send_cmd(char cmd);     // send command to the lcd				// ekraný temizlemek satýr ve sütunlara gitmek gibi belli komutlar
void lcd_send_data(char data);   // send data to the lcd
void lcd_send_string(char *str); // send string to the lcd				// yazýlan string sürekli lcd_send_data tarafýna gönderilecek.

#endif /* I2C_LCD_H_ */
