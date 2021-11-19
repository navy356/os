global setup_page_tables_2
global enable_paging_2
global pages
extern dss
section .text
bits 32
setup_page_tables_2:
	mov eax, page_table_3
	or eax, 0b11 ; present, writable
	mov ebx, page_table_4
	mov [page_table_4], eax
	
	mov eax, page_table_2
	or eax, 0b11 ; present, writable
	mov [page_table_3], eax

	mov ecx, 0 ; 

	.loop:

	mov eax, 0x200000 ; 2MiB
	mul ecx
	or eax, 0b10000011 ; present, writable, huge page
	mov edx, page_table_2
	add edx, 0x80*8
	mov [edx + ecx * 8], eax

	inc ecx ; increment counter
	cmp ecx, 384 ; checks if the whole table is mapped
	jne .loop ; if not, continue

	mov ecx,512

enable_paging_2:
	; pass page table location to cpu
	mov eax, page_table_4
	mov ebx,0x1337
	mov cr3, eax
	hlt

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