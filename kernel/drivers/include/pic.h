#pragma once

#define PIC1                0x20
#define PIC2                0x40
#define PIC1_COM            PIC1
#define PIC1_DATA           (PIC1 + 1)
#define PIC2_COM            PIC2
#define PIC2_DATA           (PIC2 + 1)

#define ICW1_INIT           0x10
#define ICW1_ICW4           0x01
#define ICW4_8086           0x01

void pic_remap(int offset1, int offset2);
void pic_send_eoi(int irq);
void pic_mask_set(int irq);
void pic_mask_clear(int irq);