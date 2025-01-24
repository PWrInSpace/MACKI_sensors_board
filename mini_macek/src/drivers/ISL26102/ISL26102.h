#ifndef ISL26102_H_
#define ISL26102_H_

#include "src/drivers/ISL26102/ISL26102.h"
#include <stdbool.h>
#include <stdint.h>

// All read address
#define ISL26102_REG_CHIP_ID     0x00
#define ISL26102_REG_OWR         0x05  // Output word rate
#define ISL26102_REG_INPUT_MUX   0x07
#define ISL26102_REG_CHX_PTR     0x08  // Channel pointer
#define ISL26102_REG_PGA_GAIN    0x17
#define ISL26102_REG_CONV_CONT   0x04  // Conversion control

#define CHIP_ID_ISL26102_MASK   0x10

// Convert read register to write register form
#define W_REG(read_reg) read_reg | 0x80

/**
 * @brief Output word rate of ISL26102
 *
 */
typedef enum {
    ISL26102_DR_2_5 = 0x00,
    ISL26102_DR_5 = 0x01,
    ISL26102_DR_10 = 0x02,
    ISL26102_DR_20 = 0x03,
    ISL26102_DR_40 = 0x04,
    ISL26102_DR_80 = 0x05,
    ISL26102_DR_100 = 0x0B,
    ISL26102_DR_160 = 0x06,
    ISL26102_DR_200 = 0x0C,
    ISL26102_DR_320 = 0x07,
    ISL26102_DR_400 = 0x0D,
    ISL26102_DR_640 = 0x08,
    ISL26102_DR_800 = 0x0E,
    ISL26102_DR_1000 = 0x11,
    ISL26102_DR_1280 = 0x09,
    ISL26102_DR_1600 = 0x0F,
    ISL26102_DR_2000 = 0x12,
    ISL26102_DR_2560 = 0x0A,
    ISL26102_DR_3200 = 0x10,
    ISL26102_DR_4000 = 0x13
} ISL26102_OWR_t;

/**
 * @brief Channel of ISL26102
 *
 */
typedef enum {
    ISL26102_CHANNEL_1 = 0x00,
    ISL26102_CHANNEL_2 = 0x01,
    ISL26102_ANALOG_SUPPLY_MONITOR = 0x02,
    ISL26102_TEMPERATURE_SENSOR = 0x03
} ISL26102_channel_t;

/**
 * @brief PGA gain of ISL26102
 *
 */
typedef enum {
    ISL26102_X1 = 0x00,
    ISL26102_X2 = 0x01,
    ISL26102_X4 = 0x02,
    ISL26102_X8 = 0x03,
    ISL26102_X16 = 0x04,
    ISL26102_X32 = 0x05,
    ISL26102_X64 = 0x06,
    ISL26102_X128 = 0x07
} ISL26102_PGA_gain_t;

/**
 * @brief Conversion control of ISL26102
 *
 */
typedef enum {
    ISL26102_STOP_CONVERSION = 0x00,
    ISL26102_SINGLE_CONVERSION = 0x01,
    ISL26102_CONTINUOUS_CONVERSION = 0x02,
    ISL26102_OFFSET_CALIBRATION = 0x04
} ISL26102_conversion_control_t;

/**
 * @brief spi write function prototype
 *
 */
typedef bool(*ISL26102_spi_write_reg)(uint8_t reg, uint8_t val);

/**
 * @brief spi read function prototype
 *
 */
typedef bool(*ISL26102_spi_read_reg)(uint8_t reg, uint8_t *out_val);

/**
 * @brief spi read data function prototype, this should wait for the SDO ready signal
 * and read 3 bytes of data from the amplifier
 *
 */
typedef bool(*ISL26102_spi_read_data)(uint8_t data_buffer[3]);

/**
 * @brief ISL26102 device struct
 *
 */
typedef struct {
    ISL26102_spi_write_reg write_reg;
    ISL26102_spi_read_reg read_reg;
    ISL26102_spi_read_data read_data;
} ISL26102_t;

/**
 * @brief Initialize ISL26102 device, this function also check if the device is ISL26102
 *
 * @param dev pointer to device struct
 * @return true the device is initialized
 * @return false unable to initialize the device, probably wrong device model or communication error
 */
bool ISL26102_init(ISL26102_t *dev);

/**
 * @brief Read register from ISL26102
 *
 * @param dev pointer to device struct
 * @param reg register address
 * @param out_val pointer to store the register value
 * @return true the register value is read successfully
 * @return false unable to read the register value
 */
bool ISL26102_read_reg(ISL26102_t *dev, uint8_t reg, uint8_t *out_val);

/**
 * @brief Write register to ISL26102. This function automatically convert
 the provided register to write register form.
 *
 * @param dev pointer to device struct
 * @param reg register address (can be read address or one of the define address)
 * @param val value to write to the register
 * @return true the register is written successfully
 * @return false unable to write the register
 */
bool ISL26102_write_reg(ISL26102_t *dev, uint8_t reg, uint8_t val);


bool ISL26102_read_raw_data(ISL26102_t *dev, uint8_t data_buffer[3]);

/**
 * @brief Read data from ISL26102. The data is stored on 24 bits of the 32 bit out data.
 *
 * @param dev pointer to device struct
 * @param out_data pointer to store the data
 * @return true the data is read successfully
 * @return false unable to read the data
 */
bool ISL26102_read_data(ISL26102_t *dev, uint32_t *out_data);

/**
 * @brief Check If lib communicate with correct amplifier model.
 *
 * @param dev pointer to device struct
 * @return true The device model is correct
 * @return false The amplifier model is ISL26104
 */
bool ISL26102_check_part_number(ISL26102_t *dev);

/**
 * @brief Set PGA gain of ISL26102. Note that the PGA gain influence the output word rate.
 *
 * @param dev pointer to device struct
 * @param pga_gain PGA gain
 * @return true the PGA gain is set successfully
 * @return false unable to set the PGA gain
 */
bool ISL26102_set_PGA_gain(ISL26102_t *dev, ISL26102_PGA_gain_t pga_gain);

/**
 * @brief Set output word rate of ISL26102. Note that the output word rate depend on the PGA gain,
 * to get the real time value of the output word rate, refer to the datasheet table 1 on page 6.
 *
 * @param dev pointer to device struct
 * @param owr output word rate
 * @return true the output word rate is set successfully
 * @return false unable to set the output word rate
 */
bool ISL26102_set_output_word_rate(ISL26102_t *dev, ISL26102_OWR_t owr);

/**
 * @brief Change channel of ISL26102. This function also set the input mux and
 * channel pointer register.
 *
 * @param dev pointer to device struct
 * @param channel channel to set
 * @return true the channel is set successfully
 * @return false unable to set the channel
 */
bool ISL26102_change_channel(ISL26102_t *dev, ISL26102_channel_t channel);

/**
 * @brief Set convertion controll type of ISL26102
 *
 * @param dev pointer to device struct
 * @param cc convertion control type
 * @return true the convertion control is set successfully
 * @return false unable to set the convertion control
 */
bool ISL26102_set_convertion_controll(ISL26102_t *dev, ISL26102_conversion_control_t cc);

#endif
