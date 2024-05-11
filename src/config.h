#ifndef CONFIG_H
#define CONFIG_H

//kernel
#define KERNEL_VER_MAJOR 0
#define KERNEL_VER_MINOR 0
#define KERNEL_VER_PATCH 1
#define KERNEL_VER_CODENAME "Deez Nutz"

//TTY
#define MAX_TTYS 4
#define MAX_LOCKED_PROCS 64

//VFS
#define MAX_OPEN_FILES 64
#define MAX_DRIVES     64
#define MAX_DRIVERS    8

//MM
#define PAGE_SIZE 4096
#define HEAP_SIZE_IN_PAGES 65536

//Loggers-poggers
#define DEBUG true
#define XTRA_DEBUG true

#define TRAC_PORT COM1
#define DBUG_PORT COM1
#define INFO_PORT COM1
#define NICE_PORT COM1
#define WARN_PORT COM1
#define EROR_PORT COM1

//Registery
#define HIVE_MAGIC 0xB16B00B569420666 // "Big Boobs, 69 420 666", "(I think "Microsoftus Neckbearditis" got me...), UNUSED BTW
#define KEY_MAGIC 0xB16B00B5 // "Big Boobs"

//Scheduler 
#define MAX_PROCESSES 64

#endif
