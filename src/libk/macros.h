#ifndef __MACROS_H__
#define __MACROS_H__

// attribute macros
#define PACKED __attribute__((packed))
#define UNUSED __attribute__((unused))

#define DRIVER_ASSERT(x) if(!x) trigger_psod(HN_ERR_DRIVER_ERROR,"Error initializing mandatory driver",NULL);

#endif