#pragma once

typedef enum
{
    LOG_INFO,
    LOG_WARN,
    LOG_ERR,
    LOG_PANIC
} LogTypes;

/*
    Display a character on the screen

    Params:
        c - the character to print
*/
void kputchar(int c);

/*
    Display a string on the screen

    Params:
        s - the string to print
*/
void kputs(const char *s);

/*
    Display a formated string to the screen

    Params:
        format - the string to format
        ... - the values corresponding to the format symbols

    Notes:
        1. Supported formats:
            %c - character
            %s - string
            %b - binary
            %o - octal
            %d - decimal
            %x - hexadecimal
            %% - percent character
*/
void kprintf(const char *restrict format, ...);

/*
    Display a formated log

    Params:
        type - the type of the log
        format - the string to format
        ... - the values corresponding to the format symbols
*/
void kprint_log(LogTypes type, const char *restrict format, ...);