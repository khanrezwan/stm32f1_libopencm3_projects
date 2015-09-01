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
#include <libopencm3/stm32/rcc.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_24mhz();
}

static void gpio_setup(void)
{
	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO11);
	gpio_set(GPIOC,GPIO11);
}

static void exti0_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0);
	exti_select_source(EXTI0, GPIOA);
	exti_set_trigger(EXTI0,EXTI_TRIGGER_FALLING);
	exti_enable_request(EXTI0);
}

static void exti0_nvic_init(void)
{
	nvic_set_priority(NVIC_EXTI0_IRQ,0x00);
	nvic_enable_irq(NVIC_EXTI0_IRQ);
	rcc_periph_clock_enable(RCC_AFIO);
}

void exti0_isr(void)
{
	int i;
	for (i = 0; i < 8000; i++)	/* Wait a bit. */
		__asm__("nop");
	//val= exti_get_flag_status(EXTI0);
	if(exti_get_flag_status(EXTI0)) // 1 if flag is enabled
	{
		gpio_toggle(GPIOC, GPIO11);

	}
	exti_reset_request(EXTI0);
}


int main(void)
{

	clock_setup();
	gpio_setup();
	exti0_init();
	exti0_nvic_init();


	while (1) {

			__asm__("nop");

	}

	return 0;
}
