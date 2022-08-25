/**
 * https://github.com/riscv/riscv-aclint/blob/main/riscv-aclint.adoc
 */

// enum {
//     RISCV_ACLINT_DEFAULT_MTIMECMP      = 0x0,
//     RISCV_ACLINT_DEFAULT_MTIME         = 0x7ff8,
//     RISCV_ACLINT_DEFAULT_MTIMER_SIZE   = 0x8000,
//     RISCV_ACLINT_DEFAULT_TIMEBASE_FREQ = 10000000,
//     RISCV_ACLINT_MAX_HARTS             = 4095,
//     RISCV_ACLINT_SWI_SIZE              = 0x4000
// };

/* CLINT timebase frequency */
#define CLINT_TIMEBASE_FREQ (10000000)

#define CLINT_BASE 0x2000000L
#define CLINT_MSIP(hartid) (CLINT_BASE + 4 * (hartid))
#define CLINT_MTIMECMP(hartid) (CLINT_BASE + 0x4000 + 8*(hartid))
#define CLINT_MTIME (CLINT_BASE + 0xBFF8) // cycles since boot.