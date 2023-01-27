#ifndef _NSCMEM_H_
#define _NSCMEM_H_

#include <stdint.h>

#ifndef ALLOCATOR_MEMORY_SIZE
#define ALLOCATOR_MEMORY_SIZE 0x100000 // 1MB
#endif

typedef struct allocator_t {
    uint_fast32_t* memory_start;
    uint_fast8_t memory[ALLOCATOR_MEMORY_SIZE];
} allocator_t;

// -------------------------------------------------------------------------------------

allocator_t     mem_init(void);
void*           mem_malloc(allocator_t*, uint_fast32_t);
void            mem_free(allocator_t*, void*);
void*           mem_copy(allocator_t*, void*, void*, uint_fast32_t);
void*           mem_realloc(allocator_t*, void*, uint_fast32_t);
uint_fast32_t   mem_size(allocator_t*);

// -------------------------------------------------------------------------------------

allocator_t mem_init(void) {
    allocator_t allocator;
    allocator.memory_start = (uint_fast32_t*)&allocator.memory;
    return allocator;
}

void* mem_malloc(allocator_t* allocator, uint_fast32_t size) {
    // if (!((size + allocator->memory) > (ALLOCATOR_MEMORY_SIZE + allocator->memory))) { 
    uint_fast32_t *ptr = allocator->memory_start;
    allocator->memory_start += size;
    return ptr;
    // }
}

void mem_free(allocator_t* allocator, void* ptr) {
    allocator->memory_start = (uint_fast32_t *)ptr;
}

void* mem_copy(allocator_t* allocator, void* src, void* dest, uint_fast32_t size) {
    char* dst8 = (char*)dest;
    char* src8 = (char*)src;

    while (size--) {
        *dst8++ = *src8++;
    }

    return dest;
}

void* mem_realloc(allocator_t* allocator, void* ptr, uint_fast32_t size) {
    void* new_ptr = mem_malloc(allocator, size);
    mem_copy(allocator, ptr, new_ptr, size);
    mem_free(allocator, ptr);
    return new_ptr;
}

uint_fast32_t mem_size(allocator_t* allocator) {
    return (uint_fast32_t)allocator->memory - (uint_fast32_t)allocator->memory_start;
}

#endif