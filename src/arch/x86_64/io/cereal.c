#include "cereal.h"
#include <arch/x86_64/io/portio.h>

// Util funcs
int is_transmit_empty(int port) { return in8(port + 5) & 0x20; }
int serial_received(int port) { return in8(port + 5) & 1; }

_bool init_cereal_port(int port) {
    o8(port + 1, 0x00); // Disable all interrupts
    o8(port + 3, 0x80); // Enable DLAB (set baud rate divisor)
    o8(port + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
    o8(port + 1, 0x00); //                  (hi byte)
    o8(port + 3, 0x03); // 8 bits, no parity, one stop bit
    o8(port + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
    o8(port + 4, 0x0B); // IRQs enabled, RTS/DSR set
    o8(port + 4, 0x1E); // Set in loopback mode, test the serial chip
    o8(port + 0, 0xAE); // Test serial chip (send byte 0xAE and check if
                           // serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if (in8(port + 0) != 0xAE) {
        return false;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    o8(port + 4, 0x0F);
    return true;
}

_bool cereal_init() {
    return init_cereal_port(COM1) && init_cereal_port(COM2) &&
           init_cereal_port(COM3) && init_cereal_port(COM4) &&
           init_cereal_port(COM5) && init_cereal_port(COM6) &&
           init_cereal_port(COM7) && init_cereal_port(COM8);
}

char cereal_read(int port) {
    __asm__ volatile("cli");
    while (serial_received(port) == 0)
        ;

    return in8(port);
    __asm__ volatile("sti");
}

void cereal_write(char a, int port) {
    __asm__ volatile("cli");
    while (is_transmit_empty(port) == 0)
        ;

    o8(port, a);
    __asm__ volatile("sti");
}
