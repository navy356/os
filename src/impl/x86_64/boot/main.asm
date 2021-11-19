global _start
global placement_address
extern Start64Bit
extern check_multiboot
extern check_cpuid
extern check_long_mode
extern init_gdt
extern kernel_main
extern kernel_offset
extern gdt_entries
extern error
extern setup_page_tables_2
extern enable_paging_2

section .text
bits 32
_start:
	mov esp, stack_top

	;call check_multiboot

	;call check_cpuid
	;call check_long_mode

	;call setup_page_tables_2
    ;hlt
	;call enable_paging

    mov eax,GDT.Pointer
    sub eax, kernel_offset
	lgdt [eax]         ; Load the 64-bit global descriptor table.
    jmp GDT.Code:Start64Bit

	hlt

section .bss
align 4096
stack_bottom:
	resb 4096 * 4
stack_top:

section .rodata
; Access bits
PRESENT        equ 1 << 7
NOT_SYS        equ 1 << 4
EXEC           equ 1 << 3
DC             equ 1 << 2
RW             equ 1 << 1
ACCESSED       equ 1 << 0
 
; Flags bits
GRAN_4K       equ 1 << 7
SZ_32         equ 1 << 6
LONG_MODE     equ 1 << 5
 
GDT:
    .Null: equ $ - GDT
        dq 0
    .Code: equ $ - GDT
        dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
        db 0                                        ; Base (mid, bits 16-23)
        db PRESENT | NOT_SYS | EXEC | RW            ; Access
        db GRAN_4K | LONG_MODE | 0xF                ; Flags & Limit (high, bits 16-19)
        db 0                                        ; Base (high, bits 24-31)
    .Data: equ $ - GDT
        dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
        db 0                                        ; Base (mid, bits 16-23)
        db PRESENT | NOT_SYS | RW                   ; Access
        db GRAN_4K | SZ_32 | 0xF                    ; Flags & Limit (high, bits 16-19)
        db 0   
    .Stk: equ $ - GDT
        dd 0xFFFF                                   ; Limit & Base (low, bits 0-15)
        db 0                                        ; Base (mid, bits 16-23)
        db PRESENT | NOT_SYS | RW | DC                  ; Access
        db GRAN_4K | SZ_32 | 0xF                    ; Flags & Limit (high, bits 16-19)
        db 0                                       ; Base (high, bits 24-31)
    .TSS: equ $ - GDT
        dd 0x00000068
        dd 0x00CF8900
    .Pointer:
        dw $ - GDT - 1
        dq GDT


section .bss
align 4096
placement_address:
    resb 0x4000