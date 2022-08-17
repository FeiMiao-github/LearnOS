OUTPUT_ARCH( "riscv" )

ENTRY(start)

SECTIONS
{
	. = ALIGN(0x1000);
	.text : {
		*(.text.entry)
		*(.text)
	}
	. = ALIGN(16);
	.data : { *(.data) }
	. = ALIGN(16);
	.bss : { *(.bss) }
}