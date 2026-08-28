#
# PlatformIO Builder Script for XR871 Arduino
# This script configures the build environment for XR871 Arduino projects.
#

from os.path import join, exists, basename
from SCons.Script import DefaultEnvironment, Builder

# Get environment
env = DefaultEnvironment()

# ============================================================
# Platform configuration
# ============================================================
platform = env.PioPlatform()
board = env.BoardConfig()
mcu = board.get("build.mcu", "xr871")
cpu = board.get("build.cpu", "cortex-m4f")

# ============================================================
# Toolchain configuration
# ============================================================
FRAMEWORK_DIR = platform.get_package_dir("framework-arduino-xr871")
TOOLCHAIN_DIR = platform.get_package_dir("toolchain-gccarmeabi")

# ============================================================
# SDK path
# ============================================================
XR871_SDK = env.GetProjectOption("custom_sdk_path", join(FRAMEWORK_DIR, "SDK"))

# ============================================================
# Build flags
# ============================================================
# CPU flags
cpu_flags = [
    "-mcpu=cortex-m4",
    "-mthumb",
    "-mfpu=fpv4-sp-d16",
    "-mfloat-abi=softfp"
]

# Common flags
common_flags = [
    "-Os",
    "-gdwarf-2",
    "-ffunction-sections",
    "-fdata-sections",
    "-Wall",
    "-Wextra",
    "-D__CONFIG_CHIP_XR871",
    "-D__CONFIG_CPU_CM4F",
    "-D__CONFIG_OS_FREERTOS",
    "-D__CONFIG_LIBC_PRINTF_FLOAT",
    "-D__CONFIG_LIBC_WRAP_STDIO",
    "-DARDUINO=10819",
    "-DARDUINO_XR871",
    "-DARDUINO_ARCH_XR871"
]

# Include paths
include_paths = [
    join(FRAMEWORK_DIR, "cores", "xr871"),
    join(FRAMEWORK_DIR, "variants", "xr871gt"),
    join(XR871_SDK, "include"),
    join(XR871_SDK, "include", "driver"),
    join(XR871_SDK, "include", "driver", "chip"),
    join(XR871_SDK, "include", "kernel"),
    join(XR871_SDK, "include", "kernel", "os"),
    join(XR871_SDK, "include", "libc"),
    join(XR871_SDK, "include", "net"),
    join(XR871_SDK, "include", "net", "wlan"),
    join(XR871_SDK, "include", "net", "lwip-1.4.1", "src", "include"),
    join(XR871_SDK, "include", "net", "lwip-1.4.1", "src", "include", "ipv4"),
    join(XR871_SDK, "include", "net", "lwip-1.4.1", "src", "include", "lwip"),
]

# C flags
env.Append(
    CFLAGS=cpu_flags + common_flags,
    CPPFLAGS=cpu_flags + common_flags,
    CXXFLAGS=cpu_flags + common_flags + ["-fno-rtti", "-fno-exceptions"],
    CPPPATH=include_paths,
    LIBPATH=[join(XR871_SDK, "lib")]
)

# ============================================================
# Linker flags
# ============================================================
linker_flags = cpu_flags + [
    "-Wl,--gc-sections",
    "-T" + join(XR871_SDK, "project", "linker_script", "gcc_xr871.ld")
]

env.Append(
    LINKFLAGS=linker_flags,
    LIBS=["c", "gcc", "m", "stdc++"]
)

# ============================================================
# Libraries
# ============================================================
libs = [
    env.BuildLibrary(
        join("$BUILD_DIR", "FrameworkArduino"),
        join(FRAMEWORK_DIR, "cores", "xr871"),
        "+" + " ".join(["-I" + p for p in include_paths])
    )
]

# Link SDK libraries
env.Append(
    LIBS=["chip", "os", "console", "util", "lwip", "wlan", "c"]
)

env.Prepend(LIBS=libs)

# ============================================================
# Upload configuration
# ============================================================
upload_protocol = env.subst("$UPLOAD_PROTOCOL") or "serial"
upload_speed = env.subst("$UPLOAD_SPEED") or "115200"

if upload_protocol == "serial":
    env.Replace(
        UPLOADER="phoenixMC",
        UPLOADERFLAGS=[
            "--port", "$UPLOAD_PORT",
            "--baud", str(upload_speed),
            "--bin", "$SOURCES",
            "--target", "xr871"
        ],
        UPLOADCMD="$UPLOADER $UPLOADERFLAGS"
    )

# ============================================================
# Custom targets
# ============================================================
env.AddPlatformTarget(
    "upload",
    "$BUILD_DIR/${PROGNAME}.bin",
    env.VerboseAction("$UPLOADCMD", "Uploading $SOURCE"),
    "Upload"
)

env.AddPlatformTarget(
    "size",
    "$BUILD_DIR/${PROGNAME}.elf",
    env.VerboseAction("$SIZE -A $SOURCE", "Calculating size"),
    "Size"
)
