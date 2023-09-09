#include <drivers/io/portio.h>
#include <stdbool.h>

// "good artists borrow, great artists steal"
// https://github.com/nanobyte-dev/nanobyte_os/

#define PIC1_COMMAND_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_COMMAND_PORT 0xA0
#define PIC2_DATA_PORT 0xA1

// Initialization Control Word 1
// -----------------------------
//  0   IC4     if set, the PIC expects to receive ICW4 during initialization
//  1   SGNL    if set, only 1 PIC in the system; if unset, the PIC is cascaded
//  with slave PICs
//              and ICW3 must be sent to controller
//  2   ADI     call address interval, set: 4, not set: 8; ignored on x86, set
//  to 0 3   LTIM    if set, operate in level triggered mode; if unset, operate
//  in edge triggered mode 4   INIT    set to 1 to initialize PIC 5-7 ignored on
//  x86, set to 0

enum {
    PIC_ICW1_ICW4 = 0x01,
    PIC_ICW1_SINGLE = 0x02,
    PIC_ICW1_INTERVAL4 = 0x04,
    PIC_ICW1_LEVEL = 0x08,
    PIC_ICW1_INITIALIZE = 0x10
} PIC_ICW1;

// Initialization Control Word 4
// -----------------------------
//  0   uPM     if set, PIC is in 80x86 mode; if cleared, in MCS-80/85 mode
//  1   AEOI    if set, on last interrupt acknowledge pulse, controller
//  automatically performs
//              end of interrupt operation
//  2   M/S     only use if BUF is set; if set, selects buffer master;
//  otherwise, selects buffer slave 3   BUF     if set, controller operates in
//  buffered mode 4   SFNM    specially fully nested mode; used in systems with
//  large number of cascaded controllers 5-7         reserved, set to 0
enum {
    PIC_ICW4_8086 = 0x1,
    PIC_ICW4_AUTO_EOI = 0x2,
    PIC_ICW4_BUFFER_MASTER = 0x4,
    PIC_ICW4_BUFFER_SLAVE = 0x0,
    PIC_ICW4_BUFFERRED = 0x8,
    PIC_ICW4_SFNM = 0x10,
} PIC_ICW4;

enum {
    PIC_CMD_END_OF_INTERRUPT = 0x20,
    PIC_CMD_READ_IRR = 0x0A,
    PIC_CMD_READ_ISR = 0x0B,
} PIC_CMD;

static uint16_t g_PicMask = 0xffff;

void i8259_SetMask(uint16_t newMask) {
    g_PicMask = newMask;
    outb8(PIC1_DATA_PORT, g_PicMask & 0xFF);
    iowait();
    outb8(PIC2_DATA_PORT, g_PicMask >> 8);
    iowait();
}

uint16_t i8259_GetMask() {
    return inb8(PIC1_DATA_PORT) | (inb8(PIC2_DATA_PORT) << 8);
}

void i8259_Configure(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEoi) {
    // Mask everything
    i8259_SetMask(0xFFFF);

    // initialization control word 1
    outb8(PIC1_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    iowait();
    outb8(PIC2_COMMAND_PORT, PIC_ICW1_ICW4 | PIC_ICW1_INITIALIZE);
    iowait();

    // initialization control word 2 - the offsets
    outb8(PIC1_DATA_PORT, offsetPic1);
    iowait();
    outb8(PIC2_DATA_PORT, offsetPic2);
    iowait();

    // initialization control word 3
    outb8(PIC1_DATA_PORT,
          0x4); // tell PIC1 that it has a slave at IRQ2 (0000 0100)
    iowait();
    outb8(PIC2_DATA_PORT, 0x2); // tell PIC2 its cascade identity (0000 0010)
    iowait();

    // initialization control word 4
    uint8_t icw4 = PIC_ICW4_8086;
    if (autoEoi) {
        icw4 |= PIC_ICW4_AUTO_EOI;
    }

    outb8(PIC1_DATA_PORT, icw4);
    iowait();
    outb8(PIC2_DATA_PORT, icw4);
    iowait();

    // mask all interrupts until they are enabled by the device driver
    i8259_SetMask(0xFFFF);
}

void i8259_SendEndOfInterrupt(int irq) {
    if (irq >= 8)
        outb8(PIC2_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT);
    outb8(PIC1_COMMAND_PORT, PIC_CMD_END_OF_INTERRUPT);
}

void i8259_Disable() { i8259_SetMask(0xFFFF); }

void i8259_Mask(int irq) { i8259_SetMask(g_PicMask | (1 << irq)); }

void i8259_Unmask(int irq) { i8259_SetMask(g_PicMask & ~(1 << irq)); }

uint16_t i8259_ReadIrqRequestRegister() {
    outb8(PIC1_COMMAND_PORT, PIC_CMD_READ_IRR);
    outb8(PIC2_COMMAND_PORT, PIC_CMD_READ_IRR);
    return ((uint16_t)inb8(PIC2_COMMAND_PORT)) |
           (((uint16_t)inb8(PIC2_COMMAND_PORT)) << 8);
}

uint16_t i8259_ReadInServiceRegister() {
    outb8(PIC1_COMMAND_PORT, PIC_CMD_READ_ISR);
    outb8(PIC2_COMMAND_PORT, PIC_CMD_READ_ISR);
    return ((uint16_t)inb8(PIC2_COMMAND_PORT)) |
           (((uint16_t)inb8(PIC2_COMMAND_PORT)) << 8);
}
