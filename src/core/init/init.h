#ifndef __INIT_H__
#define __INIT_H__
#include <limine.h>

void init_HW();
void initsys_start();
void init_sys();
void load_initramfs(struct limine_file* tar_file);
void load_config(struct limine_file* cfg_file);

#endif // __INIT_H__