#include "mm/include/heap.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static HeapBlockHeader *free_list = NULL;

void heap_init(uint32_t start, size_t size)
{
    free_list = (HeapBlockHeader *) start;
    free_list->size = size - sizeof(HeapBlockHeader);
    free_list->next = NULL;
    free_list->free = true;
}

void *heap_malloc(size_t size)
{
    size = ALIGN(size, 4);

    HeapBlockHeader *current = free_list;
    HeapBlockHeader *previous = NULL;

    while (current)
    {
        if (current->free && current->size >= size)
        {
            if (current->size >= size + sizeof(HeapBlockHeader) + MIN_BLOCK_SIZE)
            {
                HeapBlockHeader *new_block = (HeapBlockHeader *)((uint8_t *)(current + 1) + size);
                new_block->size = current->size - size - sizeof(HeapBlockHeader);
                new_block->free = true;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }

            current->free = false;
            return (void *)(current + 1);
        }

        previous = current;
        current = current->next;
    }

    return NULL;
}

void heap_free(void *ptr)
{
    if (!ptr)
        return;

    HeapBlockHeader *block = (HeapBlockHeader *) ptr - 1;
    block->free = true;

    HeapBlockHeader *current = free_list;
    while (current && current->next)
    {
        if (current->free && current->next->free)
        {
            current->size += sizeof(HeapBlockHeader) + current->next->size;
            current->next = current->next->next;
        }
        else
        {
            current = current->next;
        }
    }
}