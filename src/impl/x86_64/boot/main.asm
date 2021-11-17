global start

extern Start64Bit
extern check_multiboot
extern check_cpuid
extern check_long_mode
extern init_gdt
extern kernel_main
extern gdt_entries
extern error
extern setup_page_tables
extern enable_paging

section .text
bits 32
start:
	mov esp, stack_top

	call check_multiboot
	call check_cpuid
	call check_long_mode

	call setup_page_tables
	call enable_paging

	call init_gdt
	call Start64Bit

	hlt

section .bss
align 4096
stack_bottom:
	resb 4096 * 4
stack_top:
