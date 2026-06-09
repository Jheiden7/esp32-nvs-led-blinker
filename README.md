# ESP32 Interactive LED Blinker with NVS Persistence

A modular firmware project built on the **ESP-IDF v6.0.1** framework for the ESP32 family of microcontrollers. This application demonstrates best practices in embedded C development, using **FreeRTOS multitasking**, GPIO control, **UART VFS** virtual file system redirection, and **Non-Volatile Storage (NVS)** to persist system settings across power cycles.

---

## Table of Contents

1. [Overview](#-overview)
2. [Architecture](#-architecture)
3. [Component Overview](#-component-overview)
4. [Hardware Configuration](#-hardware-configuration)
5. [Getting Started](#-getting-started)
6. [Interactive Console Usage](#-interactive-console-usage)

---

## Overview

The goal of this application is to provide a non-blocking interactive command interface to dynamically adjust the blinking speed of a status LED. The system ensures that:

- **LED Blinking** runs smoothly in a dedicated task, unaffected by blocking console input.
- **Serial Input** is processed in real-time through standard C library calls (`fgets`, `atoi`) mapped to the ESP-IDF UART driver.
- **Blink settings** are validated and persisted to NVS flash, so the device recovers the last configured state on boot.

---

## Architecture

The application is split into a main entry point and two custom ESP-IDF components. Concurrency is managed via FreeRTOS.

### FreeRTOS Multitasking Details

1. **`Blink_Task`**: Reads the global `g_blink_delay_ms` value on each iteration, toggles the LED pin state via the LED controller, and calls `vTaskDelay` to yield processor control.
2. **`Serial_Task`**: Standard input is redirected to the UART driver using ESP-IDF's VFS driver. The task uses blocking stream reads (`fgets`) to wait for user input without consuming CPU cycles. Once valid numeric data is received, it updates the global variable and writes to NVS.

---

## Component Overview

### 1. `led_controller`

Handles hardware-level configuration of the LED GPIO.

- **Header:** [led_controller.h] `components/led_controller/include/led_controller.h`
- **Source:** [led_controller.c] `components/led_controller/led_controller.c`
- **Functions:**
  - `init_led(int led_pin)`: Resets and configures the GPIO as output.
  - `set_led_state(int led_pin, bool state)`: Writes logical levels to the GPIO.

### 2. `settings_manager`

Manages the Non-Volatile Storage (NVS) key-value records for persisting variables.

- **Header:** [settings_manager.h] `components/settings_manager/include/settings_manager.h`
- **Source:** [settings_manager.c] `components/settings_manager/settings_manager.c`
- **Functions:**
  - `init_settings_storage()`: Mounts the NVS partition. Automatically erases and reformats the partition if it encounters structure version changes or pages overflow.
  - `save_blink_delay(int delay_ms)`: Writes the configured value to the NVS key `"blink_delay"`.
  - `load_blink_delay(int default_delay_ms)`: Reads `"blink_delay"` value. Returns the fallback argument if NVS is uninitialized or the key doesn't exist.

---

## Hardware Configuration

By default, the project is configured to drive an LED connected to **GPIO 32**.

```
             ESP32 Board
          ┌──────────────┐
          │              │
          │      GPIO 32 ├─────► [R: 220Ω] ───► [LED] ───► GND
          │              │
          └──────────────┘
```

> [!NOTE]
> You can change the target pin by modifying the `LED_GPIO` macro defined inside [main.c] `main/main.c`

---

## Getting Started

### Prerequisites

1. Install the ESP-IDF command line tools or standard VS Code extension. (ESP-IDF v5.0+ recommended).
2. Set up environment paths. On Windows Powershell, run:
   ```powershell
   . $env:IDF_PATH/export.ps1
   ```

### Setup and Build

1. Clone or download this project to your local directory.
2. Initialize target (e.g., target ESP32):
   ```bash
   idf.py set-target esp32
   ```
3. Compile the firmware:
   ```bash
   idf.py build
   ```

### Flash and Monitor

Connect your development board to your computer and execute:

```bash
idf.py -p <PORT> flash monitor
```

_(Replace `<PORT>` with your serial COM port, e.g., `COM3` on Windows or `/dev/ttyUSB0` on Linux)._

To exit the serial monitor, press `Ctrl` + `]`.

---

## Interactive Console Usage

Upon boot, the device outputs system boot information followed by a prompt:

```text
System Booted. Current saved delay: 2000ms

Enter new blink delay in milliseconds:
```

### Adjusting the Delay

1. Type a numeric value between **50** (50ms) and **60000** (60 seconds) in your monitor console.
2. Press `Enter`.
3. The board updates the active blinking speed immediately and writes the setting to flash memory:
   ```text
   500
   Successfully updated and saved delay from 2000ms to: 500ms
   ```
4. If you enter invalid bounds (e.g. `20`) or string text, the validator will output:
   ```text
   Invalid input! Please enter a number between 50 and 60000.
   ```

---
