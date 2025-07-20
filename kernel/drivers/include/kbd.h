#pragma once

#include <stdbool.h>
#include <stdint.h>

#define KBD_MAP_SIZE                        128

#define K_LEFT_SHIFT                        0x2a
#define K_RIGHT_SHIFT                       0x36
#define K_CTRL                              0x1d
#define K_ALTGR                             0x38

extern const char keyboard_map_normal[KBD_MAP_SIZE];
extern const char keyboard_map_shift[KBD_MAP_SIZE];
static bool shift_pressed = false;
static bool ctrl_pressed = false;
static bool altgr_pressed = false;

/*
    Read a scancode from the keyboard

    Return:
        the read scancode
*/
uint8_t kbd_read_scancode();

/*
    Convert a scancode to an ASCII character

    Params:
        scancode - the scancode to convert

    Return:
        the ASCII character
*/
char kbd_scancode_to_char(uint8_t scancode);