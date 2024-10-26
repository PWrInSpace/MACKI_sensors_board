#ifndef APP_LOGGER_DEF_H_
#define APP_LOGGER_DEF_H_

#include "../logger/logger.h"
#include "../hw_wrappers/hw_config.h"
#include "src/app/logger_def.h"

#define LOG_COLORF_GREEN "\033[0;32m"
#define LOG_COLORF_YELLOW "\033[0;33m"
#define LOG_COLORF_RED "\033[0;31m"
#define LOG_COLORF_WHITE "\033[97m"
#define LOG_COLOR_RESET "\033[0m"

#define LOG_LEVEL_STR_DEBUG "D"
#define LOG_LEVEL_STR_INFO "I"
#define LOG_LEVEL_STR_WARN "W"
#define LOG_LEVEL_STR_ERROR "E"


#define LOG_INIT()              logger_init(LOGGER_WRITE_UART);

#define LOG(format, ...)\
    logger_write(format, ##__VA_ARGS__)

#define LOG_DEBUG(module, format, ...)\
    LOG_BASIC(LOG_COLORF_WHITE, LOG_LEVEL_STR_DEBUG, module, format, ##__VA_ARGS__)

#define LOG_INFO(module, format, ...)\
    LOG_BASIC(LOG_COLORF_GREEN, LOG_LEVEL_STR_INFO, module, format, ##__VA_ARGS__)

#define LOG_WARN(module, format, ...)\
    LOG_BASIC(LOG_COLORF_YELLOW, LOG_LEVEL_STR_WARN, module, format, ##__VA_ARGS__)

#define LOG_ERROR(module, format, ...)\
    LOG_BASIC(LOG_COLORF_RED, LOG_LEVEL_STR_ERROR, module, format, ##__VA_ARGS__)

#define LOG_BASIC(color, level, module, format, ...) \
    logger_write(color module " | " level ": " format "\r\n" LOG_COLOR_RESET, ##__VA_ARGS__)

#endif