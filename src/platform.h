#ifndef platform_h
#define platform_h

#include "common.h"

void platform_init(void);
uint32_t platform_get_time(void);
void platform_led_on(void);
void platform_led_off(void);
void platform_led_toggle(void);
void platform_led_state(bool state);

#endif
