#!/bin/bash
# XR871 Arduino IDE Installation Script for Linux
# Supports: Ubuntu, Debian, Fedora, CentOS, Arch Linux, openSUSE

set -e

echo "============================================"
echo "  XR871 Arduino IDE Installation (Linux)"
echo "============================================"
echo ""

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if running as root
if [ "$EUID" -eq 0 ]; then
    echo -e "${RED}Error: Do not run this script as root${NC}"
    exit 1
fi

# Detect architecture
ARCH=$(uname -m)
if [ "$ARCH" != "x86_64" ] && [ "$ARCH" != "aarch64" ]; then
    echo -e "${RED}Error: Unsupported architecture: $ARCH${NC}"
    echo "Only x86_64 and aarch64 are supported"
    exit 1
fi

echo -e "${GREEN}Detected architecture: $ARCH${NC}"

# Detect package manager
if command -v apt &> /dev/null; then
    PKG_MGR="apt"
    INSTALL_CMD="sudo apt install -y"
elif command -v dnf &> /dev/null; then
    PKG_MGR="dnf"
    INSTALL_CMD="sudo dnf install -y"
elif command -v yum &> /dev/null; then
    PKG_MGR="yum"
    INSTALL_CMD="sudo yum install -y"
elif command -v pacman &> /dev/null; then
    PKG_MGR="pacman"
    INSTALL_CMD="sudo pacman -S --noconfirm"
elif command -v zypper &> /dev/null; then
    PKG_MGR="zypper"
    INSTALL_CMD="sudo zypper install -y"
else
    echo -e "${YELLOW}Warning: Could not detect package manager${NC}"
    PKG_MGR="none"
    INSTALL_CMD=""
fi

echo "Package manager: $PKG_MGR"

# Check for Arduino IDE
ARDUINO_INSTALLED=false
ARDUINO_CLI_INSTALLED=false

if command -v arduino &> /dev/null; then
    ARDUINO_INSTALLED=true
    echo -e "${GREEN}Arduino IDE found${NC}"
fi

if command -v arduino-cli &> /dev/null; then
    ARDUINO_CLI_INSTALLED=true
    echo -e "${GREEN}arduino-cli found${NC}"
fi

if [ "$ARDUINO_INSTALLED" = false ] && [ "$ARDUINO_CLI_INSTALLED" = false ]; then
    echo -e "${YELLOW}Arduino IDE not found. Installing arduino-cli...${NC}"
    
    # Install arduino-cli
    ARDUINO_CLI_VERSION="1.5.1"
    ARDUINO_CLI_URL="https://github.com/arduino/arduino-cli/releases/download/v${ARDUINO_CLI_VERSION}/arduino-cli_${ARDUINO_CLI_VERSION}_Linux_${ARCH}.tar.gz"
    
    echo "Downloading arduino-cli..."
    curl -fsSL "$ARDUINO_CLI_URL" -o /tmp/arduino-cli.tar.gz
    sudo tar -xzf /tmp/arduino-cli.tar.gz -C /usr/local/bin/
    sudo chmod +x /usr/local/bin/arduino-cli
    rm /tmp/arduino-cli.tar.gz
    
    ARDUINO_CLI_INSTALLED=true
    echo -e "${GREEN}arduino-cli installed successfully${NC}"
fi

# Install ARM GCC toolchain
echo ""
echo "Installing ARM GCC toolchain..."

if command -v arm-none-eabi-gcc &> /dev/null; then
    echo -e "${GREEN}ARM GCC toolchain already installed${NC}"
    arm-none-eabi-gcc --version | head -1
