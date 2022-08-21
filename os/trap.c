#include <stdint.h>
#include <stdbool.h>

#include <stdint.h>

extern void __switch_to(context_t* ctx);
extern void __alltraps(context_t* ctx);

#define REGISTER_SIZE (32)
#define STACK_SIZE (4096)

#define INTERRUPT_CHECK_BIT (((~0lu) >> 1) ^ (~0))

typedef enum {
    Machine = 0x3,
    Hypervisor = 0x2,
    Supervisor = 0x1,
    User = 0x0
} MPP_t;

typedef enum {
    UserSoftWare = 0x0,
    SupervisorSoftWare = 0x1,
    Rservered = 0x2,
    MachineSoftWare = 0x3,
    UnknownInterrupt
} mcause_int_t;

typedef enum {
    InstructionAddressMisaligned = 0x0,
    InstructionAccessFault = 0x1,
    StoreFault = 0x7,
    UnknownException
} mcause_excp_t;

typedef struct {
    union {
        mcause_excp_t exception;
        mcause_int_t interrupt;
    } cause;
    bool is_interrupt;
} mcause_t;

typedef void(*user_entry_t)(void);
typedef void(*trap_handler_t)(context_t* ctx);

extern void user_main(void);

static uint8_t USER_STACK[STACK_SIZE];
static uint8_t KERN_STACK[STACK_SIZE];
static uint8_t *USER_STACK_TOP =  USER_STACK + STACK_SIZE - 1;
static uint8_t *KERN_STACK_TOP =  KERN_STACK + STACK_SIZE - 1;

static context_t user_ctx;

static inline uint64_t read_sstatus()
{
    uint64_t ret;
    __asm__ volatile("csrrs %0, sstatus, x0": "=g"(ret));
    return ret;
}

static inline uint64_t read_mstatus()
{
    uint64_t ret;
    __asm__ volatile("csrrs %0, mstatus, x0": "=g"(ret));
    return ret;
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

// interruption enable bits
static inline void write_mie(reg_t x)
{
	__asm__ volatile("csrw mie, %0" : : "r" (x));
}

static inline uint64_t read_mie()
{
    uint64_t ret = 0;
    __asm__ volatile("csrr %0, mie\n":"=r"(ret));
    return ret;
}

static void set_cause_interrupt(mcause_t* cause, uint64_t interrupt_id)
{
    cause->cause.interrupt = (mcause_int_t)interrupt_id;
    switch(interrupt_id)
    {
        case UserSoftWare:
            printf("UserSoftWare interrupt !");
            break;
        default:
            printf("UnknownException !");
            break;
    }
}

static void set_cause_exception(mcause_t* cause, uint64_t exception_id)
{
    cause->cause.exception = (mcause_excp_t)exception_id;
    switch(exception_id)
    {
        case StoreFault:
            printf("StoreFault !");
            break;
        default:
            printf("UnknownException!");
            break;
    }
}

static void set_cause(mcause_t* cause, uint64_t mcause)
{
    bool is_interrupt = INTERRUPT_CHECK_BIT & mcause;
    uint64_t id = (~0lu >> 1) & mcause;
    cause->is_interrupt = is_interrupt;
    if (is_interrupt)
    {
        set_cause_interrupt(cause, id);
    }
    else
    {
        set_cause_exception(cause, id);
    }
    printf("[mcause is: 0x%lx]\n", mcause);
}

static inline mcause_t get_mcause()
{
    uint64_t mcause = 0;
    mcause_t ret;
    __asm__ volatile("csrr %0, mcause" : "=g"(mcause));
    set_cause(&ret, mcause);
    return ret;
}

static inline void set_sp(context_t* ctx, uint8_t* sp)
{
    ctx->x2.sp = (uint64_t)sp;
}

static inline void set_ra(context_t* ctx, user_entry_t ra)
{
    ctx->x1.ra = (uint64_t)ra;
}

static inline void set_mepc(context_t* ctx, user_entry_t pc)
{
    ctx->mpec = (uint64_t)pc;
}

static inline void set_mpp(context_t* ctx, MPP_t mpp)
{
    uint64_t tmp = (~0lu) ^ (11 << 11);
    ctx->mstatus = read_mstatus();
    ctx->mstatus &= tmp;
    ctx->mstatus |= mpp << 11;
}

void trap_init()
{
    write_mtvec(__alltraps);
    if (read_mtvec() != (uint64_t)__alltraps)
    {
        printf("[ERROR]set trap_handler failed !\n");
    }
}

void sche_init()
{
    uint64_t mie = read_mie() | 0xFFF;
    write_mie(mie);
    printf("mie: 0x%lx\n", read_mie());
}

void trap_handler(context_t* ctx) {
    printf("trap_handler run ! (%lx)\n", ctx);
    mcause_t cause = get_mcause();
    // printf("interrupt: %d, cause: %ld\n", cause.is_interrupt, cause.cause);
    while (true);
}

void memcpy(void* dst, const void* src, uint64_t len)
{
    for (uint64_t i = 0; i < len; i++)
    {
        *((uint8_t*)dst+i) = *((uint8_t*)src+i);
    }
}

void* get_first_ctx()
{
    set_ra(&user_ctx, user_main);
    set_sp(&user_ctx, USER_STACK_TOP);
    set_mpp(&user_ctx, User);
    set_mepc(&user_ctx, user_main);
    context_t* ctx = (context_t*)(KERN_STACK_TOP - sizeof(user_ctx));
    memcpy(ctx, &user_ctx, sizeof(user_ctx));
    return ctx;
}

void schedule()
{
    context_t* ctx = get_first_ctx();
    uint64_t mstatus = read_mstatus();
    printf("mstatus = 0x%lx\n", mstatus);
    printf("mstatus ctx = 0x%lx\n", ctx->mstatus);
	__switch_to(ctx);
}

void user_main(void)  {
    printf("user_main run !\n");
    while (true);
}
