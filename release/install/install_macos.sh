#!/bin/bash
# XR871 Arduino IDE Installation Script for macOS
# Supports: macOS 11+ (Intel and Apple Silicon)

set -e

echo "============================================"
echo "  XR871 Arduino IDE Installation (macOS)"
echo "============================================"
echo ""

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Detect architecture
ARCH=$(uname -m)
echo -e "${BLUE}Detected architecture: $ARCH${NC}"

if [ "$ARCH" != "x86_64" ] && [ "$ARCH" != "arm64" ]; then
    echo -e "${RED}Error: Unsupported architecture: $ARCH${NC}"
    exit 1
fi

# Check for Homebrew
if ! command -v brew &> /dev/null; then
    echo -e "${YELLOW}Homebrew not found. Installing...${NC}"
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    
    # Add Homebrew to PATH for Apple Silicon
    if [ "$ARCH" = "arm64" ]; then
        eval "$(/opt/homebrew/bin/brew shellenv)"
    fi
fi

echo -e "${GREEN}Homebrew available${NC}"

# Check for Arduino IDE
ARDUINO_INSTALLED=false
ARDUINO_CLI_INSTALLED=false

if [ -d "/Applications/Arduino.app" ]; then
    ARDUINO_INSTALLED=true
    echo -e "${GREEN}Arduino IDE found${NC}"
fi

if command -v arduino-cli &> /dev/null; then
    ARDUINO_CLI_INSTALLED=true
    echo -e "${GREEN}arduino-cli found${NC}"
fi

# Install arduino-cli if needed
if [ "$ARDUINO_CLI_INSTALLED" = false ]; then
    echo -e "${YELLOW}Installing arduino-cli...${NC}"
    brew install arduino-cli
    ARDUINO_CLI_INSTALLED=true
    echo -e "${GREEN}arduino-cli installed${NC}"
fi

# Install ARM GCC toolchain
echo ""
echo "Installing ARM GCC toolchain..."

if command -v arm-none-eabi-gcc &> /dev/null; then
    echo -e "${GREEN}ARM GCC toolchain already installed${NC}"
    arm-none-eabi-gcc --version | head -1
else
    echo "Installing via Homebrew..."
    brew install arm-none-eabi-gcc
    
    # Alternative: install prebuilt toolchain
    if ! command -v arm-none-eabi-gcc &> /dev/null; then
        echo -e "${YELLOW}Installing prebuilt toolchain...${NC}"
        
        ARM_GCC_VERSION="13.3.rel1"
        if [ "$ARCH" = "arm64" ]; then
            ARM_GCC_URL="https://developer.arm.com/-/media/Files/downloads/gnu/${ARM_GCC_VERSION}/binrel/arm-gnu-toolchain-${ARM_GCC_VERSION}-aarch64-arm-none-eabi.tar.xz"
        else
            ARM_GCC_URL="https://developer.arm.com/-/media/Files/downloads/gnu/${ARM_GCC_VERSION}/binrel/arm-gnu-toolchain-${ARM_GCC_VERSION}-x86_64-arm-none-eabi.tar.xz"
        fi
        
        curl -fsSL "$ARM_GCC_URL" -o /tmp/arm-gcc.tar.xz
        sudo mkdir -p /opt/arm-gnu-toolchain
        sudo tar -xf /tmp/arm-gcc.tar.xz -C /opt/arm-gnu-toolchain
        
        # Add to PATH
        if [ "$ARCH" = "arm64" ]; then
            ARM_GCC_DIR="/opt/arm-gnu-toolchain/arm-gnu-toolchain-${ARM_GCC_VERSION}-aarch64-arm-none-eabi/bin"
        else
            ARM_GCC_DIR="/opt/arm-gnu-toolchain/arm-gnu-toolchain-${ARM_GCC_VERSION}-x86_64-arm-none-eabi/bin"
        fi
        
        echo "export PATH=\"$ARM_GCC_DIR:\$PATH\"" >> ~/.zshrc
        export PATH="$ARM_GCC_DIR:$PATH"
        
        rm /tmp/arm-gcc.tar.xz
        echo -e "${GREEN}ARM GCC toolchain installed${NC}"
    fi
fi

# Install XR871 board package
echo ""
echo "Installing XR871 board package..."

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PACKAGE_DIR="$(dirname "$SCRIPT_DIR")"

ARDUINO15_DIR="$HOME/.arduino15"
HARDWARE_DIR="$ARDUINO15_DIR/packages/hardware/xr871"

mkdir -p "$HARDWARE_DIR"

# Copy hardware files
cp -R "$PACKAGE_DIR/hardware/xr871/1.0.0" "$HARDWARE_DIR/"

# Copy tools
mkdir -p "$ARDUINO15_DIR/packages/xr871/tools"
cp -R "$PACKAGE_DIR/tools/"* "$ARDUINO15_DIR/packages/xr871/tools/"

# Copy package index
cp "$PACKAGE_DIR/package_xr871_index.json" "$ARDUINO15_DIR/"

echo -e "${GREEN}Hardware files installed${NC}"

# Using arduino-cli
if [ "$ARDUINO_CLI_INSTALLED" = true ]; then
    echo ""
    echo "Configuring arduino-cli..."
    
    # Add board manager URL
    arduino-cli config set board_manager.additional_urls \
        "file://${PACKAGE_DIR}/package_xr871_index.json" 2>/dev/null || true
    
    # Update index
    arduino-cli update 2>/dev/null || true
    
    # Install board package
    arduino-cli core install xr871:xr871 2>/dev/null || true
    
    echo -e "${GREEN}arduino-cli configured${NC}"
fi

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
