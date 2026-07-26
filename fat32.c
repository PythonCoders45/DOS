#include "types.h"

extern void ata_read_sector(uint32_t lba, uint8_t* buffer);

// FAT32 Boot Sector Structure (Sector 0 on disk)
typedef struct __attribute__((packed)) {
    uint8_t  bootjmp[3];
    uint8_t  oem_name[8];
    uint16_t bytes_per_sector;     // Typically 512
    uint8_t  sectors_per_cluster;  // e.g., 8 sectors = 4096 byte clusters
    uint16_t reserved_sector_count;
    uint8_t  table_count;          // Usually 2 FAT tables
    uint16_t root_entry_count;
    uint16_t total_sectors_16;
    uint8_t  media_type;
    uint16_t table_size_16;
    uint16_t sectors_per_track;
    uint16_t head_side_count;
    uint32_t hidden_sector_count;
    uint32_t total_sectors_32;
    uint32_t table_size_32;        // FAT32 Size in sectors
    uint16_t extended_flags;
    uint16_t fat_version;
    uint32_t root_cluster;         // Usually Cluster 2
} FAT32_BootSector;

// FAT32 Directory Entry Structure (32 bytes per file/folder entry)
typedef struct __attribute__((packed)) {
    uint8_t  name[11];             // 8 chars filename + 3 chars extension
    uint8_t  attr;                 // File attributes (Read-only, Hidden, System, Directory)
    uint8_t  nt_reserved;
    uint8_t  creation_time_tenth;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access_date;
    uint16_t first_cluster_hi;     // Upper 16-bits of starting cluster
    uint16_t write_time;
    uint16_t write_date;
    uint16_t first_cluster_lo;     // Lower 16-bits of starting cluster
    uint32_t file_size;            // Size in bytes
} FAT32_DirectoryEntry;

static uint32_t fat_start_sector;
static uint32_t data_start_sector;
static uint8_t  sectors_per_cluster;

void init_fat32(void) {
    uint8_t sector_buffer[512];
    ata_read_sector(0, sector_buffer);

    FAT32_BootSector* bs = (FAT32_BootSector*)sector_buffer;

    fat_start_sector = bs->reserved_sector_count;
    data_start_sector = bs->reserved_sector_count + (bs->table_count * bs->table_size_32);
    sectors_per_cluster = bs->sectors_per_cluster;
}

uint32_t cluster_to_lba(uint32_t cluster) {
    return data_start_sector + ((cluster - 2) * sectors_per_cluster);
}
