#include "rtc.h"

#include <arch/x86_64/io/portio.h>

// BCD == binary coded decimal
// its like fucking around with the bits
// to make the hexadeimal digits be equal to the decimal digits
char bcd;

unsigned char read_register(unsigned char reg) {
    __asm__ volatile("cli");
    outb8(RTC_COMMAND, reg);
    return inb8(RTC_DATA);
    __asm__ volatile("sti");
}

void write_register(unsigned char reg, unsigned char value) {
    __asm__ volatile("cli");
    outb8(RTC_COMMAND, reg);
    outb8(RTC_DATA, value);
    __asm__ volatile("sti");
}

unsigned char bcd2bin(unsigned char in_bcd) {
    return (bcd) ? ((in_bcd >> 4) * 10) + (in_bcd & 0x0F) : in_bcd;
}
void time_get(rtc_time_point *target) {
    target->seconds = bcd2bin(read_register(RTC_SECONDS));
    target->minutes = bcd2bin(read_register(RTC_MINUTES));
    target->hours = bcd2bin(read_register(RTC_HOURS));
    target->day_of_week = bcd2bin(read_register(RTC_DAY_OF_WEEK));
    target->day_of_month = bcd2bin(read_register(RTC_DAY));
    target->month = bcd2bin(read_register(RTC_MONTH));
    target->year = bcd2bin(read_register(RTC_YEAR));

    target->full[0] = target->seconds;
    target->full[1] = target->minutes;
    target->full[2] = target->hours;
    target->full[3] = target->day_of_month;
    target->full[4] = target->month;
    target->full[5] = target->year;
}

int rtc_init() {
    __asm__ volatile("cli");
    unsigned char status;
    status = read_register(RTC_STATUS);
    status |= 0x02;         // 24 hour clock
    status |= 0x10;         // update ended interrupts
    status &= ~0x20;        // no alarm interrupts
    status &= ~0x40;        // no periodic interrupt
    bcd = !(status & 0x04); // check if data type is BCD
    write_register(RTC_STATUS, status);
    __asm__ volatile("sti");
    return 0;
}