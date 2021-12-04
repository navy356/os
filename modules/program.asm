    bits 64
start:
    mov rax, 0xDEADBEEF
    add rdi, loop
    add rdi, 0x40000000
loop:
    jmp rdi