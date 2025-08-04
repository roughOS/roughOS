#pragma once

#include <stddef.h>
#include <sys/types.h>

void *kmemcpy(void *dest, const void *src, size_t n);
void *kmemmove(void *dest, const void *src, size_t n);
void *kmemset(void *s, int c, size_t n);
int kmemcmp(const void *s1, const void *s2, size_t n);
size_t kstrlen(const char *str);
int kstrcmp(const char *a, const char *b);
char *kstrcpy(char *dest, const char *src);
char *kstrncpy(char *dest, const char *src, size_t n);
char *kstrtok(char *str, const char *delim);
char *kstrchr(const char *s, int c);