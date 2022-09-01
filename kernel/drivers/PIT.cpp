#include "PIT.h"
#include "io.h"
#include <stdint.h>
// sztem a sleepd se megy
// ujra probaljuk csinalni?
// az egészet? ja
// rev is segíthet?
// rev wanna help rewrite PIT?
// how da fuk does it even work idk /shrug
// Tick is the interrupt
// This was on osdev
//  section .text
//  global TimerIRQ
//  TimerIRQ:
//      push eax
//      mov eax, [CountDown]
//      test eax, eax
//      jz TimerDone
//      dec eax
//      mov [CountDown], eax
//  TimerDone:
//      pop eax
//      iretd
// ik its assembly
// it decrements Countdown if its not zero
// in the interrupt

volatile uint_32 g_pitTicks;




volatile int CountDown;
namespace PIT
{
    void PitInit()
    {
        // this code was yoinked from https://github.com/pdoane/osdev/blob/master/time/pit.c and we totally read the license!!!!!

        uint_32 hz = 1000;
        uint_32 divisor = PIT_FREQUENCY / hz;
        outb8(PIT_CMD, CMD_BINARY | CMD_MODE3 | CMD_RW_BOTH | CMD_COUNTER0);
        outb8(PIT_COUNTER0, divisor);
        outb8(PIT_COUNTER0, divisor >> 8);
        // 
    }


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