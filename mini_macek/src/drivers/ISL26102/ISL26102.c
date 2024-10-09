#include <stdlib.h>
#include "ISL26102.h"

bool ISL26102_init(ISL26102_t *dev) {
    if (dev == NULL) {
        return false;
    }

    if (dev->write_reg == NULL || dev->read_reg == NULL || dev->read_data == NULL) {
        return false;
    }

    if (ISL26102_check_part_number(dev) == false) {
        return false;
    }

    return true;
}

bool ISL26102_read_reg(ISL26102_t *dev, uint8_t reg, uint8_t *out_val) {
    if (dev == NULL) {
        return false;
    }

    return dev->read_reg(reg, out_val);
}

bool ISL26102_write_reg(ISL26102_t *dev, uint8_t reg, uint8_t val) {
    if (dev == NULL) {
        return false;
    }

    return dev->write_reg(W_REG(reg), val);
}

bool ISL26102_read_data(ISL26102_t *dev, uint32_t *out_data) {
    if (dev == NULL) {
        return false;
    }

    uint8_t data_buffer[3] = {0};
    if (dev->read_data(data_buffer) == false) {
        return false;
    }

    // data is store on 24 bits, where idx -> MSB
    *out_data = data_buffer[0] << 16 | data_buffer[1] << 8 | data_buffer[2];

    return true;
}

bool ISL26102_check_part_number(ISL26102_t *dev) {
    uint8_t reg_value = 0;
    if (ISL26102_read_reg(dev, ISL26102_REG_CHIP_ID, &reg_value) == false) {
        return false;
    }

    return reg_value & CHIP_ID_ISL26102_MASK;
}

bool ISL26102_set_PGA_gain(ISL26102_t *dev, ISL26102_PGA_gain_t pga_gain) {
    return ISL26102_write_reg(dev, ISL26102_REG_PGA_GAIN, pga_gain);
}


bool ISL26102_set_output_word_rate(ISL26102_t *dev, ISL26102_OWR_t owr) {
    return ISL26102_write_reg(dev, ISL26102_REG_OWR, owr);
}


bool ISL26102_change_channel(ISL26102_t *dev, ISL26102_channel_t channel) {
    if (ISL26102_write_reg(dev, ISL26102_REG_INPUT_MUX, channel) == false) {
        return false;
    }

    if (ISL26102_write_reg(dev, ISL26102_REG_CHX_PTR, channel) == false) {
        return false;
    }

    return true;
}

bool ISL26102_set_convertion_controll(ISL26102_t *dev, ISL26102_conversion_control_t cc) {
    return ISL26102_write_reg(dev, ISL26102_REG_CONV_CONT, cc);
}



