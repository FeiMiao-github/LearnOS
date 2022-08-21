#include "riscv.h"

.altmacro
.macro SAVE_GP n reg
    sd x\n, \n*8(\reg)
.endm
.macro LOAD_GP n reg
    ld x\n, \n*8(\reg)
.endm

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
1: 
	csrw mscratch, a0;                                              
	mv t6, a0;                                                          
	ld x1, 1*8(t6);                                                   
	ld x3, 3*8(t6);                                                   
	.set n, 5;                                                          
	.rept 26;                                                           
		LOAD_GP % n t6;                                                       
	.set n, n + 1;                                                      
	.endr;                                                              
	ret;
