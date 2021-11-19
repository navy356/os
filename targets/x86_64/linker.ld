ENTRY(start)

SECTIONS
{
	. = 1M;

	.boot ALIGN (0x1000):
	{
		KEEP(*(.multiboot_header))
	}

	

	.text ALIGN (0x1000):
	{
		*(.text)
	}

	.idt BLOCK(0x1000): ALIGN(0X1000)
	{
		_idt = .;
		. = . + 0x1000;
	}

	.rodata ALIGN(0X1000):
	{
		*(.rodata)
	}

	.data ALIGN (0x1000):
	{
		dss = .;
		*(.data)
	}
	
	.bss ALIGN (0x1000) :
	{
			sbss = .;
			*(COMMON)
			*(.bss)
			ebss = .;
	}
}
