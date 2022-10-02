#ifndef __PIT_H_
#define __PIT_H_

#pragma once
#include <util/stdint.h>

namespace PIT {
extern int UptmeInMillis;
void PitInit();
void Sleep(uint_32 millis);
}

#endif