#ifndef _USR_SYSTEM_H
#define _USR_SYSTEM_H

#define _USR_UART_TOTAL_BYTES       64
#define _USR_SENSOR_UART_HANDLE_1   huart1
#define _USR_SENSOR_UART_HANDLE_2   huart2
#define _USR_LOG_UART_HANDLE        hlpuart1

#define _USR_SENSOR_POWER_PORT    GPIOA
#define _USR_SENSOR_POWER_PIN     GPIO_PIN_8

void UsrSystemInitial(void);
void UsrSystemGeneral(void);
void UsrSystemSensorUartCallback(void);

#endif