# STM32 Environmental Sensor Node

Small environmental monitoring node based on an STM32 microcontroller.  
The firmware reads **temperature, humidity and pressure** from I²C sensors and:

- Displays live values on a small OLED display (SSD1306)
- Sends data periodically over UART for logging on a PC

The focus of this project is **bare-metal / low-level embedded development**:
custom drivers, register-level access and a clean firmware architecture.

---

## Hardware

- **MCU:** STM32L4 (Nucleo / Discovery board)
- **Sensors:**
  - HTS221 – temperature & humidity (I²C)
  - BMP280 – pressure & temperature (I²C)
- **Display:**
  - SSD1306 OLED 128x64 (I²C)
- **Interface:**
  - UART to USB (USB-UART adapter or ST-Link virtual COM)
- Breadboard and jumper wires

---

## Firmware architecture (planned)

- `i2c_driver.c/.h`  
  Low-level I²C driver (blocking, register-based)

- `hts221.c/.h`  
  Driver for the HTS221 sensor (WHO_AM_I, temperature & humidity in physical units)

- `bmp280.c/.h`  
  Driver for the BMP280 sensor (pressure & temperature with calibration)

- `oled_ssd1306.c/.h`  
  Basic text-only UI on SSD1306 via I²C

- `uart_driver.c/.h`  
  Simple UART logger for streaming CSV data to a PC

- `timer_driver.c/.h`  
  Periodic timer used to trigger sensor readings and UART transmissions

- `system_config.c/.h`  
  Clock, GPIO and peripheral base configuration

---

## Project goals

1. Implement a **custom I²C driver** (no HAL) for STM32.
2. Integrate **HTS221** and **BMP280** sensors using that I²C driver.
3. Show live sensor data on an **OLED (SSD1306)** display.
4. Send readings over **UART** in a machine-readable format (CSV or JSON).
5. Structure the firmware in **clean, reusable modules** suitable for professional embedded development.

---

## Roadmap

- [x] Create STM32 project and base structure
- [x] Implement and test low-level I²C driver
- [x] Bring-up HTS221 (WHO_AM_I + raw readings)
- [x] Convert raw HTS221 data to °C / %RH
- [ ] Integrate BMP280 and verify pressure readings
- [ ] Show values on SSD1306 (basic text UI)
- [ ] Add UART logging (CSV)
- [ ] Refactor and document final architecture
- [ ] Record a demo video and add photos

---

## About

This project is part of my embedded systems portfolio, focused on STM32 bare-metal development, sensor integration and clean driver design.
