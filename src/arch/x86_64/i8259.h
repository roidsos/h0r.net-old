#ifndef __I8259_H__
#define __I8259_H__
#include <types/stdtypes.h>
// "good artists borrow, great artists steal"
// https://github.com/nanobyte-dev/nanobyte_os/ 

void i8259_Configure(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEoi);
void i8259_Disable();
void i8259_Mask(int irq);
void i8259_Unmask(int irq);
void i8259_SendEndOfInterrupt(int irq);

uint16_t i8259_ReadIrqRequestRegister();
uint16_t i8259_ReadInServiceRegister();

#endif // __I8259_H__