#include "types.h"

// Simple placement memory allocator starting at 4 MB in physical RAM
static uint32_t heap_curr = 0x00400000;

/**
 * @brief Allocates a block of memory from the kernel heap
 * @param size Number of bytes requested
 * @param align If true, aligns allocation to a 4 KB page boundary
 */
void* kmalloc_ext(size_t size, bool align) {
    if (align && (heap_curr & 0xFFFFF000)) {
        heap_curr &= 0xFFFFF000;
        heap_curr += 0x1000; // Align to next 4 KB boundary
    }

    void* addr = (void*)heap_curr;
    heap_curr += size;
    return addr;
}

void* kmalloc(size_t size) {
    return kmalloc_ext(size, false);
}
