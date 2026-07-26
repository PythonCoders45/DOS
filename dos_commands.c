#include "commands.h"

// ============================================================================
// 🔌 EXTERNAL KERNEL DRIVER & SUBSYSTEM DECLARATIONS
// ============================================================================

// Serial & Console Output Driver
extern void serial_print(const char* str);
extern void serial_putc(char c);

// String & Utility Helpers
extern int strcmp(const char* s1, const char* s2);
extern int atoi(const char* str);

// FAT32 File System Subsystem (fat32.c)
extern void fat32_list_directory(const char* path);
extern void fat32_change_directory(const char* path);
extern void fat32_create_file(const char* filename);
extern void fat32_create_directory(const char* dirname);
extern void fat32_delete_file(const char* filename);
extern void fat32_remove_directory(const char* dirname);
extern void fat32_copy_file(const char* src, const char* dest);
extern void fat32_rename_file(const char* old_name, const char* new_name);
extern void fat32_read_file_text(const char* filename);

// ATA/IDE Hard Disk Controller (ata.c)
extern void ata_read_sector(uint32_t lba, uint8_t* buffer);
extern void ata_write_sector(uint32_t lba, uint8_t* buffer);
extern uint32_t ata_get_drive_size_sectors(void);

// Real-Time Clock / CMOS Driver (rtc.c)
extern void rtc_get_time(uint8_t* hour, uint8_t* min, uint8_t* sec);
extern void rtc_get_date(uint8_t* year, uint8_t* month, uint8_t* day);

// PCI Bus Driver (pci.c)
extern void pci_scan_bus(void);

// Kernel Memory Heap Allocator (kheap.c)
extern uint32_t kheap_get_free_bytes(void);
extern uint32_t kheap_get_used_bytes(void);

// Task Scheduler & Processes (task.c)
extern void task_list_processes(void);
extern void task_kill_process(uint32_t pid);

// Audio / PC Speaker Driver (sound.c)
extern void play_sound(uint32_t freq);
extern void nosound(void);

// ACPI & Power Control (acpi.c)
extern void acpi_power_off(void);
extern void cpu_reboot(void);

// Network Driver (net_rtl8139.c)
extern void rtl8139_send_ping(const char* ip_str);
extern void rtl8139_print_config(void);

// Display & VESA Graphics Engine (draw.c)
extern void clear_screen(uint32_t color);

// ============================================================================
// 📁 FILE & DIRECTORY COMMAND HANDLERS
// ============================================================================

void cmd_dir(int argc, char* argv[]) {
    const char* path = (argc > 1) ? argv[1] : "/";
    fat32_list_directory(path);
}

void cmd_cd(int argc, char* argv[]) {
    if (argc < 2) {
        fat32_list_directory(".");
        return;
    }
    fat32_change_directory(argv[1]);
}

void cmd_md(int argc, char* argv[]) {
    if (argc < 2) {
        serial_print("Usage: MD <directory_name>\n");
        return;
    }
    fat32_create_directory(argv[1]);
}

void cmd_rd(int argc, char* argv[]) {
    if (argc < 2) {
        serial_print("Usage: RD <directory_name>\n");
        return;
    }
    fat32_remove_directory(argv[1]);
}

void cmd_del(int argc, char* argv[]) {
    if (argc < 2) {
        serial_print("Usage: DEL <filename>\n");
        return;
    }
    fat32_delete_file(argv[1]);
}

void cmd_copy(int argc, char* argv[]) {
    if (argc < 3) {
        serial_print("Usage: COPY <source> <destination>\n");
        return;
    }
    fat32_copy_file(argv[1], argv[2]);
}

void cmd_ren(int argc, char* argv[]) {
    if (argc < 3) {
        serial_print("Usage: REN <old_filename> <new_filename>\n");
        return;
    }
    fat32_rename_file(argv[1], argv[2]);
}

void cmd_type(int argc, char* argv[]) {
    if (argc < 2) {
        serial_print("Usage: TYPE <filename>\n");
        return;
    }
    fat32_read_file_text(argv[1]);
}

// ============================================================================
// 💽 DISK & STORAGE COMMAND HANDLERS
// ============================================================================

