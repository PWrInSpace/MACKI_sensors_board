#include <stdint.h>

#include "ti/driverlib/dl_gpio.h"
#include "ti_msp_dl_config.h"

#include "gpio.h"


void GPIO_set_mux_pin(ADG726_gpio_pin_t pin, ADG726_gpio_state_t state) {
    if (state == ADG726_HIGH) {
        DL_GPIO_setPins(GPIO_MUX_PORT, pin);
    } else {
        DL_GPIO_clearPins(GPIO_MUX_PORT, pin);
    }
}

void GPIO_set_status_led_pin(uint8_t state) {
    if (state) {
        DL_GPIO_setPins(GPIO_LED_PORT, GPIO_LED_STATUS_PIN);
    } else {
        DL_GPIO_clearPins(GPIO_LED_PORT, GPIO_LED_STATUS_PIN);
    }
}
