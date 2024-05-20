#ifndef __STDTYPES_H__
#define __STDTYPES_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

#ifdef SUPPORT_FLOAT
    #include <float.h>
#endif

// handy macro that is needed almost as often as the types
#define UNUSED __attribute__((unused))

#endif // __STDTYPES_H__
