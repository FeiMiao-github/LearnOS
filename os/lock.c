#include <stdint.h>
#include "lock.h"

spin_lock_t lock = {0};

volatile spin_lock_t* init_lock()
{
    return &lock;
}

void acquire_lock(volatile spin_lock_t* lock)
{
    while (__sync_lock_test_and_set(&lock->is_lock, 1) != 0);
}

void release_lock(volatile spin_lock_t* lock)
{
    lock->is_lock = 0;
}