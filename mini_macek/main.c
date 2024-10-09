/*
 * Copyright (c) 2023, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti/driverlib/dl_comp.h"
#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/dl_uart_extend.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"
#include "src/drivers/ADG726/ADG726.h"
#include "src/hw_wrappers/gpio.h"
#include "src/hw_wrappers/spi.h"
#include "src/app/logger_def.h"
#include "src/drivers/ISL26102/ISL26102.h"
#include <inttypes.h>
#include <stdbool.h>

#define DELAY (64000000)

int main(void) {
    SYSCFG_DL_init();
    GPIO_set_status_led_pin(STATUS_LED_ON);
    LOG_INIT()

    LOG("\r\n\033[0;35m\r\n");
    LOG("_|      _|    _|_|      _|_|_|  _|    _|  _|_|_|\r\n"
        "_|_|  _|_|  _|    _|  _|        _|  _|      _|  \r\n"
        "_|  _|  _|  _|_|_|_|  _|        _|_|        _|  \r\n"
        "_|      _|  _|    _|  _|        _|  _|      _|  \r\n"
        "_|      _|  _|    _|    _|_|_|  _|    _|  _|_|_|");
    LOG("\033[0m\r\n");

    ADG726_t mux = {
        ._set_gpio_pin = GPIO_set_mux_pin,
        .address_pins = {GPIO_MUX_A0_PIN, GPIO_MUX_A1_PIN, GPIO_MUX_A2_PIN, GPIO_MUX_A3_PIN},
        .cs_pin = GPIO_MUX_nCS_PIN,
        .wr_pin = GPIO_MUX_nWR_PIN
    };

    ISL26102_t amp = {
        .read_data = SPI_amp_read_data,
        .read_reg = SPI_amp_read_register,
        .write_reg = SPI_amp_set_register
    };

    ADG726_init(&mux);
    if (ISL26102_init(&amp) == true) {
        LOG_INFO("Amplifier initialized :D");
    } else {
        LOG_ERROR("Unable to initalize amplifier :CCC");
    }

    uint32_t data1 = 0;
    uint32_t data2 = 0;
    ADG726_change_address(&mux, 1);
    ISL26102_set_output_word_rate(&amp, ISL26102_DR_320);
    ISL26102_change_channel(&amp, ISL26102_CHANNEL_2);
    while (1) {
        ADG726_change_address(&mux, 1);
        ISL26102_set_convertion_controll(&amp, ISL26102_SINGLE_CONVERSION);
        ISL26102_read_data(&amp, &data1);

        ADG726_change_address(&mux, 2);
        ISL26102_set_convertion_controll(&amp, ISL26102_SINGLE_CONVERSION);
        ISL26102_read_data(&amp, &data2);

        LOG_DEBUG("Amplifier value\tCH1: %"PRIu32"\tCH2: %"PRIu32 , data1, data2);
        delay_cycles(DELAY);
    }
}
