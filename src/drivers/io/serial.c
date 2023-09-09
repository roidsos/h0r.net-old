#include "serial.h"
#include <drivers/io/portio.h>
uint8_t ports_present;

// Util funcs
int is_transmit_empty(int port) {
   return inb8(port + 5) & 0x20;
}
int serial_received(int port) {
   return inb8(port + 5) & 1 ;
}
uint8_t fool_proof(uint16_t port){
   switch (port)
   {
   case COM1:
      return 1;
   case COM2:
      return 2;
   case COM3:
      return 3;
   case COM4:
      return 4;
   case COM5:
      return 5;
   case COM6:
      return 6;
   case COM7:
      return 7;
   case COM8:
      return 8;
   default:
      if(!(port < 9))
         return 0;
      return port;
   }
}
bool init_serial_port(int port) {
    if(serial_is_present(port))
        return true;
    outb8(port + 1, 0x00);    // Disable all interrupts
    outb8(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb8(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb8(port + 1, 0x00);    //                  (hi byte)
    outb8(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb8(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb8(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb8(port + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb8(port + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
    // Check if serial is faulty (i.e: not same byte as sent)
    if(inb8(port + 0) != 0xAE) {
       return false;
    }
 
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb8(port + 4, 0x0F);
    return true;
}

//visible funcs

void serial_init(){
   init_serial_port(COM1);
   init_serial_port(COM2);
   init_serial_port(COM3);
   init_serial_port(COM4);
   init_serial_port(COM5);
   init_serial_port(COM6);
   init_serial_port(COM7);
   init_serial_port(COM8);
}
bool serial_is_present(int port){
    return (ports_present << fool_proof(port)) & 1;
}
void serial_write_string(char* str,int port){
      while (*str) {
         serial_write(*str,port);
         str++;
      }
}
 
char serial_read(int port) {
   while (serial_received(port) == 0);
 
   return inb8(port);
}

void serial_write(char a,int port) {
   while (is_transmit_empty(port) == 0);
 
   outb8(port,a);
}