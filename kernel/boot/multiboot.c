#pragma once

#include "boot/include/multiboot.h"
#include "klibc/include/kstdio.h"
#include "mm/include/pmm.h"
#include <stdint.h>

void parse_mmap(void *mb2_addr, Frame *frames, size_t frame_count)
{
    uint8_t *ptr = (uint8_t *) mb2_addr;
    ptr += 8;

    while (1)
    {
        struct multiboot_tag *tag = (struct multiboot_tag *) ptr;
        if (tag->type == 0)
            break;

        if (tag->type == MULTIBOOT2_TAG_TYPE_MMAP)
        {
            struct multiboot_tag_mmap *mmap_tag = (struct multiboot_tag_mmap *) tag;
            uint32_t entry_size = mmap_tag->entry_size;
            uint32_t entry_count = (mmap_tag->size - sizeof(struct multiboot_tag_mmap)) / entry_size;

            uint8_t *entry_ptr = (uint8_t *) mmap_tag->entries;

            for (uint32_t i = 0; i < entry_count; i++)
            {
                struct multiboot_mmap_entry *entry = (struct multiboot_mmap_entry *) entry_ptr;

                if (entry->type == 1)
                {
                    kprint_log(LOG_INFO, "Usable memory region: 0x%x - 0x%x (%d KB)", (void*)entry->addr, (void*)(entry->addr + entry->len), (uint32_t)(entry->len / 1024));
                    
                    uint64_t base = entry->addr;
                    uint64_t length = entry->len;

                    size_t first_frame = base / FRAME_SIZE;
                    size_t last_frame = (base + length) / FRAME_SIZE;

                    for (size_t i = first_frame; i < last_frame && i < frame_count; i++)
                        FRAME_SET_FREE(&frames[i]);
                }

                entry_ptr += entry_size;
            }
        }

        ptr += (tag->size + 7) & ~7;
    }
}