set disassemble-next-line on
# b _start
target remote : 1234
display $sp
display $pc
# b __switch_to
# b plic_init
# b trap_handler
# b __alltraps
b __switch_to
display $ra
c
# b schedule
