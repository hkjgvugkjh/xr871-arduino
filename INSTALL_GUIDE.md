# XR871 Arduino Core v1.0.0 安装指南

## 目录

1. [系统要求](#系统要求)
2. [安装ARM工具链](#安装arm工具链)
3. [获取XR871 SDK](#获取xr871-sdk)
4. [安装Arduino Board Manager包](#安装arduino-board-manager包)
5. [编译测试](#编译测试)
6. [烧录固件](#烧录固件)
7. [常见问题](#常见问题)

---

## 系统要求

- **操作系统**: Ubuntu 20.04+ / macOS 10.15+ / Windows 10+
- **ARM GCC工具链**: arm-none-eabi-gcc 10+ (推荐 13.2.rel1)
- **XR871 SDK**: [XradioTech/XR871SDK](https://github.com/XradioTech/XR871SDK)
- **Arduino IDE**: 1.8.x 或 2.x
- **磁盘空间**: 至少 500MB

---

## 安装ARM工具链

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install -y gcc-arm-none-eabi binutils-arm-none-eabi libnewlib-arm-none-eabi
```

### macOS

```bash
brew install arm-none-eabi-gcc
```

### Windows

1. 下载 [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
2. 安装到默认目录，确保添加到系统 PATH

### 验证安装

```bash
arm-none-eabi-gcc --version
# 应显示类似: arm-none-eabi-gcc (15:13.2.rel1-2) 13.2.1
```

---

## 获取XR871 SDK

```bash
git clone https://github.com/XradioTech/XR871SDK.git
```

SDK包含：
- `include/` - 硬件驱动头文件
- `src/` - 驱动源码
- `lib/` - 预编译库 (libwlan.a, liblwip.a等)
- `project/` - 示例项目
- `project/linker_script/` - 链接脚本

---

## 安装Arduino Board Manager包

### 方法1: Arduino IDE (推荐)

1. 打开 Arduino IDE
2. 文件 → 偏好设置
3. 在 "附加开发板管理器网址" 中添加:
   ```
   https://github.com/hkjgvugkjh/xr871-arduino/releases/download/v1.0.0/package_xr871_index.json
   ```
4. 工具 → 开发板 → 开发板管理器
5. 搜索 "XR871" 并安装

### 方法2: 手动安装

```bash
# 下载发布包
wget https://github.com/hkjgvugkjh/xr871-arduino/releases/download/v1.0.0/xr871-arduino-1.0.0.zip

# 解压到Arduino硬件目录
cd ~/Arduino/hardware/
mkdir -p xr871
cd xr871
unzip ~/Downloads/xr871-arduino-1.0.0.zip
```

### 方法3: 从源码构建

```bash
git clone https://github.com/hkjgvugkjh/xr871-arduino.git
cd xr871-arduino
git clone https://github.com/XradioTech/XR871SDK.git XR871SDK
make lib
```

---

## 编译测试

### 编译Arduino核心库

```bash
cd xr871-arduino
make lib
```

成功后会生成 `build/libxr871_arduino_core.a`

### 编译示例 (Blink)

```bash
cd xr871-arduino
make all
```

### 编译WiFi Station示例

确保已设置环境变量指向SDK:

```bash
export XR871_SDK=./XR871SDK
make all
```

---

## 烧录固件

### 使用phoenixMC (XR871专用烧录工具)

```bash
# 1. 连接XR871开发板 (进入烧录模式: 按住BOOT上电)
# 2. 查找串口设备
ls /dev/ttyUSB*  # Linux
ls /dev/cu.*     # macOS

# 3. 烧录
./phoenixMC --port /dev/ttyUSB0 --baud 115200 --bin firmware.bin --target xr871
```

### 使用OpenOCD (JTAG/SWD)

```bash
openocd -f interface/jlink.cfg -f target/xr871.cfg -c "program firmware.bin verify reset exit 0x08000000"
```

---

## 常见问题

### 1. 编译报错: `arm-none-eabi-gcc: command not found`

**解决**: 安装ARM工具链并确保在PATH中

```bash
which arm-none-eabi-gcc
export PATH=$PATH:/path/to/gcc-arm-none-eabi/bin
```

### 2. 编译报错: `FreeRTOS.h: No such file or directory`

**解决**: 确保XR871 SDK已克隆且路径正确

```bash
# 在Makefile中修改SDK路径
XR871SDK ?= ./XR871SDK
```

### 3. 链接报错: `cannot find -lchip`

**解决**: 确保SDK的lib目录包含libchip.a

```bash
ls XR871SDK/lib/
```

### 4. 烧录失败: 无法进入烧录模式

**解决**:
- 按住BOOT键不放，然后按RESET键
- 松开RESET，再松开BOOT
- 系统进入下载模式，重新运行烧录命令

### 5. WiFi示例无法连接AP

**解决**:
- 检查SSID和密码是否正确
- 确认开发板天线连接正常
- 查看串口输出的错误信息

---

## 项目结构

```
xr871-arduino/
├── Arduino.h/.c          # Arduino核心API
├── String.h/.cpp         # Arduino String类
├── Wire.h/.cpp           # I2C接口
├── SPI.h/.cpp            # SPI接口
├── PWM.h/.cpp            # PWM输出
├── WiFi.h/.cpp           # WiFi功能
├── WiFiClient.h/.cpp     # TCP客户端
├── WiFiUDP.h/.cpp        # UDP通信
├── HTTPClient.h/.cpp     # HTTP客户端
├── WebServer.h/.cpp      # Web服务器
├── DNSServer.h/.cpp      # DNS服务器
├── MDNS.h/.cpp           # mDNS服务发现
├── OTA.h/.cpp            # 在线升级
├── Camera.h/.cpp         # 摄像头接口
├── Audio.h/.cpp          # 音频输入输出
├── Makefile              # 构建配置
├── boards/               # 开发板定义
├── library/              # 库属性文件
├── SDK/                  # SDK参考头文件
├── XR871SDK/             # 官方SDK (需单独克隆)
└── examples/             # 示例程序
```

---

## 引脚映射

| Arduino | XR871 GPIO | 功能 |
|---------|-----------|------|
| 0-22 | PA0-PA22 | GPIO, UART, I2C, SPI, PWM, ADC |
| 23-38 | PB0-PB15 | GPIO, SPI, PWM |
| LED_BUILTIN | PA6 | 板载LED |

### I2C引脚
- Wire (I2C0): SCL=PA2, SDA=PA3
- Wire1 (I2C1): SCL=PA4, SDA=PA5

### SPI引脚
- SPI0: MOSI=PB4, MISO=PB5, CLK=PB7, CS=PB6
- SPI1: MOSI=PA0, MISO=PA1, CLK=PA2, CS=PA3

### PWM引脚
- Pin 8-15: PWM通道0-7

---

## API兼容性

本移植提供ESP32 Arduino API兼容，现有ESP32代码通常只需少量修改即可运行。

### 已支持API
- [x] GPIO: pinMode, digitalWrite, digitalRead
- [x] 时间: delay, millis, micros
- [x] I2C: Wire.begin, Wire.write, Wire.read
- [x] SPI: SPI.begin, SPI.transfer
- [x] PWM: analogWrite
- [x] WiFi: WiFi.begin, WiFi.localIP, WiFi.scanNetworks
- [x] TCP: WiFiClient.connect, WiFiClient.write
- [x] UDP: WiFiUDP.beginPacket, WiFiUDP.write
- [x] HTTP: HTTPClient.begin, HTTPClient.GET
- [x] WebServer: WebServer.on, WebServer.send
- [x] OTA: ArduinoOTA.begin, ArduinoOTA.handle
- [x] Camera: Camera.begin, Camera.capture
- [x] Audio: AudioOutput.begin, AudioOutput.write

---

## 获取帮助

- GitHub Issues: https://github.com/hkjgvugkjh/xr871-arduino/issues
- XR871 SDK文档: https://github.com/XradioTech/XR871SDK
- 全志开发者论坛: https://bbs.aw-ol.com
