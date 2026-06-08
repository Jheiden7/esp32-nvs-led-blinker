/**
 * @file led_controller.h
 * @brief LED controller component for configuring and controlling status LEDs.
 *
 * This component provides simple interfaces to initialize GPIO pins as LED outputs and
 * set their states.
 *
 * @author Jhonny Ibarbo
 * @date 2026-06-08
 * @copyright Copyright (c) 2026 Jhonny Ibarbo
 * SPDX-License-Identifier: MIT
 */

#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes a GPIO pin as an LED output.
 * 
 * Resets the specified pin and configures its direction to output mode.
 * 
 * @param led_pin GPIO pin number to initialize.
 */
void init_led(int led_pin);

/**
 * @brief Sets the state (high/low) of the specified LED.
 * 
 * @param led_pin GPIO pin number of the LED.
 * @param state   True to turn the LED on (GPIO high), false to turn it off (GPIO low).
 */
void set_led_state(int led_pin, bool state);

#ifdef __cplusplus
}
#endif

#endif // LED_CONTROLLER_H