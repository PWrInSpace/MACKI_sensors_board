#ifndef HW_WRAPPERS_SPI_H_
#define HW_WRAPPERS_SPI_H_

#include <stdint.h>


bool SPI_amp_set_register(uint8_t address, uint8_t value);

bool SPI_amp_read_register(uint8_t address, uint8_t *out_val);

bool SPI_amp_read_data(uint32_t *data);

#endif