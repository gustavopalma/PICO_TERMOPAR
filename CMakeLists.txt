cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(spi_temp C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#Caso seja a Raspberry Pi Pico W
set(PICO_BOARD pico_w)
#Caso seja a Raspberry Pi Pico 
#set(PICO_BOARD pico)

pico_sdk_init()

add_executable(spi_temp main.c)

#Caso seja a Raspberry Pi Pico 
#target_link_libraries(int pico_stdlib)

#Caso seja a Raspberry Pi Pico W
target_link_libraries(spi_temp pico_cyw43_arch_none pico_stdlib hardware_spi)

pico_enable_stdio_usb(spi_temp 1)
pico_enable_stdio_uart(spi_temp 0)

pico_add_extra_outputs(spi_temp)