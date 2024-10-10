#ifndef HW_WRAPPERS_UART_H_
#define HW_WRAPPERS_UART_H_

#include <string.h>

void uart_service_write(const char *message, size_t len);

void uart_com_write(uint8_t *data, size_t len);

#endif