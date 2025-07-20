#include "drivers/include/pic.h"
#include "cpu/include/io.h"
#include <stdint.h>

void pic_remap(int offset1, int offset2)
{
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    outb(PIC1_COM, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COM, ICW1_INIT | ICW1_ICW4);

    outb(PIC1_DATA, offset1);
    outb(PIC2_DATA, offset2);

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void pic_send_eoi(int irq)
{
    if (irq >= 8)
        outb(PIC2_COM, 0x20);

    outb(PIC1_COM, 0x20);
}

void pic_mask_set(int irq)
{
    uint16_t port;
    uint8_t value;

    if (irq < 8)
        port = 0x21;
    else
    {
        port = 0xa1;
        irq -= 8;
    }

    value = inb(port) | (1 << irq);
    outb(port, value);
}

void pic_mask_clear(int irq)
{
    uint16_t port;
    uint8_t value;

    if (irq < 8)
        port = 0x21;
    else
    {
        port = 0xA1;
        irq -= 8;
    }

    value = inb(port) & ~(1 << irq);
    outb(port, value);
}