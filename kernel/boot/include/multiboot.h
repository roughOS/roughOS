#pragma once

#include "mm/include/pmm.h"
#include <stdint.h>

#define MULTIBOOT2_TAG_TYPE_MMAP 6

struct multiboot_tag {
    uint32_t type;
    uint32_t size;
} __attribute__((packed));

struct multiboot_tag_mmap {
    uint32_t type;
    uint32_t size;
    uint32_t entry_size;
    uint32_t entry_version;
    struct multiboot_mmap_entry {
        uint64_t addr;
        uint64_t len;
        uint32_t type;
        uint32_t zero;
    } __attribute__((packed)) entries[];
} __attribute__((packed));

void parse_mmap(void *mb2_addr, Frame *frames, size_t frame_count);