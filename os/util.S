.altmacro
.macro SAVE_GP n reg
    sd x\n, \n*8(\reg)
.endm
.macro LOAD_GP n reg
    ld x\n, \n*8(\reg)
.endm
