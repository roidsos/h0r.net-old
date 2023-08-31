#ifndef __PAGING_H__
#define __PAGING_H__

#include <stdint.h>

struct PageDIREntry
{
    bool present : 1;
    bool RW : 1;
    bool user_super : 1;
    bool present : 1;
    bool no_cash : 1;// :(
    bool access : 1; 
};
//TODO: continue at https://youtu.be/e47SApmmx44?list=PLxN4E629pPnJxCQCLy7E0SQY_zuumOVyZ&t=493

#endif // __PAGING_H__