GLOBAL gdt_flush    ; Allows the C code to call gdt_flush().
extern kernel_main
extern gdt_entries
extern gdt_ptr
extern Start64Bit
global init_gdt
section .text

bits 32

EnterProtectedMode:
   call EnableA20
   mov eax, cr0 
   or al, 1       ; set PE (Protection Enable) bit in CR0 (Control Register 0)
   mov cr0, eax
   mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
   lgdt [eax]        ; Load the new GDT pointer
   jmp gdt_flush

bits 32 

init_gdt:
   push    ebp 
   mov     ebp, esp
   push    ebx
   sub     esp, 0x4
   ;add     ebx, 0x2
   mov     ebx,gdt_ptr
   mov     word [ebx], 0x27  ;{data_26}
   ;lea     eax, [ebx]  ;{data_26}
   mov     eax, gdt_entries  ;{data_26}
   mov     dword [ebx+0x2], eax  ;{data_26}  {data_28}
   sub     esp, 0xc
   push    0x0 ;{var_1c}
   push    0x0 ;{var_20}
   push    0x0 ;{var_24}
   push    0x0 ;{var_28}
   push    0x0 ;{var_2c}
   
   call    gdt_set_gate
   add     esp, 0x20
   sub     esp, 0xc
   push    0xcf ;{var_1c}
   push    0x9a ;{var_20}
   push    0xffffffff ;{var_24}  {0xffffffff}
   push    0x0 ;{var_28}
   push    0x1 ;{var_2c}

   call    gdt_set_gate
   add     esp, 0x20
   sub     esp, 0xc
   push    0xcf ;{var_1c}
   push    0x92 ;{var_20}
   push    0xffffffff ;{var_24}  {0xffffffff}
   push    0x0 ;{var_28}
   push    0x2 ;{var_2c}

   call    gdt_set_gate
   add     esp, 0x20
   sub     esp, 0xc
   push    0xcf ;{var_1c}
   push    0xfa ;{var_20}
   push    0xffffffff ;{var_24}  {0xffffffff}
   push    0x0 ;{var_28}
   push    0x3 ;{var_2c}
   call    gdt_set_gate
   add     esp, 0x20
   sub     esp, 0xc
   push    0xcf ;{var_1c}
   push    0xf2 ;{var_20}
   push    0xffffffff ;{var_24}  {0xffffffff}
   push    0x0 ;{var_28}
   push    0x4 ;{var_2c}
   call    gdt_set_gate
   add     esp, 0x20
   lea     eax, [ebx]  ;{data_26}
   sub     esp, 0xc
   push    eax ;{var_1c}  {data_26}
   call    EnterProtectedMode;gdt_flush
   add     esp, 0x10
   nop     
   mov     ebx, dword [ebp-0x4]
   leave
   retn

gdt_set_gate:
   push    ebp 
   mov     ebp, esp 
   sub     esp, 0x8
   push    eax
   mov     eax,gdt_entries
   ;add     eax, 0x1
   mov     ecx, dword [ebp+0x14] ;{arg4}
   mov     edx, dword [ebp+0x18] ;{arg5}
   mov     byte [ebp-0x4], cl ;{var_8}
   mov     byte [ebp-0x8], dl ;{var_c}
   mov     edx, dword [ebp+0xc] ;{arg2}
   mov     ecx, edx
   mov     edx, dword [ebp+0x8] ;{arg1}
   mov     word [eax+edx*8+0x2], cx
   mov     edx, dword [ebp+0xc] ;{arg2}
   shr     edx, 0x10
   mov     ecx, edx
   mov     edx, dword [ebp+0x8] ;{arg1}
   mov     byte [eax+edx*8+0x4], cl
   mov     edx, dword [ebp+0xc] ;{arg2}
   shr     edx, 0x18
   mov     ecx, edx
   mov     edx, dword [ebp+0x8] ;{arg1}
   mov     byte [eax+edx*8+0x7], cl
   mov     edx, dword [ebp+0x10] ;{arg3}
   mov     ecx, edx
   mov     edx, dword [ebp+0x8] ;{arg1}
   mov     word [eax+edx*8], cx
   mov     edx, dword [ebp+0x10] ;{arg3}
   shr     edx, 0x10
   mov     ecx, edx
   and     ecx, 0xf
   mov     edx, dword [ebp+0x8] ;{arg1}
   mov     byte [eax+edx*8+0x6], cl
   mov     edx, dword [ebp+0x8] ;{arg1}
   movzx   edx, byte [eax+edx*8+0x6]
   mov     ecx, edx
   movzx   edx, byte [ebp-0x8] ;{var_c}
   and     edx, 0xfffffff0
   or      edx, ecx
   mov     ecx, edx
   mov     edx, dword [ebp+0x8] ;{arg1}
   mov     byte [eax+edx*8+0x6], cl
   mov     edx, dword [ebp+0x8] ;{arg1}
   movzx   ecx, byte [ebp-0x4] ;{var_8}
   mov     byte [eax+edx*8+0x5], cl
   nop  
   pop eax  
   leave 
   retn 

EnableA20:
	in al, 0x92
	or al, 2
	out 0x92, al
	ret

gdt_flush:
   cli
   mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
   mov ds, ax        ; Load all data segment selectors
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax
   jmp 0x08:Start64Bit