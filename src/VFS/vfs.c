#include "vfs.h"
#include "drive_manager.h"
#include <types/string.h>

struct node first_node;
struct node base_dir[4];

void vfs_init()
{
    first_node.flags = FLAGS_PRESENT | FLAGS_ISDIR;
    first_node.name = "/";

    ((struct ext_dir*)first_node.ext)->creation_date_unix_time = 0;
    ((struct ext_dir*)first_node.ext)->last_modified_unix_time = 0;
    ((struct ext_dir*)first_node.ext)->disk_id = 0;
    ((struct ext_dir*)first_node.ext)->owner_group = 0;
    ((struct ext_dir*)first_node.ext)->owner_user = 0;

    ((struct ext_dir*)first_node.ext)->files = base_dir;

    base_dir[0].name = "/sys";
    base_dir[1].name = "/bin";
    base_dir[2].name = "/usr";
    base_dir[3].name = "/dev";

    ((struct ext_dir*)base_dir[0].ext)->files = get_dir_contents(0,"/sys");
    ((struct ext_dir*)base_dir[1].ext)->files = get_dir_contents(0,"/bin");
    ((struct ext_dir*)base_dir[2].ext)->files = get_dir_contents(0,"/usr");
    //((struct ext_dir*)base_dir[3].ext)->files = get_dir_contents(0,"/dev"); NOPE!


    for (size_t i = 0; i < 4; i++)
    {
        base_dir[i].flags = FLAGS_PRESENT | FLAGS_ISDIR;
       
        ((struct ext_dir*)base_dir[i].ext)->creation_date_unix_time = 0;
        ((struct ext_dir*)base_dir[i].ext)->last_modified_unix_time = 0;
        ((struct ext_dir*)base_dir[i].ext)->disk_id = 0;
        ((struct ext_dir*)base_dir[i].ext)->owner_group = 0;
        ((struct ext_dir*)base_dir[i].ext)->owner_user = 0;
    }
    
}

struct node* get_file(char* path)
{
    char** splitstring = split(path,'/');
    struct node* currnode = &first_node;
    uint32_t i = 1;
    while (true)
    {
        if (splitstring[i][0] == 0)
        {
            return currnode;
        }
        
        if(!(currnode->flags & ~FLAGS_ISDIR)){
            return NULL;
        }  
        bool found = false;
        for (size_t j = 0; j < ((struct ext_dir*)currnode->ext)->num_dirs; j++)
        {
            if (strcmp(((struct ext_dir*)currnode->ext)->files[j].name,splitstring[i]))
            {
                currnode = &((struct ext_dir*)currnode->ext)->files[j];
                i++;
                break;
            }
        }
        if(!found){
            return NULL;
        }
    }
}