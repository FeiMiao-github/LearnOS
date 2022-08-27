#include "riscv.h"

    .equ STACK_SIZE, 8192

    .text
    .global _start
_start:
    # setup stacks per hart
    csrr t0, mhartid                # read current hart id
    mv tp, t0                       # keep CPU's hartid in its tp for later usage.
    bnez t0, park		            # if we're not on the hart 0
                                    # we park the hart

	# Set all bytes in the BSS section to zero.
	la	a0, _bss_start
	la	a1, _bss_end
	bgeu	a0, a1, 2f
1:
	sw	zero, (a0)
	addi	a0, a0, 4
	bltu	a0, a1, 1b
2:
    la	sp, stacks_top

  
    li t0, (1 << 53) - 1
    csrw pmpaddr0, t0
    li t0, PMP_NAPOT | PMP_R | PMP_W | PMP_X
    csrw pmpcfg0, t0

.align 2; 
    j    os_main                    # hart 0 jump to c

park:
    wfi
    j park

stacks:
    .skip STACK_SIZE * 4            # allocate space for the harts stacks
stacks_top:
