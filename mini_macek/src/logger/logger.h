#ifndef LOGGER_H_
#define LOGGER_H_

#include <string.h>
#include <stdbool.h>

#define MAX_BUFFER_SIZE 255

typedef void(*logger_write_callback)(const char *message, size_t len);

bool logger_init(logger_write_callback write_callback);

void logger_write(const char *format, ...);

void logger_write_message(const char *message, size_t len);

#endif