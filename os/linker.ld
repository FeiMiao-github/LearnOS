OUTPUT_ARCH(riscv)
ENTRY(_start)
BASE_ADDRESS = 0x80000000;

MEMORY
{
	ram   (wxa!ri) : ORIGIN = BASE_ADDRESS, LENGTH = 128M
}

SECTIONS
{
    . = BASE_ADDRESS;
    skernel = .;

    stext = .;
    .text : {
        *(.text.entry)
        *(.text .text.*)
    } > ram

    . = ALIGN(4K);
    etext = .;
    srodata = .;
    .rodata : {
        *(.rodata .rodata.*)
        *(.srodata .srodata.*)
    } > ram

    . = ALIGN(4K);
    erodata = .;
    sdata = .;
    .data : {
        *(.data .data.*)
        *(.sdata .sdata.*)
    } > ram

    . = ALIGN(4K);
    edata = .;
    .bss : {
        PROVIDE(_bss_start = .);
        *(.bss.stack)
        sbss = .;
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        PROVIDE(_bss_end = .);
    } > ram

    . = ALIGN(4K);
    ebss = .;
    ekernel = .;

    /DISCARD/ : {
       *(.eh_frame)
    } > ram
    

    PROVIDE(_memory_start = ORIGIN(ram));
	PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));

	PROVIDE(_heap_start = _bss_end);
	PROVIDE(_heap_size = _memory_end - _heap_start);
}