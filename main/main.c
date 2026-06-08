/**
 * @file main.c
 * @brief Main entry point of the ESP32 blinking led & settings controller application.
 *
 * This application demonstrates a basic ESP-IDF project utilizing FreeRTOS tasks,
 * NVS (Non-Volatile Storage) for persisting system parameters (blink delay), 
 * and custom components for LED control and settings management.
 *
 * @author Jhonny Ibarbo
 * @date 2026-06-08
 * @copyright Copyright (c) 2026 Jhonny Ibarbo
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/uart_vfs.h"

#include "led_controller.h"
#include "settings_manager.h"

/* --- Configuration Macros --- */
#define LED_GPIO 32         ///< GPIO pin mapped to the status LED
#define DEFAULT_DELAY 2000  ///< Fallback blink delay in milliseconds if NVS is empty

/* --- Global Variables --- */
/**
 * @brief Global variable containing the current LED blink interval.
 * 
 * Declared volatile as it is written by the serial task and read by the blink task.
 */
volatile int g_blink_delay_ms;

/* --- Private Function Declarations --- */
void blink_task(void *pvParameters);
void serial_task(void *pvParameters);

/**
 * @brief Main application entry point.
 * 
 * Initializes NVS storage, configures the LED GPIO, loads the saved blink interval,
 * redirects stdin/stdout to the UART driver, and spawns the FreeRTOS tasks.
 */
void app_main(void)
{
    // Initialize Non-Volatile Storage (NVS)
    ESP_ERROR_CHECK(init_settings_storage());
    
    // Configure LED GPIO pin direction
    init_led(LED_GPIO);

    // Retrieve last saved blink delay interval from flash memory
    g_blink_delay_ms = load_blink_delay(DEFAULT_DELAY);
    printf("System Booted. Current saved delay: %dms\n", g_blink_delay_ms);

    // Configure and install UART driver for interactive console input
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0));

    // Tell VFS to use UART driver for standard I/O streams
    uart_vfs_dev_use_driver(UART_NUM_0);

    // Spawn the LED blinking task
    xTaskCreate(
        blink_task,
        "Blink_Task",
        2048,
        NULL,
        5,
        NULL
    );

    // Spawn the interactive console parser task
    xTaskCreate(
        serial_task,
        "Serial_Task",
        4096,
        NULL,
        5,
        NULL
    );
}

/**
 * @brief FreeRTOS task responsible for toggling the status LED.
 * 
 * Continuously toggles the LED state at the interval specified by the global variable.
 */
void blink_task(void *pvParameters)
{
    bool led_state = false;

    while (1) {
        led_state = !led_state;
        set_led_state(LED_GPIO, led_state);

        // Block task execution for the duration of the current blink delay
        vTaskDelay(pdMS_TO_TICKS(g_blink_delay_ms));
    }
}

/**
 * @brief FreeRTOS task responsible for handling interactive serial input.
 * 
 * Listens on stdin for numeric delay values (in milliseconds), validates the input range,
 * and updates both the active runtime variable and the persistent NVS storage.
 */
void serial_task(void *pvParameters)
{
    char buffer[16];
    printf("\nEnter new blink delay in milliseconds:\n");

    while (1) {
        // Read input line from console
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Remove newline character if present
            buffer[strcspn(buffer, "\n")] = '\0';

            // Parse input string to integer
            int new_delay = atoi(buffer);

            // Validate the input bounds (50ms to 60000ms / 1 minute)
            if (new_delay >= 50 && new_delay <= 60000) {
                printf("Successfully updated and saved delay from %dms to: %dms\n", g_blink_delay_ms, new_delay);
                g_blink_delay_ms = new_delay;
                save_blink_delay(new_delay);
            }
            else {
                printf("Invalid input! Please enter a number between 50 and 60000.\n");
            }
        } 
    }
}
