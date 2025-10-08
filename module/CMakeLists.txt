# =========================================================================
#
# @file CMakeLists.txt
# @date 13-03-2025
# @author Maksym Tkachuk <max.r.tkachuk@gmail.com>
#
# @brief ElfLoader Module project
#
# =========================================================================

cmake_minimum_required(VERSION 3.27)

# Required variables
set(PROJECT_DIR          "${CMAKE_CURRENT_LIST_DIR}/../")
set(SDK_DIR              "${CMAKE_CURRENT_LIST_DIR}/../sdk")
set(PROJECT_NAME         "ELFLoaderMod")

# Additional configuration
set(PROJECT_SKIP_BOARD 1)
set(PROJECT_VERBOSE    0)
set(GENERATE_HEX       0)
set(GENERATE_BIN       1)
set(GENERATE_MAP       1)
set(SHOW_SIZE          1)

# Include utils
include(${SDK_DIR}/toolchain/compiler.cmake)
include(${SDK_DIR}/toolchain/project.cmake)
include(${SDK_DIR}/toolchain/utils.cmake)

# Setup project
project_init()

# Setup compiler
compiler_setup(GCC arm-none-eabi)

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

# Include board options directly, because we skipped board inclusion
include(${PROJECT_DIR}/boards/${BOARD}/options.cmake)

# Compile Options
project_add_compile_options(ALL
    -lnosys -nostdlib -nostartfiles -ffreestanding -fpic
)

# Link Options
project_add_link_options(ALL
     -shared
     -lnosys -nostdlib -nostartfiles -ffreestanding -fpic
     -Wl,--allow-shlib-undefined
)

project_add_ld_scripts(${PROJECT_DIR}/module/pic.ld)

# Add project headers and sources
project_add_inc_recursive("${PROJECT_DIR}")
project_add_src_files("${PROJECT_DIR}/module/pic.c")
project_add_ld_paths("${PROJECT_DIR}/module")

project_finish()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_STRIP} --strip-unneeded ${CMAKE_BINARY_DIR}/mod/${PROJECT_NAME}.elf -o ${CMAKE_BINARY_DIR}/mod/${PROJECT_NAME}_stripped.elf
    COMMAND xxd -n __module_data -i ${CMAKE_BINARY_DIR}/mod/${PROJECT_NAME}_stripped.elf \"module.h\")
