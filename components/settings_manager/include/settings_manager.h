/**
 * @file settings_manager.h
 * @brief Settings manager component using Non-Volatile Storage (NVS) for configuration persistence.
 *
 * This component provides simple wrappers for initializing the NVS storage partition
 * and loading/saving settings (such as blink delay) to persist across system resets.
 *
 * @author Jhonny Ibarbo
 * @date 2026-06-08
 * @copyright Copyright (c) 2026 Jhonny Ibarbo
 * SPDX-License-Identifier: MIT
 */

#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the Non-Volatile Storage (NVS) flash partition.
 * 
 * Attempts to initialize the default NVS partition. If the partition has no free pages,
 * or contains data in a format different from the current version, it will be erased
 * and reinitialized.
 * 
 * @return esp_err_t ESP_OK on success, or an error code indicating the cause of failure.
 */
esp_err_t init_settings_storage(void);

/**
 * @brief Saves the current blink delay to NVS.
 * 
 * Opens the storage namespace in read-write mode and writes the delay value.
 * Commits the write to ensure it is flushed to flash.
 * 
 * @param delay_ms The LED blink delay in milliseconds.
 * @return esp_err_t ESP_OK on success, or an error code indicating the cause of failure.
 */
esp_err_t save_blink_delay(int delay_ms);

/**
 * @brief Loads the blink delay from NVS.
 * 
 * Opens the storage namespace in read-only mode and attempts to retrieve the delay value.
 * If the key does not exist or storage cannot be opened, the default delay is returned.
 * 
 * @param default_delay_ms The fallback delay to return if no saved delay is found.
 * @return int The loaded delay from flash, or the fallback default delay.
 */
int load_blink_delay(int default_delay_ms);

#ifdef __cplusplus
}
#endif

#endif // SETTINGS_MANAGER_H