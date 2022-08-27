#include <stdbool.h>
#include "page.h"

#ifdef PAGE_TEST
extern void page_alloc_test();
#endif // PAGE_TEST

extern void uart_init();
extern void sched_init(void);
extern void trap_init();
extern void plic_init();
extern void timer_init();
extern void uart_puts(const char*);
extern void schedule();
extern void shutdown();

int os_main(void)
{
	clear_bss();

	uart_init();
	uart_puts("Hello OS!\n");
	
	page_init();
#ifdef PAGE_TEST
	page_alloc_test();
#endif // PAGE_TEST

	trap_init();
	plic_init();
	timer_init();
	sched_init();
    schedule();

	// unreachable
	shutdown();
	return 0;
}