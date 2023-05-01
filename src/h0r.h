#ifndef _H0R_H
#define _H0R_H
#if __GNUC__ >= 3
#pragma GCC system_header
#endif

#include "util/colors.h"
#include "util/logger.h"
#include "util/stdint.h"
#include "lib/printf.h"
#include "drivers/memory/memory.h"
#include "drivers/keyboard.h"
#include "drivers/memory/Heap.h"
#include "drivers/filesystem/fat.h"
#include "util/logger.h"
#include "util/math.h"

#define VERSION 1

#define SUCCESS 0

#define FAILURE 2
