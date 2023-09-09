#ifndef LOGGER_H
#define LOGGER_H
#include "utils/error-handling/error.h"
#include "utils/error-handling/error-screen.h"

#define LOGGER_OUTPUT_VGA    0
#define LOGGER_OUTPUT_DEBUG  1
#define LOGGER_OUTPUT_SERIAL 2

#define LOGGER_PREFIX_INFO         "[INFO=]"
#define LOGGER_PREFIX_ERR          "[ERROR]"
#define LOGGER_PREFIX_WRN          "[WARN=]"
#define LOGGER_PREFIX_CRITICAL     "[FATAL]"
void logger_set_output(char ID);

void log_error(char* format,...);
void log_info(char* format,...);
void log_warning(char* format,...);
//Note: log_CRITICAL causes KERNEL PANIC
void log_CRITICAL(Registers* regs,uint8_t error_code,char* format,...);
#endif
