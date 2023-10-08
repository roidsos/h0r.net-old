#ifndef __SOCKET_H__
#define __SOCKET_H__
#include <stdbool.h>
#include <stdint.h>
#include <VFS/defines.h>

enum socket_type{
    VFS_SOCKET,
    NETWORK_SOCKET,
    AUDIO_SOCKET
};

void reset_socket_list();
unsigned short gimme_socket(unsigned short type,char* path);
void change_type(unsigned short ID,char type);

struct file_buffer read(unsigned short ID,uint64_t offset,uint64_t bufsize);
bool write(unsigned short ID,uint64_t offset,char* buf,uint64_t bufsize);

#endif // __SOCKET_H__