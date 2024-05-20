#ifndef __TIME_H__
#define __TIME_H__

#define RTC_COMMAND         0x70
#define RTC_DATA            0x71
#define RTC_STATUS          0x0B

#define RTC_SECONDS         0x00
#define RTC_MINUTES         0x02
#define RTC_HOURS           0x04
#define RTC_DAY_OF_WEEK     0x06
#define RTC_DAY             0x07
#define RTC_MONTH           0x08
#define RTC_YEAR            0x09

typedef struct {
    int seconds;
    int minutes;
    int hours;
    int day_of_week;
    int day_of_month;
    int month;
    int year;
    int full[6];
} rtc_time_point;

void time_get(rtc_time_point *time);
int rtc_init();

#endif // __TIME_H__