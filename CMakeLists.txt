cmake_minimum_required(VERSION 3.27)

add_custom_target(ELFLoader)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/loader app)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/module mod)

add_dependencies(ELFLoaderApp ELFLoaderMod)

project(ELFLoader)

add_dependencies(ELFLoader ELFLoaderApp ELFLoaderMod)
