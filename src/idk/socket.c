#include "socket.h"
#include <stdio.h>
#include <VFS/vfs.h>

struct socket
{
    char type;
    char* path;
};
struct socket socks[1024];
unsigned short lastsockID;
void reset_socket_list(){
    lastsockID = 0;
}

unsigned short gimme_socket(unsigned short type,char* path)
{
    socks[lastsockID].type = type;
    socks[lastsockID].path = path;
    return lastsockID++;
}

void change_type(unsigned short ID,char type)
{
    socks[ID].type = type;
}

struct file_buffer read(unsigned short ID,uint64_t offset,uint64_t bufsize)
{
    if (socks[ID].type == VFS_SOCKET)
    {
        return vfs_read(socks[ID].path,offset,bufsize);
    }
    return (struct file_buffer){0,0};
    
}

bool write(unsigned short ID,uint64_t offset,char* buf,uint64_t bufsize)
{
    if (socks[ID].type == VFS_SOCKET)
    {
        vfs_write(socks[ID].path,(struct file_buffer){buf,bufsize},offset);
        return true;
    }
    return false;
}
