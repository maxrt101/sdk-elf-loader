# =========================================================================
#
# @file CMakeLists.txt
# @date 13-03-2025
# @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
#
# @brief ElfLoader project
#
# =========================================================================

cmake_minimum_required(VERSION 3.27)

# Required variables
set(PROJECT_DIR          "${CMAKE_CURRENT_LIST_DIR}/../")
set(SDK_DIR              "${CMAKE_CURRENT_LIST_DIR}/../sdk")
set(PROJECT_NAME         "ELFLoaderApp")
set(PROJECT_VER_SW_MAJOR 0)
set(PROJECT_VER_SW_MINOR 1)
set(PROJECT_VER_SW_PATCH 0)

# Additional configuration
set(PROJECT_VERBOSE 0)
set(GENERATE_HEX    1)
set(GENERATE_BIN    1)
set(GENERATE_MAP    1)
set(SHOW_SIZE       1)

# Include utils
include(${SDK_DIR}/toolchain/project.cmake)
include(${SDK_DIR}/toolchain/utils.cmake)

# Setup project
project_init()

# Setup CMake project
project(${PROJECT_NAME} C ASM)


# Add sdk
project_add_inc_recursive("${SDK_DIR}")
project_add_inc_dirs(
        "${SDK_DIR}"
        "${SDK_DIR}/hal"
        "${SDK_DIR}/lib"
        "${SDK_DIR}/drv"
)
project_add_src_recursive(
        "${SDK_DIR}/hal"
        "${SDK_DIR}/lib"
)

# Add defines
project_add_define(
    # Project Info
    "PROJECT_NAME=\"${PROJECT_NAME}\""
    "PROJECT_VERSION=\"${PROJECT_VER_SW_MAJOR}.${PROJECT_VER_SW_MINOR}.${PROJECT_VER_SW_PATCH}\""
    "PROJECT_VERSION_SW_MAJOR=${PROJECT_VER_SW_MAJOR}"
    "PROJECT_VERSION_SW_MINOR=${PROJECT_VER_SW_MINOR}"
    "PROJECT_VERSION_SW_PATCH=${PROJECT_VER_SW_PATCH}"

    # SDK Configuration
    "USE_COLOR_LOG=1"
    "USE_LOG_RESET_CURSOR=1"
    "USE_PRINT_STARTUP_INFO=1"
    "USE_SHELL_ENV=1"
    "USE_OS_HEAP_DEFRAG_ON_FREE=1"
    "USE_OS_HEAP_DEFRAG_ON_NO_MEM=1"
    "TTY_ASCII_KEY_BACKSPACE=0x7F"

    # FS
    "VFS_IOCTL_CMD_PORT=VFS_IOCTL_RESET_DEVICE, VFS_IOCTL_RESET_DEVICE_DEFERRED, VFS_IOCTL_WRITE_DETECTED, VFS_IOCTL_WRITE_DETECTED_CLEAR, VFS_IOCTL_READ_TIMEOUT_ENABLE"

    # Debug Log Configuration (Modules)
    "LOG_ENABLE_MAIN=1"
    "LOG_ENABLE_SH=1"
    "LOG_ENABLE_STORAGE=1"
    "LOG_ENABLE_HAL_NVM=1"
    "LOG_ENABLE_HAL_1W=0"
    "LOG_ENABLE_HAL_SPI=1"
    "LOG_ENABLE_HAL_UART=1"
    "LOG_ENABLE_HAL_APP_PORT=1"
    "LOG_ENABLE_LED=1"
    "LOG_ENABLE_PWM=1"
    "LOG_ENABLE_HEAP=1"
    "LOG_ENABLE_SHELL=1"
    "LOG_ENABLE_IRQ=1"
    "LOG_ENABLE_RST=1"
    "LOG_ENABLE_TASK=1"
    "LOG_ENABLE_DS28=0"
    "LOG_ENABLE_RA02=0"

    # Debug Log Configuration (Features)
    "SHELL_DEBUG_PRINT_TOKENS=0"
    "SHELL_DEBUG_DEBUG_TOKENS_HEX=0"
)

# Compile Options
project_add_compile_options(ALL
        -ffunction-sections
        -fdata-sections
        -fshort-enums
        -Os
)

# Link Options
project_add_link_options(ALL
        -ffunction-sections -fdata-sections
        -Wl,--gc-sections,--sort-common
        -Wl,--start-group -lc -lm -Wl,--end-group
)

# Add project headers and sources
project_add_inc_dirs("${PROJECT_DIR}/loader" "${CMAKE_BINARY_DIR}/mod")
project_add_inc_recursive("${PROJECT_DIR}/loader")
project_add_src_recursive("${PROJECT_DIR}/loader")
project_add_ld_paths("${SDK_DIR}/lib/loader/")
project_add_ld_paths("${SDK_DIR}/lib/shell")

project_finish()
