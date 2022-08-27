#ifndef CTX_H__
#define CTX_H__

#include <stdint.h>

typedef struct {
    union {
        uint64_t x0;
        uint64_t zero;
    };
    union {
        uint64_t x1;
        uint64_t ra;
    };
    union {
        uint64_t x2;
        uint64_t sp;
    };
    union {
        uint64_t x3;
        uint64_t gp;
    };
    union {
        uint64_t x4;
        uint64_t tp;
    };
    union {
        uint64_t x5;
        uint64_t t0;
    };
    union {
        uint64_t x6;
        uint64_t t1;
    };
    union {
        uint64_t x7;
        uint64_t t2;
    };
    union {
        uint64_t x8;
        uint64_t s0;
        uint64_t fp;
    };
    union {
        uint64_t x9;
        uint64_t s1;
    };
    union {
        uint64_t x10;
        uint64_t a0;
    };
    union {
        uint64_t x11;
        uint64_t a1;
    };
    union {
        uint64_t x12;
        uint64_t a2;
    };
    union {
        uint64_t x13;
        uint64_t a3;
    };
    union {
        uint64_t x14;
        uint64_t a4;
    };
    union {
        uint64_t x15;
        uint64_t a5;
    };
    union {
        uint64_t x16;
        uint64_t a6;
    };
    union {
        uint64_t x17;
        uint64_t a7;
    };
    union {
        uint64_t x18;
        uint64_t s2;
    };
    union {
        uint64_t x19;
        uint64_t s3;
    };
    union {
        uint64_t x20;
        uint64_t s4;
    };
    union {
        uint64_t x21;
        uint64_t s5;
    };
    union {
        uint64_t x22;
        uint64_t s6;
    };
    union {
        uint64_t x23;
        uint64_t s7;
    };
    union {
        uint64_t x24;
        uint64_t s8;
    };
    union {
        uint64_t x25;
        uint64_t s9;
    };
    union {
        uint64_t x26;
        uint64_t s10;
    };
    union {
        uint64_t x27;
        uint64_t s11;
    };
    union {
        uint64_t x28;
        uint64_t t3;
    };
    union {
        uint64_t x29;
        uint64_t t4;
    };
    union {
        uint64_t x30;
        uint64_t t5;
    };
    union {
        uint64_t x31;
        uint64_t t6;
    };
    uint64_t mstatus;
    uint64_t mepc;
} context_t;

typedef void(*trap_handler_t)();

static inline void write_mscratch(uint64_t mscratch)
{
    __asm__ volatile("csrw mscratch, %0"::"r"(mscratch));
}

static inline void write_mtvec(trap_handler_t trap_handler)
{
	__asm__ volatile("csrw mtvec, %0" :: "r" (trap_handler));
}

static inline uint64_t read_mtvec()
{
    uint64_t ret = 0;
    __asm__ volatile("csrr %0, mtvec" : "=g"(ret));
    return ret;
}

static inline uint64_t read_tp()
{
	uint64_t x;
	asm volatile("mv %0, tp" : "=r" (x) );
	return x;
}

/* which hart (core) is this? */
static inline uint64_t read_mhartid()
{
	uint64_t x;
	asm volatile("csrr %0, mhartid" : "=r" (x) );
	return x;
}

// interruption enable bits
static inline void write_mie(uint64_t x)
{
	__asm__ volatile("csrw mie, %0" : : "r" (x));
}

static inline uint64_t read_mie()
{
    uint64_t ret = 0;
    __asm__ volatile("csrr %0, mie\n":"=r"(ret));
    return ret;
}

static inline void write_mstatus(uint64_t status)
{
    __asm__ volatile("csrrw x0, mstatus, %0":: "r"(status));
}

static inline uint64_t read_mstatus()
{
    uint64_t ret;
    __asm__ volatile("csrrs %0, mstatus, x0": "=g"(ret));
    return ret;
}

static inline uint64_t read_sp()
{
    uint64_t ret;
    __asm__ volatile("mv %0, sp": "=g"(ret));
    return ret;
}

static inline void fence()
{
    __asm__ volatile("fence");
}

#endif // CTX_H__