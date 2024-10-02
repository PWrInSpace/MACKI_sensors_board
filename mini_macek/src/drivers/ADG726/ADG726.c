#include "src/drivers/ADG726/ADG726.h"
#include <stdlib.h>

bool ADG726_init(ADG726_t *dev) {
    if (dev == NULL) {
        return false;
    }

    if (dev->_set_gpio_pin == NULL) {
        return false;
    }

    bool ret = ADG726_change_address(dev, INIT_INPUT_PIN_EMABLE);

    return ret;
}

static void _enable_address_change(ADG726_t *dev) {
    dev->_set_gpio_pin(dev->cs_pin, ADG726_LOW);
    dev->_set_gpio_pin(dev->wr_pin, ADG726_LOW);
}

static void _set_address_pins(ADG726_t *dev, uint8_t input_pin) {
    // INPUT PIN RANGE: 1 to 16 should be checked before calling this function
    // input pin starts from 1, so we need to subtract 1 to get the correct address
    // addresses starts from 0 to 15 and are increased by one, the pins are S1 to S16
    input_pin -= 1;

    for (uint8_t addr_pin = 0; addr_pin < NB_ADDRESS_PINS; ++addr_pin) {
        // each address pin coressponds to the bit of the input_pin, so we can easily
        // get address pin state from the input pin number (arg) input_pin & (1 << addr_pin):
        // eg. in_pin = 2, addr_pin = A1 0b0010 & (1 << 1) -> 0b0010 & 0010 -> 0010 -> true
        ADG726_gpio_state_t state = input_pin & (1 << addr_pin) ? ADG726_HIGH : ADG726_LOW;
        dev->_set_gpio_pin(dev->address_pins[addr_pin], state);
    }
}

static void _enable_input(ADG726_t *dev) {
    dev->_set_gpio_pin(dev->wr_pin, ADG726_HIGH);
    dev->_set_gpio_pin(dev->cs_pin, ADG726_HIGH);
}

bool ADG726_change_address(ADG726_t * dev, uint8_t input_pin) {
    if (dev == NULL) {
        return false;
    }

    if (dev->_set_gpio_pin == NULL) {
        return false;
    }

    if (input_pin < MIN_INPUT_PIN || input_pin > MAX_INPUT_PIN) {
        return false;
    }

    _enable_address_change(dev);
    _set_address_pins(dev, input_pin);
    _enable_input(dev);

    return true;
}
