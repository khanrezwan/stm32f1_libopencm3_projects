/*
 * Copyright (C) 2013 Rezwan <github.com/khanrezwan>
 * This source code is an example for STM32F1xx external interrupt 0 ( EXTI0 ). I am using Waveshare Open103R board
 * LED connected with PortC11 is toggled if interrupt generated
*/
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

static void clock_setup(void) //Setup clock
{
	rcc_clock_setup_in_hse_8mhz_out_24mhz();
}

static void gpio_setup(void)
{

	rcc_periph_clock_enable(RCC_GPIOC);// Enable GPIOC clock.
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO11);// Set GPIOC 11 as output
	gpio_set(GPIOC,GPIO11);// Turn off LED
}

static void exti0_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);// Enable GPIOA clock.
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0);// Set GPIOA 0 as Input
	exti_select_source(EXTI0, GPIOA); // Connect Interrupt Line 0 with GPIOA 0
	exti_set_trigger(EXTI0,EXTI_TRIGGER_FALLING);// Interrupt will be triggered by falling edge
	exti_enable_request(EXTI0);// Enable EXTI0
}

static void exti0_nvic_init(void)
{
	nvic_set_priority(NVIC_EXTI0_IRQ,0x00); // set EXTI0 priority
	nvic_enable_irq(NVIC_EXTI0_IRQ); // Enable Interrupt Request
	rcc_periph_clock_enable(RCC_AFIO); // Finally start clock for alternate function I/O
}

void exti0_isr(void)
{
	int i;
	for (i = 0; i < 8000; i++)	/* debounce. ToDo: write better delay */
		__asm__("nop");
	if(exti_get_flag_status(EXTI0)) // 1 if flag is enabled
	{
		gpio_toggle(GPIOC, GPIO11); // Toggle PortC11

	}
	exti_reset_request(EXTI0); // Reset Interrupt flag
}


int main(void)
{

	clock_setup(); // setup clock
	gpio_setup(); //setup PortC11 as output
	exti0_init(); // setup PORTA0 as interrupt source
	exti0_nvic_init(); // add EXTI0 interrupt to NVIC

	// Loop Forever
	while (1) {

			__asm__("nop"); //No operation

	}

	return 0;
}
