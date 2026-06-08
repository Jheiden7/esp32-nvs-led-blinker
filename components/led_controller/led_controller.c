/**
 * @file led_controller.c
 * @brief Implementation of the LED controller component.
 *
 * Configures the GPIO peripheral and controls the physical state of the status LED.
 *
 * @author Jhonny Ibarbo
 * @date 2026-06-08
 * @copyright Copyright (c) 2026 Jhonny Ibarbo
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include "led_controller.h"
#include "driver/gpio.h"

void init_led(int led_pin)
{
    // Reset the pin to its default state to clear previous configurations
    gpio_reset_pin(led_pin);

    // Configure the GPIO pin as a digital output
    gpio_set_direction(led_pin, GPIO_MODE_OUTPUT);
}

void set_led_state(int led_pin, bool state)
{
    // Write high or low level to the configured GPIO pin
    gpio_set_level(led_pin, state);
}
