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
set(BOARD_NAME       "STM32F103")
set(BOARD_VERSION    "1.0")
set(PROJECT_VER_HW   1)
set(MCU              "STM32F103C8T6")
set(CMAKE_C_STANDARD 17)

set(BOARD_DIR "${CMAKE_CURRENT_LIST_DIR}")

set(FREERTOS_PORT GCC_ARM_CM3 CACHE STRING "" FORCE)
set(FREERTOS_HEAP "3" CACHE STRING "" FORCE)

####################    COMPILER    ####################
include(${SDK_DIR}/toolchain/compiler.cmake)

compiler_setup(GCC arm-none-eabi)

####################    PLATFORM    ####################
project_setup_platform(STM32F1xx)

####################    OPTIONS     ####################
project_add_define(
    # Needed by ST HAL/LL
    "STM32F102xB"

    # For emulation
    "QEMU=0"

    # SDK Configuration
    "CONSOLE_UART_INDEX=1"
    "CONSOLE_FILE=\"/dev/console\""
    "BSP_LED_COUNT=1"
    "BSP_LED_MAIN=0"
    "BSP_BTN_COUNT=1"
    "BSP_BTN_MAIN=0"

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
project_add_src_files("${BOARD_DIR}/Core/Startup/startup_stm32f103c8tx.s")

project_add_ld_scripts("${BOARD_DIR}/STM32F103C8TX.ld")

message(STATUS "Using ${BOARD_NAME} board (${BOARD_DIR})")
