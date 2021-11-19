ENTRY(start)

SECTIONS
{
	. = 1M;

	kernel_start = 0x10000;
	kernel_offset = 0x40000000;
	. = kernel_start + kernel_offset;
	.text ALIGN (0x1000): AT(ADDR(.text) - kernel_offset)
	{
		KEEP(*(.multiboot_header))
		KEEP(*(.multiboot.text))
		*(.text)
	}

	.idt ALIGN(0X1000) : AT(ADDR(.idt) - kernel_offset)
	{
		_idt = .;
		. = . + 0x1000;
	}

	.rodata ALIGN(0X1000): AT(ADDR(.rodata) - kernel_offset)
	{
		*(.rodata)
	}

	.data ALIGN (0x1000):AT(ADDR(.data) - kernel_offset)
	{
		dss = .;
		*(.data)
	}
	
	.bss ALIGN (0x1000) : AT(ADDR(.bss) - kernel_offset)
	{
			sbss = .;
			*(COMMON)
			*(.bss)
			ebss = .;
	}

	_kernel_end = .;
}
