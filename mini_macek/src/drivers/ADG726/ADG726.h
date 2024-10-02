#include <stdbool.h>
#include <stdint.h>

#define NB_ADDRESS_PINS 4

#define INIT_INPUT_PIN_EMABLE 0

#define MIN_INPUT_PIN 1
#define MAX_INPUT_PIN 16

/**
 * @brief Type definition for the GPIO pin
 *
 */
typedef uint32_t ADG726_gpio_pin_t;

/**
 * @brief Enum containing the possible states of a GPIO pin
 *
 */
typedef enum {
    ADG726_LOW = 0,
    ADG726_HIGH = 1
} ADG726_gpio_state_t;

/**
 * @brief Pointer to a function that sets the state of a GPIO pin
 * Can be optimized by changing this function to call all 4 address pins at once.
 */
typedef void (*ADG726_set_gpio_pin)(ADG726_gpio_pin_t, ADG726_gpio_state_t);

/**
 * @brief Struct containing all the necessary information for the ADG726 multiplexer
 *
 */
typedef struct {
    ADG726_set_gpio_pin _set_gpio_pin;
    ADG726_gpio_pin_t address_pins[NB_ADDRESS_PINS];
    ADG726_gpio_pin_t wr_pin;
    ADG726_gpio_pin_t cs_pin;  // currently only differential output is implemented
    uint8_t current_input_pin;
} ADG726_t;

/**
 * @brief ADG726 multiplexer initializationn
 *
 * @param dev struct containing the ADG726_t object
 * @return true the initialization was successful
 * @return false invalid pointer to function or struct
 */
bool ADG726_init(ADG726_t *dev);

/**
 * @brief Change the input pin of the ADG726 multiplexer
 *
 * @param dev struct containing the ADG726_t object
 * @param input_pin the new input pin to be selected it is a value of the pin, starting from 1 (S1) to 16 (S16)
 * @return true the input pin was successfully changed
 * @return false invalid pointer to function or struct
 */
bool ADG726_change_address(ADG726_t * dev, uint8_t input_pin);
