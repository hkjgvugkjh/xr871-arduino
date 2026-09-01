# XR871 Arduino v1.4.0

Arduino Board Manager package for XR871 WiFi SoC (XR871GT).

## Features

- **Full ARM GCC 13.2.rel1** toolchain bundled (no manual installation)
- **One-click flash** via Python BROM (reverse-engineered) + phoenixMC fallback
- **Multi-platform**: Linux x64/aarch64/arm, macOS x64, Windows
- **Arduino API**: pinMode, digitalWrite, analogRead, Serial, Wire, SPI, WiFi, OTA
- **Core libraries**: HardwareSerial, Wire (I2C), SPI, ADC, PWM, WiFi, Audio (I2S/DMIC), OTA

## Installation

1. Arduino IDE → File → Preferences
2. Additional Board Manager URLs:
   ```
   https://raw.githubusercontent.com/hkjgvugkjh/xr871-arduino/main/package_xr871_index.json
   ```
3. Board Manager → Search "XR871" → Install

## Build & Upload

```c
// Blink.ino
void setup() { pinMode(6, OUTPUT); }  // PA6 = LED
void loop() {
  digitalWrite(6, HIGH); delay(500);
  digitalWrite(6, LOW);  delay(500);
}
```

Select "XR871 EVB" → Compile → Upload (auto-detects serial port)

## Memory Map

| Region   | Address    | Size  | Purpose       |
|----------|------------|-------|---------------|
| RAM      | 0x00010000 | 448K  | Code + Data   |
| Flash    | 0x10000000 | 1MB   | XIP Execute   |

## Pin Mapping

| Arduino Pin | GPIO    | ADC  | PWM  | Special          |
|-------------|---------|------|------|------------------|
| 0-7         | PA0-PA7 | -    | -    | UART0, UART1     |
| 8-15        | PA8-PA15| CH0-7| CH0-7| ADC + PWM        |
| 16-22       | PA16-22 | -    | -    | GPIO             |
| 23-38       | PB0-PB15 | -    | -    | SPI0, GPIO       |

## Build System

```
Sketch (.ino) → g++ → .o
                ↓
Arduino Core (.cpp) → g++ → .o
                ↓
SDK lib (libxr871.a) → ld → firmware.elf → bin
                ↓
BROM Tool → USB Serial → Flash
```

## Flashing

| Method | Tool | Speed | Notes |
|--------|------|-------|-------|
| BROM (priority) | Python | 921600 | Pure Python, cross-platform |
| phoenixMC (fallback) | Binary | 921600 | Original XRADIO tool |

## Implemented Libraries

| Library | Status | Notes |
|---------|--------|-------|
| Arduino Core | ✅ | pinMode, digitalWrite, analogRead, delay, millis, attachInterrupt |
| HardwareSerial | ✅ | Serial.begin/print/read/flush |
| Wire (I2C) | ✅ | I2C0/I2C1 master mode |
| SPI | ✅ | SPI0/SPI1 master mode |
| ADC | ✅ | 9 channels, 12-bit |
| PWM | ✅ | 8 channels, 16-bit |
| WiFi | ✅ | STA/AP mode, scan, connect |
| Audio | ✅ | I2S TX/RX, DMIC |
| OTA | ✅ | HTTP OTA update |
| Camera | ⚠️ | CSI interface (stub) |
| WebServer | ⚠️ | HTTP server (stub) |
| HTTPClient | ⚠️ | HTTP client (stub) |

## Source Code

- https://github.com/hkjgvugkjh/xr871-arduino
- https://github.com/hkjgvugkjh/xr871-burn

## License

LGPL-2.1 (matches Arduino Core license)
