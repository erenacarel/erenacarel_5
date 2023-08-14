#include "usr_general.h"

_io uint8_t m_globalRxBuffer[_USR_UART_TOTAL_BYTES];
_io bool m_dataOkFlag = false;
_io bool m_distanceOkFlag = false;

RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UART_Receive_IT(&_USR_SENSOR_UART_HANDLE_1, m_globalRxBuffer, _USR_UART_TOTAL_BYTES);
}


void UsrSystemInitial(void)
{
    HAL_GPIO_WritePin(_USR_SENSOR_POWER_PORT, _USR_SENSOR_POWER_PIN, GPIO_PIN_SET);

    _USR_SENSOR_UART_HANDLE_1.Instance->CR3 |= (uint32_t)0x00000001;
    _USR_SENSOR_UART_HANDLE_1.Instance->CR1 |= (uint32_t)0x00000010;

    rtc_set_time(&sTime);
    rtc_set_date(&sDate);

    HAL_UART_Receive_IT(&_USR_SENSOR_UART_HANDLE_1, m_globalRxBuffer, _USR_UART_TOTAL_BYTES);
}

void UsrSystemGeneral(void)
{
    if(m_dataOkFlag)
    {
        uint16_t counter = _USR_SENSOR_UART_HANDLE_1.RxXferSize - _USR_SENSOR_UART_HANDLE_1.RxXferCount;

        m_distanceOkFlag = true;

        if((counter >= 4) && (m_distanceOkFlag == true))
        {
            uint16_t receiverChecksum = (m_globalRxBuffer[0] + m_globalRxBuffer[1] + m_globalRxBuffer[2]) & 0x00FF;
            if(receiverChecksum == m_globalRxBuffer[3])
            {
                uint16_t distance = ((uint16_t)(m_globalRxBuffer[2] << 0) | (uint16_t)(m_globalRxBuffer[1] << 8));
                
                HAL_GPIO_WritePin(_USR_SENSOR_POWER_PORT, GPIO_PIN_4, GPIO_PIN_SET);
                HAL_GPIO_WritePin(_USR_SENSOR_POWER_PORT, GPIO_PIN_5, GPIO_PIN_SET);
                HAL_GPIO_WritePin(_USR_SENSOR_POWER_PORT, GPIO_PIN_6, GPIO_PIN_SET);
                /*
                char buffer[64];
                char* time_buffer[16] = {rtc_get_time(&sTime)};
                char* date_buffer[16] = {rtc_get_date(&sDate)};
                HAL_UART_Transmit(&_USR_LOG_UART_HANDLE, (uint8_t*)buffer, sprintf(buffer, "Time: %s, Date: %s\n",(char*)time_buffer, (char*)date_buffer), 100);
                */
                char main_buffer[64];
                char time_buffer[16];
                char date_buffer[16];

                strcpy(time_buffer, rtc_get_time(&sTime));
                strcpy(date_buffer, rtc_get_date(&sDate));
                
                HAL_UART_Transmit(&_USR_LOG_UART_HANDLE, (uint8_t*)main_buffer, sprintf(main_buffer, "Distance: %d, Time: %s, Date: %s\n",distance, time_buffer, date_buffer), 100);
                
                if(distance == 0)
                {
                    m_distanceOkFlag = false;
                }
            }
            counter = 0;
        }
        
        if(m_distanceOkFlag == false)
        {
          HAL_GPIO_WritePin(_USR_SENSOR_POWER_PORT, GPIO_PIN_4, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(_USR_SENSOR_POWER_PORT, GPIO_PIN_5, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(_USR_SENSOR_POWER_PORT, GPIO_PIN_6, GPIO_PIN_RESET);
        }


        HAL_UART_Abort_IT(&_USR_SENSOR_UART_HANDLE_1);
        HAL_UART_Receive_IT(&_USR_SENSOR_UART_HANDLE_1, m_globalRxBuffer, _USR_UART_TOTAL_BYTES);
        m_dataOkFlag = false;
    } 
}


void UsrSystemSensorUartCallback(void)
{
    if(_USR_SENSOR_UART_HANDLE_1.Instance->ISR & ((uint32_t)0x1f))
    {
        if((_USR_SENSOR_UART_HANDLE_1.Instance->ISR & 0x10) == 0)
            HAL_UART_Receive_IT(&_USR_SENSOR_UART_HANDLE_1, m_globalRxBuffer, _USR_UART_TOTAL_BYTES);
        _USR_SENSOR_UART_HANDLE_1.Instance->ICR = 0x1f;
        m_dataOkFlag = true;
    }
}



