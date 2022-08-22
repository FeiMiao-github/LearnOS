#include "riscv.h"
#include "util.S"

	.text;                                                             
	.globl __switch_to;                                                
	.align 3;                                                         
__switch_to:                                                        
	csrrw t6, mscratch, t6;                                             
	beqz t6, 1f;                                                        
																		
	sd x1, 1*8(t6);                                                   
	sd x3, 3*8(t6);                                                    
	.set n, 5;                                                          
	.rept 26;                                                           
		SAVE_GP % n t6;                                                    
	.set n, n + 1;                                                      
	.endr;

	mv t5, t6;
	csrr t6, mscratch;
	sd t6, 31*8(t5);                                                            
1: 
	csrw mscratch, a0;
	mv t6, a0;
	ld x1, 1*8(t6);                                                   
	ld x3, 3*8(t6);                                                   
	.set n, 5;                                                          
	.rept 27;                                                           
		LOAD_GP % n t6;                                                       
	.set n, n + 1;                                                      
	.endr;
	ret;
