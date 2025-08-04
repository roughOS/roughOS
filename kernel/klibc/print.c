#include "drivers/include/vga.h"
#include "klibc/include/kstdio.h"
#include "klibc/include/kstdarg.h"
#include "klibc/include/kstdlib.h"

/*
    kprintf() but using a va_list instead of a ...
*/
static void kvprintf(const char *restrict format, va_list args)
{
    if (!format) return;

    char buffer[32];
    char c;
    while ((c = *format++))
    {
        if (c != '%')
        {
            kputchar(c);
            continue;
        }

        c = *format++;

        switch (c)
        {
            case 'c':
            {
                char ch = (char) va_arg(args, int);
                kputchar(ch);
                break;
            }

            case 's':
            {
                char *s = va_arg(args, char *);
                kputs(s);
                break;
            }

            case 'b':
            {
                int val = va_arg(args, int);
                kitoa(val, buffer, 2);
                kputs(buffer);
                break;
            }

            case 'o':
            {
                int val = va_arg(args, int);
                kitoa(val, buffer, 8);
                kputs(buffer);
                break;
            }

            case 'd':
            {
                int val = va_arg(args, int);
                kitoa(val, buffer, 10);
                kputs(buffer);
                break;
            }

            case 'x':
            {
                int val = va_arg(args, int);
                kitoa(val, buffer, 16);
                kputs(buffer);
                break;
            }

            case '%':
                kputchar('%');
                break;

            default:
                kputchar('%');
                kputchar(c);
                break;
        }
    }
}

void kputchar(int c)
{
    vga_putchar(c, VGA_COLOR(VGA_WHITE, VGA_BLACK));
}

void kputs(const char *s)
{
    if (!s) return;

    vga_puts((char *) s, VGA_COLOR(VGA_WHITE, VGA_BLACK));
}

void kprintf(const char *restrict format, ...)
{
    if (!format) return;

    va_list args;
    va_start(args, format);
    kvprintf(format, args);
    va_end(args);
}

void kprint_log(LogTypes log, const char *restrict format, ...)
{
    if (!format) return;

    va_list args;
    va_start(args, format);

    switch ((int)log)
    {
        case LOG_INFO:
            vga_puts("[ INFO ] ", VGA_COLOR(VGA_YELLOW, VGA_BLACK));
            break;
        
        case LOG_WARN:
            vga_puts("[ WARN ] ", VGA_COLOR(VGA_LIGHT_RED, VGA_BLACK));
            break;

        case LOG_ERR:
            vga_puts("[ ERR ]  ", VGA_COLOR(VGA_RED, VGA_BLACK));
            break;

        case LOG_PANIC:
            vga_puts("[PANIC]  ", VGA_COLOR(VGA_MAGENTA, VGA_BLACK));
            break;

        default:
            break;
    }

    kvprintf(format, args);

    va_end(args);

    vga_puts("\n\r", VGA_COLOR(VGA_WHITE, VGA_BLACK));
}