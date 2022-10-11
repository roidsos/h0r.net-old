#include <util/colors.h>
#include <io/io.h>

char *PREFIX_INFO = "[INFO]";
char *PREFIX_ERR = "[ERR]";
char *PREFIX_WRN = "[WRN]";
char *PREFIX_FATAL = "[FATAL]"; 

void LogINFO(char *msg){
    print(PREFIX_INFO,LIGHT_GREEN);
    print(msg,0xff);
}

void LogERR(char *msg){
    print(PREFIX_ERR,LIGHT_RED);
    print(msg,0xff);
}
void LogWRN(char *msg){
    print(PREFIX_WRN,LIGHT_YELLOW);
    print(msg,0xff);
}
void LogFATAL(char *msg){
    print(PREFIX_FATAL,RED);
    print(msg,0xff);
}