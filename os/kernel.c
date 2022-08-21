#include "uart.h"
#include "page.h"
#include "type.h"

#ifdef PAGE_TEST
extern void page_alloc_test();
#endif // PAGE_TEST
extern void sched_init(void);
extern void schedule();

int os_main(void)
{
	clear_bss();
	lib_puts("Hello OS!\n");

	page_init();
#ifdef PAGE_TEST
	page_alloc_test();
#endif // PAGE_TEST

	sched_init();
	schedule();

	while (true)
	{
	}
	return 0;
}