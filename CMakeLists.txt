cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
project(pico_examples C CXX ASM)

pico_sdk_init()

add_executable(pico_examples
        main.cpp
    )

pico_enable_stdio_usb(pico_examples 1)
pico_enable_stdio_uart(pico_examples 0)

target_link_libraries(pico_examples pico_stdlib hardware_i2c) #hardware_spi hardware_i2c
pico_add_extra_outputs(pico_examples)