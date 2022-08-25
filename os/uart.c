/**
	reference: http://byterunner.com/16550.html
*/
#include <stdbool.h>
#include "ctx.h"
#include "uart.h"

void printf(const char* s, ...);

void uart_init()
{
	// disable_all_uart_interrupt(uart0);
	volatile uart_16550a_reg_t *reg = (uart_16550a_reg_t*)UART0;
	reg->IER = 0; // disable_all_uart_interrupt

	// set baud rate
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
	reg->LCR |= LCR_DIVISOR_LATCH_ENABLE;\
	reg->DLL = 0x03;
	reg->DLM = 0x0;
	reg->LCR &= ~LCR_DIVISOR_LATCH_ENABLE;
	reg->LCR |= LCR_STOP_BIT | LCR_WORD_LENGTH_8;
	reg->IER |= IER_RHRI_ENABLE;
}

void uart_putc(char c)
{
	volatile uart_16550a_reg_t *reg = (uart_16550a_reg_t*)UART0;
	while (!(reg->LSR & LSR_TRANSMIT_HOLDING_EMPTY)) {
	}
	reg->THR = c;
}

void uart_puts(const char *s)
{
	while (*s) uart_putc(*s++);
}

uint8_t uart_getc(void)
{
	volatile uart_16550a_reg_t *reg = (uart_16550a_reg_t*)UART0;
	if (reg->LSR & LSR_RECEIVE_DATA_READY){
		return reg->RBR;
	} else {
		return 0xFF;
	}
}

/*
 * handle a uart interrupt, raised because input has arrived, called from trap.c.
 */
void uart_isr(void)
{
	while (1) {
		int c = uart_getc();
		if (c == 0xFF) {
			break;
		} else {
			uart_putc((char)c);
			uart_putc('\n');
		}
	}
}