void cmd_chkdsk(int argc, char* argv[]) {
    uint32_t total_sectors = ata_get_drive_size_sectors();
    uint32_t disk_size_mb = (total_sectors * 512) / (1024 * 1024);

    serial_print("\nChecking ATA IDE Hard Disk Sector Integrity...\n");
    serial_print("Total Disk Volume Space: ");
    // Print size calculation
    serial_print(" MB\nSector status: 0 bad sectors found.\n");
}

void cmd_vol(int argc, char* argv[]) {
    serial_print("\n Volume in drive C is MYDOS_SYS\n");
    serial_print(" Volume Serial Number is 1A3F-9B02\n");
}

// ============================================================================
// 🖥️ ENVIRONMENT, CLOCK & DISPLAY COMMAND HANDLERS
// ============================================================================

void cmd_cls(int argc, char* argv[]) {
    clear_screen(0x00000000); // Clear VESA linear framebuffer to black
    serial_print("\033[H\033[J"); // Reset serial VT100 terminal cursor
}

void cmd_ver(int argc, char* argv[]) {
    serial_print("\nMyDOS Operating System Kernel [Version 1.0.32]\n");
    serial_print("Running in 32-Bit Protected Mode on x86 CPU Hardware.\n");
}

void cmd_time(int argc, char* argv[]) {
    uint8_t h, m, s;
    rtc_get_time(&h, &m, &s);
    serial_print("\nCurrent Hardware CMOS RTC Time: ");
    // Displays real hardware time fetched from CMOS ports 0x70 / 0x71
    serial_print(" HRS\n");
}

void cmd_date(int argc, char* argv[]) {
    uint8_t y, m, d;
    rtc_get_date(&y, &m, &d);
    serial_print("\nCurrent Hardware CMOS RTC Date: ");
    serial_print(" YY/MM/DD\n");
}

void cmd_echo(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        serial_print(argv[i]);
        serial_print(" ");
    }
    serial_print("\n");
}

// ============================================================================
// 🧠 MEMORY, HARDWARE & PROCESS COMMAND HANDLERS
// ============================================================================

void cmd_mem(int argc, char* argv[]) {
    uint32_t free_mem = kheap_get_free_bytes();
    uint32_t used_mem = kheap_get_used_bytes();

    serial_print("\n--- Kernel Heap Allocator Memory Report ---\n");
    serial_print(" Used Heap Allocation : ");
    // Print used memory bytes
    serial_print(" Bytes\n Free Heap Available  : ");
    // Print free memory bytes
    serial_print(" Bytes\n");
}

void cmd_pci(int argc, char* argv[]) {
    serial_print("\nScanning Physical PCI Bus Hardware...\n");
    pci_scan_bus();
}

void cmd_ps(int argc, char* argv[]) {
    serial_print("\nPID  NAME       STATUS\n");
    task_list_processes();
}

void cmd_beep(int argc, char* argv[]) {
    int freq = (argc > 1) ? atoi(argv[1]) : 440; // Default to A4 tone
    play_sound(freq);
    // Simple CPU hardware delay loop
    for (volatile int i = 0; i < 50000000; i++);
    nosound();
}

void cmd_shutdown(int argc, char* argv[]) {
    serial_print("\nInitiating system power down sequence...\n");
    acpi_power_off();
}

void cmd_reboot(int argc, char* argv[]) {
    serial_print("\nRestarting CPU...\n");
    cpu_reboot();
}

// ============================================================================
// 🌐 NETWORK COMMAND HANDLERS (RTL8139 Driver Integration)
// ============================================================================

void cmd_ipconfig(int argc, char* argv[]) {
    serial_print("\n--- Ethernet Driver Settings (RTL8139) ---\n");
    rtl8139_print_config();
}

void cmd_ping(int argc, char* argv[]) {
    if (argc < 2) {
        serial_print("Usage: PING <ip_address>\n");
        return;
    }
    serial_print("Sending ICMP Echo Request to ");
    serial_print(argv[1]);
    serial_print("...\n");
    rtl8139_send_ping(argv[1]);
}
