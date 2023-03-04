#include <util/colors.h>
#include <drivers/ACPI.h>
#include <lib/printf.h>
#include <io/io.h>

char *PREFIX_INFO = "[INFO]";
char *PREFIX_ERR = "[ERR]";
char *PREFIX_WRN = "[WRN]";
char *PREFIX_FATAL = "[FATAL]"; 

void LogINFO(char *msg){
    dprintf("%s %s",PREFIX_INFO,msg);
    print(PREFIX_INFO,LIGHT_GREEN);
    printf(msg,0xff);
}

void LogERR(char *msg){
    dprintf("%s %s",PREFIX_ERR,msg);
    print(PREFIX_ERR,LIGHT_RED);
    print(msg,0xff);
}
void LogWRN(char *msg){
    dprintf("%s %s",PREFIX_WRN,msg);
    print(PREFIX_WRN,LIGHT_YELLOW);
    print(msg,0xff);
}
void LogFATAL(char *msg){
    dprintf("%s %s",PREFIX_FATAL,msg);
    print(PREFIX_FATAL,RED);
    print(msg,0xff);
    reboot();
}