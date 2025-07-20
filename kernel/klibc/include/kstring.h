#pragma once

#include <stddef.h>

/*
    Copy bytes from a source to a destination

    Params:
        dest - the destination
        src - the source
        n - the number of bytes to copy

    Return:
        A pointer to dest
*/
void *kmemcpy(void *dest, const void *src, size_t n);

/*
    Move bytes from a source to a destination (safe for overlapping regions)

    Params:
        dest - the destination
        src - the source
        n - the number of bytes to move

    Return:
        A pointer to dest
*/
void *kmemmove(void *dest, const void *src, size_t n);

/*
    Set a block of memory to a specific byte value

    Params:
        s - the memory block to fill
        c - the byte value to set
        n - the number of bytes to set

    Return:
        A pointer to s
*/
void *kmemset(void *s, int c, size_t n);

/*
    Compare two blocks of memory

    Params:
        s1 - first memory block
        s2 - second memory block
        n  - number of bytes to compare

    Return:
        0 if equal, <0 if s1 < s2, >0 if s1 > s2
*/
int kmemcmp(const void *s1, const void *s2, size_t n);