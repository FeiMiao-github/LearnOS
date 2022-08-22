#include "ctx.h"
#include "riscv.h"
#include "type.h"
#include <stdint.h>

#define MAX_EXCEPTIONS (8)

extern void __alltraps(void);
extern int printf(const char* s, ...);

static void print_exception(uint64_t mcause);

static uint64_t* ptmp = 0;
static uint64_t tmp = 0;

const char* EXCEPTIONS[MAX_EXCEPTIONS] = {
    "UNKNOWN", // 0
    "UNKNOWN", // 1
    "UNKNOWN", // 2
    "UNKNOWN", // 3
    "UNKNOWN", // 4
    "UNKNOWN", // 5
    "UNKNOWN", // 6
    "Store/AMO access fault" // 7
};

void trap_init()
{
    write_mtvec(__alltraps);
}

void trap_handler(uint64_t mcause, context_t* ctx)
{
    if (INTERRUPT_BIT & mcause)
    {
        // printf("interrupt code: 0x%lx\n", (~INTERRUPT_BIT) & mcause);
    }
    else
    {
        print_exception(mcause);
        ptmp = &tmp;
        ctx->x15.a5 = (uint64_t)ptmp;
    }
}

#ifdef TRAP_TEST
void trap_test()
{
    *ptmp = 100;
    printf("back from trap ! (0x%lx)\n", *ptmp);
}
#endif // TRAP_TEST

static void print_exception(uint64_t mcause)
{
    if (mcause >= MAX_EXCEPTIONS)
    {
        printf("unknown exception code: 0x%lx\n", mcause);
    }
    else
    {
        printf("%s (code: 0x%lx)\n", EXCEPTIONS[mcause], mcause);
    }
}