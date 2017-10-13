ENTRY(Reset_Handler)

MEMORY
{
	sram (xrw) : ORIGIN = 0x20000000 - 16k / 4, LENGTH = 16k
	flash (rx) : ORIGIN = 0x00000000, LENGTH = 128k
}


/*
 *  * The '__stack' definition is required by crt0, do not remove it.
 *   */
__stack = ORIGIN(sram) + LENGTH(sram) ;
/*_start = Reset_Handler; */
PROVIDE(end = __stack);
heap_size = 256;

SECTIONS
{
	.isr_vector : ALIGN(4)
	{
		_vector_start_ = .;
		FILL(0xff)
		KEEP(*(.isr_vector)) /* Interrupt vectors */
		/* Freescale has positioned special registers at 
		 * 0x400 in the flash.  The following line advances the 
		 * linker write address and positions the special values
		 * properly.
		 */
		. = _vector_start_ + 0x400;
		KEEP(*(.cfmconfig)) /* Freescale configuration */
	} > flash


	.text : 
	{
		. = ALIGN(4);
		*(.text)
		*(.text*)
		*(.rodata)
		*(.rodata*)


		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;

		. = ALIGN(4);
		KEEP (*(.init))
		. = ALIGN(4);
		__init_array_start = .;
		*(.init_array*)
		__init_array_end = .;

		. = ALIGN(4);
		KEEP (*(.fini))
		. = ALIGN(4);
		__fini_array_start = .;
		*(.fini_array*)
		__fini_array_end = .;

		*(.jcr)
		__text_end__ = .;
	} > flash

	.ARM :
	{
		__exidx_start = .;
		*(.ARM.exidx*) 
		__exidx_end = .;
	} > flash
	/* 
	 * Initialized data has two addresses, the load address
	 * and the run address.  Upon boot, the initialized data 
	 * must be transferred from the load address in flash to the run
	 * address in ram
	 */

	/* package all initialized variable data in rodata */

	/* initialized data sections go here */
	.data :  
	{
		__data_load__ = LOADADDR(.data);
		__data_start__ = .;
		*(.data)
		*(.data*) 
		__data_end__ = .;
	} > sram AT > flash 

	.bss : 
	{
		__bss_start__ = .;
		*(.bss)
		*(.bss*)
		__bss_end__ = .;
	} > sram

	.heap : 
	{
		_heap_start = .;
		. += heap_size;
		_heap_end = .;
	} > sram
	
	
	.periph		0x40000000 (NOLOAD) : 
	{
		periph_begin = .;
		. = periph_begin + 4k*(15);
		*(.periph.gpio)
		. = periph_begin + 4k*(56);
		*(.periph.tpm0)
		. = periph_begin + 4k*(57);
		*(.periph.tpm1)
		. = periph_begin + 4k*(58);
		*(.periph.tpm2)
		. = periph_begin + 4k*(71);
		*(.periph.sim)
		. = periph_begin + 4k*(73);
		*(.periph.porta) 
		. = periph_begin + 4k*(74);
		*(.periph.portb) 
		. = periph_begin + 4k*(75);
		*(.periph.portc) 
		. = periph_begin + 4k*(76);
		*(.periph.portd) 
		. = periph_begin + 4k*(77);
		*(.periph.porte)
		. = periph_begin + 4k*(100);
		*(.periph.mcg)
		. = periph_begin + 4k*(102);
		*(.periph.i2c0)
		. = periph_begin + 4k*(103);
		*(.periph.i2c1)
		. = periph_begin + 4k*(106);
		*(.periph.uart0)



	}

}
