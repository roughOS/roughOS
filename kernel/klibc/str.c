#include "klibc/include/kstring.h"
#include <stddef.h>
#include <string.h>

size_t kstrlen(const char *str)
{
    if (!str)
        return 0;

    size_t length = 0;
    while (str[length] != '\0')
        length += 1;

    return length;
}

int kstrcmp(const char *a, const char *b)
{
    if (!a || !b)
        return -1;

    while (*a && (*a == *b))
    {
        a++;
        b++;
    }

    return *(unsigned char *)a - *(unsigned char *)b;
}

char *kstrcpy(char *dest, const char *src)
{
    if (!dest || !src)
        return NULL;

    char *ret = dest;

    while ((*dest++ = *src++));

    return ret;
}

char *kstrncpy(char *dest, const char *src, size_t n)
{
    if (!dest || !src)
        return NULL;

    size_t i = 0;
    for (; i < n && src[i]; i++)
        dest[i] = src[i];

    for (; i < n; i++)
        dest[i] = '\0';

    return dest;
}

char *kstrchr(const char *s, int c)
{
    if (!s)
        return NULL;

    while (*s)
    {
        if (*s == (char)c)
            return (char *)s;
        s++;
    }

    return NULL;
}

char *kstrtok(char *str, const char *delim)
{
    static char *next = NULL;

    if (str)
        next = str;
    if (!next || !*next)
        return NULL;

    while (*next && kstrchr(delim, *next))
        next++;

    if (!*next)
        return NULL;

    char *start = next;

    while (*next && !kstrchr(delim, *next))
        next++;

    if (*next) {
        *next = '\0';
        next++;
    } else {
        next = NULL;
    }

    return start;
}