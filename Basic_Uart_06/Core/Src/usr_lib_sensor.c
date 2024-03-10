#include "usr_general.h"

#define _USR_SENSOR_DISTANCE_SAMPLE 4

_io bool m_dataOkFlag;
uint8_t m_globalRxBuffer[_USR_UART_TOTAL_BYTES];
uint8_t *ptr = m_globalRxBuffer;
bool m_distanceOkFlag;

_io S_ACCONEER_SENSOR_PARAMETERS m_sAcconeerParameter;

#define _USR_ACCONEER_SENSOR_RAW_CHANNEL      m_sAcconeerParameter.pUart->Instance
#define _USR_ACCONEER_SENSOR_POWER(x)         HAL_GPIO_WritePin(m_sAcconeerParameter.pSensorOnOffPort, m_sAcconeerParameter.sensorOnOffPin, (GPIO_PinState)x)

_io void SleepSensorGpioPinsProc(GPIO_TypeDef *f_pGpio, uint16_t f_pinGroup, GPIO_PinState f_ePinstate);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UART_Receive_IT(m_sAcconeerParameter.pUart, m_globalRxBuffer, _USR_UART_TOTAL_BYTES);
}


bool UL_AcconeerSensorInitial(S_ACCONEER_SENSOR_PARAMETERS *f_pSensor)
{
    m_sAcconeerParameter = *f_pSensor;
    _USR_ACCONEER_SENSOR_POWER(1);
    __HAL_UART_ENABLE_IT(m_sAcconeerParameter.pUart, UART_IT_IDLE);
    return true;
}


void UL_AcconeerSensorPeripheral(EAcconeerSensorControl f_eControl)
{
    if (f_eControl == enableAcconeerSensor)
    {
        // HAL_GPIO_WritePin(m_sAcconeerParameter.pSensorOnOffPort, m_sAcconeerParameter.sensorOnOffPin, (GPIO_PinState)m_sAcconeerParameter.sensorPowerStatus);
        HAL_UART_DeInit(m_sAcconeerParameter.pUart);
        _USR_ACCONEER_SENSOR_RAW_INIT_FUNC();
        _USR_ACCONEER_DELAY(100);
        _USR_ACCONEER_SENSOR_RAW_CHANNEL->ICR  = 0xFFFFFFFF;
        _USR_ACCONEER_SENSOR_RAW_CHANNEL->CR3 |= ((uint32_t)0x00000001);
        _USR_ACCONEER_SENSOR_RAW_CHANNEL->CR1 |= ((uint32_t)0x00000010);
        __HAL_UART_ENABLE_IT(m_sAcconeerParameter.pUart, UART_IT_IDLE);
        HAL_UART_Receive_IT(m_sAcconeerParameter.pUart, (uint8_t *)m_globalRxBuffer, _USR_UART_TOTAL_BYTES);
    }
    else
    {
        // HAL_GPIO_WritePin(m_sAcconeerParameter.pSensorOnOffPort, m_sAcconeerParameter.sensorOnOffPin, (GPIO_PinState)!m_sAcconeerParameter.sensorPowerStatus);
        HAL_UART_DeInit(m_sAcconeerParameter.pUart);
        SleepSensorGpioPinsProc(_USR_ACCONEER_SENSOR_TX_GPIO_PORT, _USR_ACCONEER_TX_GPIO_PIN | _USR_ACCONEER_RX_GPIO_PIN, GPIO_PIN_RESET);
    }
    _USR_ACCONEER_SENSOR_POWER(f_eControl);
}


// Timeout must be entered
int UL_AcconeerSensorGetValue(uint32_t f_timeoutMilisecond)
{
    uint32_t startTime = HAL_GetTick();
    int m_distance = 0;
    while (HAL_GetTick() - startTime < f_timeoutMilisecond)
    {
        if (m_dataOkFlag)
        {
            m_sAcconeerParameter.pUart->pRxBuffPtr = ptr;
            m_sAcconeerParameter.pUart->RxXferCount = 0;
            if (sscanf((char*)m_globalRxBuffer, "%d", &m_distance))
            {
                if (m_distance > 0)
                {
                    m_distanceOkFlag = true;
                }
                else
                {
                    m_distanceOkFlag = false;
                }
            }
            HAL_UART_Abort_IT(m_sAcconeerParameter.pUart);
            HAL_UART_Receive_IT(m_sAcconeerParameter.pUart, m_globalRxBuffer, _USR_UART_TOTAL_BYTES);
            if (m_distanceOkFlag)
            {
                m_dataOkFlag = false;
                return m_distance;
            }
            m_dataOkFlag = false;
        }
    }
    return _USR_ACCONEER_SENSOR_ERROR_VALUE;
}


int CalculateAcconeerSensorValueProc(void)
{
    uint8_t sampleCount = 0;
    int m_UltrasonicRawDistance = 0;
    int m_DistanceSum = 0;

    for (uint8_t i = 0; i < _USR_SENSOR_DISTANCE_SAMPLE; i++)
    {
        m_UltrasonicRawDistance = UL_AcconeerSensorGetValue(10000);
        // __logse("%d \n", m_UltrasonicRawDistance);
        if (m_UltrasonicRawDistance != _USR_ACCONEER_SENSOR_ERROR_VALUE)
        {
            sampleCount++;
            m_DistanceSum += m_UltrasonicRawDistance;
        }
    }

    if (sampleCount <= (_USR_SENSOR_DISTANCE_SAMPLE / 2))
        return _USR_ACCONEER_SENSOR_ERROR_VALUE;
    else 
        return ((m_DistanceSum) / sampleCount);

}



void UL_AcconeerSensorCallback(void)
{
	if (__HAL_UART_GET_FLAG(&_USR_SYSTEM_UART_1_CHANNEL, UART_FLAG_IDLE))
	{
		__HAL_UART_CLEAR_IDLEFLAG(&_USR_SYSTEM_UART_1_CHANNEL);
		m_dataOkFlag = true;
	}      
}


_io void SleepSensorGpioPinsProc(GPIO_TypeDef *f_pGpio, uint16_t f_pinGroup, GPIO_PinState f_ePinstate)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    HAL_GPIO_WritePin(f_pGpio, f_pinGroup, f_ePinstate);
    GPIO_InitStruct.Pin = f_pinGroup;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init(f_pGpio, &GPIO_InitStruct);
}










