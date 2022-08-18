#include "uart.h"
#include "trap.h"
#include "page.h"
#include "batch.h"

#ifdef PAGE_TEST
extern void page_alloc_test();
#endif // PAGE_TEST

int os_main(void)
{
	clear_bss();
	lib_puts("Hello OS!\n");

	page_init();
#ifdef PAGE_TEST
	page_alloc_test();
#endif // PAGE_TEST

	trap_init();
	sche_init();
	schedule();
	while (1) {}
	return 0;
}