#include "usr_general.h"
#include "usr_rtc.h"

void rtc_set_time(RTC_TimeTypeDef *s_timeRtc)
{
    s_timeRtc->Hours = 10;
    s_timeRtc->Minutes = 0;
    s_timeRtc->Seconds = 0;

    HAL_RTC_SetTime(&_RTC_INIT_HANDLE, s_timeRtc, RTC_FORMAT_BIN);
}


char* rtc_get_time(RTC_TimeTypeDef *g_timeRtc)
{
    HAL_RTC_GetTime(&_RTC_INIT_HANDLE, g_timeRtc, RTC_FORMAT_BIN);

    uint8_t hour = g_timeRtc->Hours;
    uint8_t minute = g_timeRtc->Minutes;
    uint8_t second = g_timeRtc->Seconds;

    //char* text_1 = malloc(16*sizeof(char));
    char text_1[16];
    
    sprintf(text_1, "%d: %d: %d\n", hour, minute, second);
    
    return text_1;
}

void rtc_set_date(RTC_DateTypeDef *s_dateRtc)
{
    s_dateRtc->Date = 14;
    s_dateRtc->Month = RTC_MONTH_JUNE;
    s_dateRtc->Year = 23;

    HAL_RTC_SetDate(&_RTC_INIT_HANDLE, s_dateRtc, RTC_FORMAT_BIN);
}


char* rtc_get_date(RTC_DateTypeDef *g_dateRtc)
{
    HAL_RTC_GetDate(&_RTC_INIT_HANDLE, g_dateRtc, RTC_FORMAT_BIN);

    uint8_t day = g_dateRtc->Date;
    uint8_t month = g_dateRtc->Month;
    uint8_t year = g_dateRtc->Year;

    //char* text_2 = malloc(16*sizeof(char));
    char text_2[16];
    
    sprintf(text_2, "%d: %d: 20%d\n", day, month, year);

    return text_2;
}


