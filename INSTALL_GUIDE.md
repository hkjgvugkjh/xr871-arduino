# XR871 Arduino 安装指南

## 快速安装 (Arduino IDE)

### 1. 添加开发板URL

打开 Arduino IDE → 文件 → 首选项 → 附加开发板管理器网址：

```
https://raw.githubusercontent.com/hkjgvugkjh/xr871-arduino/main/package_xr871_index.json
```

### 2. 安装XR871开发板

工具 → 开发板 → 开发板管理器 → 搜索 "XR871" → 点击安装

安装过程会自动下载：
- ARM GCC 13.2.1 编译器 (62MB)
- xr871_tool 烧录工具 (1.1MB, 含phoenixMC+mkimage)

### 3. 选择开发板和端口

- 开发板: XR871 EVB
- 端口: 选择对应的串口 (如 /dev/ttyUSB0)

### 4. 编写并上传代码

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

点击 "上传" 按钮，自动完成编译和烧录。

## 系统要求

- **操作系统**: Linux x86_64 (Ubuntu 18.04+)
- **Arduino IDE**: 1.8.x 或 2.0+
- **Python**: 3.x (用于烧录工具)
- **磁盘空间**: 至少 500MB (工具链+核心)

## 功能支持

| 功能 | 状态 | 说明 |
|------|------|------|
| GPIO | ✅ | pinMode/digitalWrite/digitalRead |
| ADC | ✅ | 9通道12位ADC |
| PWM | ✅ | 8通道PWM |
| I2C | ✅ | 2个I2C端口 |
| SPI | ✅ | 2个SPI端口 |
| UART | ✅ | 串口通信 |
| WiFi | ✅ | STA/AP模式，lwIP协议栈 |
| OTA | ✅ | HTTP空中升级 |
| I2S | ✅ | I2S音频输入输出 |
| DMIC | ✅ | 数字麦克风 |

## 硬件连接

### 串口烧录

```
XR871 EVB          USB转串口
--------          ---------
TX  <-------------> RX
RX  <-------------> TX
GND <-------------> GND
```

### WiFi天线

确保天线正确连接，否则WiFi性能会受影响。

## 常见问题

### Q: 安装时下载工具链失败？

A: 检查网络连接，或手动下载工具链包后放到 `~/.arduino15/packages/xr871/tools/` 目录下。

### Q: 上传失败？

A: 检查：
1. 串口是否正确选择
2. 开发板是否进入烧录模式
3. 串口是否被其他程序占用

### Q: WiFi连接失败？

A: 检查：
1. SSID和密码是否正确
2. 信号强度是否足够
3. 是否支持2.4GHz频段 (XR871不支持5GHz)

## 高级使用

### 手动编译

```bash
cd xr871-arduino
make XR871SDK=/path/to/xr871sdk
```

### 手动烧录

```bash
# 使用phoenixMC烧录
./tools/xr871_tool/phoenixMC -c /dev/ttyUSB0 -b 921600 -i firmware.img

# 使用mkimage打包镜像
./tools/xr871_tool/mkimage -c image.cfg -o firmware.img
```

## 相关链接

- [GitHub仓库](https://github.com/hkjgvugkjh/xr871-arduino)
- [SDK文档](https://github.com/XradioTech/XR871-OLD)
- [问题反馈](https://github.com/hkjgvugkjh/xr871-arduino/issues)
