#pragma once

#include <stdint.h>

/*
    Output a byte to a given I/O port

    Params:
        port - the I/O port to write to
        value - the byte to output
*/
static inline void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %0, %1" :: "a"(value), "Nd"(port));
}

/*
    Output a word to a given I/O port

    Params:
        port - the I/O port to write to
        value - the word to output
*/
static inline void outw(uint16_t port, uint16_t value)
{
    asm volatile ("outw %0, %1" :: "a"(value), "Nd"(port));
}

/*
    Output a double to a given I/O port

    Params:
        port - the I/O port to write to
        value - the double to output
*/
static inline void outl(uint16_t port, uint32_t value)
{
    asm volatile ("outl %0, %1" :: "a"(value), "Nd"(port));
}

/*
    Read a byte from a given I/O port

    Params:
        port - the I/O port to read

    Return:
        result - the read byte
*/
static inline uint8_t inb(uint16_t port)
{
    uint8_t result;
    asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

/*
    Read a word from a given I/O port

    Params:
        port - the I/O port to read

    Return:
        result - the read word
*/
static inline uint16_t inw(uint16_t port)
{
    uint16_t result;
    asm volatile ("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

/*
    Read a double from a given I/O port

    Params:
        port - the I/O port to read

    Return:
        result - the read double
*/
static inline uint32_t inl(uint32_t port)
{
    uint32_t result;
    asm volatile ("inl %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

/*
    Read a byte from an I/O port to the memory.

    Params:
        port - the port to read
        address - pointer to destination memory
        count - number of bytes to read
*/
static inline void insb(uint16_t port, void *address, int count)
{
    asm volatile ("rep insb" : "+D"(address), "+c"(count) : "d"(port) : "memory");
}

/*
    Read a word from an I/O port to the memory.

    Params:
        port - the port to read
        address - pointer to destination memory
        count - number of words to read
*/
static inline void insw(uint16_t port, void *address, int count)
{
    asm volatile ("rep insw" : "+D"(address), "+c"(count) : "d"(port) : "memory");
}

/*
    Read a double from an I/O port to the memory.

    Params:
        port - the port to read
        address - pointer to destination memory
        count - number of doubles to read
*/
static inline void insl(uint16_t port, void *address, int count)
{
    asm volatile ("rep insl" : "+D"(address), "+c"(count) : "d"(port) : "memory");
}

/*
    Read a byte from the memory to an I/O port.

    Params:
        port - the port to write to
        address - pointer to source memory
        count - number of bytes to read
*/
static inline void outsb(uint16_t port, void *address, int count)
{
    asm volatile ("rep outsb" : "+S"(address), "+c"(count) : "d"(port));
}

/*
    Read a word from the memory to an I/O port.

    Params:
        port - the port to write to
        address - pointer to source memory
        count - number of words to read
*/
static inline void outsw(uint16_t port, void *address, int count)
{
    asm volatile ("rep outsw" : "+S"(address), "+c"(count) : "d"(port));
}

/*
    Read a double from the memory to an I/O port.

    Params:
        port - the port to write to
        address - pointer to source memory
        count - number of doubles to read
*/
static inline void outsl(uint16_t port, void *address, int count)
{
    asm volatile ("rep outsl" : "+S"(address), "+c"(count) : "d"(port));
}