#ifndef CONFIG_H
#define CONFIG_H

//version
#define KERNEL_VER_MAJOR 0
#define KERNEL_VER_MINOR 0
#define KERNEL_VER_PATCH 1
#define KERNEL_VER_CODENAME "Deez Nutz"

//general
#define CHAR_WIDTH 8
#define PAGE_SIZE 4096
#define STACK_SIZE 4 // In PAGES

//VFS
#define MAX_OPEN_FILES 64

//Devman
#define MAX_DEVICES 64

//Scheduler 
#define MAX_PROCESSES 64

//Loggers-poggers
#define DEBUG true
#define XTRA_DEBUG true

#define TRAC_PORT COM1
#define DBUG_PORT COM1
#define INFO_PORT COM1
#define NICE_PORT COM1
#define WARN_PORT COM1
#define EROR_PORT COM1

//Magic values
#define REG_HIVE_MAGIC 0xB16B00B5 // "Big Boobs", "(I think "Microsoftus Neckbearditis" got me...)
#define REG_KEY_MAGIC 0x69420666 // "69 420 666"

#endif
