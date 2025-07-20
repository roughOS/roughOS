#pragma once

#include <stdint.h>

#define VGA_START                       0xb8000
#define VGA_WIDTH                       80
#define VGA_HEIGHT                      25

#define C_BS                            '\b'
#define C_NL                            '\n'
#define C_CR                            '\r'
#define C_TAB                           '\t'
#define C_NULL                          '\0'

#define VGA_COLOR(fg, bg)               ((bg << 4) | (fg & 0x0f))

/*
    Enumeration of the 16 VGA colors from 0x0 to 0xf
*/
typedef enum
{
    VGA_BLACK,
    VGA_BLUE,
    VGA_GREEN,
    VGA_CYAN,
    VGA_RED,
    VGA_MAGENTA,
    VGA_BROWN,
    VGA_WHITE,
    VGA_GRAY,
    VGA_LIGHT_BLUE,
    VGA_LIGHT_GREEN,
    VGA_LIGHT_CYAN,
    VGA_LIGHT_RED,
    VGA_LIGHT_MAGENTA,
    VGA_YELLOW,
    VGA_BRIGHT_WHITE
} VGA_Colors;

/*
    Enumeration of the screen scrolling modes

    VGA_SCROLL_UP - tell the function to scroll n lines up
    VGA_SCROLL_DOWN - tell the function to scroll n lines down
*/
typedef enum
{
    VGA_SCROLL_UP,
    VGA_SCROLL_DOWN
} VGA_ScrollModes;

/*
    Display a character with a given color in VGA text mode

    Params:
        c - the character to display
        color - the character color
*/
void vga_putchar(uint8_t c, VGA_Colors color);

/*
    Display a string with a given color in VGA text mode

    Params:
        s - the string to display
        color - the character color
*/
void vga_puts(char *s, VGA_Colors color);

/*
    Scroll the screen of a given number of lines in a given direction in VGA text mode

    Params:
        mode - the vertical scroll direction
        n - the number of lines to scroll
        color - the new lines color
*/
void vga_vscroll(VGA_ScrollModes mode, uint32_t n, VGA_Colors color);

/*
    Clear the entire screen in VGA text mode

    Params:
        color - the color of the screen
*/
void vga_clear(VGA_Colors color);