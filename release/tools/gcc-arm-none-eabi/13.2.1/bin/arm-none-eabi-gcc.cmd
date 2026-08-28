@echo off
REM XR871 Arduino Compiler Wrapper - Windows
REM This script wraps the ARM GCC compiler for XR871

setlocal enabledelayedexpansion

REM Get the basename of this script
set "BASENAME=%~n0"

REM Check for system tool
where %BASENAME% >nul 2>nul
if %errorlevel% equ 0 (
    %BASENAME% %*
    exit /b %errorlevel%
)

REM Check common install paths
if exist "C:\Program Files (x86)\GNU Tools ARM Embedded\bin\%BASENAME%.exe" (
    "C:\Program Files (x86)\GNU Tools ARM Embedded\bin\%BASENAME%.exe" %*
    exit /b %errorlevel%
)

if exist "C:\Program Files\GNU Tools ARM Embedded\bin\%BASENAME%.exe" (
    "C:\Program Files\GNU Tools ARM Embedded\bin\%BASENAME%.exe" %*
    exit /b %errorlevel%
)

echo ERROR: %BASENAME% not found!
echo Please install the ARM GCC toolchain:
echo   - Via Arduino IDE (Tools ^> Board ^> Boards Manager ^> XR871)
echo   - Or download from: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm
exit /b 1
