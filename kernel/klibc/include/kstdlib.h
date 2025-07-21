#pragma once

#include <stddef.h>

/*
    Convert an integer into ASCII

    Params:
        i - the integer to convert
        buffer - the buffer where the ASCII will be stored
        base - binary, octal, decimal, hexadecimal...
*/
void kitoa(int i, char *buffer, int base);

/*
    Allocate memory on the heap

    Params:
        size - the size of the allocated space
*/
void *kmalloc(size_t size);

/*
    Allocate memory on the heap and set to 0

    Params:
        nmemb - number of elements to allocate
        size  - size of each element in bytes
*/
void *kcalloc(size_t nmemb, size_t size);

/*
    Free allocated memory on the heap

    Params:
        ptr - the pointer to the allocated block to free
*/
void kfree(void *ptr);