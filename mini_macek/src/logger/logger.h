#ifndef LOGGER_H_
#define LOGGER_H_

#include <string.h>
#include <stdbool.h>

#define MAX_BUFFER_SIZE 255

/**
 * @brief Callback function to write message
 *
 * @param message Message to write
 * @param len Length of message
 */
typedef void(*logger_write_callback)(const char *message, size_t len);

/**
 * @brief Initialize logger
 *
 * @param write_callback Callback function to write message
 * @return true if logger was initialized successfully
 * @return false if logger was not initialized
 */
bool logger_init(logger_write_callback write_callback);

/**
 * @brief Write formatted message to logger
 *
 * @param format Format string
 */
void logger_write(const char *format, ...);

/**
 * @brief Write message to logger
 *
 * @param message Message to write
 * @param len Length of message
 */
void logger_write_message(const char *message, size_t len);

#endif