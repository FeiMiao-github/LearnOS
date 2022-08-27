OUTPUT_ARCH(riscv)
ENTRY(_start)
BASE_ADDRESS = 0x80000000;

MEMORY
{
	ram   (wxa!ri) : ORIGIN = BASE_ADDRESS, LENGTH = 256M
}

SECTIONS
{
    . = BASE_ADDRESS;
    skernel = .;

    .text : {
        *(.text.entry)
        *(.text .text.*)
    } > ram

    . = ALIGN(4K);
    .rodata : {
        *(.rodata .rodata.*)
        *(.srodata .srodata.*)
    } > ram

    . = ALIGN(4K);
    .data : {
        *(.data .data.*)
        *(.sdata .sdata.*)
    } > ram

    . = ALIGN(4K);
    .bss : {
        PROVIDE(_bss_start = .);
        *(.bss.stack)
        sbss = .;
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        PROVIDE(_bss_end = .);
    } > ram

    . = ALIGN(4K);

    /DISCARD/ : {
       *(.eh_frame)
    } > ram
    

    PROVIDE(_memory_start = ORIGIN(ram));
	PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));

	PROVIDE(_heap_start = _bss_end);
	PROVIDE(_heap_size = _memory_end - _heap_start);
}