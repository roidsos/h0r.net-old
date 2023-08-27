#include "logger.h"
#include <vendor/printf.h>

char current_output;

char *PREFIX_INFO     =  "[INFO=]"
char *PREFIX_ERR      =  "[ERROR]"
char *PREFIX_WRN      =  "[WARN=]"
char *PREFIX_CRITICAL =  "[FATAL]"


char logger_set_output(char ID){
    current_output = ID;
}

char log_error(char* format,...){
    va_list va;
    va_start(va, msg);
    
    if(current_output == LOGGER_OUTPUT_VGA)
        printf_(PREFIX_ERR);
        vprintf_(msg,va);

    if(current_output == LOGGER_OUTPUT_DEBUG)
        dprintf_(PREFIX_ERR);
        vdprintf_(msg,va);

    va_end(va);
}
char log_info(char* format,...){
    va_list va;
    va_start(va, msg);
    
    if(current_output == LOGGER_OUTPUT_VGA)
        printf_(PREFIX_INFO);
        vprintf_(msg,va);

    if(current_output == LOGGER_OUTPUT_DEBUG)
        dprintf_(PREFIX_INFO);
        vdprintf_(msg,va);

    va_end(va);
}
char log_warning(char* format,...){
    va_list va;
    va_start(va, msg);
    
    if(current_output == LOGGER_OUTPUT_VGN)
        printf_(PREFIX_WAN);
        vprintf_(msg,va);

    if(current_output == LOGGER_OUTPUT_DEBUG)
        dprintf_(PREFIX_WRN);
        vdprintf_(msg,va);

    va_end(va);
}
//Note: log_CRITICAL causes KERNEL PANIC
char log_CRITICAL(char error_code,char* format,...){
    va_list va;
    va_start(va, msg);
    
    if(current_output == LOGGER_OUTPUT_VGA)
        printf_(PREFIX_CRITICAL);
        vprintf_(msg,va);

    if(current_output == LOGGER_OUTPUT_DEBUG)
        dprintf_(PREFIX_CRITICAL);
        vdprintf_(msg,va);

    va_end(va);
}
