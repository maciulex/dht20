cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
project(dht20 C CXX ASM)

pico_sdk_init()

add_executable(dht20
        main.cpp
    )

pico_enable_stdio_usb(dht20 1)
pico_enable_stdio_uart(dht20 0)

target_link_libraries(dht20 pico_stdlib hardware_i2c) #hardware_spi hardware_i2c
pico_add_extra_outputs(dht20)