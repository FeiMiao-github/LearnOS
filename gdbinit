set disassemble-next-line on
b _start
target remote : 1234
display $sp
display $pc
c
b schedule
