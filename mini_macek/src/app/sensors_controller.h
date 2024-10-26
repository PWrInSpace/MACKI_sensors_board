#ifndef APP_SENSOR_CONTROLLER_H_
#define APP_SENSOR_CONTROLLER_H_

#include <stdbool.h>
#include <stdint.h>

#include "src/app/sensors_controller.h"
#include "src/drivers/ISL26102/ISL26102.h"

#define SENSORS_NUMBER 32
#define BYTES_PER_VALUE 3
#define TOTAL_BYTES BYTES_PER_VALUE * SENSORS_NUMBER
// Max number of sensor is 32
#define DEFAULT_OWR ISL26102_DR_4000

bool sens_ctrl_init(void);
bool sens_ctrl_move_to_next_address(void);
bool sens_ctrl_read_addr_data(void);

bool sens_ctrl_get_packed_data(uint8_t buffer[TOTAL_BYTES], uint8_t buffer_len);

#endif