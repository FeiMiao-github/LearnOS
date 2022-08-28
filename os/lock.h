#ifndef LOCK_H__
#define LOCK_H__

#include <stdint.h>

typedef struct {
    volatile uint64_t is_lock;
} spin_lock_t;

#endif // LOCK_H__