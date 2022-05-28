OUTPUT_FORMAT(binary)

ENTRY(protect)
INPUT
(
pm.o
kernel.o
)
OUTPUT
(
    kernel.bin
)
SECTIONS
{
    . = 0x8000;
    
    .text : ALIGN(0x512){
        *(.text)
    }
}