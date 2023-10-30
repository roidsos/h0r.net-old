#include "vfs.h"
#include "drive_mgr.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>

bool vfs_load_contents(struct node *node, char *path) {
    struct dir_report rep = iterate_dir(node->ext.ext_dir.disk_id, path);
    node->ext.ext_dir.num_dirs = rep.num_entries;
    node->ext.ext_dir.files = rep.entries;
    node->flags |= FLAGS_LOADED;
    return true;
}

struct node first_node;

bool vfs_is_dir(char *path) {
    struct node *fileinq = vfs_inspect(path);
    if (fileinq == NULL)
        return false;
    return fileinq->flags & FLAGS_ISDIR;
}

bool vfs_mount(uint16_t disk_id, char *path) {
    struct node *currnode = &first_node;
    if (strcmp(path, "/") != 0) {
        char currpath[255];
        uint8_t currpathidx = 0;
        while (true) {
            if (path[currpathidx] == '\0') {
                break;
            }
            if (currnode == NULL) {
                currnode->flags = FLAGS_PRESENT | FLAGS_ISDIR | FLAGS_LOADED;
                uint8_t currseglen = 0;
                while (currpath[currpathidx] != '/') {
                    currseglen++;
                }

                currnode->name = malloc(currseglen + 1);
                currnode->name[currseglen] = 0;
                memcpy(currnode->name, path + currpathidx + 1, currseglen - 1);

                currnode->ext.ext_dir.creation_date_unix_time = 0;
                currnode->ext.ext_dir.last_modified_unix_time = 0;
                currnode->ext.ext_dir.disk_id = 0;
                currnode->ext.ext_dir.owner_group = 0;
                currnode->ext.ext_dir.owner_user = 0;

                currnode->ext.ext_dir.num_dirs = 1;
                currnode->ext.ext_dir.files = NULL;
                currnode = currnode->ext.ext_dir.files;
                continue;
            }
            if (!(currnode->flags & FLAGS_ISDIR)) {
                return false;
            }
            if (!(currnode->flags & FLAGS_LOADED)) {
                vfs_load_contents(currnode, currpath);
                continue;
            }
            bool found = false;
            uint8_t nextslashin = 0;
            for (size_t i = 0; true; i++) {
                if (path[currpathidx + i] == '/') {
                    nextslashin = i;
                    break;
                } else if (nextslashin == 0 && path[currpathidx + i] == 0) {
                    nextslashin = i;
                    break;
                }
            }

            for (size_t i = 0; i < currnode->ext.ext_dir.num_dirs; i++) {
                for (size_t i = 0; i < currnode->ext.ext_dir.num_dirs; i++) {
                    if (strncmp(currnode->ext.ext_dir.files[i].name,
                                path + (currpathidx + 1), nextslashin) == 0) {
                        currpath[currpathidx] = '/';
                        memcpy(&currpath[currpathidx + 1],
                               currnode->ext.ext_dir.files[i].name,
                               strlen(currnode->ext.ext_dir.files[i].name) + 1);
                        currpathidx +=
                            strlen(currnode->ext.ext_dir.files[i].name) + 1;

                        currnode = &currnode->ext.ext_dir.files[i];
                        found = true;
                        break;
                    }
                }
            }
            if (!found) {
                return false;
            }
        }
    } else {
        currnode->flags = FLAGS_PRESENT | FLAGS_ISDIR | FLAGS_LOADED;
        currnode->name = "/";

        currnode->ext.ext_dir.creation_date_unix_time = 0;
        currnode->ext.ext_dir.last_modified_unix_time = 0;
        currnode->ext.ext_dir.disk_id = 0;
        currnode->ext.ext_dir.owner_group = 0;
        currnode->ext.ext_dir.owner_user = 0;
        currnode->ext.ext_dir.num_dirs = 1;

        currnode->ext.ext_dir.files = NULL;
    }
    currnode->flags |= FLAGS_MOUNT_POINT;
    struct dir_report rep = iterate_dir(disk_id, "/");
    currnode->ext.ext_dir.num_dirs = rep.num_entries;
    currnode->ext.ext_dir.files = rep.entries;

    return true;
}
struct file_buffer vfs_read(char *path, uint64_t offset, uint64_t size) {
    struct node *file = vfs_inspect(path);
    return read_file(file->ext.ext_file.disk_id,
                     (char *)file->ext.ext_file.contents_on_disk_path, offset,
                     size);
}
void vfs_write(char *path, struct file_buffer buf, uint64_t offset) {
    struct node *file = vfs_inspect(path);
    write_file(file->ext.ext_file.disk_id,
               (char *)file->ext.ext_file.contents_on_disk_path, offset, buf);
}

struct node *vfs_inspect(char *path) {
    if (strcmp(path, "/") == 0) {
        return &first_node;
    }

    struct node *currnode = &first_node;
    char currpath[255];
    uint8_t currpathidx = 0;
    while (true) {
        if (!(currnode->flags & FLAGS_LOADED) &&
            (currnode->flags & FLAGS_ISDIR)) {
            vfs_load_contents(currnode, currpath);
        }
        if (path[currpathidx] == '\0') {
            return currnode;
        }
        if (currnode == NULL) {
            return NULL;
        }
        if (!(currnode->flags & FLAGS_ISDIR)) {
            return NULL;
        }
        bool found = false;
        uint8_t nextslashin = 0;
        for (size_t i = 0; true; i++) {
            if (currpathidx + i == 0) {
                continue;
            }
            if (path[currpathidx + i] == '/') {
                nextslashin = i;
                break;
            } else if (nextslashin == 0 && path[currpathidx + i] == 0) {
                nextslashin = i;
                break;
            }
        }

        for (size_t i = 0; i < currnode->ext.ext_dir.num_dirs; i++) {
            if (strncmp(
                    currnode->ext.ext_dir.files[i].name, &path[currpathidx + 1],
                    nextslashin == 0 ? nextslashin : nextslashin - 1) == 0) {
                currpath[currpathidx] = '/';
                memcpy(&currpath[currpathidx + 1],
                       currnode->ext.ext_dir.files[i].name,
                       strlen(currnode->ext.ext_dir.files[i].name) + 1);
                currpathidx += strlen(currnode->ext.ext_dir.files[i].name) + 1;

                currnode = &currnode->ext.ext_dir.files[i];
                found = true;
                break;
            }
        }
        if (!found) {
            return NULL;
        }
    }
}

struct dir_report vfs_iterate_dir(char *path) {
    struct node *file = vfs_inspect(path);
    if (file != NULL) {
        if (!(file->flags & FLAGS_ISDIR))
            return (struct dir_report){0, 0};
        return (struct dir_report){file->ext.ext_dir.num_dirs,
                                   file->ext.ext_dir.files};
    }
    return (struct dir_report){0, 0};
}