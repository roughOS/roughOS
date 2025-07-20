extern isr_default, isr_clock, isr_kbd, isr_ide
extern isr_fault_DE, isr_fault_GP, isr_fault_TS, isr_fault_PF

global _asm_default, _asm_irq_0, _asm_irq_1, _asm_irq_14
global _asm_except_DE, _asm_except_GP, _asm_except_TS, _asm_except_PF

_asm_default:
    call isr_default
    mov al, 0x20
    out 0x20, al
    iret

_asm_irq_0:
    call isr_clock
    mov al, 0x20
    out 0x20, al
    iret

_asm_irq_1:
    call isr_kbd
    mov al, 0x20
    out 0x20, al
    iret

_asm_irq_14:
    call isr_ide
    mov al, 0x20
    out 0x20, al
    iret

_asm_except_DE:
    call isr_fault_DE
    iret

_asm_except_GP:
    call isr_fault_GP
    iret

_asm_except_TS:
    call isr_fault_TS
    iret

_asm_except_PF:
    call isr_fault_PF
    iret