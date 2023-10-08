#include "vfs.h"
#include "stdio.h"
#include "drive_mgr.h"
#include "stdbool.h"
#include "stddef.h"
#include <stdlib.h>
#include <string.h>

bool l_load_contents(struct node* node,char* path){
    if ((node->flags & ~FLAGS_ISDIR) == 0) return false;
    struct dir_report rep = iterate_dir(((struct ext_dir*)node->ext)->disk_id,path);
    ((struct ext_dir *)node->ext)->num_dirs = rep.num_entries;
    ((struct ext_dir *)node->ext)->files = rep.entries;
    node->flags |= FLAGS_LOADED;
    return true;
}

struct node first_node;

bool vfs_is_dir(char *path) {
    struct node *fileinq = vfs_inspect(path);
    if (fileinq == NULL)
        return false;
    return (fileinq->flags & FLAGS_ISDIR) >> 1;
}

bool vfs_mount(uint16_t disk_id,char* path)
{
    struct node *currnode = &first_node;
    if (strcmp(path, "/") != 0) {
        char currpath[255];
        uint8_t currpathidx = 0;
        uint32_t i = 0;
        while (true) {
            if (path[currpathidx] == '\0') {
                break;
            }
            if (currnode == NULL) {
                currnode->flags = FLAGS_PRESENT | FLAGS_ISDIR | FLAGS_LOADED;
                uint8_t currseglen = 0;
                while (currpath[currpathidx] != '/')
                {
                    currseglen++;
                }
                
                currnode->name = malloc(currseglen + 1);
                currnode->name[currseglen] = 0;
                memcpy(currnode->name,path + currpathidx + 1,currseglen - 1);
                currnode->ext = malloc(sizeof(struct ext_dir));

                ((struct ext_dir *)currnode->ext)->creation_date_unix_time = 0;
                ((struct ext_dir *)currnode->ext)->last_modified_unix_time = 0;
                ((struct ext_dir *)currnode->ext)->disk_id = 0;
                ((struct ext_dir *)currnode->ext)->owner_group = 0;
                ((struct ext_dir *)currnode->ext)->owner_user = 0;

                ((struct ext_dir *)currnode->ext)->num_dirs = 1;
                ((struct ext_dir *)currnode->ext)->files = NULL;
                currnode = ((struct ext_dir *)currnode->ext)->files;
                continue;
            }
            if (!(currnode->flags & FLAGS_ISDIR)) {
                return false;
            }
            if (!(currnode->flags & FLAGS_LOADED)) {
                l_load_contents(currnode,currpath);
                continue;
            }
            bool found = false;
            for (size_t j = 0; j < ((struct ext_dir *)currnode->ext)->num_dirs;
                 j++) {
                if (strcmp(((struct ext_dir *)currnode->ext)->files[j].name,
                           path + (currpathidx + 1)) == 0) {
                        
                    currpath[currpathidx] = '/';
                    memcpy(&currpath[currpathidx++],((struct ext_dir *)currnode->ext)->files[j].name,strlen(((struct ext_dir *)currnode->ext)->files[j].name));
                    currpathidx += strlen(((struct ext_dir *)currnode->ext)->files[j].name);

                    currnode = &((struct ext_dir *)currnode->ext)->files[j];
                    i++;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
    }else{
        currnode->flags = FLAGS_PRESENT | FLAGS_ISDIR | FLAGS_LOADED;
        currnode->name = "/";
        currnode->ext = malloc(sizeof(struct ext_dir));

        ((struct ext_dir *)currnode->ext)->creation_date_unix_time = 0;
        ((struct ext_dir *)currnode->ext)->last_modified_unix_time = 0;
        ((struct ext_dir *)currnode->ext)->disk_id = 0;
        ((struct ext_dir *)currnode->ext)->owner_group = 0;
        ((struct ext_dir *)currnode->ext)->owner_user = 0;

        ((struct ext_dir *)currnode->ext)->num_dirs = 1;
        ((struct ext_dir *)currnode->ext)->files = NULL;
    }
    currnode->flags |= FLAGS_MOUNT_POINT;
    struct dir_report rep = iterate_dir(disk_id,"/");
    ((struct ext_dir *)currnode->ext)->num_dirs = rep.num_entries;
    ((struct ext_dir *)currnode->ext)->files = rep.entries;
    
    return true;
}
struct file_buffer vfs_read(char* path,uint64_t offset,uint64_t size){
    struct node* file = vfs_inspect(path);
    return read_file(((struct ext_file *)file->ext)->disk_id,(char*)((struct ext_file *)file->ext)->contents_on_disk_path,offset,size);
}
void vfs_write(char* path,struct file_buffer buf,uint64_t offset){
    struct node* file = vfs_inspect(path);
    write_file(((struct ext_file *)file->ext)->disk_id,(char*)((struct ext_file *)file->ext)->contents_on_disk_path,offset,buf);
}

struct node *vfs_inspect(char *path) {
    if (strcmp(path, "/") == 0) {
        return &first_node;
    }

    struct node *currnode = &first_node;
    char currpath[255];
    uint8_t currpathidx = 0;
    uint32_t i = 0;
    while (true) {
        if (path[currpathidx] == '\0') {
            return currnode;
        }
        if (currnode == NULL) {
            return NULL;
        }
        if (!(currnode->flags & FLAGS_ISDIR)) {
            return NULL;
        }
        if (!(currnode->flags & FLAGS_LOADED)) {
            l_load_contents(currnode, currpath);
            continue;
        }
        bool found = false;
        for (size_t j = 0;((struct ext_dir *)currnode->ext)->files[j].flags; j++) {
            if (strcmp(((struct ext_dir *)currnode->ext)->files[j].name, path + (currpathidx + 1)) == 0) {
                currpath[currpathidx] = '/';
                memcpy(&currpath[currpathidx + 1], ((struct ext_dir *)currnode->ext)->files[j].name, strlen(((struct ext_dir *)currnode->ext)->files[j].name) + 1);
                currpathidx += strlen(((struct ext_dir *)currnode->ext)->files[j].name) + 1;
                i += strlen(((struct ext_dir *)currnode->ext)->files[j].name);

                currnode = &((struct ext_dir *)currnode->ext)->files[j];
                found = true;
                break;
            }
        }
        if (!found) {
            return NULL;
        }
    }
}

struct dir_report vfs_iterate_dir(char* path)
{
    struct node* file = vfs_inspect(path);
    return (struct dir_report){((struct ext_dir*)file->ext)->num_dirs,((struct ext_dir*)file->ext)->files};
}