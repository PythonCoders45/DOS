#ifndef DRIVER_H
#define DRIVER_H

#include "types.h"

typedef enum {
    DRIVER_TYPE_STORAGE,
    DRIVER_TYPE_NETWORK,
    DRIVER_TYPE_GRAPHICS,
    DRIVER_TYPE_AUDIO,
    DRIVER_TYPE_INPUT
} driver_type_t;

typedef struct driver {
    const char* name;
    driver_type_t type;
    int (*init)(void);
    int (*read)(uint64_t offset, void* buffer, uint32_t size);
    int (*write)(uint64_t offset, const void* buffer, uint32_t size);
    void (*unload)(void);
} driver_t;

void register_driver(driver_t* drv);
driver_t* get_driver_by_name(const char* name);

#endif // DRIVER_H
