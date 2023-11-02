#ifndef LOGGER_H
#define LOGGER_H
#include "core/error-handling/error.h"
#include "core/error-handling/error-screen.h"

#define LOGGER_OUTPUT_VGA    0
#define LOGGER_OUTPUT_DEBUG  1
#define LOGGER_OUTPUT_COM1 0x3F8
#define LOGGER_OUTPUT_COM2 0x2F8
#define LOGGER_OUTPUT_COM3 0x3E8
#define LOGGER_OUTPUT_COM4 0x2E8
#define LOGGER_OUTPUT_COM5 0x5F8
#define LOGGER_OUTPUT_COM6 0x4F8
#define LOGGER_OUTPUT_COM7 0x5E8
#define LOGGER_OUTPUT_COM8 0x4E8 

#define LOGGER_PREFIX_INFO         "[ INFO ]"
#define LOGGER_PREFIX_DEBUG        "[ DEBU ]"
#define LOGGER_PREFIX_ERR          "[ FUCK ]"
#define LOGGER_PREFIX_WRN          "[ WARN ]"
#define LOGGER_PREFIX_CRITICAL     "[ CRIT ]"

void logger_set_output(uint16_t ID);

void log_error(char* format,...);
void log_info(char* format,...);
void log_warning(char* format,...);

//Note: log_CRITICAL causes KERNEL PANIC
void log_CRITICAL(Registers* regs,uint8_t error_code,char* format,...);
#endif
