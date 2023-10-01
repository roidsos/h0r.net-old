#include <parsing/tar.h>
#include <VFS/drive_manager.h>
#include <types/string.h>
#include <types/vector.h>
#include <kernel.h>
#include <utils/heapdef.h>
#include <arch/x86/essential.h>

struct node* tar_get_dir_contents  (char* path);

struct node* tar_get_file_header   (char* path){
    struct tar_header* header = find_file(&data.initramfs,(uint8_t *)path);
    struct node* node_ = malloc(sizeof(struct node));

    char** splitpath = split(path,'/');
    for (size_t i = 0; splitpath[i]; i++)
    {
        node_->name = splitpath[i];
    }
    
    if(strcmp((char*)header->mode,"0000755")){
        node_->flags = FLAGS_ISDIR | FLAGS_PRESENT;
        ((struct ext_dir*)node_->ext)->owner_group = parse_size(header->GID);
        ((struct ext_dir*)node_->ext)->owner_user  = parse_size(header->UID);
        ((struct ext_dir*)node_->ext)->last_modified_unix_time = parse_size(header->mtime);
        ((struct ext_dir*)node_->ext)->creation_date_unix_time = parse_size(header->mtime);
        ((struct ext_dir*)node_->ext)->disk_id = 0;

        ((struct ext_dir*)node_->ext)->files = tar_get_dir_contents(path);

    }else{
        node_->flags = FLAGS_PRESENT;
        ((struct ext_file*)node_->ext)->owner_group = parse_size(header->GID);
        ((struct ext_file*)node_->ext)->owner_user  = parse_size(header->UID);
        ((struct ext_file*)node_->ext)->last_modified_unix_time = parse_size(header->mtime);
        ((struct ext_file*)node_->ext)->creation_date_unix_time = parse_size(header->mtime);
        ((struct ext_file*)node_->ext)->contents_on_disk_ptr = 0; // unused since tar is not a disk
        ((struct ext_file*)node_->ext)->disk_id = 0;
    }
    return node_;
}
struct node* tar_get_dir_contents  (char* path){
    vector_static(struct node,nodes);        
   vector_init(&nodes);

    for (size_t i = 0; i < data.initramfs.hdr_num; i++) {
        struct tar_header *hdr = data.initramfs.headers[i];
        if(strncmp(path, (char*)hdr->filename, strlen(path)) && !strcmp(path,(char*)hdr->filename)){ // check if starts with the path but not the same
            vector_push_back(&nodes,*tar_get_file_header((char*)hdr->filename));
        }
    }
    
    struct node zero;
    zero.flags = 0;    
    vector_push_back(&nodes,zero);

    return nodes.data;
}
void* tar_get_file_contents (char* path){

    struct node* header = tar_get_file_header(path);
    return  (char *)header + 512;
}

void tar_init()
{
    struct fs_driver driver = {true,tar_get_file_header,tar_get_dir_contents,tar_get_file_contents};
    
    fs_driver_register(driver);

}
 