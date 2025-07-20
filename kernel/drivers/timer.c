#include "drivers/include/timer.h"
#include "cpu/include/io.h"
#include <stdint.h>

void pit_init(uint32_t frequency)
{
    uint16_t divisor = PIT_DEFAULT_FREQUENCY / frequency;

    outb(PIT_COM, 0x36);

    outb(PIT_CHANNEL0, divisor & 0xff);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xff);
}