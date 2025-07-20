#include "klibc/include/kstdio.h"
#include "cpu/include/gtd.h"
#include "cpu/interrupts/idt.h"
#include "drivers/include/timer.h"
#include "drivers/include/pic.h"
#include "drivers/include/ps2.h"
#include "drivers/include/ide.h"
#include "boot/include/multiboot.h"
#include "mm/include/pmm.h"
#include "mm/include/heap.h"
#include <stdint.h>
#include <stddef.h>

static Frame frames[MAX_FRAMES];
static uint16_t ide_buffer[256];

void kmain(void *mb2_addr)
{
    init_frame(frames, MAX_FRAMES);
    parse_mmap(mb2_addr, frames, MAX_FRAMES);
    kprint_log(LOG_INFO, "Frame table starts at 0x%x (%d frames)", (uint32_t)(uintptr_t)frames, MAX_FRAMES, (MAX_FRAMES * sizeof(Frame)) / 1024);

    heap_init(0x1000000, 1024*1024);
    kprint_log(LOG_INFO, "Initialized heap");

    gdt_init();
    kprint_log(LOG_INFO, "Initialized GDT");

    pic_remap(0x20, 0x28);
    kprint_log(LOG_INFO, "Initialized PIC");

    idt_init();
    kprint_log(LOG_INFO, "Initialized IDT");

    pit_init(100);
    kprint_log(LOG_INFO, "Initialized PIT with 100Hz frequency");

    ps2_controller_init();
    kprint_log(LOG_INFO, "Initialized PS/2 driver");

    pic_mask_clear(0);
    pic_mask_clear(1);
    pic_mask_set(14);

    if (ide_identify(ide_buffer))
    {
        kprint_log(LOG_INFO, "IDE device detected");

        uint32_t sectors = ide_buffer[60] | ((uint32_t)ide_buffer[61] << 16);
        kprint_log(LOG_INFO, "Disk size: %d sectors (%d MiB)", sectors, (sectors / 2048));
    }
    else
    {
        kprint_log(LOG_ERR, "No present IDE device");
    }

    asm volatile ("sti");

    while (1);
}