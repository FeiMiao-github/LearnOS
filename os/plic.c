
#include <stdint.h>
#include "ctx.h"
#include "riscv.h"
#include "uart.h"

/* Plic Memory Map */
#define PLIC_BASE                   (0xc000000lu)

/* 
    0: Reserved, interrupt source 0 does not exist
    1 ~ 1023:  Interrupt source 1 ~ 1023 priority
*/
#define PLIC_PRIORITY_MIN           (0x0)
#define PLIC_PRIORITY_MAX           (0x7)
// #define PLIC_PRIORITY(id)           ((uint32_t*)(PLIC_BASE + ((id) << 2)))

// /*
//      Interrupt Pending Bits
// */
// #define PLIC_PENDING(id)            ((uint32_t*)((PLIC_BASE + 0x1000) | ((id) >> 5)))

// /*
//     Interrupt Enables Bits
// */
// #define PLIC_MENABLE(hart,id)       ((uint32_t*)((PLIC_BASE + 0x2000 + (hart) * 0x80) | ((id) >> 5)))

// /*
//     Priority Thresholds
// */
// #define PLIC_THRESHOLD(hart)        ((uint32_t*)(PLIC_BASE + 0x200000 + (hart << 12)))

/*
    Interrupt Claim Process
*/
#define PLIC_MCLAIM(hart)           (PLIC_BASE + 0x200004 + (hart) * 0x2000)

/*
    Interrupt Completion
*/
#define PLIC_MCOMPLETE(hart)        (PLIC_BASE + 0x200004 + (hart) * 0x2000)

extern void printf(const char *s, ...);

static void set_plic_prority(uint16_t id, uint32_t proprity)
{
    if (proprity >= PLIC_PRIORITY_MIN && proprity <= PLIC_PRIORITY_MAX)
    {
        uint32_t* addr = (uint32_t*)(PLIC_BASE + (id << 2));
        *addr = proprity;
    }
}

static void set_plic_enable(uint16_t id, uint32_t hart)
{
    uint32_t* addr = (uint32_t*)(PLIC_BASE + 0x2000);
    *addr |= (1 << id);
}

static void set_plic_threshold(uint32_t hart, uint32_t proprity)
{
    if (proprity >= PLIC_PRIORITY_MIN && proprity <= PLIC_PRIORITY_MAX)
    {
        uint32_t* addr = (uint32_t*)(PLIC_BASE + 0x200000);
        *addr = proprity;
    }
}

void plic_init(void)
{
    uint64_t hart = read_tp();

    /* set uart priority */
    *(uint32_t*)(PLIC_BASE + PLIC_UART0*4) = 1;
    
    /* enable uart interrupt */
    set_plic_enable(PLIC_UART0, hart);

    set_plic_threshold(hart, 0);

    /* enable machine-mode external interrupts. */
	write_mie(read_mie() | MIE_MEIE);
	/* enable machine-mode global interrupts. */
	write_mstatus(read_mstatus() | MSTATUS_MIE);

    printf("[PLIC] UART0 priority: 0x%x\n", *(uint32_t*)(PLIC_BASE + PLIC_UART0*4));
    printf("[PLIC] UART0 enable: 0x%x\n", (*(uint32_t*)0xc002000));
    printf("[PLIC] UART0 threshold: 0x%x\n", *(uint32_t*)(0xc200000));
    printf("[PLIC] mie: 0x%x\n", read_mie());
    printf("[PLIC] mstatus: 0x%x\n", read_mstatus());
}

uint32_t plic_claim(void)
{
	uint64_t hart = read_tp();
	uint32_t irq = *(uint32_t*)PLIC_MCLAIM(hart);
	return irq;
}

void plic_complete(uint32_t irq)
{
	uint64_t hart = read_tp();
	*(uint32_t*)PLIC_MCOMPLETE(hart) = irq;
}

