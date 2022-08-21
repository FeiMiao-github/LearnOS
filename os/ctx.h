#ifndef CTX_H__
#define CTX_H__

#include <stdint.h>

#define DEF_UNION_REG_ARGUMENTS(ri, ai) \
    typedef union { \
        uint64_t x##ri;\
        uint64_t a##ai; \
    } x ## ri ##_t

#define DEF_UNION_REG_SAVED(ri, si) \
    typedef union { \
        uint64_t x##ri; \
        uint64_t s##si; \
    } x ## ri ## _t

#define DEF_UNION_REG_TMP(ri, ti) \
    typedef union { \
        uint64_t x##ri; \
        uint64_t t##ti; \
    } x##ri##_t

typedef union {
    uint64_t x0;
    uint64_t zero;
} x0_t;

typedef union {
    uint64_t x1;
    uint64_t ra;
} x1_t;

typedef union {
    uint64_t x2;
    uint64_t sp;
} x2_t;

typedef union {
    uint64_t x3;
    uint64_t gp;
} x3_t;

typedef union {
    uint64_t x4;
    uint64_t tp;
} x4_t;

DEF_UNION_REG_TMP(5, 0);
DEF_UNION_REG_TMP(6, 1);
DEF_UNION_REG_TMP(7, 2);

typedef union {
    uint64_t x8;
    uint64_t s0;
    uint64_t fp;
} x8_t;

DEF_UNION_REG_SAVED(9, 1);
DEF_UNION_REG_ARGUMENTS(10, 0);
DEF_UNION_REG_ARGUMENTS(11, 1);
DEF_UNION_REG_ARGUMENTS(12, 2);
DEF_UNION_REG_ARGUMENTS(13, 3);
DEF_UNION_REG_ARGUMENTS(14, 4);
DEF_UNION_REG_ARGUMENTS(15, 5);
DEF_UNION_REG_ARGUMENTS(16, 6);
DEF_UNION_REG_ARGUMENTS(17, 7);
DEF_UNION_REG_SAVED(18, 2);
DEF_UNION_REG_SAVED(19, 3);
DEF_UNION_REG_SAVED(20, 4);
DEF_UNION_REG_SAVED(21, 5);
DEF_UNION_REG_SAVED(22, 6);
DEF_UNION_REG_SAVED(23, 7);
DEF_UNION_REG_SAVED(24, 8);
DEF_UNION_REG_SAVED(25, 9);
DEF_UNION_REG_SAVED(26, 10);
DEF_UNION_REG_SAVED(27, 11);
DEF_UNION_REG_TMP(28, 3);
DEF_UNION_REG_TMP(29, 4);
DEF_UNION_REG_TMP(30, 5);
DEF_UNION_REG_TMP(31, 6);

typedef struct {
    x0_t x0;
    x1_t x1;
    x2_t x2;
    x3_t x3;
    x4_t x4;
    x5_t x5;
    x6_t x6;
    x7_t x7;
    x8_t x8;
    x9_t x9;
    x10_t x10;
    x11_t x11;
    x12_t x12;
    x13_t x13;
    x14_t x14;
    x15_t x15;
    x16_t x16;
    x17_t x17;
    x18_t x18;
    x19_t x19;
    x20_t x20;
    x21_t x21;
    x22_t x22;
    x23_t x23;
    x24_t x24;
    x25_t x25;
    x26_t x26;
    x27_t x27;
    x28_t x28;
    x29_t x29;
    x30_t x30;
    x31_t x31;

    uint64_t mstatus;
    uint64_t mpec;
} context_t;

static inline void write_mscratch(uint64_t mscratch)
{
    __asm__ volatile("csrw mscratch, %0"::"r"(mscratch));
}

#endif // CTX_H__