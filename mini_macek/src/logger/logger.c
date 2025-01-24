#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "logger.h"

static struct {
    logger_write_callback _write;
    bool initialized;
    char buffer[MAX_BUFFER_SIZE];
} logger = {
    ._write = NULL,
    .initialized = false
};


bool logger_init(logger_write_callback write_callback) {
    if (write_callback == NULL) {
        return false;
    }

    logger._write = write_callback;
    logger.initialized = true;

    return true;
}


void logger_write(const char *format, ...) {
    memset(logger.buffer, 0, sizeof(logger.buffer));

    va_list args;
    va_start(args, format);

    // do not check return value, due to lack of possibility to signalize error
    (void) vsnprintf(logger.buffer, sizeof(logger.buffer), format, args);
    logger_write_message(logger.buffer, strlen(logger.buffer));

    va_end(args);
}


void logger_write_message(const char *message, size_t len) {
    if (logger.initialized == false) {
        return;
    }

    logger._write(message, len);
}