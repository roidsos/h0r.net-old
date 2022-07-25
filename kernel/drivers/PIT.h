
#pragma once
#include <stdint.h>

namespace PIT {
    extern double TimeSinceBoot;
    const uint_64 BaseFrequency = 1193182;

    void Sleepd(double seconds);
    void Sleep(uint_64 milliseconds);

    void SetDivisor(uint_16 divisor);
    uint_64 GetFrequency();
    void SetFrequency(uint_64 frequency);
    void Tick();
}
