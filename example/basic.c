#include "../include/nscmem.h"
#include "../include/nscstr.h"
#include <stdio.h>
#include <stdint.h>

int main(void) {
    allocator_t allocator = mem_init();

    str_t string = str_from("hello ", &allocator);

    str_append(&string, "world");
    printf("%s\n", string.chars);

    printf("%s", str_to_cstring((str_split_first(&string, ' '))));

    // int* p = mem_malloc(&allocator, 10);

    // p[4] = 10;
    // printf("%d", p[4]);

    return 0;
}