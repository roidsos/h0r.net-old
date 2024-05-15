#include "registery.h"
#include <config.h>
#include <endian.h>
#include <utils/log.h>
#include <klibc/string.h>

#include <core/sys/resman/SIV.h>
#include <core/mm/heap.h>

hive_header* read_hive(char* path)
{
    uint32_t fd = siv_open(0,path,SIV_INTENTS_READ);
    if(fd == UINT32_MAX){
        log_error("Failed to open hive at %s\n",path);
        return NULL;
    }
    size_t size = siv_get_file(fd).size;
    hive_header* hive = (hive_header*)malloc(sizeof(hive_header));
    siv_read(fd,0,(char*)hive,size);
    siv_close(fd);

    if(be32toh(hive->magic) != HIVE_MAGIC){
        log_error("Invalid hive at %s: Wrong magic value(0x%x)\n",path,hive->magic);
        free(hive);
        return NULL;
    }

    //TODO: check checksum

    return hive;
}

void free_hive(hive_header* hive)
{
    free(hive);
}

key_header* read_key(hive_header* hive, char* path)
{
    if(hive == NULL || path == NULL) return NULL;  
    char* pointer = (char*)hive + sizeof(hive_header);

    //TODO: subkey support
    for(uint32_t i = 0; i < be32toh(hive->num_keys); i++){
        key_header* key = (key_header*)pointer;
        log_debug("key magic: %s\n",key->magic);
        if(strcmp(key->name,path) == 0){
            return key;
        }
    }
    return NULL;
}

entry_header* read_entry(key_header* key, char* name)
{
    if (key == NULL || name == NULL) return NULL;
    entry_header* entry = (entry_header*)((char*)key + sizeof(key_header));
    for (uint32_t i = 0; i < be32toh(key->num_entries); i++) {
        if (strcmp((char*)entry->name, name) == 0) {
            return entry;
        }
        entry = (entry_header*)((char*)entry + sizeof(entry_header) + be32toh(entry->length));
    }
    return NULL;
}
