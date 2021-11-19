global start
global page_table_2
global page_table_3
global page_table_4
extern _start

extern kernel_offset

section .multiboot.text
start:
bits 32
setup_page_tables:
	mov eax,page_table_3
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
	mov [eax], edx

	mov ecx, 0 ; counter
	mov eax, kernel_offset

.loop:

	mov eax, 0x200000 ; 2MiB
	mul ecx
	;add eax, kernel_offset
	or eax, 0b10000011 ; present, writable, huge page
	mov edx, page_table_2
	sub edx, kernel_offset
	;0x80*8 = 1024
	add edx, 0x80*8
	mov [edx + ecx * 8], eax

	inc ecx ; increment counter
	cmp ecx, 384 ; checks if the whole table is mapped
	jne .loop ; if not, continue

	mov ecx,512


enable_paging:
	; pass page table location to cpu
	mov eax, page_table_4
	sub eax, kernel_offset
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

	mov ebx,0x1337
	hlt


	mov eax, [_start]
	jmp eax


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