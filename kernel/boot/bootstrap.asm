bits 32

section .multiboot
align 8
mb2_header:
    dd 0xe85250d6
    dd 0
    dd mb2_end - mb2_header
    dd -(0xe85250d6 + 0 + (mb2_end - mb2_header))

    dw 0
    dw 0
    dd 8
mb2_end:

section .text
    extern kmain
    global _start

; Entry point
; The multiboot tag is stored into EBX
_start:
    push ebx
    call kmain

    hlt