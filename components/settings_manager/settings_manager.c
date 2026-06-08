/**
 * @file settings_manager.c
 * @brief Implementation of settings manager using ESP-IDF Non-Volatile Storage (NVS).
 *
 * Implements NVS initialization with recovery for corrupted or out-of-date partitions,
 * and read/write operations for configuration settings.
 *
 * @author Jhonny Ibarbo
 * @date 2026-06-08
 * @copyright Copyright (c) 2026 Jhonny Ibarbo
 * SPDX-License-Identifier: MIT
 */

#include "settings_manager.h"
#include "nvs_flash.h"
#include "nvs.h"

#define STORAGE_NAMESPACE "storage"
#define DELAY_KEY "blink_delay"

esp_err_t init_settings_storage(void)
{
    // Try initializing NVS flash partition
    esp_err_t ret = nvs_flash_init();
    
    // If NVS is out of free space or has a new version, erase it and try again
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        esp_err_t erase_err = nvs_flash_erase();
        if (erase_err != ESP_OK) {
            return erase_err;
        }
        ret = nvs_flash_init();
    }
    return ret;
}

esp_err_t save_blink_delay(int delay_ms)
{
    nvs_handle_t my_handle;
    esp_err_t err;

    // Open NVS handle in read-write mode
    err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        return err;
    }

    // Set signed 32-bit integer value for delay
    err = nvs_set_i32(my_handle, DELAY_KEY, (int32_t)delay_ms);
    if (err != ESP_OK) {
        nvs_close(my_handle);
        return err;
    }

    // Commit changes to flash storage
    err = nvs_commit(my_handle);

    // Close handle to release resources
    nvs_close(my_handle);
    return err;
}

int load_blink_delay(int default_delay_ms)
{
    nvs_handle_t my_handle;
    int32_t saved_delay = default_delay_ms;

    // Open NVS handle in read-only mode. If it fails, default value is returned.
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &my_handle);
    if (err == ESP_OK) {
        nvs_get_i32(my_handle, DELAY_KEY, &saved_delay);
        nvs_close(my_handle);
    }

    return (int)saved_delay;
}