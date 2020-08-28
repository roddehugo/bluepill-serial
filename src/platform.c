#include "platform.h"

#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

/* GPIO configuration for on-board LED. */
#define LED_RCCP RCC_GPIOC
#define LED_PORT GPIOC
#define LED_GPIO GPIO13
#define GPIO_MODE_OUTPUT GPIO_MODE_OUTPUT_2_MHZ
#define GPIO_CONF_OUTPUT GPIO_CNF_OUTPUT_PUSHPULL

static volatile uint32_t systime_ms = 0;

static void clock_setup(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

static void systick_setup(void)
{
    /* Setup heartbeat timer every 1ms. */
    /* 72MHz / 8 => 9000000 counts per second. */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
    /* 9000000/9000 = 1000 overflows per second - interrupt every 1ms. */
    /* Interrupt every N clock pulses: set reload to N-1. */
    systick_set_reload(8999);
    /* The systick counter value might be undefined upon startup. */
    /* To get predictable behavior, clear the counter after set reload. */
    systick_clear();
    systick_interrupt_enable();
    systick_counter_enable();
}

static void led_setup(void)
{
    rcc_periph_clock_enable(LED_RCCP);
    gpio_set_mode(LED_PORT, GPIO_MODE_OUTPUT, GPIO_CONF_OUTPUT, LED_GPIO);
}

void sys_tick_handler(void)
{
    systime_ms++;
}

void platform_init(void)
{
    clock_setup();
    systick_setup();
    led_setup();
}

uint32_t platform_get_time(void)
{
    return systime_ms;
}

void platform_led_on(void)
{
    gpio_clear(LED_PORT, LED_GPIO);
}

void platform_led_off(void)
{
    gpio_set(LED_PORT, LED_GPIO);
}

void platform_led_toggle(void)
{
    gpio_toggle(LED_PORT, LED_GPIO);
}

void platform_led_state(bool state)
{
    if (state)
        platform_led_on();
    else
        platform_led_off();
}

