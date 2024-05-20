#ifndef __STDDEF_H__
#define __STDDEF_H__

#define NULL ((void *)0)

#define offsetof(type, member) ((usize)&((type *)0)->member)

typedef unsigned long usize;
typedef unsigned long size_t;

#endif // __STDDEF_H__