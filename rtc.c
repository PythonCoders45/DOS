#include "types.h"

#define CMOS_ADDRESS 0x70
#define CMOS_DATA    0x71

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint32_t year;
} RTCTime;

// Helper function to read a single CMOS register byte
static uint8_t get_rtc_register(int reg) {
    outb(CMOS_ADDRESS, reg);
    return inb(CMOS_DATA);
}

// Convert Binary Coded Decimal (BCD) to standard 8-bit integer
static uint8_t bcd_to_bin(uint8_t bcd) {
    return ((bcd / 16) * 10) + (bcd % 16);
}

/**
 * @brief Reads the current hardware time from the CMOS chip
 */
void read_rtc(RTCTime* time_out) {
    // Wait if CMOS update is currently in progress (bit 7 of Register A)
    outb(CMOS_ADDRESS, 0x0A);
    while (inb(CMOS_DATA) & 0x80);

    // Read BCD time values from hardware registers
    time_out->second = get_rtc_register(0x00);
    time_out->minute = get_rtc_register(0x02);
    time_out->hour   = get_rtc_register(0x04);
    time_out->day    = get_rtc_register(0x07);
    time_out->month  = get_rtc_register(0x08);
    time_out->year   = get_rtc_register(0x09);

    uint8_t register_b = get_rtc_register(0x0B);

    // Convert BCD to standard integers if necessary
    if (!(register_b & 0x04)) {
        time_out->second = bcd_to_bin(time_out->second);
        time_out->minute = bcd_to_bin(time_out->minute);
        time_out->hour   = bcd_to_bin(time_out->hour & 0x7F);
        time_out->day    = bcd_to_bin(time_out->day);
        time_out->month  = bcd_to_bin(time_out->month);
        time_out->year   = bcd_to_bin(time_out->year);
    }

    // Adjust 2-digit year (e.g., 26 -> 2026)
    time_out->year += 2000;
}
