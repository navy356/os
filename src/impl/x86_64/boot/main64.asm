global Start64Bit

section .text
bits 64
extern kernel_main

Start64Bit:
	call kernel_main
	jmp $
    hlt