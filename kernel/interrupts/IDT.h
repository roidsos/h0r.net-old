#include <stdint.h>
struct IDT64{
uint_16 offset_low;
uint_16 selector;
uint_8  ist;
uint_8  type_attr;
uint_16 offset_mid;
uint_32 offset_high;
uint_32 zero;
};
void initidt();