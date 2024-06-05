#ifndef __USER_H__
#define __USER_H__

//thanks to https://github.com/asterd-og/ZanOS/blob/main/kernel/src/sys/user.h

#define MSR_EFER  0xC0000080
#define MSR_STAR  0xC0000081
#define MSR_LSTAR 0xC0000082
#define MSR_CSTAR 0xC0000083

void user_init();

#endif // __USER_H__