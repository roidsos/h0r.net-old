#ifndef LOGGER_H
#define LOGGER_H

#define LOGGER_OUTPUT_VGA    0
#define LOGGER_OUTPUT_DEBUG  1
#define LOGGER_OUTPUT_SERIAL 2

char logger_set_output(char ID);

char log_error(char* format,...);
char log_info(char* format,...);
char log_warning(char* format,...);
//Note: log_CRITICAL causes KERNEL PANIC
char log_CRITICAL(char error_code,char* format,...);
#endif
