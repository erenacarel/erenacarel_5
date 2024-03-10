#include "usr_general.h"

_io void PeripheralInitialProc(void);

S_ACCONEER_SENSOR_PARAMETERS  g_sAcconeerParameters;

int g_distanceValue;

void UsrSystemInitial(void)
{
    PeripheralInitialProc();
    UL_AcconeerSensorInitial(&g_sAcconeerParameters);
    // UL_AcconeerSensorPeripheral(enableAcconeerSensor);
    __logse("measurement started\n");
}


void UsrSystemGeneral(void)
{
    UL_AcconeerSensorPeripheral(enableAcconeerSensor);
    for (uint8_t i = 0; i < 10; i++)
    {
        g_distanceValue = UL_AcconeerSensorGetValue(10000);  // bu da denenebilir, zaten radar modulundeki islemciden islenmis datalar geliyor
        // g_distanceValue = CalculateAcconeerSensorValueProc();
        if ((g_distanceValue != _USR_ACCONEER_SENSOR_ERROR_VALUE))
            break;
        else
            HAL_Delay(500);
    }
    UL_AcconeerSensorPeripheral(disableAcconeerSensor);
    __logsi("%d\n", g_distanceValue);
}


_io void PeripheralInitialProc(void)
{
    g_sAcconeerParameters.pUart = &_USR_SYSTEM_UART_1_CHANNEL;
    g_sAcconeerParameters.pSensorOnOffPort = ACCONEER_SENSOR_ON_OFF_GPIO_Port;
    g_sAcconeerParameters.sensorOnOffPin = ACCONEER_SENSOR_ON_OFF_Pin;
    g_sAcconeerParameters.sensorPowerStatus = GPIO_PIN_SET;

    UL_AcconeerSensorPeripheral(disableAcconeerSensor);
}


