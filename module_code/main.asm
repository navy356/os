bits 64
global start

section .start
    section .text
    bits 64
    start:
    bits 64
        ; push argv
        ; push argc
        mov rax,0xdeadbeef
        mov rax,0xdeadbeef
        ; main has returned, eax is return value
        jmp  $    ; loop forever