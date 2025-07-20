#pragma once

#include <stdint.h>

#define PIT_CHANNEL0                0x40
#define PIT_COM                     0x43

#define PIT_DEFAULT_FREQUENCY       1193182

/*
    Initialize the PIT

    Params:
        frequency - the PIT frequency (in Hz)
*/
void pit_init(uint32_t frequency);