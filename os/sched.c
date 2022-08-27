#include <stdbool.h>
#include <stdint.h>
#include "ctx.h"
#include "clint.h"

#define STACK_SIZE (512)
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
extern void shutdown();

void schedule();

static void task0();
static void task1();
static void task2();

static uint8_t stack_task[TASK_NUM][STACK_SIZE];

static context_t task_context[] = {
    {
        .sp = (uint64_t)(stack_task[0] + STACK_SIZE - 1),
        .mepc = (uint64_t)&task0
    },
    {
        .sp = (uint64_t)(stack_task[1] + STACK_SIZE - 1),
        .mepc = (uint64_t)&task1
    },
    {
        .sp = (uint64_t)(stack_task[2] + STACK_SIZE - 1),
        .mepc = (uint64_t)&task2
    }
};

typedef enum {
    Ready,
    Exit
} TASK_STATUS_T;

static uint8_t const num_of_tasks = sizeof(task_context) / sizeof(context_t);
static TASK_STATUS_T status[] = {Ready, Ready, Ready};
static uint8_t current_task = num_of_tasks - 1;
static uint64_t last_sp[] = {
    (uint64_t)(stack_task[0] + STACK_SIZE - 1),
    (uint64_t)(stack_task[1] + STACK_SIZE - 1),
    (uint64_t)(stack_task[2] + STACK_SIZE - 1),
};

static void task_delay(volatile int count)
{
	count *= 5000;
	while (count--);
}

static void task_yield()
{
    printf("task yield !\n");
    int id = read_tp();
	*(uint32_t*)CLINT_MSIP(id) = 1;
}

static void task0()
{
    uint64_t i = 0;
    while (i++ < 100)
    {
        printf("task 0 run start!\n");
        uint64_t sp = read_sp();
        printf("[SP] task0 sp: 0x%x, last sp:0x%x, read_sp: 0x%x \n", task_context[0].sp, last_sp[0], read_sp());
        task_delay(10000);
// #ifndef PREEMPTIVE_TEST
        // schedule();
        // task_yield();
// #endif // PREEMPTIVE_TEST
        printf("task 0 run stop !\n");
    }
    printf("task 0 run stop [i is %lu] \n", i);
}

static void task1()
{
    uint64_t i = 0;
    while (i++ < 100)
    {
        printf("task 1 run!\n");
        // printf("[SP] task1 sp: 0x%x, last sp:0x%x read_sp: 0x%x \n", task_context[1].sp, last_sp[1], read_sp());
        task_delay(10000);
#ifndef PREEMPTIVE_TEST
        schedule();
#endif // PREEMPTIVE_TEST
    }
    printf("task 1 run stop [i is %lu] \n", i);
}

static void task2()
{
#ifdef TRAP_TEST
    void trap_test();
#endif // TRAP_TEST
    uint64_t i = 0;
    while (i++ < 100)
    {
        printf("task 2 run!\n");
        printf("[SP] task2 sp: 0x%x, last sp:0x%x \n", task_context[2].sp, last_sp[2]);
        task_delay(10000);
#ifdef TRAP_TEST
        trap_test();
        // printf("return from exception !\n");
#endif // TRAP_TEST

#ifndef PREEMPTIVE_TEST
        schedule();
#endif // PREEMPTIVE_TEST
    }
    printf("task 2 run stop [i is %lu] \n", i);
}

bool find_next_task()
{
    for (uint64_t i = 0; i < num_of_tasks; i++)
    {
        current_task = (current_task + 1) % num_of_tasks;
        if (status[current_task] == Ready)
        {
            return true;
        }
    }
    return false;
}

void stop_cur_task_and_schedule()
{
    status[current_task] = Exit;
    schedule();
}

void sched_init(void)
{
    write_mscratch(0);
}

void schedule()
{
    printf("[%d, %d, %d]\n", status[0], status[1], status[2]);
    if (!find_next_task())
    {
        printf("all task finished\n");
        shutdown();
        return;
    }

    __switch_to(&task_context[current_task]);
}
