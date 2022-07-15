
#pragma once
#include <stdint.h>

namespace PIT {
    extern uint_64 tickssincestart;
    const uint_64 BaseFrequency = 1193182;

    void Sleep(uint_64 miliseconds);

    uint_64 GetFrequency();
    void SetDivisor(uint_16 Divisor);
    void SetFrequency(uint_64 Frequency);
}