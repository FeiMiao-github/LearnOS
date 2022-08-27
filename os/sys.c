#include <stdint.h>

#define TEST (0x100000)
#define SHUTDOWN_VALUE (0x5555)

void shutdown()
{
    *(uint32_t*)TEST = SHUTDOWN_VALUE;
}