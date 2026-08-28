@echo off
REM XR871 Arduino IDE Installation Script for Windows
REM Supports: Windows 10/11 (64-bit)

echo ============================================
echo   XR871 Arduino IDE Installation (Windows)
echo ============================================
echo.

setlocal enabledelayedexpansion

REM Check architecture
if not "%PROCESSOR_ARCHITECTURE%"=="AMD64" (
    echo Error: Only 64-bit Windows is supported
    echo Current architecture: %PROCESSOR_ARCHITECTURE%
    exit /b 1
)

echo Detected: Windows 64-bit

REM Check for Arduino IDE
set "ARDUINO_FOUND=false"
set "ARDUINO_CLI_FOUND=false"

where arduino >nul 2>nul
if %errorlevel% equ 0 (
    set "ARDUINO_FOUND=true"
    echo Arduino IDE found
)

where arduino-cli >nul 2>nul
if %errorlevel% equ 0 (
    set "ARDUINO_CLI_FOUND=true"
    echo arduino-cli found
)

if "%ARDUINO_FOUND%"=="false" if "%ARDUINO_CLI_FOUND%"=="false" (
    echo Arduino IDE not found. Installing arduino-cli...
    
    set "ARDUINO_CLI_VERSION=1.5.1"
    set "ARDUINO_CLI_URL=https://github.com/arduino/arduino-cli/releases/download/v!ARDUINO_CLI_VERSION!/arduino-cli_!ARDUINO_CLI_VERSION!_Windows_amd64.zip"
    
    echo Downloading arduino-cli...
    powershell -Command "Invoke-WebRequest -Uri '!ARDUINO_CLI_URL!' -OutFile 'C:\Temp\arduino-cli.zip'"
    
    if not exist "C:\Temp" mkdir "C:\Temp"
    powershell -Command "Expand-Archive -Path 'C:\Temp\arduino-cli.zip' -DestinationPath 'C:\arduino-cli' -Force"
    
    set "PATH=%PATH%;C:\arduino-cli"
    set "ARDUINO_CLI_FOUND=true"
    echo arduino-cli installed
)

REM Install ARM GCC toolchain
echo.
echo Installing ARM GCC toolchain...

where arm-none-eabi-gcc >nul 2>nul
if %errorlevel% equ 0 (
    echo ARM GCC toolchain already installed
    arm-none-eabi-gcc --version
) else (
    echo Downloading ARM GCC toolchain...
    
    set "ARM_GCC_URL=https://developer.arm.com/-/media/Files/downloads/gnu/13.3.rel1/binrel/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar.xz"
    
    powershell -Command "Invoke-WebRequest -Uri '!ARM_GCC_URL!' -OutFile 'C:\Temp\arm-gcc.tar.xz'"
    
    echo Extracting ARM GCC toolchain...
    if not exist "C:\Program Files\ARM GCC" mkdir "C:\Program Files\ARM GCC"
    
    REM Use 7-Zip or tar if available
    where tar >nul 2>nul
    if %errorlevel% equ 0 (
        tar -xf "C:\Temp\arm-gcc.tar.xz" -C "C:\Program Files\ARM GCC"
    ) else (
        echo Please extract C:\Temp\arm-gcc.tar.xz to "C:\Program Files\ARM GCC" manually
    )
    
    set "PATH=%PATH%;C:\Program Files\ARM GCC\arm-none-eabi\bin"
    echo ARM GCC toolchain installed
)

REM Install XR871 board package
echo.
echo Installing XR871 board package...

set "SCRIPT_DIR=%~dp0"
set "PACKAGE_DIR=%SCRIPT_DIR%.."

set "ARDUINO15_DIR=%USERPROFILE%\.arduino15"
set "HARDWARE_DIR=%ARDUINO15_DIR%\packages\hardware\xr871"

if not exist "%HARDWARE_DIR%" mkdir "%HARDWARE_DIR%"

REM Copy hardware files
xcopy /E /I /Y "%PACKAGE_DIR%\hardware\xr871\1.0.0" "%HARDWARE_DIR%\1.0.0"

REM Copy tools
if not exist "%ARDUINO15_DIR%\packages\xr871\tools" mkdir "%ARDUINO15_DIR%\packages\xr871\tools"
xcopy /E /I /Y "%PACKAGE_DIR%\tools\*" "%ARDUINO15_DIR%\packages\xr871\tools\"

REM Copy package index
copy /Y "%PACKAGE_DIR%\package_xr871_index.json" "%ARDUINO15_DIR%\"

echo XR871 board package installed

REM Verify installation
echo.
echo Verifying installation...

if exist "%HARDWARE_DIR%\1.0.0" (
    echo [OK] Hardware package installed
) else (
    echo [FAIL] Hardware package not found
)

where arm-none-eabi-gcc >nul 2>nul
if %errorlevel% equ 0 (
    echo [OK] ARM GCC toolchain available
) else (
    echo [FAIL] ARM GCC toolchain not found
)

REM Test compilation
echo.
echo Testing compilation...

set "TEST_SKETCH=%USERPROFILE%\Arduino\XR871_Test"
if not exist "%TEST_SKETCH%" mkdir "%TEST_SKETCH%"

echo void setup() {> "%TEST_SKETCH%\XR871_Test.ino"
echo     Serial.begin(115200);>> "%TEST_SKETCH%\XR871_Test.ino"
echo     delay(1000);>> "%TEST_SKETCH%\XR871_Test.ino"
echo     Serial.println("XR871 Arduino Test!");>> "%TEST_SKETCH%\XR871_Test.ino"
echo }>> "%TEST_SKETCH%\XR871_Test.ino"
echo.>> "%TEST_SKETCH%\XR871_Test.ino"
echo void loop() {>> "%TEST_SKETCH%\XR871_Test.ino"
echo     Serial.println("Hello from XR871!");>> "%TEST_SKETCH%\XR871_Test.ino"
echo     delay(1000);>> "%TEST_SKETCH%\XR871_Test.ino"
echo }>> "%TEST_SKETCH%\XR871_Test.ino"

if "%ARDUINO_CLI_FOUND%"=="true" (
    arduino-cli compile --fqbn xr871:xr871:xr871gt "%TEST_SKETCH%"
)

echo.
echo ============================================
echo Installation Complete!
echo ============================================
echo.
echo Next steps:
echo   1. Open Arduino IDE
echo   2. Go to Tools ^> Board ^> XR871 Arduino ^> XR871GT Development Board
echo   3. Open File ^> Examples ^> WiFi ^> WiFiStation
echo   4. Click Upload
echo.
echo For more information:
echo   https://github.com/xradio-tech/xr871-arduino
echo.

pause
