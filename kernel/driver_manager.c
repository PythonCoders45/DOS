#include "driver.h"

#define MAX_DRIVERS 32

static driver_t* driver_registry[MAX_DRIVERS];
static uint32_t registered_count = 0;

void register_driver(driver_t* drv) {
    if (registered_count >= MAX_DRIVERS || !drv) return;

    if (drv->init() == 0) { // Call driver initialization
        driver_registry[registered_count++] = drv;
    }
}

// Example XP-Class Network Driver Definition (Intel E1000 NIC)
static int e1000_init(void) {
    // Initialize Intel E1000 PCI Network Controller
    return 0; // Success
}

static driver_t net_driver_e1000 = {
    .name = "Intel E1000 Gigabit NIC",
    .type = DRIVER_TYPE_NETWORK,
    .init = e1000_init,
    .read = 0,
    .write = 0,
    .unload = 0
};

void init_all_drivers(void) {
    register_driver(&net_driver_e1000);
    // Register Storage (AHCI), Sound (AC'97), Graphics (VESA) drivers here...
}
