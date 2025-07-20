#pragma once

#include <stdint.h>

#define GDT_ENTRIES                 5

/*
    Structure of a GDT entry

    Fields:
        limit_low - bits 0-15 of the limit
        base_low - bits 0-15 of the base address
        base_middle - bits 16-23
        access - access flags
        granularity - granularity + flags
        base_high - bits 24-31
*/
typedef struct
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__ ((packed)) gdt_entry_t;

/*
    Structure of a GDT pointer

    Fields:
        limit - GDT limit
        base - GDT base
*/
typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed)) gdt_ptr_t;

/*
    Create a GDT entry

    Params:
        index - the position of the entry in the GDT
        base - the entry base address
        limit - the entry limit
        access - access flags
        granularity - granularity + flags
*/
void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

/*
    Initliaze the GDT
*/
void gdt_init();