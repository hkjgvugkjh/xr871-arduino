# XR871 Arduino移植项目 (方案C)

[![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)](https://github.com/hkjgvugkjh/xr871-arduino/releases)

## 项目概述

本方案在XR871官方SDK基础上移植Arduino API，实现：
- **Arduino.h** - Arduino API头文件声明
- **Arduino.c** - Arduino API实现（HAL层封装）
- **Wire.h/Wire.cpp** - I2C驱动库（TwoWire类）
- **SPI.h/SPI.cpp** - SPI驱动库（SPIClass类）
- **PWM.h/PWM.cpp** - PWM驱动库（analogWrite支持）
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
├── Blink.ino              # Blink示例
├── I2C_Scan.ino           # I2C扫描示例
├── SPI_Test.ino           # SPI测试示例
├── PWM_Test.ino           # PWM测试示例
├── Makefile               # 编译配置
├── README.md              # 本文件
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
| **Time** | ✅ | delay/millis/micros 已实现 |
| **Interrupt** | ✅ | attachInterrupt/detachInterrupt 已实现 |
| **Math** | ✅ | min/max/constrain 等宏定义完成 |
| **Bit** | ✅ | bitSet/bitClear/bitRead 等宏定义完成 |

### 待实现模块

| 模块 | 状态 | 说明 |
|------|------|------|
| **ADC** | ❌ | 需要封装 hal_adc.h |
| **WiFi** | ❌ | 需要封装 lwIP 协议栈 |
| **OTA** | ❌ | 需要封装 hal_ota.h |
| **I2S** | ❌ | 需要封装 hal_i2s.h |
| **DMIC** | ❌ | 需要封装 hal_dmic.h |

## 硬件引脚映射

### GPIO引脚映射

| Arduino Pin | XR871 GPIO | 说明 |
|-------------|------------|------|
| 0-22 | GPIOA0-A22 | GPIOA 端口 |
| 23-38 | GPIOB0-B15 | GPIOB 端口 |
| LED_BUILTIN | PA6 | 板载LED |

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
analogRead(pin);          // TODO
analogWrite(pin, value);  // 已实现 (PWM)

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

1. **ADC驱动移植**
   - 封装hal_adc.h -> analogRead
   - 支持12位分辨率

2. **WiFi库移植**
   - 封装lwIP协议栈 -> WiFiClass
   - 实现WiFi.begin/available/send等标准接口
   - 支持STA/AP模式

3. **PlatformIO集成**
   - 创建boards/xr871.json开发板定义
   - 创建platform描述文件
   - 支持Arduino IDE直接开发

4. **OTA升级**
   - 实现固件空中升级功能
   - 利用XR871 Flash分区机制

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
