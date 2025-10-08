# =========================================================================
#
# @file board.cmake
# @date 13-03-2025
# @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
#
# @brief Board definition for generic STM32F100
#
# =========================================================================

include_guard(GLOBAL)

####################    VARIABLES    ####################
set(BOARD_NAME       "STM32F100")
set(BOARD_VERSION    "1.0")
set(PROJECT_VER_HW   1)
set(MCU              "STM32F100RBT6")
set(CMAKE_C_STANDARD 17)

set(BOARD_DIR "${CMAKE_CURRENT_LIST_DIR}")

####################    COMPILER    ####################
include(${SDK_DIR}/toolchain/compiler.cmake)

compiler_setup(GCC arm-none-eabi)

####################    PLATFORM    ####################
project_setup_platform(STM32F1xx)

####################    OPTIONS     ####################
project_add_define(
    # Needed by ST HAL/LL
    "STM32F100xB"

    # For emulation
    "QEMU=1"

    # SDK Configuration
    "CONSOLE_UART_INDEX=1"
    "CONSOLE_FILE=\"/dev/console\""
    "HAS_TRX_RA02_SUPPORT=0"
    "USE_DS28EA00_CHAIN_DETECT_BIT_FLIP_FIX=1"
    "USE_SPI_FIRST_READ_OUT_OF_SYNC_FIX=1"
    "BSP_LED_COUNT=1"
    "BSP_LED_MAIN=0"
    "BSP_BTN_COUNT=1"
    "BSP_BTN_MAIN=0"
    "TRX_ASYNC_TIMEOUT_MS=500"
    "USE_PRINT_INCOMING_PACKETS=0"
    "USE_PRINT_OUTGOING_PACKETS=0"
    "USE_PRINT_FRAME=0"
    "USE_PRINT_CRC=0"

    # Board Info
    "BOARD_NAME=\"${BOARD_NAME}\""
    "BOARD_VERSION=\"${BOARD_VERSION}\""
    "PROJECT_VERSION_HW=${PROJECT_VER_HW}"
    "MCU=\"${MCU}\""
)

include(${BOARD_DIR}/options.cmake)

####################   SOURCES    ####################
project_add_inc_dirs(
        "${BOARD_DIR}/bsp"
        "${BOARD_DIR}/bsp/hal"
        "${BOARD_DIR}/bsp/port"
        "${BOARD_DIR}/Core/Inc"
)

project_add_inc_recursive("${BOARD_DIR}")
project_add_src_recursive("${BOARD_DIR}")
project_add_src_recursive("${BOARD_DIR}/Core")
project_add_src_files("${BOARD_DIR}/Core/Startup/startup_stm32f100rbtx.s")

project_add_ld_scripts("${BOARD_DIR}/STM32F100RBTX.ld")

message(STATUS "Using ${BOARD_NAME} board (${BOARD_DIR})")
