.align 3
    .section .data
    .global _num_app
_num_app:
    .quad 1
    .quad app_0_start
    .quad app_0_end

    .section .data
    .global app_0_start
    .global app_0_end
app_0_start:
    .incbin "./bin/hello_user_mode.bin"
app_0_end: