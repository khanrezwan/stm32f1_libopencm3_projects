/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2013 Joshua Harlan Lifton <joshua.harlan.lifton@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>

volatile uint32_t systick_millis = 0;

static void delay(uint32_t milliseconds)
{
  uint32_t target = systick_millis + milliseconds;
  while (target > systick_millis);
}

static void systick_setup(void) {
  //  the open103r CPU will use an internal 24Mhz oscillator for the CPU
  // clock speed.  To make the systick timer reset every millisecond (or 1000
  // times a second) set its reload value to:
  //   CPU_CLOCK_HZ / 1000
  systick_set_reload(24000);
  // Set the systick clock source to the main CPU clock and enable it and its
  // reload interrupt.
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_counter_enable();
  systick_interrupt_enable();
}

void sys_tick_handler(void) {
  // Increment the global millisecond count.
  systick_millis++;
}

static void clock_setup(void) //Setup clock
{
	rcc_clock_setup_in_hse_8mhz_out_24mhz();
}

static void gpio_setup(void)
{
	/* Enable GPIOC clock. */
	/* Manually: */
	// RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;
	/* Using API functions: */

	rcc_periph_clock_enable(RCC_GPIOC);
	/* Set GPIO9 (in GPIO port C) to 'output push-pull'. */
	/* Manually: */
	// GPIOC_CRH = (GPIO_CNF_OUTPUT_PUSHPULL << (((9 - 8) * 4) + 2));
	// GPIOC_CRH |= (GPIO_MODE_OUTPUT_2_MHZ << ((9 - 8) * 4));
	/* Using API functions: */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO11);
	gpio_set(GPIOC,GPIO11);
}

int main(void)
{
//	int i;
	clock_setup();
	gpio_setup();
	systick_setup();
	/* Blink LED1 (PC9) on the board. */
	while (1) {
		/* Using API function gpio_toggle(): */
		gpio_toggle(GPIOC, GPIO11);

		delay(500);
	}

	return 0;
}
