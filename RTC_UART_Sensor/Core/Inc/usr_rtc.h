#ifndef _USR_RTC_H
#define _USR_RTC_H

#define _RTC_INIT_HANDLE hrtc

void rtc_set_time(RTC_TimeTypeDef *s_timeRtc);
char* rtc_get_time(RTC_TimeTypeDef *g_timeRtc);

void rtc_set_date(RTC_DateTypeDef *s_dateRtc);
char* rtc_get_date(RTC_DateTypeDef *g_dateRtc);

#endif