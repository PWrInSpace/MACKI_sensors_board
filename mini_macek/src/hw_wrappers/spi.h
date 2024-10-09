#ifndef HW_WRAPPERS_SPI_H_
#define HW_WRAPPERS_SPI_H_

#include <stdint.h>

/**
 * @brief Set amplifier register value
 * 
 * @param address register address
 * @param value register value
 * @return true Register has been set
 * @return false unable to set register - spi busy timeout
 */
bool SPI_amp_set_register(uint8_t address, uint8_t value);

/**
 * @brief Read register value 
 * 
 * @param address register address
 * @param out_val buffer to store the output value
 * @return true Register successfully read
 * @return false error during register read due to spi busy timeout or 
 * data was not received
 */
bool SPI_amp_read_register(uint8_t address, uint8_t *out_val);

/**
 * @brief Read measurement data from the amplifier
 * This function also waits for data ready signal and reconfigure SPI phase
 * to PHA1, at the end this function do not reconfigure the SPI phase to the default!
 * @param data data buffer to store received data (index 0 -> MSB, index 3 -> LSB)
 * @return true Data sucessfully read
 * @return false Error -  spi busy timeout, wait for ready signal timeout or data 
 * was not received
 */
bool SPI_amp_read_data(uint8_t read_buffer[3]);

#endif