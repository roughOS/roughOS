#include "include/kfuncs.h"
#include <stdint.h>
#include <stddef.h>

void template();

void kmain(void *mb2_addr)
{
    memory_init(mb2_addr);
    gdt_install();
    interrupts_install();
    drivers_install();
    disk_check();
    fs_install();

    asm volatile ("sti");

    template();

    while (1);
}