ENTRY(start)

SECTIONS
{
	. = 1M;

	.boot :
	{
		KEEP(*(.multiboot_header))
	}

	.idt BLOCK(0x1000) : ALIGN(0x1000)
	{
		_idt = .;
		. = . + 0x1000;
	}

	.text :
	{
		*(.text)
	}

	.data :
	{
		*(.data)
	}

	.rodata :
	{
		*(.rodata)
	}

	.bss :
	{
		*(.bss)
	}

	.text :
	{
		*(.text)
	}
}