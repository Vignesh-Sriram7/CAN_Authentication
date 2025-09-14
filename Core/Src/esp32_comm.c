#include "esp32_comm.h"
#include <string.h>

extern UART_HandleTypeDef huart3;   // defined in main.c or usart.c

#define ESP32_RX_BUFFER_SIZE 256

static uint8_t rx_buffer[ESP32_RX_BUFFER_SIZE];  // circular buffer
static uint16_t rx_index = 0;                     // current index

// Start reception in interrupt mode
void esp32_comm_init(void) {
    rx_index = 0;
    HAL_UART_Receive_IT(&huart3, &rx_buffer[rx_index], 1);
}

// UART RX complete callback (called by HAL automatically)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        rx_index++;
        if (rx_index >= ESP32_RX_BUFFER_SIZE) {
            rx_index = 0; // wrap if overflow
        }

        // Restart reception for next byte
        HAL_UART_Receive_IT(&huart3, &rx_buffer[rx_index], 1);
    }
}

// Extract newline-terminated message from buffer
int receive_message_from_esp32(char *message, uint16_t max_len) {
    for (uint16_t i = 0; i < rx_index; i++) {
        if (rx_buffer[i] == '\n') {
            // Copy message up to newline
            uint16_t msg_len = (i < max_len) ? i : max_len - 1;
            memcpy(message, rx_buffer, msg_len);
            message[msg_len] = '\0';

            // Shift buffer after the newline
            memmove(rx_buffer, &rx_buffer[i + 1], rx_index - (i + 1));
            rx_index -= (i + 1);

            return 1; //
        }
    }
    return 0; //
}


