#include "ctx.h"
#include "riscv.h"
#include "clint.h"

void sip_init()
{
    write_mie(read_mie() | MIE_MSIE);
}

void trigger_sip()
{
    int id = read_tp();
	*(uint32_t*)CLINT_MSIP(id) = 1;
}

void clean_sip()
{
    int id = read_tp();
    *(uint32_t*)CLINT_MSIP(id) = 0;
}