global setup_page_tables
global enable_paging
global pages
extern dss
section .text
bits 32
setup_page_tables:
	mov eax, page_table_3
	or eax, 0b11 ; present, writable
	mov [page_table_4], eax
	
	mov eax, page_table_2
	or eax, 0b11 ; present, writable
	mov [page_table_3], eax

	mov ecx, 0 ; counter

	.loop:

	mov eax, 0x200000 ; 2MiB
	mul ecx
	or eax, 0b10000011 ; present, writable, huge page
	mov [page_table_2 + ecx * 8], eax

	inc ecx ; increment counter
	cmp ecx, 512 ; checks if the whole table is mapped
	jne .loop ; if not, continue

	mov ecx,512

enable_paging:
	; pass page table location to cpu
	mov eax, page_table_4
	mov cr3, eax

	; enable PAE
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	; enable long mode
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 8
	wrmsr

	; enable paging, protected mode
	mov eax, cr0
	or eax, 1 << 31 | 1 << 0
	mov cr0, eax

	ret

section .bss
align 4096
page_table_4:
	resb 4096
page_table_3:
	resb 4096
page_table_2:
	resb 4096

section .data
pages:
	dq page_table_4