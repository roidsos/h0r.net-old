#include <vendor/printf.h>
#include "utils/error-handling/error-screen.h"
#include "logger.h"

char current_output;

char logger_set_output(char ID){
    current_output = ID;
}

char log_error(char* format,...){
    va_list va;
    va_start(va, format);
    
    if(current_output == LOGGER_OUTPUT_VGA){
        printf(LOGGER_PREFIX_ERR);
        vprintf(format,va);
        printf("\n");
    }
    if(current_output == LOGGER_OUTPUT_DEBUG){
        dprintf(LOGGER_PREFIX_ERR);
        vdprintf(format,va);
        printf("\n");
    }
    va_end(va);
}
char log_info(char* format,...){
    va_list va;
    va_start(va, format);
    
    if(current_output == LOGGER_OUTPUT_VGA){
        printf(LOGGER_PREFIX_INFO);
        vprintf(format,va);
        printf("\n");
    }
    if(current_output == LOGGER_OUTPUT_DEBUG){
        dprintf(LOGGER_PREFIX_INFO);
        vdprintf(format,va);
        dprintf("\n");
    }
    va_end(va);
}
char log_warning(char* format,...){
    va_list va;
    va_start(va, format);
    
    if(current_output == LOGGER_OUTPUT_VGA){
        printf(LOGGER_PREFIX_WRN);
        vprintf(format,va);
        printf("\n");
    }
    if(current_output == LOGGER_OUTPUT_DEBUG){
        dprintf(LOGGER_PREFIX_WRN);
        vdprintf(format,va);
        dprintf("\n");
    }
    va_end(va);
}
//Note: log_CRITICAL causes KERNEL PANIC
char log_CRITICAL(char error_code,char* format,...){
    va_list va;
    va_start(va, format);
    
    if(current_output == LOGGER_OUTPUT_VGA){
        printf(LOGGER_PREFIX_CRITICAL);
        vprintf(format,va);
        printf("(%s)\n",Hornet_error_codes[error_code]);
    }
    if(current_output == LOGGER_OUTPUT_DEBUG){
        dprintf(LOGGER_PREFIX_CRITICAL);
        vdprintf(format,va);
        dprintf("(%s)\n",Hornet_error_codes[error_code]);
    }
    char resolve[256];
    vsnprintf_(resolve,256,format,va);

    va_end(va);
    trigger_error(error_code,resolve);
}
