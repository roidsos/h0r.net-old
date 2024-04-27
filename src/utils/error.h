#ifndef ERROR_H
#define ERROR_H

#define HN_ERR_KERNEL_EXITED    0
#define HN_ERR_NO_ACPI          1

void trigger_psod(int error_code,char* details);

#endif
