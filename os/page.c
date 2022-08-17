#include "printf.h"
#include "type.h"
#include <stdint.h>

extern uint64_t _memory_start;
extern uint64_t _memory_end;
extern uint64_t _heap_start;
extern uint64_t _heap_size;
extern uint64_t _bss_start;
extern uint64_t _bss_end;
extern uint64_t etext;

void clear_bss()
{
    for (uint64_t* start = &_bss_start; start < &_bss_end; start++)
    {
        *start = 0;
    }
}

int page_init()
{
    printf("_memory_start = 0x%lx\n", &_memory_start);
    printf("_memory_end   = 0x%lx\n", &_memory_end);
    printf("_heap_start   = 0x%lx\n", &_heap_start);
    printf("_heap_end     = 0x%lx\n", (uint64_t)(&_heap_start) + (uint64_t)(&_heap_size));
    printf("_heap_size    = 0x%lx\n", (uint64_t)&_heap_size);
    printf("etext         = 0x%lx\n", &etext);
    printf("_bss_start    = 0x%lx\n", &_bss_start);
    printf("_bss_end      = 0x%lx\n", (uint64_t)&_bss_end);

    return 0;
}