#include "drivers/include/ps2.h"
#include "cpu/include/io.h"
#include "klibc/include/kstdio.h"
#include <stdint.h>

static void wait_ibf_clear()
{
    while (inb(KBD_STATUS) & KBD_STATUS_IBF);
}

static void wait_obf_full()
{
    while (!(inb(KBD_STATUS) & KBD_STATUS_OBF));
}

void ps2_set_typematic_rate(uint8_t rate)
{
    wait_ibf_clear();
    outb(KBD_DATA, 0xf3);
    wait_obf_full();
    uint8_t res = inb(KBD_DATA);
    if (res != 0xfa)
    {
        kprint_log(LOG_WARN, "Failed to set typematic on PS/2 controller, got: %d", res);
        return;
    }

    wait_ibf_clear();
    outb(KBD_DATA, rate);
    wait_obf_full();
    uint8_t res2 = inb(KBD_DATA);
    if (res2 != 0xfa)
    {
        kprint_log(LOG_WARN, "Bad configuration setting for PS/2 controller, got: %d", res2);
        return;
    }
}

void ps2_controller_init()
{
    wait_ibf_clear();
    outb(KBD_STATUS, KBD_DISABLE_PORT1);

    while (inb(KBD_STATUS) & KBD_STATUS_OBF)
        (void)inb(KBD_DATA);

    wait_ibf_clear();
    outb(KBD_STATUS, KBD_READ_CONF);
    wait_obf_full();
    uint8_t config = inb(KBD_DATA);

    config |= 0x01;
    config &= ~0x40;

    wait_ibf_clear();
    outb(KBD_STATUS, KBD_WRITE_CONF);
    wait_ibf_clear();
    outb(KBD_DATA, config);

    wait_ibf_clear();
    outb(KBD_STATUS, KBD_SELF_TEST);
    wait_obf_full();
    uint8_t self_test_res = inb(KBD_DATA);
    if (self_test_res != 0x55)
    {
        kprint_log(LOG_ERR, "PS/2 controller self-test failed, got: %d", self_test_res);
        return;
    }

    wait_ibf_clear();
    outb(KBD_DATA, 0xff);

    wait_obf_full();
    uint8_t acknowledge = inb(KBD_DATA);
    if (acknowledge != 0xFA)
    {
        kprint_log(LOG_ERR, "Failed to reset PS/2 keyboard, got: %d", acknowledge);
        return;
    }

    wait_obf_full();
    uint8_t kbd_reset = inb(KBD_DATA);
    if (kbd_reset != 0xAA)
    {
        kprint_log(LOG_ERR, "PS/2 keyboard self-test failed, got: %d", kbd_reset);
        return;
    }

    wait_ibf_clear();
    outb(KBD_DATA, 0xF0);
    wait_obf_full();
    if (inb(KBD_DATA) != 0xFA)
    {
        kprint_log(LOG_WARN, "PS/2 keyboard did not acknowledge expected value");
        return;
    }

    wait_ibf_clear();
    outb(KBD_DATA, 0x01);
    wait_obf_full();
    if (inb(KBD_DATA) != 0xFA)
    {
        kprint_log(LOG_WARN, "PS/2 keyboard did not accept scancode set 1");
        return;
    }

    outb(KBD_STATUS, KBD_ENABLE_PORT1);
}