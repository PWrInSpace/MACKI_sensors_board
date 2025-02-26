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

#include "src/drivers/ISL26102/ISL26102.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti/driverlib/m0p/sysctl/dl_sysctl_mspm0l11xx_l13xx.h"
#include "ti_msp_dl_config.h"
#include "src/app/logger_def.h"
#include "src/app/sensors_controller.h"
#include "src/hw_wrappers/uart.h"
#include <string.h>
#include <inttypes.h>

#define MODULE "main"

#define PREFIX_SIZE 4
static uint8_t message_prefix[PREFIX_SIZE] = {0x01, 0x02, 0x03, 0x04};

void print_welcome_message();
void dump_sensors_info(uint8_t *buffer);

int main(void) {
    SYSCFG_DL_init();
    LOG_INIT();
    delay_cycles(3200000);
    if (sens_ctrl_init() == false) {
       DL_SYSCTL_RESET_CPU;
    }

    GPIO_set_status_led_pin(STATUS_LED_ON);

    print_welcome_message();

    uint8_t buffer[PREFIX_SIZE + TOTAL_BYTES] = {0};
    while (1) {
        for (uint8_t i = 0; i < 32; ++i) {
            if (sens_ctrl_move_to_next_address() == false) {
                DL_SYSCTL_RESET_CPU;
            }

            if (sens_ctrl_read_addr_data() == false) {
                DL_SYSCTL_RESET_CPU;
            }
        }

        // copy prefix
        memcpy(buffer, message_prefix, PREFIX_SIZE);

        // copy data to the remining bytes of buffer
        sens_ctrl_get_packed_data(buffer + PREFIX_SIZE, TOTAL_BYTES);

        // write message
        uart_com_write(buffer, sizeof(buffer));

        dump_sensors_info(buffer + PREFIX_SIZE);

        delay_cycles(8000000);
    }
}

void print_welcome_message(void) {
    LOG("\r\n\033[0;35m\r\n");
    LOG("_|      _|    _|_|      _|_|_|  _|    _|  _|_|_|\r\n"
        "_|_|  _|_|  _|    _|  _|        _|  _|      _|  \r\n"
        "_|  _|  _|  _|_|_|_|  _|        _|_|        _|  \r\n"
        "_|      _|  _|    _|  _|        _|  _|      _|  \r\n"
        "_|      _|  _|    _|    _|_|_|  _|    _|  _|_|_|");
    LOG("\033[0m\r\n");
}


static uint8_t detected_sensors[32] = {0};

void dump_sensors_info(uint8_t *buffer) {
    uint32_t data;

    LOG("\033[2J");
    for (uint8_t i = 0; i < 32; ++i) {
        if (i % 8 == 0) {
            LOG("\r\n");
        }

        data = (buffer[i * 3] << 16) | (buffer[i * 3 + 1] << 8) | buffer[i * 3 + 2];
        if (data < 7888607 || data > 9388607) {
            LOG(LOG_COLORF_YELLOW "%02d: %"PRIu32"\t"LOG_COLOR_RESET, i, data);
            detected_sensors[i] = 1;
        } else if (detected_sensors[i] == 1) {
            LOG(LOG_COLORF_GREEN "%02d: %"PRIu32"\t"LOG_COLOR_RESET, i, data);
        } else {
            LOG("%02d: %"PRIu32"\t", i, data);
        }
    }
}