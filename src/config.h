#ifndef CONFIG_H
#define CONFIG_H

//kernel
#define KERNEL_VER_MAJOR 0
#define KERNEL_VER_MINOR 0
#define KERNEL_VER_PATCH 1
#define KERNEL_VER_CODENAME "Deez Nutz"

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
#define HEAP_MAGIC 0xB16B00B5B116D1CC // "Big Boobs, Big dick"(I think "Microsoftus Neckbearditis" got me...), UNUSED BTW
#define KEY_MAGIC 0xB16B00B5 // "Big Boobs"

#endif
