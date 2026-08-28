# XR871 Arduino Core - Release Package

ESP32-compatible Arduino core for XR871 WiFi SoC (ARM Cortex-M4F, 192MHz, 448KB SRAM, 1MB Flash).

## Package Contents

```
xr871-arduino-1.0.0/
├── hardware/
│   └── xr871/
│       └── 1.0.0/
│           ├── boards.txt              # Board definitions
│           ├── platform.txt            # Platform build configuration
│           ├── programmers.txt         # Programmer definitions
│           ├── package_xr871_index.json # Board manager index
│           ├── README.txt              # Package readme
│           ├── cores/
│           │   └── xr871/
│           │       ├── Arduino.h       # Core header
│           │       ├── Arduino.cpp     # Core implementation
│           │       ├── Wire.h/cpp      # I2C library
│           │       ├── SPI.h/cpp       # SPI library
│           │       ├── PWM.h/cpp       # PWM library
│           │       ├── WiFi.h/cpp      # WiFi library
│           │       ├── WiFiClient.h/cpp # TCP client
│           │       ├── WiFiUDP.h/cpp   # UDP library
│           │       ├── HTTPClient.h/cpp # HTTP client
│           │       ├── OTA.h/cpp       # OTA updates
│           │       ├── WebServer.h/cpp # Web server
│           │       ├── DNSServer.h/cpp # DNS server
│           │       ├── MDNS.h/cpp      # mDNS
│           │       ├── Camera.h/cpp    # Camera interface
│           │       └── Audio.h/cpp     # Audio interface
│           ├── variants/
│           │   └── xr871gt/
│           │       ├── pins_arduino.h  # Pin definitions
│           │       ├── variant.cpp     # Pin mapping
│           │       └── linker.ld       # Linker script
│           └── libraries/
│               ├── WiFi/examples/
│               ├── Wire/examples/
│               ├── SPI/examples/
│               ├── HTTPClient/examples/
│               ├── WebServer/examples/
│               └── OTA/examples/
├── tools/
│   ├── gcc-arm-none-eabi/13.2.1/bin/   # ARM GCC toolchain wrappers
│   └── phoenixMC/1.0.0/                # Upload tool
├── install/
│   ├── install_linux.sh                # Linux installer
│   ├── install_macos.sh                # macOS installer
│   └── install_windows.bat             # Windows installer
├── package_xr871_index.json            # Board manager index
├── README.md                           # This file
└── HOWTO_INSTALL.txt                   # Installation guide
```

## Supported Platforms

| Platform | Architecture | Status |
|----------|--------------|--------|
| Linux | x86_64 | ✅ Supported |
| Linux | aarch64 | ✅ Supported |
| Windows | x86_64 (64-bit) | ✅ Supported |
| macOS | x86_64 (Intel) | ✅ Supported |
| macOS | arm64 (Apple Silicon) | ✅ Supported |

## Installation Methods

### Method 1: Arduino IDE Board Manager (Recommended)

1. Open Arduino IDE
2. Go to **File > Preferences**
3. In **Additional Boards Manager URLs**, add:
   ```
   https://raw.githubusercontent.com/xradio-tech/xr871-arduino/main/package_xr871_index.json
   ```
4. Go to **Tools > Board > Boards Manager**
5. Search for "XR871" and click **Install**
6. Select **Tools > Board > XR871 Arduino > XR871GT Development Board**

### Method 2: Arduino CLI

```bash
# Add board manager URL
arduino-cli config set board_manager.additional_urls \
  "https://raw.githubusercontent.com/xradio-tech/xr871-arduino/main/package_xr871_index.json"

# Update index
arduino-cli update

# Install board package
arduino-cli core install xr871:xr871

# Compile a sketch
arduino-cli compile --fqbn xr871:xr871:xr871gt ~/Arduino/Blink/
```

### Method 3: Manual Installation

#### Linux
```bash
# Run the installer
chmod +x install/install_linux.sh
./install/install_linux.sh

# Or manually copy to Arduino15 directory
cp -r hardware/xr871/1.0.0 ~/.arduino15/packages/hardware/xr871/
cp -r tools/* ~/.arduino15/packages/xr871/tools/
```

