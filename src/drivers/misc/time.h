#ifndef __TIME_H__
#define __TIME_H__


typedef struct {
    int seconds;
    int minutes;
    int hours;
    int day_of_week;
    int day_of_month;
    int month;
    int year;
    int full[6];
} i_time_t;

void time_get(i_time_t *time);
int rtc_init();

#endif // __TIME_H__