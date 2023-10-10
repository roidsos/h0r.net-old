#include <VFS/drive_mgr.h>
#include <klibc/memory.h>
#include <kernel.h>
#include <parsing/tar.h>
#include <types/string.h>
#include <types/vector.h>
#include <utils/heapdef.h>

struct file_buffer tar_read_file(char *path,__attribute__((unused)) uint64_t offset,__attribute__((unused)) uint64_t size) { // ignore offset and size cuz fuck them 
    if(strcmp((char*)path,"/hello.txt") == 0){
        return (struct file_buffer){"Hello World!",13};
    }
    return (struct file_buffer){0,0};
}

bool tar_write_file(__attribute__((unused)) char* path,__attribute__((unused)) uint64_t offset,__attribute__((unused)) struct file_buffer buf)
{
    return 0;
}
struct  dir_report tar_iterate_dir(char *path) {
    if(strcmp((char*)path,"/") == 0){
        struct ext_file* ext = malloc(sizeof(struct ext_file));
        ext->disk_id = 0;
        ext->contents_on_disk_path = malloc(11);
        memcpy(ext->contents_on_disk_path,"/hello.txt",11);
        struct node* nodes = malloc(sizeof(struct node));
        nodes[0].name = "hello.txt";
        nodes[0].flags = FLAGS_PRESENT;
        nodes[0].ext = (uint8_t*)ext;
        return (struct dir_report){1,nodes};
    }else{
        return (struct dir_report){0,0};
    }
}

struct node *tar_inspect(char *path) {
    if(strcmp((char*)path,"/") == 0){
        struct ext_file* ext = malloc(sizeof(struct ext_file));
        ext->disk_id = 0;
        ext->contents_on_disk_path = malloc(11);
        memcpy(ext->contents_on_disk_path,"/hello.txt",11);
        struct node* nodes = malloc(sizeof(struct node));
        nodes[0].name = "hello.txt";
        nodes[0].flags = FLAGS_PRESENT;
        nodes[0].ext = (uint8_t*)ext;
        return nodes;
    }
    return 0;
}

void tar_init() {
    struct fs_driver driver = {true, tar_inspect, tar_iterate_dir,tar_read_file,tar_write_file};

    fs_driver_register(driver);
}