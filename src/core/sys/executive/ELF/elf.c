#include "elf.h"
#include "libk/macros.h"
#include <core/sys/resman/SIV.h>
#include <libk/endian.h>
#include <vendor/printf.h>
#include <utils/log.h>

void exec_elf(char *path,UNUSED char *procname, UNUSED _bool user) {
    elfheader64_t elf_header;
    u32 fd = siv_open(0, path,SIV_FLAGS_CACHE);
    siv_read(fd,0,(char*)&elf_header, sizeof(elfheader64_t));

    if (be32toh(elf_header.magic) != ELF_MAGIC) {
        log_error("Invalid ELF at %s: Wrong magic value(0x%x)\n", path,
                  be32toh(elf_header.magic));
        siv_close(fd);
        return;
    }
    printf("passed!!\n");
}