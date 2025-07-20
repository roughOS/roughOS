#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MIN_BLOCK_SIZE   16

#define ALIGN(x, a)      (((x) + ((a) - 1)) & ~((a) - 1))

typedef struct HeapBlockHeader
{
    size_t size;
    struct HeapBlockHeader *next;
    bool free;
} HeapBlockHeader;

void heap_init(uint32_t start, size_t size);
void *heap_malloc(size_t size);
void heap_free(void *ptr);