#ifndef __REGISTERY_H__
#define __REGISTERY_H__

#include <libk/stdint.h>
#include <libk/macros.h>

typedef struct {
    u32 magic;   // a magic value meant to verify whether this file is actually a hive file
    u32 num_keys; // the number of keys in the file
    u8 checksum; // a checksum that makes all bytes add up to 0x00
    char name[64];    // the name of the hive
} PACKED hive_header;

typedef struct {
    u32 magic;         // magic number
    u32 num_entries;   // the number of entries
    u32 num_subkeys;   // the number of subkeys
    char name[64];          // the name of the key
} PACKED key_header;

typedef struct {
    char name[64]; // The name of the entry
    u8 type;   // The type of the entry.
    u8 length; // The length of the entry not including the header.
} PACKED entry_header;

hive_header* read_hive(char* path);
void free_hive(hive_header* hive);

key_header* read_key(hive_header* hive, char* path);
entry_header* read_entry(key_header* key, char* name);

#endif 