#ifndef APP_HARDWARE_CONFIG_H_
#define APP_HARDWARE_CONFIG_H_

#include "ti_msp_dl_config.h"
#include "gpio.h"
#include "spi.h"
#include "uart.h"

// Multiplexer configuration
#define MUX_SET_GPIO_PIN GPIO_set_mux_pin

#define MUX_A0_PIN GPIO_MUX_A0_PIN
#define MUX_A1_PIN GPIO_MUX_A1_PIN
#define MUX_A2_PIN GPIO_MUX_A2_PIN
#define MUX_A3_PIN GPIO_MUX_A3_PIN

#define MUX_nCS_PIN GPIO_MUX_nCS_PIN
#define MUX_nWR_PIN GPIO_MUX_nWR_PIN


// Amplifier configuration
#define AMP_SPI_READ_DATA   SPI_amp_read_data
#define AMP_SPI_READ_REG    SPI_amp_read_register
#define AMP_SPI_WRITE_REG   SPI_amp_set_register


// Logger configuration
#define LOGGER_WRITE_UART    uart_service_write

// Communication uart
#define COM_WRITE_UART     uart_com_write

#endif