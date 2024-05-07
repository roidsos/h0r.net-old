#ifndef LOGGER_H
#define LOGGER_H
#include <types/stdtypes.h>
#include <config.h>

#define LOG_TRAC 0
#define LOG_DBUG 1
#define LOG_INFO 2
#define LOG_NICE 3
#define LOG_WARN 4
#define LOG_EROR 5

#if DEBUG
    #define log_debug(...) log(LOG_DBUG, __FILE__, __LINE__,__func__, __VA_ARGS__)
#else
    #define log_debug(...) // nothing lul
#endif


#if XTRA_DEBUG
    #define log_trace(...) log(LOG_TRAC, __FILE__, __LINE__,__func__, __VA_ARGS__)
#else
    #define log_trace(...) // nothing lul
#endif

#define log_info(...)  log(LOG_INFO, __FILE__, __LINE__,__func__, __VA_ARGS__)
#define log_nice(...)  log(LOG_NICE, __FILE__, __LINE__,__func__, __VA_ARGS__)
#define log_warn(...)  log(LOG_WARN, __FILE__, __LINE__,__func__, __VA_ARGS__)
#define log_error(...) log(LOG_EROR, __FILE__, __LINE__,__func__, __VA_ARGS__)

void log(uint8_t level,const char* file,uint32_t line,const char* func,char* msg,...);

#endif
