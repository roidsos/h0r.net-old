#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <io/io.h>

void LogINFO (char *msg);
void LogWRN (char *msg);
void LogERR (char *msg);
void LogFATAL (char *msg);

#endif // __LOGGER_H__