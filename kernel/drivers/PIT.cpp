#include "PIT.h"
#include "io/io.h"

volatile int CountDown;
namespace PIT
{
    void PitInit()
    {
        // this code was yoinked from https://github.com/pdoane/osdev/blob/master/time/pit.c and we totally read the license!!!!!
        // uint_32 PIT_FREQUENCY  = 1193182;
        uint_32 hz = 1000;
        uint_32 divisor = PIT_FREQUENCY / hz;
        outb8(PIT_CMD, CMD_BINARY | CMD_MODE3 | CMD_RW_BOTH | CMD_COUNTER0);
        outb8(PIT_COUNTER0, divisor);
        outb8(PIT_COUNTER0, divisor >> 8);
        //  still not working?!?!?!?!?!?!??
        // just blank screen oh wait
        // we still need to fix PIT:Sleep right?
        // 
    }
/// we dont even get the interrupt for some odd reason
// what does that mean I'm dumb for this xDDD
// hmm
    void Sleep(uint_32 millis) 
    {
        CountDown = millis;
        while (CountDown > 0)
        {
            
            
        }
    }
}

extern "C" void Tick()
{
    if (CountDown != 0)
    {
        CountDown--;
    }
    print(hex2str(CountDown));
}