#include "sensors_controller.h"
#include "src/drivers/ADG726/ADG726.h"
#include "src/drivers/ISL26102/ISL26102.h"
#include "src/hw_wrappers/hw_config.h"
#include "logger_def.h"

#define MODULE "SENS"

#define MAX_SENSORS_NUMBER 32
#define MIN_SENSORS_NUMBER 1

#define MUX_MAX_ADDRESSES 16

#define MUX_CH1 1
#define MUX_CH2 2
#define CH1_SENS_NUM_MAX 15
#define CH1_SENS_NUM_MIN 0
#define CH2_SENS_NUM_MIN 16



static struct {
    ADG726_t mux;
    ISL26102_t amp;
    uint8_t sensors_data[TOTAL_BYTES];
    uint8_t current_sensor;
} ctx = {
    .mux = {
        .set_gpio_pin = MUX_SET_GPIO_PIN,
        .address_pins = {MUX_A0_PIN, MUX_A1_PIN, MUX_A2_PIN, MUX_A3_PIN},
        .cs_pin = MUX_nCS_PIN,
        .wr_pin = MUX_nWR_PIN
    },
    .amp = {
        .read_data = AMP_SPI_READ_DATA,
        .read_reg = AMP_SPI_READ_REG,
        .write_reg = AMP_SPI_WRITE_REG
    },
    .sensors_data = {0},
    .current_sensor = 0
};

static bool _configure_default(void) {
    if (ADG726_change_address(&ctx.mux, MUX_CH1) == false) {
        LOG_ERROR(MODULE, "Unable to set default mux channel");
        return false;
    }

    if (ISL26102_change_channel(&ctx.amp, ISL26102_CHANNEL_1) == false) {
        LOG_ERROR(MODULE, "Unable to set amplifier default channel");
        return false;
    }

    if (ISL26102_set_output_word_rate(&ctx.amp, DEFAULT_OWR) == false) {
        LOG_ERROR(MODULE, "Unable to set default output word rate");
        return false;
    }

    return true;
}

bool sens_ctrl_init(void) {
    if (MIN_SENSORS_NUMBER < 0 || SENSORS_NUMBER > MAX_SENSORS_NUMBER) {
        LOG_ERROR(MODULE, "Sensors number should be in range <1;32>");
        return false;
    }

    if (ADG726_init(&ctx.mux) == false) {
        LOG_ERROR(MODULE, "Unable to initialize multiplexer");
        return false;
    }

    if (ISL26102_init(&ctx.amp) == false) {
        LOG_ERROR(MODULE, "Unable to initialize amplifier");
        return false;
    }

    if (_configure_default() == false) {
        return false;
    }

    LOG_INFO(MODULE, "Sensor control initialized");
    return true;
}

bool sens_ctrl_move_to_next_address(void) {
    ctx.current_sensor = (ctx.current_sensor + 1) % SENSORS_NUMBER;

    uint8_t multiplexer_input_pin = ctx.current_sensor % MUX_MAX_ADDRESSES;
    if (ADG726_change_address(&ctx.mux, multiplexer_input_pin) == false) {
        LOG_ERROR(MODULE, "Unable to set multiplexer pin");
        return false;
    }

    // If it is first sensor for given channel, change channel
    if (ctx.current_sensor == CH1_SENS_NUM_MIN) {
        if (ISL26102_change_channel(&ctx.amp, ISL26102_CHANNEL_1) == false) {
            LOG_ERROR(MODULE, "Unable to change amplifier channel to 1");
            return false;
        }
    } else if (ctx.current_sensor == CH2_SENS_NUM_MIN) {
        if (ISL26102_change_channel(&ctx.amp, ISL26102_CHANNEL_2) == false) {
            LOG_ERROR(MODULE, "Unable to change amplifier channel to 2");
            return false;
        }
    }

    return true;
}

bool sens_ctrl_read_addr_data(void) {
    if (ISL26102_set_convertion_controll(&ctx.amp, ISL26102_SINGLE_CONVERSION) == false) {
        LOG_ERROR(MODULE, "Unable to set convertion controll");
        return false;
    }

    // Trust me I'm an enginner!
    uint8_t *buffer_pointer = ctx.sensors_data + (ctx.current_sensor * 3);
    if (ISL26102_read_raw_data(&ctx.amp, buffer_pointer) == false) {
        LOG_ERROR(MODULE, "Unable to read data");
        return false;
    }

    return true;
}

bool sens_ctrl_get_packed_data(uint8_t buffer[TOTAL_BYTES], uint8_t buffer_len) {
    if (buffer_len < TOTAL_BYTES) {
        LOG_ERROR(MODULE, "Buffer for data is too small");
        return false;
    }

    memcpy(buffer, ctx.sensors_data, sizeof(ctx.sensors_data));

    return true;
}