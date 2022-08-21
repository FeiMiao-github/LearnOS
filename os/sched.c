#include "trap.h"
#include "type.h"
#include "ctx.h"

#define STACK_SIZE (4096)
#define TASK_NUM   (2)

typedef struct _stack_t {
    uint8_t* base;
    uint8_t* top;
} stack_t;

typedef struct {
    void (*func)(void);
    stack_t* stack;
} task_t;

extern int printf(const char* s, ...);
extern void __switch_to(void* ctx);

static void task0();
static void task1();

static uint8_t stack_task[TASK_NUM][STACK_SIZE];

static context_t task_context[] = {
    {
        .x1.ra = (uint64_t)&task0,
        .x2.sp = (uint64_t)(stack_task[0] + STACK_SIZE - 1)
        
    },
    {
        .x1.ra =  (uint64_t)&task1,
        .x2.sp = (uint64_t)(stack_task[1] + STACK_SIZE - 1)
    }
};

static const uint8_t num_of_tasks = sizeof(task_context) / sizeof(context_t);
static uint8_t current_task = num_of_tasks - 1;

static void task0()
{
    while (true)
    {
        printf("task 0 run !\n");
        schedule();
    }
}

static void task1()
{
    while (true)
    {
        printf("task 1 run!\n");
        schedule();
    }
}

void sched_init(void)
{
    write_mscratch(0);
}

void schedule()
{
    printf("[SCHED] scheduling task !\n");
    current_task = (current_task + 1) % num_of_tasks;
    __switch_to(&task_context[current_task]);
}
