#include "drivers/include/vga.h"
#include <stdint.h>

static uint16_t cursor_x = 0;
static uint16_t cursor_y = 0;

/*
    Display a newline ('\n')
*/
static void display_newline()
{
    (cursor_y)++;
}

/*
    Display a carriage return ('\r')
*/
static void display_carriage_return()
{
    (cursor_x) = 0;
}

/*
    Display a tabulation ('\t`)
*/
static void display_tab()
{
    (cursor_x) += 4;
}

/*
    Display a backspace ('\b')
*/
static void display_backspace()
{
    (cursor_x)--;
}

/*
    Display a complete newline ("\n\r")
*/
static void display_full_newline()
{
    display_newline();
    display_carriage_return();
}

void vga_putchar(uint8_t c, VGA_Colors color)
{
    volatile char *vga = (volatile char *) VGA_START;
    if (!vga) return;

    switch (c)
    {
        case C_NL:
            display_newline();
            display_carriage_return();
            break;

        case C_CR:
            display_carriage_return();
            break;

        case C_TAB:
            display_tab();
            if (cursor_x >= VGA_WIDTH) {
                display_newline();
                cursor_x = 0;
            }
            break;

        case C_BS:
            if (cursor_x > 0)
                display_backspace();
            break;

        default:
            {
                uint32_t index = 2 * (cursor_y * VGA_WIDTH + cursor_x);
                vga[index] = c;
                vga[index + 1] = color;
                cursor_x++;
                if (cursor_x >= VGA_WIDTH)
                    display_full_newline();
            }
            break;
    }

    if (cursor_y >= VGA_HEIGHT)
    {
        vga_vscroll(VGA_SCROLL_UP, 1, VGA_COLOR(VGA_BLACK, VGA_BLACK));
        cursor_y = VGA_HEIGHT - 1;
    }
}

void vga_puts(char *s, VGA_Colors color)
{
    if (!s) return;

    while (*s != C_NULL)
    {
        vga_putchar(*s, color);
        s++;
    }
}

void vga_vscroll(VGA_ScrollModes mode, uint32_t n, VGA_Colors color)
{
    if (n == 0)
        return;

    volatile uint16_t *vga = (volatile uint16_t *) VGA_START;
    if (!vga) return;

    switch((uint8_t)mode)
    {
        case VGA_SCROLL_UP:
                for (uint32_t row = 0; row < VGA_HEIGHT - n; row++)
                    for (uint32_t col = 0; col < VGA_WIDTH; col++) 
                        vga[row * VGA_WIDTH + col] = vga[(row + n) * VGA_WIDTH + col];

                for (uint32_t row = VGA_HEIGHT - n; row < VGA_HEIGHT; row++)
                    for (uint32_t col = 0; col < VGA_WIDTH; col++)
                        vga[row * VGA_WIDTH + col] = (color << 8) | ' ';

                break;
    
        case VGA_SCROLL_DOWN:
            for (int row = VGA_HEIGHT - 1; row >= (int)n; row--)
                for (uint32_t col = 0; col < VGA_WIDTH; col++)
                    vga[row * VGA_WIDTH + col] = vga[(row - n) * VGA_WIDTH + col];

            for (uint32_t row = 0; row < n; row++)
                for (uint32_t col = 0; col < VGA_WIDTH; col++)
                    vga[row * VGA_WIDTH + col] = (color << 8) | ' ';

            break;

        default:
            break;
    }
}

void vga_clear(VGA_Colors color)
{
    volatile uint16_t *vga = (volatile uint16_t *) VGA_START;
    if (!vga) return;

    uint8_t fg = VGA_BLACK;
    uint8_t bg = color;
    uint8_t attribute = (bg << 4) | fg;
    uint16_t entry = ((uint16_t)attribute << 8) | ' ';

    for (int x = 0; x < VGA_WIDTH; x++)
        for (int y = 0; y < VGA_HEIGHT; y++)
            vga[y * VGA_WIDTH + x] = entry;
}