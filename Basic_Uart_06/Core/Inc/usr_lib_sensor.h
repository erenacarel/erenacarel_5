#ifndef __USR_LIB_SENSOR_H
#define __USR_LIB_SENSOR_H

#include "usr_general.h"

#define _USR_UART_TOTAL_BYTES 512

#define _USR_ACCONEER_SENSOR_RAW_INIT_FUNC    MX_USART1_UART_Init
#define _USR_ACCONEER_DELAY                   HAL_Delay

#define _USR_ACCONEER_SENSOR_TX_GPIO_PORT     TX_SENSOR_GPIO_Port
#define _USR_ACCONEER_TX_GPIO_PIN             TX_SENSOR_Pin
#define _USR_ACCONEER_RX_GPIO_PIN             RX_SENSOR_Pin

#define _USR_ACCONEER_SENSOR_POWER(x) (x ? (_USR_ACCONEER_SENSOR_TX_GPIO_PORT->BSRR = ACCONEER_SENSOR_ON_OFF_Pin) : (_USR_ACCONEER_SENSOR_TX_GPIO_PORT->BRR = ACCONEER_SENSOR_ON_OFF_Pin))

#define _USR_ACCONEER_SENSOR_ERROR_VALUE        (int)-100

typedef struct S_ACCONEER_SENSOR_PARAMETERS_TAG
{
    UART_HandleTypeDef *pUart;
    GPIO_TypeDef *pSensorOnOffPort;
    int sensorOnOffPin;
    uint8_t sensorPowerStatus;

}S_ACCONEER_SENSOR_PARAMETERS;


typedef enum 
{
    disableAcconeerSensor,
    enableAcconeerSensor,
}EAcconeerSensorControl;


void UL_AcconeerSensorCallback(void);
int UL_AcconeerSensorGetValue(uint32_t f_timeoutMilisecond);
bool UL_AcconeerSensorInitial(S_ACCONEER_SENSOR_PARAMETERS *f_pSensor);
void UL_AcconeerSensorPeripheral(EAcconeerSensorControl f_eControl);
int CalculateAcconeerSensorValueProc(void);

#endif