# ELF Loader

Example of PIE ELF loader, complete with relocation patching and shared APIs  

![demo](https://github.com/maxrt101/sdk-elf-loader/blob/master/files/demo.png?raw=true)

### How to run
Prerequisites:  
 - `arm-none-eabi-gcc`  
 - `cmake`  
 - `make`  
 - `qemu-system-arm`  

Steps:
 - `cmake --preset "STM32F100 Debug"`  
 - `cd cmake-build-stm32f100-debug`  
 - `make`  
 - `./run.sh`  
