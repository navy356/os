global edit_page_tables
global enable_paging_edit
global pages
extern dss
extern kernel_offset
extern kernel_end
global kernel_end_virtual
global flush_cr3
section .text
bits 32
edit_page_tables:
	mov eax, page_table_3
	sub eax, kernel_offset
	or eax, 0b11 ; present, writable
	mov ebx, page_table_4
	sub ebx, kernel_offset
	mov [ebx], eax

	mov eax,ebx
	or eax,0b11
	mov [ebx+0x1ff*8],eax;recursive mapping for level 4 page table
	
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

	EnableA20:
	in al, 0x92
	or al, 2
	out 0x92, al

	ret

bits 64
flush_cr3:
	push rax
	mov cr3,rdi
	bits 32
	mov eax, cr0
	or eax, 1 << 31 | 1 << 0
	mov cr0,eax
	bits 64
	pop rax
	ret

bits 32
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
kernel_end_virtual:
	dq kernel_end