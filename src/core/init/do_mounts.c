#include <core/VFS/vfs.h>
#include <core/logging/logger.h>

void do_mounts() {
    vfs_mount(0, "/");
    log_info("Kernel Init Target reached: VFS\n");
}