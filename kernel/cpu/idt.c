#include "cpu/interrupts/idt.h"
#include <stdint.h>

void _asm_default();
void _asm_irq_0();
void _asm_irq_1();
void _asm_irq_14();
void _asm_except_DE();
void _asm_except_GP();
void _asm_except_TS();
void _asm_except_PF();

static idt_entry_t idt[256];
static idt_ptr_t idtp;

void idt_set_entry(int index, uint32_t base, uint16_t selector, uint8_t flags)
{
    idt[index].offset_low = base & 0xffff;
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].type_attr = flags | 0x80;
    idt[index].offset_high = (base >> 16) & 0xffff;
}

void idt_init()
{
    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t) &idt;

    idt_set_entry(0, (uint32_t) _asm_except_DE, 0x08, 0x8e);
    idt_set_entry(10, (uint32_t) _asm_except_TS, 0x08, 0x8e);
    idt_set_entry(13, (uint32_t) _asm_except_GP, 0x08, 0x8e);
    idt_set_entry(14, (uint32_t) _asm_except_PF, 0x08, 0x8e);
    idt_set_entry(32, (uint32_t) _asm_irq_0, 0x08, 0x8e);
    idt_set_entry(33, (uint32_t) _asm_irq_1, 0x08, 0x8e);
    idt_set_entry(46, (uint32_t) _asm_irq_14, 0x08, 0x8e);

    asm volatile ("lidt (%0)" :: "r"(&idtp));
}