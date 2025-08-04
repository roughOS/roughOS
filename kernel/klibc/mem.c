#include "klibc/include/kstring.h"
#include "klibc/include/kstdlib.h"
#include "mm/include/heap.h"
#include <stddef.h>
#include <stdint.h>

void *kmemcpy(void *dest, const void *src, size_t n)
{
    if (!dest || !src) return NULL;

    uint8_t *pdest = (uint8_t *)dest;
    if (!pdest) return NULL;

    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++)
        pdest[i] = psrc[i];

    return dest;
}

void *kmemmove(void *dest, const void *src, size_t n)
{
    if (!dest || !src) return NULL;

    uint8_t *pdest = (uint8_t *)dest;
    if (!pdest) return NULL;

    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest)
        for (size_t i = 0; i < n; i++)
            pdest[i] = psrc[i];
    else if (src < dest)
        for (size_t i = n; i > 0; i--)
            pdest[i-1] = psrc[i-1];

    return dest;
}

void *kmemset(void *s, int c, size_t n)
{
    if (!s) return NULL;

    uint8_t *p = (uint8_t *)s;
    if (!p) return NULL;

    for (size_t i = 0; i < n; i++)
        p[i] = (uint8_t)c;

    return s;
}

int kmemcmp(const void *s1, const void *s2, size_t n)
{
    if (!s1 || !s2) return NULL;

    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;
    if (!p1 || !p2) return NULL;

    for (size_t i = 0; i < n; i++)
        if (p1[i] != p2[i])
            return p1[i] < p2[i] ? -1 : 1;

    return 0;
}

void *kmalloc(size_t size)
{
    return heap_malloc(size);
}

void *kcalloc(size_t nmemb, size_t size)
{
    size_t total = nmemb * size;
    void *ptr = kmalloc(total);
    if (!ptr)
        return NULL;
    kmemset(ptr, 0, total);
    return ptr;
}

void kfree(void *ptr)
{
    if (!ptr) return;

    heap_free(ptr);
}