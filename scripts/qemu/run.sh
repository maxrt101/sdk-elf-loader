#!/usr/bin/env bash

qemu-system-arm -machine stm32vldiscovery -kernel cmake-build-stm32f100-debug/app/ELFLoaderApp.bin  -monitor stdio
