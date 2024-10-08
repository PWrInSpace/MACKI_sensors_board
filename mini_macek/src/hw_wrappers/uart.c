#include "ti/driverlib/dl_uart_extend.h"
#include "ti_msp_dl_config.h"

#include "uart.h"


void uart_service_write(const char *message, size_t len) {
    if (message == NULL) {
        return;
    }
    
    for (size_t i = 0; i < len; ++i) {
        DL_UART_Extend_transmitDataBlocking(UART_SERVICE_INST, (uint8_t) message[i]);
    }
}

