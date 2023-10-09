#include "madt.h"
#include <drivers/Memory/scubadeeznutz.h>
#include <kernel.h>
#include <logging/logger.h>
#include <types/string.h>

void init_madt(struct MADT *madt)
{
	(void)madt;
	log_info("madt");
}