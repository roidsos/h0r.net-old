#ifndef __ERROR_H__
#define __ERROR_H__

#define HN_ERR_OUT_OF_MEM 0
#define HN_ERR_GDT_FAULT 1
#define HN_ERR_UNIMPLEMENTED 2
#define HN_ERR_ACPI_FAULT 3
#define HN_ERR_INVALID_CONF 4

extern char* Hornet_error_codes[];

#endif // __ERROR_H__
