/* startup.c */

#include <stdint.h>
#include "MKL25Z4.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_1                0xffffffff
#define CONFIG_2                0xffffffff
#define CONFIG_3                0xffffffff
#define CONFIG_4                0xfffffffe //b5=1,b4=1,b0=1 div1 fast

/* Addresses pulled in from the linker script */
extern uint32_t __stack;
extern uint32_t __data_load__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

/* Application main() called in reset handler */
extern int main(void);

#define WEAK_ALIAS(x) __attribute__ ((weak, alias(#x)))

/* Cortex M3 core interrupt handlers */
void Reset_Handler(void);
void NMI_Handler(void) WEAK_ALIAS(Dummy_Handler);
void HardFault_Handler(void) WEAK_ALIAS(Dummy_Handler);
void MemManage_Handler(void) WEAK_ALIAS(Dummy_Handler);
void BusFault_Handler(void) WEAK_ALIAS(Dummy_Handler);
void UsageFault_Handler(void) WEAK_ALIAS(Dummy_Handler);
void SVC_Handler(void) WEAK_ALIAS(Dummy_Handler);
void DebugMon_Handler(void) WEAK_ALIAS(Dummy_Handler);
void PendSV_Handler(void) WEAK_ALIAS(Dummy_Handler);
void SysTick_Handler(void) WEAK_ALIAS(Dummy_Handler);


/* KL25Z specific interrupt handlers */
void DMA0_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void DMA1_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void DMA2_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void DMA3_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void Reserved20_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void FTFA_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void LVD_LVW_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void LLW_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void I2C0_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void I2C1_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void SPI0_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void SPI1_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void UART0_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void UART1_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void UART2_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void ADC0_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void CMP0_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void TPM0_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void TPM1_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void TPM2_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void RTC_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void RTC_Seconds_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void PIT_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void Reserved39_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void USB0_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void DAC0_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void TSI0_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void MCG_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void LPTimer_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void Reserved45_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void PORTA_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);
void PORTD_IRQHandler(void) WEAK_ALIAS(Dummy_Handler);

void Dummy_Handler(void);

/* Stack top and vector handler table */
void *vector_table[] __attribute__ ((section(".isr_vector"))) = {
	&__stack,
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0,
	0,
	0,
	0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
	
	DMA0_IRQHandler, // DMA channel 0 transfer complete/error interrupt
	DMA1_IRQHandler, // DMA channel 1 transfer complete/error interrupt
	DMA2_IRQHandler, // DMA channel 2 transfer complete/error interrupt
	DMA3_IRQHandler, // DMA channel 3 transfer complete/error interrupt
	Reserved20_IRQHandler, // Reserved interrupt 20
	FTFA_IRQHandler, // FTFA command complete/read collision interrupt
	LVD_LVW_IRQHandler, // Low Voltage Detect, Low Voltage Warning
	LLW_IRQHandler, // Low Leakage Wakeup
	I2C0_IRQHandler, // I2C0 interrupt
	I2C1_IRQHandler, // I2C0 interrupt 25
	SPI0_IRQHandler, // SPI0 interrupt
	SPI1_IRQHandler, // SPI1 interrupt
	UART0_IRQHandler, // UART0 status/error interrupt
	UART1_IRQHandler, // UART1 status/error interrupt
	UART2_IRQHandler, // UART2 status/error interrupt
	ADC0_IRQHandler, // ADC0 interrupt
	CMP0_IRQHandler, // CMP0 interrupt
	TPM0_IRQHandler, // TPM0 fault, overflow and channels interrupt
	TPM1_IRQHandler, // TPM1 fault, overflow and channels interrupt
	TPM2_IRQHandler, // TPM2 fault, overflow and channels interrupt
	RTC_IRQHandler, // RTC interrupt
	RTC_Seconds_IRQHandler, // RTC seconds interrupt
	PIT_IRQHandler, // PIT timer interrupt
	Reserved39_IRQHandler, // Reserved interrupt 39
	USB0_IRQHandler, // USB0 interrupt
	DAC0_IRQHandler, // DAC0 interrupt
	TSI0_IRQHandler, // TSI0 interrupt
	MCG_IRQHandler, // MCG interrupt
	LPTimer_IRQHandler, // LPTimer interrupt
	Reserved45_IRQHandler, // Reserved interrupt 45
	PORTA_IRQHandler, // Port A interrupt
	PORTD_IRQHandler // Port D interrupt

};

uint32_t __attribute__ ((section(".cfmconfig"))) gConfigs[] =
{
	CONFIG_1,
	CONFIG_2,
	CONFIG_3,
	CONFIG_4 //
};


void Reset_Handler(void) {
	uint8_t *src, *dst;


	/* Copy with byte pointers to obviate unaligned access problems */

	/* Copy data section from Flash to RAM */
	src = (uint8_t *)&__data_load__;
	dst = (uint8_t *)&__data_start__;
	while (dst < (uint8_t *)&__data_end__)
		*dst++ = *src++;

	/* Clear the bss section */
	dst = (uint8_t *)&__bss_start__;
	while (dst < (uint8_t *)&__bss_end__)
		*dst++ = 0;

	SystemInit();

	main();
}



uint32_t mySysTick_Config(uint32_t ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);                                                   /* Reload value impossible */
  }

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
  return (0UL);                                                     /* Function successful */
}

void put_char(char c)
{
	asm (
		"mov r0, #0x03\n"   /* SYS_WRITEC */
		"mov r1, %[msg]\n"
		"bkpt #0xAB\n"
		:
		: [msg] "r" (&c)
		: "r0", "r1"
    	);
}

void Dummy_Handler(void) {
	while (1);

}


#ifdef __cplusplus
}
#endif
