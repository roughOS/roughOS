#pragma once

#include <stdint.h>

#define IDT_ENTRIES             256

/*
    Structure of an IDT entry

    Fields:
        offset_low - offset bits 0-15
        selector - segment selector
        zero - always zero
        type_attr - type and attributes
        offset_high - offset bits 16-31
*/
typedef struct
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__ ((packed)) idt_entry_t;

/*
    Structure of an IDT pointer

    Fields:
        limit - IDT limit
        base - IDT base
*/
typedef struct
{
    uint16_t limit;
    uint32_t base;
} __attribute__ ((packed)) idt_ptr_t;

/*
    Create an IDT entry

    Params:
        index - the position of the entry in the IDT
        base - the entry base address
        selector - the segment selector
        flags - the entry flags
*/
void idt_set_entry(int index, uint32_t base, uint16_t selector, uint8_t flags);

/*
    Initialize the IDT
*/
void idt_init();