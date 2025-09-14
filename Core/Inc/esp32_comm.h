#ifndef ESP32_COMM_H
#define ESP32_COMM_H

#include "stm32f3xx_hal.h"
#include <stdint.h>

// Initialize UART reception
void esp32_comm_init(void);

// Try to get a full message from buffer
int receive_message_from_esp32(char *message, uint16_t max_len);

#endif