else
    if [ "$PKG_MGR" = "apt" ]; then
        sudo apt update
        sudo apt install -y gcc-arm-none-eabi binutils-arm-none-eabi libnewlib-arm-none-eabi
    elif [ "$PKG_MGR" = "dnf" ]; then
        sudo dnf install -y arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-newlib
    elif [ "$PKG_MGR" = "yum" ]; then
        sudo yum install -y arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-newlib
    elif [ "$PKG_MGR" = "pacman" ]; then
        sudo pacman -S --noconfirm arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-newlib
    elif [ "$PKG_MGR" = "zypper" ]; then
        sudo zypper install -y cross-arm-none-eabi-gcc cross-arm-none-eabi-binutils cross-arm-none-eabi-newlib
    else
        echo -e "${YELLOW}Please install ARM GCC toolchain manually:${NC}"
        echo "  sudo apt install gcc-arm-none-eabi (Debian/Ubuntu)"
        echo "  sudo dnf install arm-none-eabi-gcc (Fedora)"
        echo "  sudo pacman -S arm-none-eabi-gcc (Arch)"
    fi
fi

# Install XR871 board package
echo ""
echo "Installing XR871 board package..."

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PACKAGE_DIR="$(dirname "$SCRIPT_DIR")"

# Method 1: Using arduino-cli
if [ "$ARDUINO_CLI_INSTALLED" = true ]; then
    echo "Using arduino-cli to install..."
    
    # Add board manager URL
    arduino-cli config set board_manager.additional_urls \
        "file://${PACKAGE_DIR}/package_xr871_index.json" 2>/dev/null || true
    
    # Update index
    arduino-cli update
    
    # Install board package
    arduino-cli core install xr871:xr871
    
    echo -e "${GREEN}XR871 board package installed via arduino-cli${NC}"
fi

# Method 2: Manual installation
echo ""
echo "Performing manual installation..."

ARDUINO15_DIR="$HOME/.arduino15"
HARDWARE_DIR="$ARDUINO15_DIR/packages/hardware/xr871"

mkdir -p "$HARDWARE_DIR"

# Copy hardware files
cp -r "$PACKAGE_DIR/hardware/xr871/1.0.0" "$HARDWARE_DIR/"

# Copy tools
mkdir -p "$ARDUINO15_DIR/packages/xr871/tools"
cp -r "$PACKAGE_DIR/tools/"* "$ARDUINO15_DIR/packages/xr871/tools/"

# Copy package index
cp "$PACKAGE_DIR/package_xr871_index.json" "$ARDUINO15_DIR/"

echo -e "${GREEN}Manual installation complete${NC}"

# Verify installation
echo ""
echo "Verifying installation..."

if [ -d "$HARDWARE_DIR/1.0.0" ]; then
    echo -e "${GREEN}✓ Hardware package installed${NC}"
else
    echo -e "${RED}✗ Hardware package not found${NC}"
fi

if command -v arm-none-eabi-gcc &> /dev/null; then
    echo -e "${GREEN}✓ ARM GCC toolchain available${NC}"
else
    echo -e "${RED}✗ ARM GCC toolchain not found${NC}"
fi

# Test compilation
echo ""
echo "Testing compilation..."

TEST_SKETCH="$HOME/Arduino/XR871_Test"
mkdir -p "$TEST_SKETCH"

cat > "$TEST_SKETCH/XR871_Test.ino" << 'EOF'
void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("XR871 Arduino Test!");
}

void loop() {
    Serial.println("Hello from XR871!");
    delay(1000);
}
EOF

if [ "$ARDUINO_CLI_INSTALLED" = true ]; then
    arduino-cli compile --fqbn xr871:xr871:xr871gt "$TEST_SKETCH" 2>&1 | tail -5
fi

echo ""
echo "============================================"
echo -e "${GREEN}Installation Complete!${NC}"
echo "============================================"
echo ""
echo "Next steps:"
echo "  1. Open Arduino IDE"
echo "  2. Go to Tools > Board > XR871 Arduino > XR871GT Development Board"
echo "  3. Open File > Examples > WiFi > WiFiStation"
echo "  4. Click Upload"
echo ""
echo "For more information:"
echo "  https://github.com/xradio-tech/xr871-arduino"
echo ""
