#include "cpu/include/gtd.h"
#include <stdint.h>

static gdt_entry_t gdt[GDT_ENTRIES];
static gdt_ptr_t gdtp;

void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
    gdt[index].base_low = (base & 0xffff);
    gdt[index].base_middle = (base >> 16) & 0xff;
    gdt[index].base_high = (base >> 24) & 0xff;

    gdt[index].limit_low = (limit & 0xffff);

    gdt[index].granularity = ((limit >> 16) & 0x0f) | (granularity & 0xf0);
    gdt[index].access = access;
}

void gdt_init()
{
    gdtp.limit = sizeof(gdt) - 1;
    gdtp.base = (uint32_t) &gdt;

    gdt_set_entry(0, 0, 0x00000000, 0x00, 0x00);    // null
    gdt_set_entry(1, 0, 0xfffff, 0x9a, 0xc0);       // kernel code
    gdt_set_entry(2, 0, 0xfffff, 0x92, 0xc0);       // kernel data
    gdt_set_entry(3, 0, 0xfffff, 0x92, 0xcf);       // kernel stack
    asm volatile (
        "lgdt (%0)\n\t"
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        "ljmp $0x08, $next\n\t"
        "next:\n\t"
        :
        : "r" (&gdtp)
        : "memory", "ax"
    );
}