global edit_page_tables
global enable_paging_edit
global pages
extern dss
extern kernel_offset
section .text
bits 32
edit_page_tables:
	mov eax, page_table_3
	sub eax, kernel_offset
	or eax, 0b11 ; present, writable
	mov ebx, page_table_4
	sub ebx, kernel_offset
	mov [ebx], eax
	
	mov edx, page_table_2
	sub edx, kernel_offset
	or edx, 0b11 ; present, writable
	mov eax,page_table_3
	sub eax, kernel_offset
	mov [eax+0x1*8], edx

	mov ecx, 0 ; 

	.loop:

	mov eax, 0x200000 ; 2MiB
	mul ecx
	or eax, 0b10000011 ; present, writable, huge page
	mov edx, page_table_2
	sub edx, kernel_offset
	mov [edx + ecx * 8], eax

	inc ecx ; increment counter
	cmp ecx, 512 ; checks if the whole table is mapped
	jne .loop ; if not, continue

	ret

enable_paging_edit:
	; pass page table location to cpu
	mov eax, page_table_4
	sub eax, kernel_offset
	mov cr3, eax

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