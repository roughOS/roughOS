#include "drivers/include/pic.h"
#include "drivers/include/kbd.h"
#include "drivers/include/ps2.h"
#include "klibc/include/kstdio.h"
#include "cpu/include/io.h"

void isr_default()
{
    kputs("Default interrupt\n\r");
}

void isr_clock()
{
    static int tic = 0;
    static int sec = 0;

    tic++;
    if (tic % 100 == 0)
    {
        sec++;
        tic = 0;
    }

    pic_send_eoi(0);
}

void isr_kbd()
{
    uint8_t scancode = inb(KBD_DATA);
    char c = kbd_scancode_to_char(scancode);
    if (c != 0)
        kputchar(c);
    pic_send_eoi(1);
}

void isr_ide()
{
    pic_send_eoi(14);
}

void isr_fault_DE()
{
    kprint_log(LOG_PANIC, "Division by zero (#DE)");
    for (;;) asm volatile ("hlt");
}

void isr_fault_GP()
{
    kprint_log(LOG_PANIC, "General protection fault (#GP)");
    for (;;) asm volatile ("hlt");
}

void isr_fault_PF()
{
    kprint_log(LOG_PANIC, "Page fault (#PF)");
    for (;;) asm volatile ("hlt");
}

void isr_fault_TS()
{
    kprint_log(LOG_PANIC, "Invalid TSS (#TS)");
    for (;;) asm volatile ("hlt");
}