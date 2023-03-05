#include <util/colors.h>
#include <drivers/ACPI.h>
#include <lib/printf.h>
#include <io/io.h>

char *PREFIX_INFO   =  "[INFO] ";
char *PREFIX_ERR    =  "[ERROR] ";
char *PREFIX_WRN    =  "[WARN] ";
char *PREFIX_FATAL  =  "[FATAL] "; 

void LogINFO(char *msg,...){
    va_list va;
    va_start(va, msg);
    
    dprintf(PREFIX_INFO);
    vdprintf(msg,va);

    printf(PREFIX_INFO);
    vprintf(msg,va);
    va_end(va);
}

void LogERR(char *msg,...){
    va_list va;
    va_start(va, msg);

    dprintf(PREFIX_ERR);
    vdprintf(msg,va);

    printf(PREFIX_ERR);
    vprintf(msg,va);
    va_end(va);
}
void LogWRN(char *msg,...){
    va_list va;
    va_start(va, msg);

    dprintf(PREFIX_WRN);
    vdprintf(msg,va);

    printf(PREFIX_WRN);
    vprintf(msg,va);
    va_end(va);
}
void LogFATAL(char *msg,...){
    va_list va;
    va_start(va, msg);

    dprintf(PREFIX_FATAL);
    vdprintf(msg,va);

    printf(PREFIX_FATAL);
    vprintf(msg,va);
    va_end(va);

    //reboot cuz its fatal
    reboot();
}