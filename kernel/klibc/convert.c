#include "klibc/include/kstdlib.h"

void kitoa(int i, char *buffer, int base)
{
    if (!buffer) return;

    if (base < 2 || base > 16)
    {
        buffer[0] = '?';
        buffer[1] = '\0';
        return;
    }

    char *ptr = buffer;
    if (!ptr) return;
    char *ptr1 = buffer;
    if (!ptr1) return;

    char tmp_char;
    int tmp_val;
    int is_negative = 0;

    if (i == 0)
    {
        *ptr++ = '0';
        *ptr = '\0';
        return;
    }

    if (i < 0 && base == 10)
    {
        is_negative = 1;
        i = -i;
    }

    while (i)
    {
        tmp_val = i % base;
        *ptr++ = (tmp_val < 10) ? (tmp_val + '0') : (tmp_val - 10 + 'a');
        i /= base;
    }

    if (is_negative)
        *ptr++ = '-';

    *ptr = '\0';

    ptr1 = buffer;
    char *end = ptr - 1;
    if (!end) return;

    while (ptr1 < end)
    {
        tmp_char = *ptr1;
        *ptr1 = *end;
        *end = tmp_char;
        ptr1++;
        end--;
    }
}