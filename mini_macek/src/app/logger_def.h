#ifndef APP_LOGGER_DEF_H_
#define APP_LOGGER_DEF_H_

#include "../logger/logger.h"
#include "../hw_wrappers/uart.h"

#define LOG_COLORF_GREEN "\033[0;32m"
#define LOG_COLORF_YELLOW "\033[0;33m"
#define LOG_COLORF_RED "\033[0;31m"

#define LOG_COLOR_RESET "\033[0m"

#define LOG_INIT()              logger_init(uart_service_write);

#define LOG(format, ...)\
    logger_write(format"\r\n", ##__VA_ARGS__)

#define LOG_DEBUG(format, ...)\
    logger_write("D: "format"\r\n", ##__VA_ARGS__)

#define LOG_INFO(format, ...)\
    logger_write(LOG_COLORF_GREEN"I: "format"\r\n"LOG_COLOR_RESET, ##__VA_ARGS__)

#define LOG_WARN(format, ...)\
    logger_write(LOG_COLORF_YELLOW"W: "format"\r\n"LOG_COLOR_RESET, ##__VA_ARGS__)

#define LOG_ERROR(format, ...)\
    logger_write(LOG_COLORF_RED"E: "format"\r\n"LOG_COLOR_RESET, ##__VA_ARGS__)

#endif