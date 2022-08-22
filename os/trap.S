#include "util.S"

    .section .text
    .globl __alltraps
    .align 3
__alltraps:
    # move curret ctx pointer to t6
	csrrw t6, mscratch, t6;                                             
																		
	sd x1, 1*8(t6);                                                   
	sd x3, 3*8(t6);                                                    
	.set n, 5;                                                          
	.rept 26;                                                           
		SAVE_GP % n t6;                                                    
		.set n, n + 1;                                                      
	.endr;

	mv a1, t6;
	csrr t6, mscratch;
	sd t6, 31*8(a1);
    csrw mscratch, a1;

    csrr t0, mstatus;
    csrr t1, mepc;
    sd t0, 32*8(a1);
    sd t1, 33*8(a1);

	# csrr	a0, mepc
	csrr	a0, mcause;
	call	trap_handler;

	csrr t6, mscratch;
	ld t0, 32*8(t6)
	ld t1, 33*8(t6);
	csrw mstatus, t0;
	csrw mepc, t1;

	ld x1, 1*8(t6);                                                   
	ld x3, 3*8(t6);
	.set n, 5; 
	.rept 27;
		LOAD_GP % n t6;
		.set n, n+1;
	.endr;
	mret;
