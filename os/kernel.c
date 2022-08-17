#include "uart.h"
#include "trap.h"
#include "page.h"
#include "batch.h"

int os_main(void)
{
	clear_bss();

	lib_puts("Hello OS!\n");
	page_init();
	trap_init();
	sche_init();
	schedule();
	while (1) {}
	return 0;
}