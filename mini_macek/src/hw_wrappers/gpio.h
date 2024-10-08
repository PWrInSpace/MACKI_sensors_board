#ifndef HW_WRAPPERS_GPIO_H_
#define HW_WRAPPERS_GPIO_H_

#include "../drivers/ADG726/ADG726.h"


#define STATUS_LED_ON 1
#define STATUS_LED_OFF 0


void GPIO_set_mux_pin(ADG726_gpio_pin_t pin, ADG726_gpio_state_t state);

void GPIO_set_status_led_pin(uint8_t state);

#endif