#ifndef _NSCSTR_H_
#define _NSCSTR_H_

#include "nscmem.h"
#include <stdio.h>

typedef struct str_t {
    uint16_t size;
    uint16_t capacity;
    allocator_t* allocator;
    char* chars;
} str_t;

// -------------------------------------------------------------------------------------

str_t   str_from(const char*, allocator_t*);
char*   str_to_cstring(str_t);
void    str_append(str_t*, const char*);
void    str_remove(str_t*, const char*);
str_t   str_split_first(str_t*, const char);

// -------------------------------------------------------------------------------------

uint16_t cstring_length(const char* c_string) {
    uint16_t length = 0;
    while (c_string[length] != '\0') length++;
    return length;
}

str_t str_from(const char* c_string, allocator_t* allocator) {
    str_t string = {
        .size = cstring_length(c_string),
        .capacity = 2 * cstring_length(c_string),
        .allocator = allocator
    };

    string.chars = (char*)mem_malloc(allocator, string.size);
    mem_copy(allocator, (void *)c_string, (void *)string.chars, string.size);
    return string;
}

char* str_to_cstring(str_t string) {
    return string.chars;
}

void str_append(str_t* string, const char* str_a) {
    uint16_t str_a_length = cstring_length(str_a);
    if (string->size + str_a_length > string->capacity) {
        string->capacity = 2 * (string->size + str_a_length);
        string->chars = (char*)mem_realloc(string->allocator, (void *)string->chars, string->capacity);
    }

    mem_copy(string->allocator, (void *)str_a, (void *)(string->chars + string->size), string->size);
    string->size += str_a_length;
}

void str_remove(str_t* string, const char* str_a) {
    uint16_t str_a_length = cstring_length(str_a);
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t k = 0;
    char* new_string = (char*)mem_malloc(string->allocator, string->size - str_a_length);

    while (i < string->size) {
        if (string->chars[i] == str_a[j]) {
            j++;
            if (j == str_a_length) {
                i++;
                j = 0;
                continue;
            }
        } else {
            new_string[k] = string->chars[i];
            k++;
        }
        i++;
    }

    string->size -= str_a_length;
    string->chars = new_string;
}

str_t str_split_first(str_t* string, const char delimiter) {
    str_t returner;
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t k = 0;
    char* new_string = (char*)mem_malloc(string->allocator, string->size);

    while (i < string->size) {
        if (string->chars[i] == delimiter) {
            returner.chars = new_string;
            returner.size = k;
            returner.capacity = k;
            returner.allocator = string->allocator;
            return returner;
        } else {
            new_string[k] = string->chars[i];
            k++;
        }
        i++;
    }
}

#endif