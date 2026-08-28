# XR871 Arduino移植项目 (方案C)

[![Version](https://img.shields.io/badge/version-1.1.0-blue.svg)](https://github.com/hkjgvugkjh/xr871-arduino/releases)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

## 项目概述

本方案在XR871官方SDK基础上移植Arduino API，实现：
- **Arduino.h** - Arduino API头文件声明
- **Arduino.c** - Arduino API实现（HAL层封装）
- **Wire.h/Wire.cpp** - I2C驱动库（TwoWire类）
- **SPI.h/SPI.cpp** - SPI驱动库（SPIClass类）
- **PWM.h/PWM.cpp** - PWM驱动库（analogWrite支持）
- **ADC.h/ADC.cpp** - ADC驱动库（analogRead支持）
- **WiFi.h/WiFi.cpp** - WiFi库（lwIP协议栈封装）
- **OTA.h/OTA.cpp** - OTA升级库
- **Audio.h/Audio.cpp** - I2S/DMIC音频库
- **Blink.ino** - Blink示例程序
- **Makefile** - 编译系统集成

## 项目结构

```
xr871-arduino/
├── Arduino.h              # Arduino API头文件
├── Arduino.c              # Arduino API实现
├── Wire.h                 # I2C头文件
├── Wire.cpp               # I2C实现
├── SPI.h                  # SPI头文件
├── SPI.cpp                # SPI实现
├── PWM.h                  # PWM头文件
├── PWM.cpp                # PWM实现
├── ADC.h                  # ADC头文件
├── ADC.cpp                # ADC实现
├── WiFi.h                 # WiFi头文件
├── WiFi.cpp               # WiFi实现
├── WiFiClient.h           # WiFi客户端
├── WiFiClient.cpp         # WiFi客户端实现
├── WiFiUDP.h              # WiFi UDP
├── WiFiUDP.cpp            # WiFi UDP实现
├── OTA.h                  # OTA头文件
├── OTA.cpp                # OTA实现
├── Audio.h                # 音频头文件
├── Audio.cpp              # 音频实现
├── Blink.ino              # Blink示例
├── I2C_Scan.ino           # I2C扫描示例
├── SPI_Test.ino           # SPI测试示例
├── PWM_Test.ino           # PWM测试示例
├── WiFi_Scan.ino          # WiFi扫描示例
├── WiFi_Station.ino       # WiFi STA示例
├── WiFi_AP.ino            # WiFi AP示例
├── HTTP_Client.ino        # HTTP客户端示例
├── Web_Server.ino         # Web服务器示例
├── Makefile               # 编译配置
├── README.md              # 本文件
├── INSTALL_GUIDE.md       # 安装指南
├── package_xr871_index.json  # Board Manager索引
├── platform.txt           # 平台配置
├── boards.txt             # 开发板配置
└── SDK/                   # 参考SDK头文件
    └── include/
```

## 移植说明

### 已完成模块

| 模块 | 状态 | 说明 |
|------|------|------|
| **GPIO** | ✅ | pinMode/digitalWrite/digitalRead 已实现 |
| **UART/Serial** | ✅ | serialBegin/serialPrint/serialRead 已实现 |
| **I2C (Wire)** | ✅ | TwoWire类，支持I2C0/I2C1，主模式 |
| **SPI** | ✅ | SPIClass类，支持SPI0/SPI1，4种模式 |
| **PWM** | ✅ | 8通道PWM，支持analogWrite |
| **ADC** | ✅ | 9通道12位ADC，analogRead支持 |
| **WiFi** | ✅ | WiFiClass，STA/AP模式，lwIP协议栈 |
| **OTA** | ✅ | ArduinoOTAClass，HTTP OTA升级 |
| **I2S** | ✅ | AudioOutput/AudioInput，I2S音频 |
| **DMIC** | ✅ | 数字麦克风输入支持 |
| **Time** | ✅ | delay/millis/micros 已实现 |
| **Interrupt** | ✅ | attachInterrupt/detachInterrupt 已实现 |
| **Math** | ✅ | min/max/constrain 等宏定义完成 |
| **Bit** | ✅ | bitSet/bitClear/bitRead 等宏定义完成 |

### 待实现模块

| 模块 | 状态 | 说明 |
|------|------|------|
| **Camera** | 🔶 | 需要封装hal_csi.h |
| **IR** | 🔶 | 需要封装hal_irrx.h/hal_irtx.h |
| **RTC** | 🔶 | 需要封装hal_rtc.h |
| **Crypto** | 🔶 | 需要封装hal_crypto.h |
| **SD/MMC** | 🔶 | 需要封装hal_sdmmc.h |

## 硬件引脚映射

### GPIO引脚映射

| Arduino Pin | XR871 GPIO | 说明 |
|-------------|------------|------|
| 0-22 | GPIOA0-A22 | GPIOA 端口 |
| 23-38 | GPIOB0-B15 | GPIOB 端口 |
| LED_BUILTIN | PA6 | 板载LED |

### ADC引脚映射

| Arduino Pin | GPIO | ADC通道 |
|-------------|------|---------|
| A0 | PA0 | ADC_CH0 |
| A1 | PA1 | ADC_CH1 |
| A2 | PA2 | ADC_CH2 |
| A3 | PA3 | ADC_CH3 |
| A4 | PA4 | ADC_CH4 |
| A5 | PA5 | ADC_CH5 |
| A6 | PA6 | ADC_CH6 |
| A7 | PA7 | ADC_CH7 |
| VBAT | PA8 | ADC_CH8 (电池电压) |

### I2C引脚映射

| I2C端口 | SCL | SDA |
|---------|-----|-----|
| I2C0 (Wire) | PA2 | PA3 |
| I2C1 (Wire1) | PA4 | PA5 |

### SPI引脚映射

| SPI端口 | MOSI | MISO | CLK | CS |
|---------|------|------|-----|-----|
| SPI0 | PB4 | PB5 | PB7 | PB6 |
| SPI1 | PA0 | PA1 | PA2 | PA3 |

### PWM引脚映射

| Arduino Pin | GPIO | PWM通道 | 组别 |
|-------------|------|---------|------|
| 8 | PA8 | CH0 | GROUP0 |
| 9 | PA9 | CH1 | GROUP0 |
| 10 | PA10 | CH2 | GROUP1 |
| 11 | PA11 | CH3 | GROUP1 |
| 12 | PA12 | CH4 | GROUP2 |
| 13 | PA13 | CH5 | GROUP2 |
| 14 | PA14 | CH6 | GROUP3 |
| 15 | PA15 | CH7 | GROUP3 |

### I2S引脚映射

| 信号 | GPIO | 说明 |
|------|------|------|
| BCLK | PA10 | 位时钟 |
| LRCK | PA11 | 左右声道选择 |
| DATA_OUT | PA12 | I2S输出 |
| DATA_IN | PA13 | I2S输入 |

### DMIC引脚映射

| 信号 | GPIO | 说明 |
|------|------|------|
| CLK | PA14 | DMIC时钟 |
| DATA | PA15 | DMIC数据 |

## 编译步骤

### 前置条件

1. **安装GCC ARM Embedded工具链**
   ```bash
   # Ubuntu/Debian
   sudo apt install gcc-arm-none-eabi
   
   # 或下载特定版本
   # https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
   ```

2. **获取XR871 SDK**
   ```bash
   git clone https://github.com/XradioTech/XR871-OLD.git
   cd XR871-OLD/03_SDK/xr871sdk
   ```

3. **编译**
   ```bash
   # 在Arduino项目目录下
   make XR871SDK=/path/to/xr871sdk
   ```

### 输出文件

- `build/xr871_arduino_blink.elf` - ELF可执行文件
- `build/xr871_arduino_blink.bin` - 二进制烧录文件
- `build/xr871_arduino_blink.lst` - 反汇编列表

## 使用说明

### Arduino API兼容性

本移植支持以下Arduino API：

```cpp
// 数字I/O
pinMode(pin, mode);
digitalWrite(pin, value);
int value = digitalRead(pin);

// 模拟I/O
int adcValue = analogRead(pin);     // 已实现 (12位ADC)
analogWrite(pin, value);            // 已实现 (PWM)

// 时间
delay(ms);
delayMicroseconds(us);
unsigned long now = millis();

// 中断
attachInterrupt(pin, ISR, mode);
detachInterrupt(pin);

// 串口
serialBegin(115200);
serialPrintln("Hello");

// I2C (Wire)
Wire.begin();
Wire.beginTransmission(addr);
Wire.write(data);
Wire.endTransmission();
Wire.requestFrom(addr, bytes);

// SPI
SPI.begin();
SPI.setDataMode(SPI_MODE0);
SPI.setBitOrder(MSBFIRST);
SPI.transfer(data);

// WiFi
WiFi.begin("SSID", "password");
WiFi.softAP("AP_SSID");
WiFi.localIP();
WiFi.scanNetworks();

// OTA
ArduinoOTA.begin();
ArduinoOTA.handle();

// Audio (I2S/DMIC)
audioOut.begin(AUDIO_SAMPLE_RATE_16K, AUDIO_BITS_PER_SAMPLE_16, AUDIO_CHANNELS_MONO);
audioOut.write(data, len);
audioIn.begin(AUDIO_SAMPLE_RATE_16K, AUDIO_BITS_PER_SAMPLE_16, AUDIO_CHANNELS_MONO);
audioIn.read(data, len);

// 数学
min(a, b); max(a, b); constrain(val, lo, hi);
```

### Blink示例

```cpp
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
}
```

### ADC示例

```cpp
void setup() {
    serialBegin(115200);
}

void loop() {
    int value = analogRead(A0);  // 读取ADC通道0
    serialPrint("ADC: ");
    serialPrintInt(value);
    serialPrint("\n");
    delay(100);
}
```

### WiFi STA示例

```cpp
void setup() {
    serialBegin(115200);
    WiFi.begin("SSID", "password");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    serialPrint("IP: ");
    serialPrintln(WiFi.localIP().toString().c_str());
}

void loop() {
    // Your code here
}
```

### WiFi AP示例

```cpp
void setup() {
    serialBegin(115200);
    WiFi.softAP("XR871_AP", "12345678");
    serialPrint("AP IP: ");
    serialPrintln(WiFi.softAPIP().toString().c_str());
}

void loop() {
    // Your code here
}
```

### WiFi扫描示例

```cpp
void setup() {
    serialBegin(115200);
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++) {
        serialPrint(WiFi.SSID(i).c_str());
        serialPrint(" RSSI: ");
        serialPrintInt(WiFi.RSSI(i));
        serialPrint("\n");
    }
}

void loop() {
    delay(5000);
}
```

### OTA升级示例

```cpp
void setup() {
    WiFi.begin("SSID", "password");
    ArduinoOTA.begin();
    ArduinoOTA.onStart([]() {
        serialPrintln("OTA Start");
    });
    ArduinoOTA.onEnd([]() {
        serialPrintln("OTA End");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        serialPrint("Progress: ");
        serialPrintInt(progress * 100 / total);
        serialPrintln("%");
    });
}

void loop() {
    ArduinoOTA.handle();
}
```

### I2S音频输出示例

```cpp
void setup() {
    audioOut.begin(AUDIO_SAMPLE_RATE_44K, AUDIO_BITS_PER_SAMPLE_16, AUDIO_CHANNELS_STEREO);
}

void loop() {
    // 生成正弦波
    static int16_t sample = 0;
    static float phase = 0;
    phase += 0.05;
    if (phase > 6.28) phase = 0;
    sample = (int16_t)(sin(phase) * 32767);
    audioOut.writeSamples(&sample, 1);
}
```

### DMIC音频输入示例

```cpp
void setup() {
    audioIn.setInputMode(INPUT_MODE_DMIC);
    audioIn.begin(AUDIO_SAMPLE_RATE_16K, AUDIO_BITS_PER_SAMPLE_16, AUDIO_CHANNELS_MONO);
}

void loop() {
    int16_t sample;
    if (audioIn.readSamples(&sample, 1) > 0) {
        // 处理音频样本
    }
}
```

### I2C扫描示例

```cpp
void setup() {
    serialBegin(115200);
    Wire.begin(I2C0_ID, WIRE_CLOCK_100K);
    
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        uint8_t error = Wire.endTransmission();
        if (error == 0) {
            serialPrint("Device at 0x");
            serialPrintInt(addr, HEX);
        }
    }
}
```

### SPI测试示例

```cpp
void setup() {
    serialBegin(115200);
    SPI.begin(SPI0, SPI_TCTRL_SS_SEL_SS0);
    SPI.setDataMode(SPI_MODE0);
    SPI.setBitOrder(MSBFIRST);
    
    uint8_t txData = 0xA5;
    uint8_t rxData = SPI.transfer(txData);
}
```

### PWM测试示例

```cpp
void setup() {
    pinMode(8, OUTPUT);  // PA8 - PWM CH0
}

void loop() {
    for (int i = 0; i <= 255; i++) {
        analogWrite(8, i);  // Fade LED
        delay(10);
    }
}
```

## 后续计划

1. **Camera驱动移植**
   - 封装hal_csi.h -> Camera类
   - 支持OV2640/OV5640等摄像头

2. **PlatformIO集成**
   - 创建boards/xr871.json开发板定义
   - 创建platform描述文件
   - 支持Arduino IDE直接开发

3. **IR红外遥控**
   - 封装hal_irrx.h/hal_irtx.h
   - 支持NEC/RC5协议

4. **RTC实时时钟**
   - 封装hal_rtc.h
   - 支持定时唤醒

## 已知问题

1. **WiFi协议栈** - NET80211协议栈可能不完全开源，需要厂商提供库文件
2. **Flash烧录** - 需要专门的烧录工具（可能与Arduino IDE不兼容）
3. **社区资源** - 相比ESP32/ESP8266，社区资源较少

## 参考资源

- [XR871官方产品页](http://www.xradiotech.com/product/XR871.php)
- [XR871开发套件](http://www.xradiotech.com/devkit/Kit_XR871.php)
- [XR871 SDK (GitHub)](https://github.com/XradioTech/XR871-OLD)
- [全志在线开发者论坛](https://bbs.aw-ol.com)
- [充电头网XR871介绍](https://www.sohu.com/a/259893649_296845)
