#include "types.h"

volatile uint32_t timer_ticks = 0;

/**
 * @brief Called whenever the hardware PIT timer fires (IRQ0)
 */
void timer_handler(void) {
    timer_ticks++;
}

/**
 * @brief Sleeps execution for a specified number of milliseconds
 */
void sleep(uint32_t ms) {
    uint32_t start_ticks = timer_ticks;
    // Assuming PIT is configured to 100 Hz (1 tick = 10 ms)
    uint32_t ticks_to_wait = ms / 10;
    while ((timer_ticks - start_ticks) < ticks_to_wait) {
        __asm__ __volatile__("hlt"); // Sleep CPU until next timer interrupt
    }
}
