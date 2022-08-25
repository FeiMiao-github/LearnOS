#include "ctx.h"
#include "clint.h"
#include "riscv.h"
#include <stdint.h>

#define TIMER_INTERVAL (CLINT_TIMEBASE_FREQ)

extern void printf(const char *s, ...);

static void timer_load(uint64_t interval);

static uint64_t tick = 0;

void timer_init()
{
    timer_load(TIMER_INTERVAL);
}

static void timer_load(uint64_t interval)
{
    uint64_t hartid = read_tp();
    *(uint64_t*)CLINT_MTIMECMP(hartid) = *(uint64_t*)CLINT_MTIME + interval;

    // enable machine-mode timer interrupt
	write_mie(read_mie() | MIE_MTIE);
}

void timer_handler() 
{
    tick++;
	printf("tick: %d\n", tick);
	timer_load(TIMER_INTERVAL);
}