#### macOS
```bash
# Run the installer
chmod +x install/install_macos.sh
./install/install_macos.sh

# Or manually copy
cp -r hardware/xr871/1.0.0 ~/.arduino15/packages/hardware/xr871/
cp -r tools/* ~/.arduino15/packages/xr871/tools/
```

#### Windows
```batch
# Run the installer (as Administrator)
install\install_windows.bat

# Or manually copy
xcopy /E /I hardware\xr871\1.0.0 %USERPROFILE%\.arduino15\packages\hardware\xr871\1.0.0
xcopy /E /I tools %USERPROFILE%\.arduino15\packages\xr871\tools\
```

## Board Specifications

| Feature | Specification |
|---------|---------------|
| Chip | XR871 (ARM Cortex-M4F + FPU) |
| Clock | 192 MHz |
| SRAM | 448 KB |
| Flash | 1 MB |
| WiFi | 802.11 b/g/n |
| GPIO | 38 pins |
| I2C | 2 interfaces |
| SPI | 2 interfaces |
| PWM | 8 channels |
| UART | 2 interfaces |
| ADC | 12-bit |

## Pin Mapping

| Arduino Pin | XR871 Pin |
|-------------|-----------|
| 0-22 | GPIOA0-A22 |
| 23-38 | GPIOB0-B15 |
| LED_BUILTIN | PA6 |

## Examples

### Blink
```cpp
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}
```

### WiFi Station
```cpp
#include <WiFi.h>

const char* ssid = "YourSSID";
const char* password = "YourPassword";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected!");
    Serial.println(WiFi.localIP());
}

void loop() {}
```

### Web Server
```cpp
#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

void handleRoot() {
    server.send(200, "text/plain", "Hello from XR871!");
}

void setup() {
    WiFi.begin("ssid", "password");
    while (WiFi.status() != WL_CONNECTED) delay(500);
    
    server.on("/", handleRoot);
    server.begin();
}

void loop() {
    server.handleClient();
}
```

## ESP32 API Compatibility

This core is designed to be maximally compatible with ESP32 Arduino API:

| ESP32 API | XR871 Support |
|-----------|---------------|
| WiFi | ✅ Full |
| Wire (I2C) | ✅ Full |
| SPI | ✅ Full |
| analogWrite (PWM) | ✅ Full |
| HTTPClient | ✅ Full |
| WebServer | ✅ Full |
| ArduinoOTA | ✅ Full |
| DNSServer | ✅ Full |
| MDNS | ✅ Full |
| Camera | ✅ Stub |
| Audio | ✅ Stub |

## Building from Source

### Prerequisites
- Arduino IDE 2.x or arduino-cli
- ARM GCC toolchain (arm-none-eabi-gcc)
- Python 3 (for build scripts)

### Build Steps
```bash
# Clone the repository
git clone https://github.com/xradio-tech/xr871-arduino.git
cd xr871-arduino

# Build the package
python3 build.py

# The package will be in dist/
```

## Troubleshooting

### "arm-none-eabi-gcc not found"
Install the ARM GCC toolchain:
- Linux: `sudo apt install gcc-arm-none-eabi`
- macOS: `brew install arm-none-eabi-gcc`
- Windows: Download from [ARM Developer](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)

### "Board not found"
1. Make sure the board package is installed
2. Restart Arduino IDE
3. Check **Tools > Board > XR871 Arduino**

### Upload fails
1. Check USB connection
2. Install USB-to-Serial drivers (CP2102/CH340)
3. Press BOOT button while uploading

## License

LGPL-2.1-or-later

## Links

- [GitHub Repository](https://github.com/xradio-tech/xr871-arduino)
- [Documentation](https://github.com/xradio-tech/xr871-arduino/wiki)
- [XR871 SDK](https://github.com/XradioTech/XR871-OLD)
- [Issue Tracker](https://github.com/xradio-tech/xr871-arduino/issues)
