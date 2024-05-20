#ifndef __CEREAL_H__
#define __CEREAL_H__

#include <libk/stdtypes.h>

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8
#define COM5 0x5F8
#define COM6 0x4F8
#define COM7 0x5E8
#define COM8 0x4E8 

char cereal_read(int port);
void cereal_write(char a,int port);
bool cereal_init();
#endif // __SERIAL_H__
