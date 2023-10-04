#include "vfs.h"
#include "drive_manager.h"
#include <logging/logger.h>
#include <types/string.h>

struct node first_node;

void vfs_init() {
    first_node.flags = FLAGS_PRESENT | FLAGS_ISDIR;
    first_node.name = "/";

    ((struct ext_dir *)first_node.ext)->creation_date_unix_time = 0;
    ((struct ext_dir *)first_node.ext)->last_modified_unix_time = 0;
    ((struct ext_dir *)first_node.ext)->disk_id = 0;
    ((struct ext_dir *)first_node.ext)->owner_group = 0;
    ((struct ext_dir *)first_node.ext)->owner_user = 0;

    ((struct ext_dir *)first_node.ext)->files = get_dir_contents(0, "");
}

bool is_dir(char *path) {
    struct node *fileinq = get_file(path);
    if (fileinq == NULL)
        return false;
    return (fileinq->flags & FLAGS_ISDIR) >> 1;
}

struct node *get_file(char *path) {
    if (strcmp(path, "/")) {
        return &first_node;
    }

    char **splitstring = split(path, '/');
    struct node *currnode = &first_node;
    uint32_t i = 0;
    while (true) {
        if (splitstring[i][0] == 0) {
            return currnode;
        }
        if (currnode == NULL) {
            return NULL;
        }
        if (!(currnode->flags & FLAGS_ISDIR)) {
            return NULL;
        }
        bool found = false;
        for (size_t j = 0; j < ((struct ext_dir *)currnode->ext)->num_dirs;
             j++) {
            if (strcmp(((struct ext_dir *)currnode->ext)->files[j].name,
                       splitstring[i])) {
                currnode = &((struct ext_dir *)currnode->ext)->files[j];
                i++;
                break;
            }
        }
        if (!found) {
            return NULL;
        }
    }
}