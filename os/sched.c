#include <stdbool.h>
#include "ctx.h"

#define STACK_SIZE (4096)
#define TASK_NUM   (3)

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
void schedule();

static void task0();
static void task1();
static void task2();

static uint8_t stack_task[TASK_NUM][STACK_SIZE];

static context_t task_context[] = {
    {
        .ra = (uint64_t)&task0,
        .sp = (uint64_t)(stack_task[0] + STACK_SIZE - 1)
        
    },
    {
        .ra =  (uint64_t)&task1,
        .sp = (uint64_t)(stack_task[1] + STACK_SIZE - 1)
    },
    {
        .ra =  (uint64_t)&task2,
        .sp = (uint64_t)(stack_task[2] + STACK_SIZE - 1)
    }
};

static const uint8_t num_of_tasks = sizeof(task_context) / sizeof(context_t);
static uint8_t current_task = num_of_tasks - 1;

static void task_delay(volatile int count)
{
	count *= 5000;
	while (count--);
}

static void task0()
{
    while (true)
    {
        printf("task 0 run !\n");
        task_delay(10000);
        schedule();
    }
}

static void task1()
{
    while (true)
    {
        printf("task 1 run!\n");
        task_delay(10000);
        schedule();
    }
}

static void task2()
{
#ifdef TRAP_TEST
    void trap_test();
#endif // TRAP_TEST
    while (true)
    {
        printf("task 2 run!\n");
        task_delay(10000);
#ifdef TRAP_TEST
        trap_test();
#endif // TRAP_TEST
        printf("return from exception !\n");
        schedule();
    }
}

void sched_init(void)
{
    write_mscratch(0);
}

void schedule()
{
    current_task = (current_task + 1) % num_of_tasks;
    __switch_to(&task_context[current_task]);
}
