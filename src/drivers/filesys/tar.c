#include <VFS/drive_mgr.h>
#include <klibc/memory.h>
#include <kernel.h>
#include <parsing/tar.h>
#include <types/string.h>
#include <types/vector.h>
#include <utils/heapdef.h>

static int disk_id;

struct node tar_inspect(char *path) {
    struct tar_header *header = find_file(&data.initramfs, path);
    struct node node_;
    size_t pre_latest_slash = 0;
    size_t latest_slash = 0;
    for (size_t i = 0; path[i]; i++) {
        if(path[i] == '/'){
            pre_latest_slash = latest_slash;
            latest_slash = i;  
        }
    }
    if(strlen(path + latest_slash + 1) == 0){
        node_.name = malloc(strlen(path + pre_latest_slash));
        strcpy(node_.name,path + pre_latest_slash);
        node_.name[strlen(node_.name) - 1] = 0;
    }else
    {
        node_.name = malloc(strlen(path + latest_slash + 1));
        strcpy(node_.name,path + latest_slash + 1);
    }
    

    if (header->typeflag == 5) {
        node_.flags = FLAGS_ISDIR | FLAGS_PRESENT;
        node_.ext.ext_dir.owner_group = parse_size(header->GID);
        node_.ext.ext_dir.owner_user = parse_size(header->UID);
        node_.ext.ext_dir.last_modified_unix_time =
            parse_size(header->mtime);
        node_.ext.ext_dir.creation_date_unix_time =
            parse_size(header->mtime);
        node_.ext.ext_dir.disk_id = disk_id;

        node_.ext.ext_dir.files = NULL; // we dont want to load the contents

    } else {
        node_.flags = FLAGS_PRESENT;
        node_.ext.ext_file.owner_group = parse_size(header->GID);
        node_.ext.ext_file.owner_user = parse_size(header->UID);
        node_.ext.ext_file.last_modified_unix_time =
            parse_size(header->mtime);
        node_.ext.ext_file.creation_date_unix_time =
            parse_size(header->mtime);
            node_.ext.ext_file.size =
            parse_size(header->size);
        node_.ext.ext_file.contents_on_disk_path = (char*)header; 
        node_.ext.ext_file.disk_id = disk_id;
    }
    return node_;
}
struct file_buffer tar_read_file(char *path, uint64_t offset, uint64_t size) {
    struct tar_header *thdr = (struct tar_header*)path;
    size_t fsize = parse_size(thdr->size);
    return (struct file_buffer){((char*)thdr) + 512 + offset,size > fsize? fsize : size};
}

bool tar_write_file(__attribute__((unused)) char* path,__attribute__((unused)) uint64_t offset,__attribute__((unused)) struct file_buffer buf)
{
    return 0;
}
bool contains(char* str,char ch){
    for (size_t i = 0; str[i]; i++)
    {
        if(str[i] == ch)
            return true;
    }
    return false;
    
}

struct  dir_report tar_iterate_dir(char *path) {
    vector_static(struct node, nodes);
    vector_init(&nodes);
    
    char* path2 = malloc(strlen(path) + 2);
    strcpy(path2,path);
    if(strcmp(path,"/") != 0){
        path2[strlen(path2)] = '/';
        path2[strlen(path2) + 1] = 0;
    }
    for (size_t i = 0; i < data.initramfs.hdr_num; i++) {
        struct tar_header *hdr = data.initramfs.headers[i];
        if (strncmp(path2 + 1, (char *)hdr->filename, strlen(path2 + 1)) == 0 &&
            !(strcmp(path2 + 1, (char *)hdr->filename) == 0) &&
            (!contains((char*)hdr->filename + strlen(path2),'/') || hdr->filename[strlen((char*)hdr->filename) - 1] == '/')
            ) { // check if starts with the
                                                    // path but not the same
            vector_push_back(&nodes,
                             tar_inspect((char *)hdr->filename));
        }
    }
    return (struct dir_report){nodes.len,nodes.data};
}

void tar_init() {
    struct fs_driver driver = {true, tar_inspect, tar_iterate_dir,tar_read_file,tar_write_file};

    disk_id = fs_driver_register(driver);
}