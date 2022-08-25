#include "ctx.h"
#include "riscv.h"
#include "uart.h"

#define MAX_EXCEPTIONS (16)
#define MAX_INTERRUPTS (12)

extern void __alltraps(void);
extern int printf(const char* s, ...);
extern uint32_t plic_claim(void);
extern void plic_complete(uint32_t);
extern void uart_isr(void);;

static void print_exception(uint64_t mcause);
static void print_interrupt(uint64_t mcause);
static void handle_external_interrupt();

static uint64_t* ptmp = 0;
static uint64_t tmp = 0;

const char* const EXCEPTIONS[MAX_EXCEPTIONS] = {
    "Instruction address misaligned", // 0
    "Instruction access fault", // 1
    "Illegal instruction", // 2
    "Breakpoint", // 3
    "Load address misaligned", // 4
    "Load access fault", // 5
    "Store/AMO address misaligned", // 6
    "Store/AMO access fault", // 7
    "Environment call from U-mode", // 8
    "Environment call from S-mode", // 9
    "Reserved", // 10
    "Environment call from M-mode", // 11
    "Instruction page fault", // 12
    "Load page fault", // 13
    "Reserved", // 14
    "Store/AMO page fault", // 15
};

const char* const INTERRUPTS[MAX_INTERRUPTS] = {
    "Reserved", // 0
    "Supervisor software interrupt", // 1
    "Reserved", // 2
    "Machine software interrupt", // 3
    "Reserved", // 4
    "Supervisor timer interrupt", // 5
    "Reserved", // 6
    "Machine timer interrupt", // 7
    "Reserved", // 8
    "Supervisor external interrupt", // 9
    "Reserved", // 10
    "Machine external interrupt", // 11
};

void trap_init()
{
    write_mtvec(__alltraps);
}

void trap_handler(uint64_t mcause, context_t* ctx)
{
    if (INTERRUPT_BIT & mcause)
    {
        print_interrupt((~INTERRUPT_BIT) & mcause);
        uint16_t cause_code = mcause & 0xFFF;
        switch(cause_code)
        {
            case 11: // external interrupt
                handle_external_interrupt();
                break;
        }
    }
    else
    {
        print_exception(mcause);
        ptmp = &tmp;
        ctx->a5 = (uint64_t)ptmp;
    }
}

#ifdef TRAP_TEST
void trap_test()
{
    *ptmp = 100;
    // printf("back from trap ! (0x%lx)\n", *ptmp);
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

static void print_interrupt(uint64_t mcause)
{
    if (mcause >= MAX_INTERRUPTS)
    {
        printf("unknown interrupt code: 0x%lx\n", mcause);
    }
    else
    {
        printf("%s (code: 0x%lx)\n", INTERRUPTS[mcause], mcause);
    }
}

static void handle_external_interrupt()
{
    uint32_t ireq = plic_claim();
    printf("handle_external_interrupt: 0x%lx\n", ireq);
    switch(ireq)
    {
        case PLIC_UART0:
            uart_isr();
            break;
        default:
            printf("unknown external interrupt code: 0x%lx\n", ireq);
            break;
    }
    if (ireq)
    {
        plic_complete(ireq);
    }
}