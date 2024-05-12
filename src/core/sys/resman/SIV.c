#include "SIV.h"
#include <config.h>
#include <klibc/string.h>
#include <core/mm/heap.h>
#include <stdint.h>
#include <utils/log.h>

siv_drive_t siv_drives[MAX_DRIVES];
uint16_t siv_num_drives = 0;

block_driver_t siv_drivers[MAX_DRIVERS];
uint16_t siv_num_drivers = 0;

file_t open_files[MAX_OPEN_FILES];
char*  open_file_contents[MAX_OPEN_FILES];
uint32_t siv_num_open_files = 0;

void siv_init() {
    memset(siv_drives, 0, sizeof(siv_drives));
    memset(siv_drivers, 0, sizeof(siv_drivers));
}

uint16_t siv_register_drive(siv_drive_t drive)
{
    siv_drives[siv_num_drives] = drive;
    siv_num_drives++;
    return siv_num_drives - 1;
}

uint16_t siv_register_driver(block_driver_t driver)
{
    siv_drivers[siv_num_drivers] = driver;
    siv_num_drivers++;
    return siv_num_drivers - 1;
}

uint32_t siv_open(uint32_t drive_id,char* path,uint8_t intents)
{
    log_trace("siv_open(%d, %s, %d)\n", drive_id, path, intents);
    siv_drive_t drive = siv_drives[drive_id];
    block_driver_t driver = siv_drivers[drive.driver_id];

    uint32_t file_desc = siv_num_open_files;
    siv_num_open_files++;

    open_files[file_desc] = driver.get_props(drive.driver_specific_data, path);

    //TODO: check user perms
    if (open_files[file_desc].size == 0 && open_files[file_desc].full_path == NULL) {
        goto error;
    }

    if(open_files[file_desc].isdir) {
        // this is not how you open directories lmao
        goto error;
    }
    
    if(!driver.is_virtual && (intents & SIV_INTENTS_READ)){
        log_trace("Preloading file %s\n", path);
        open_file_contents[file_desc] = malloc(open_files[file_desc].size);
        driver.read(drive.driver_specific_data, path, 0, open_file_contents[file_desc], open_files[file_desc].size);
    }

    return file_desc;

    error:
    open_files[file_desc] = (file_t){0};
    return UINT32_MAX;
}

void siv_close(uint32_t file_desc)
{
    log_trace("siv_close(%d)\n", file_desc);
    //TODO: check user perms
    if (open_file_contents[file_desc] != NULL) {
        free(open_file_contents[file_desc]);
        open_file_contents[file_desc] = NULL;
    }

    open_files[file_desc] = (file_t){0};
}
file_t siv_get_file(uint32_t file_desc)
{
    return open_files[file_desc];
}

void siv_read(uint32_t file_desc, uint32_t offset, char* buf, uint32_t size)
{
    log_trace("siv_read(%d, %d, %d)\n", file_desc, offset, size);
    //TODO: check user perms
    if(open_file_contents[file_desc] != NULL) {
        memcpy(buf, open_file_contents[file_desc] + offset, size);
    } else {
        uint16_t drive_id = open_files[file_desc].drive_id;
        siv_drivers[siv_drives[drive_id].driver_id].read(siv_drives[drive_id].driver_specific_data, open_files[file_desc].full_path, offset, buf, size);
    }

}

void siv_write(uint32_t file_desc, uint32_t offset, char* data, uint32_t size)
{
    log_trace("siv_write(%d, %d, %d)\n", file_desc, offset, size);
    uint16_t drive_id = open_files[file_desc].drive_id;
    siv_drivers[siv_drives[drive_id].driver_id].write(siv_drives[drive_id].driver_specific_data, open_files[file_desc].full_path, offset, data, size);
}