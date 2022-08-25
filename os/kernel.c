#include <stdbool.h>
#include "page.h"

#ifdef PAGE_TEST
extern void page_alloc_test();
#endif // PAGE_TEST

void uart_init();
void sched_init(void);
void trap_init();
void plic_init();
void uart_puts(const char*);
void schedule();

int os_main(void)
{
	clear_bss();

	uart_init();
	uart_puts("Hello OS!\n");

	// page_init();
#ifdef PAGE_TEST
	page_alloc_test();
#endif // PAGE_TEST

	trap_init();
	plic_init();
	sched_init();
    schedule();

	while (true);
	return 0;
}