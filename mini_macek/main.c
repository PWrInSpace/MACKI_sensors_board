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
#include "src/app/sensors_controller.h"
#include <inttypes.h>
#include <stdbool.h>

#define DELAY (16000000)

#define MODULE "main"

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


    sens_ctrl_init();

    uint8_t buffer[TOTAL_BYTES] = {0};
    uint32_t data1 = 0;
    uint32_t data2 = 0;
    while (1) {
        sens_ctrl_move_to_next_address();
        sens_ctrl_read_addr_data();

        sens_ctrl_move_to_next_address();
        sens_ctrl_read_addr_data();

        sens_ctrl_get_packed_data(buffer, sizeof(buffer));

        data1 = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
        data2 = (buffer[3] << 16) | (buffer[4] << 8) | buffer[5];
        LOG_DEBUG(MODULE, "Amplifier value\tCH1: %"PRIu32"\tCH2: %"PRIu32 , data1, data2);
        delay_cycles(DELAY);
    }
}
