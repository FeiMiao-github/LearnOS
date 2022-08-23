/**
	reference: http://byterunner.com/16550.html
*/
#include "uart.h"
#include "type.h"

static uart_16550a_reg_t *uart0 = (uart_16550a_reg_t*)UART0;

static void disable_uart_interrupt(uart_16550a_reg_t *reg) {
	uart0->IER.MSI = 0;
	uart0->IER.OTHER = 0;
	uart0->IER.RHRI = 0;
	uart0->IER.RISI = 0;
	uart0->IER.THRI = 0;
}

static void enable_divisor_latch(uart_16550a_reg_t *reg) {
	reg->LCR.DIVISOR_LATCH_ENABLE = 1;
}

static void disable_divisor_latch(uart_16550a_reg_t *reg) {
	reg->LCR.DIVISOR_LATCH_ENABLE = 0;
}

/*----------------------------------------------------------------
All boards will typically have the 1.8432 MHZ crystal. Some Byte Runner boards will also allow selection of the 7.3728 MHZ clock.

   BAUD RATE  1.8432 MHZ clock  7.3728 MHZ clock

   50           2304               9216
   75           1536               6144
   110          1047               4188
   134.5         857               3428
   150           768               3072
   300           384               1536
   600           192                768
   1200           96                384
   2400           48                192
   3600           32                128
   4800           24                 96
   7200           16                 64
   9600           12                 48
   19.2K           6                 24
   38.4K           3                 12
   57.6K           2                  8
   115.2K          1                  4
   230.4K       not possible          2
   460.8K       not possible          1
------------------------------------------------------------*/
static void set_baud_rate(uart_16550a_reg_t *reg, uint16_t baud) {
	enable_divisor_latch(reg);
	/* 
		split the value of 3(0x0003) into two bytes, DLL stores the low byte,
		DLM stores the high byte.
	*/
	uart0->DLL = baud & 0xFF;
	uart0->DLM = (baud >> 8) & 0xFF;
	disable_divisor_latch(reg);
}

static void set_lcr(uart_16550a_reg_t *reg, LCR_t* lcr)
{
	reg->LCR = *lcr;
}

static bool is_transmit_holding_ready(uart_16550a_reg_t *reg)
{
	return reg->LSR.TRANSMIT_HOLDING_EMPTY == 1;
}

// int uart_putc(char ch)
// {
// 	while ((uart_read_reg(LSR) & LSR_TX_IDLE) == 0);
// 	return uart_write_reg(THR, ch);
// }

void uart_init()
{
	disable_uart_interrupt(uart0);
	set_baud_rate(uart0, 0x3);
	
	/*
	 * Continue setting the asynchronous data communication format.
	 * - number of the word length: 8 bits
	 * - number of stop bits：1 bit when word length is 8 bits
	 * - no parity
	 * - no break control
	 * - disabled baud latch
	 */
	LCR_t lcr = {0};
	set_lcr(uart0, &lcr);
}

void uart_putc(char c)
{
	while (!is_transmit_holding_ready(uart0));
	uart0->THR = c;
}

void uart_puts(char *s)
{
	while (*s) uart_putc(*s++);
}
